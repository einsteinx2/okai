#include "okai_recorder_common.h"

#include <memory>

#include "nSettings.h"

static HWND playingWindowHandle;
static HWND PlayerWindowHandle;

okaiModulesHandler * modHandler;

KSSDFA_ * kssdfa;

void EndGame();

int playerFrameNo;

// ========== playback stuff ==========================================================
struct PlaybackBufferType {
	unsigned char * buffer; // 5 kb buffer will so...
	int len;
	union {
		unsigned char * ucptr;
		unsigned short * usptr;
		unsigned int * uiptr;
	};

	inline void cleanup(){
		if (buffer) {
			free (buffer);
			buffer = 0;
		}		
	}
	inline void allocate(const int size){
		cleanup();
		buffer = (unsigned char *) malloc(size);
		len = size;
	}
	inline void reset(){
		ucptr = buffer;
	}
	inline int elapsed() {
		return (ucptr - buffer);
	}
	inline int left() {
		return len - elapsed();
	}
	inline void get_bytes(void * bytes, const int plen){
		memcpy(bytes, ucptr, plen);
		ucptr += plen;
	}
} PlaybackBuffer = {0};


static char file_name[2048];
static record_file_header file_header;
static DWORD file_len;
static DWORD file_data_len;
static bool playing = false;

int load_and_decompose_file(){
	//open file
	OFSTRUCT of;
	HFILE in = OpenFile(file_name, &of, OF_READ);
	if (in == HFILE_ERROR) {
		return 1;
	}

	//check size
	DWORD file_len = GetFileSize((HANDLE)in, 0);
	if (file_len < 516) {
		MessageBox(playingWindowHandle, "The file is too small", "Error", MB_ICONERROR);
		return 1;
	}

	//read header
	_lread(in, (char*)&file_header, sizeof(file_header));

	file_data_len = file_len - (sizeof(file_header) + 4);

	//we will not list older or newer versions
	if (file_header.VERSION != KREC_VERSION){
		MessageBox(playingWindowHandle, "Infompatible file version or invalid file", "Error", MB_ICONERROR);
		return 1;
	}

	if (gamelist.gameListFind(file_header.Game)==0){
		char xermsg[512];
		wsprintf(xermsg, "The game of the record file '%s' is not on your list", file_header.Game);
		MessageBox(playingWindowHandle, xermsg, "Error", MB_ICONERROR);
		return 1;
	}

	// we will not show different app record files either
	if (strcmp(file_header.App, okai->appName)!=0) {
		char xermsg[512];
		wsprintf(xermsg,"Different Applications and/or versions detected.\nExpected %s and found %s\nContinuing may cause things to behave unexpectedly. Do you want to continue?", okai->appName, file_header.App);
		if (MessageBox(playingWindowHandle, xermsg, "Error", MB_ICONERROR | MB_YESNO)==IDNO)
			return 1;
	}

	//read the size
	DWORD TotalBufferLength;
	_lread(in, (char*)&TotalBufferLength, 4);

	//allocate playback buffer
	PlaybackBuffer.allocate(TotalBufferLength);

	if (file_header.Compressed) {
		//its compressed...allocate compressed bufrer
		void* comp_buffer = malloc(file_data_len);
		_lread(in, (char*)comp_buffer, file_data_len);

		//uncompress
		if (Z_OK == uncompress((unsigned char*)PlaybackBuffer.buffer, &TotalBufferLength, (unsigned char*)comp_buffer, file_data_len)) {
			PlaybackBuffer.reset();
			free(comp_buffer);
		} else {
			MessageBox(playingWindowHandle, "Failed to uncompress the record file.", "Error", MB_ICONERROR);
			PlaybackBuffer.cleanup();
			free(comp_buffer);
			return 1;
		}
		file_data_len = TotalBufferLength;
	} else {
		_lread(in, (char*)PlaybackBuffer.buffer, file_data_len);
		PlaybackBuffer.reset();
		if(file_data_len != TotalBufferLength) {
			file_data_len = min(TotalBufferLength,file_data_len);
		}
	}
	
	//we're done with the file
	_lclose(in);
	return 0;
}

int recompose_and_save_file(bool compressIfSmaller = true){
	//open file
	OFSTRUCT of;
	HFILE OpenedFile = OpenFile(file_name, &of, OF_READ);

	if (OpenedFile != HFILE_ERROR) {

		//put current buffer togeather with all the other buffers
		int TotalLength = file_data_len;

		uLongf CompressedBufrerLen;
		unsigned char * CompressedBufrer;

		if (compressIfSmaller) {
			//make ompression buffer
			CompressedBufrerLen = (TotalLength * 3) >> 1;
			CompressedBufrer = (unsigned char*)malloc(CompressedBufrerLen);

			//compress
			compress2(CompressedBufrer, &CompressedBufrerLen, (unsigned char*)PlaybackBuffer.buffer, TotalLength, 9);
		}

		//is it worth writing the compressed data?
		unsigned char WriteCompressed = (compressIfSmaller && CompressedBufrerLen < TotalLength)? 1:0;


		//initialize header
		file_header.Compressed = WriteCompressed;
		file_header.crc = crc32(crc32(0L, Z_NULL, 0), WriteCompressed? CompressedBufrer: (unsigned char *)PlaybackBuffer.buffer, WriteCompressed? CompressedBufrerLen: TotalLength);


		//write header
		_lwrite(OpenedFile, (LPCCH)&file_header, sizeof(file_header));

		_lwrite(OpenedFile, (LPCCH)&TotalLength, sizeof(TotalLength));			

		//write body
		_lwrite(OpenedFile, WriteCompressed? (char *)CompressedBufrer: (char *)PlaybackBuffer.buffer, WriteCompressed? CompressedBufrerLen: TotalLength);

		//free bufrers
		if (compressIfSmaller)
			free(CompressedBufrer);
		
		PlaybackBuffer.cleanup();

		//close file
		CloseHandle((HANDLE)OpenedFile);

	} else {
		MessageBox(playingWindowHandle, "Error opening File", "Error", MB_ICONERROR);
		return 1;
	}
	return 0;
}


// =========== GUI ====================================================================




#include <windows.h>
#include "resource.h"
#include <commctrl.h>

//=====================================================

int CALLBACK OkaiPlayDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	static HWND stats;
	static nProgressBar ppos;
	static UINT_PTR timrer;
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			playingWindowHandle = hDlg;
			stats = GetDlgItem(hDlg, IDC_STATS);
			ppos.handle = GetDlgItem(hDlg, IDC_SLIDER);

			playerFrameNo = 0;

			SetWindowText(hDlg, file_header.Game);

			SetWindowText(stats, "Loading record");

			if (load_and_decompose_file()==0) {

				SetWindowText(stats, "Record loaded");

				strcpy(mod_dev.game, file_header.Game);
				*mod_dev.numPlayers = file_header.TotalPlayers;
				*mod_dev.playerNo = file_header.PlayerNo;
				mod_dev.kssdfa->input = KSSDFA_START_GAME;

				kprintf("left=%i", PlaybackBuffer.left());

				playing = true;

				ppos.SetRange(0, PlaybackBuffer.len);
				

				timrer = SetTimer(hDlg, 0, 100, 0);

			} else {
				SendMessage(hDlg, WM_CLOSE, 0, 0);
			}
		}
		return TRUE;
	case WM_TIMER:
		if (playing) {
			

			if (PlaybackBuffer.elapsed()) {
				char stat[1234];
				ppos.SetValue(PlaybackBuffer.elapsed());

				int noofframes = playerFrameNo;
				int noofsecond = noofframes / 60;
				int noofmins = noofsecond / 60;
				noofsecond %= 60;

				sprintf(stat, "Parsed bytes %i of %i\nElapsed: %3i:%02i", PlaybackBuffer.elapsed(), PlaybackBuffer.len, noofmins, noofsecond);

				SetWindowText(stats, stat);
			} else {
				SetWindowText(stats, "Loading game");				
			}
			
		} else {
			SetWindowText(stats, "Not playing");
			SendMessage(hDlg, WM_CLOSE, 0, 0);
		}
		break;
	case WM_CLOSE:
		__try {
		if (playing)
			EndGame();

		KillTimer(hDlg, timrer);

		PlaybackBuffer.cleanup();//memory leaks n stuff

		EndDialog(hDlg, 0);

		} __except(EXCEPTION_EXECUTE_HANDLER){
			MessageBox(0,0,"xxx",0);
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam)==BTN_STOP) {
			SendMessage(hDlg, WM_CLOSE, 0, 0);
		}
		break;
	};
	return 0;
}




///////////////////////////////////////////////////////////////////////////////
// This is for listing n stuff ////////////////////////////////////////////////
nLVw replays_list;

// Load the selected file

bool FileExists(char * filename){
	OFSTRUCT of;
	return OpenFile(filename, &of, OF_EXIST) != HFILE_ERROR;

}
OPENFILENAME off = {0};
char BrowseOpenKrecFileName[2048];
char * BrowseOpenKrecFile(){
	off.lStructSize = sizeof(off);
	off.hwndOwner = PlayerWindowHandle;
	off.lpstrFile = BrowseOpenKrecFileName;
	off.nMaxFile = sizeof(BrowseOpenKrecFileName);
	off.lpstrFilter = "Recorded files\0*.krec\0";
	off.nFilterIndex = 0;
	off.lpstrFileTitle = 0;
	off.nMaxFileTitle = 0;
	off.lpstrInitialDir = 0;
	off.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&off)==TRUE)
		return BrowseOpenKrecFileName;
	return 0;
}

char BrowseSaveKrecFileName[2048];
char * BrowseSaveKrecFile(){
	off.lStructSize = sizeof(off);
	off.hwndOwner = PlayerWindowHandle;
	off.lpstrFile = BrowseSaveKrecFileName;
	off.nMaxFile = sizeof(BrowseSaveKrecFileName);
	off.lpstrFilter = "Recorded files\0*.krec\0";
	off.nFilterIndex = 0;
	off.lpstrFileTitle = 0;
	off.nMaxFileTitle = 0;
	off.lpstrInitialDir = 0;
	off.Flags = OFN_PATHMUSTEXIST | 0;

	if (GetSaveFileName(&off)==TRUE)
		return BrowseSaveKrecFileName;
	return 0;
}

void replays_list_play_selected() {

	int s = replays_list.SelectedRow();
	if (s >=0 && s < replays_list.RowsCount()){
		replays_list.CheckRow(file_name, 2048, 8, s);

		if (FileExists(file_name))
			DialogBox(hx, (LPCTSTR)PLAYER_PLAY, PlayerWindowHandle, OkaiPlayDialogProc);
		
	}
}

void replays_browse() {
	char * s = 0;
	if (s = BrowseOpenKrecFile()) {
		strcpy(file_name, s);
		if (FileExists(file_name))
			DialogBox(hx, (LPCTSTR)PLAYER_PLAY, PlayerWindowHandle, OkaiPlayDialogProc);
	}
}

void replays_list_save_selected() {
	int s = replays_list.SelectedRow();
	if (s >=0 && s < replays_list.RowsCount()){
		replays_list.CheckRow(file_name, 2048, 8, s);

		if (FileExists(file_name)) {

			char * c = 0;
			if (c=BrowseSaveKrecFile()) {
				if (strstr(c, ".krec")==0)
					strcat(c, ".krec");

				CopyFile(file_name, c, FALSE);
			}
		}
	}
}

// add an item to list
void replays_list_add_record(char * filename, int index) {

	//open file
	OFSTRUCT of;
	HFILE in = OpenFile(filename, &of, OF_READ);
	if (in == HFILE_ERROR) {
		return;
	}

	//check size
	DWORD len = GetFileSize((HANDLE)in, 0);
	if (len < 516) {
		return;
	}

	//read header
	record_file_header recheader;
	_lread(in, (char*)&recheader, sizeof(recheader));

	//we've done enough
	_lclose(in);

	//we will not list older or newer versions
	if (recheader.VERSION != KREC_VERSION){
		return;
	}

#if 0
	// we will not show different app record files either
	if (strcmp(recheader.App, okai->appName)!=0) {
		return;	
	}
#endif


	//populate listview

	char temp[20];
	
	replays_list.AddRow(recheader.Game, index);

	index = replays_list.Find(index);

	replays_list.FillRow(recheader.App, 1, index);

	time_t xx = recheader.TIMESTAMP;
	tm * ecx = localtime(&xx);
	sprintf(temp, "%02d:%02d - %02d/%02d",ecx->tm_hour,ecx->tm_min, ecx->tm_mday, ecx->tm_mon+1);
	replays_list.FillRow(temp, 2, index);

	if (len <= 1024) {
		wsprintf(temp,"%i B", len);
	} else {
		len /= 1024;
		if (len < 100) {
			wsprintf(temp,"%i kB", len);
		} else {
			int mb = len / 1000;
			int frc = (len % 1000)/100;
			wsprintf(temp,"%i.%i MB", mb, frc);
		}
	}
	replays_list.FillRow(temp, 3, index);

	wsprintf(temp, "%i/%i", recheader.PlayerNo, recheader.TotalPlayers);
	replays_list.FillRow(temp, 4, index);

	replays_list.FillRow(recheader.Comment, 5, index);

	wsprintf(temp, "%0x", recheader.crc);
	replays_list.FillRow(temp, 6, index);

	replays_list.FillRow(recheader.Compressed? "Y":"N", 7, index);

	replays_list.FillRow(filename, 8, index);

}
// populate records list
void replays_list_repopulate(){
	replays_list.DeleteAllRows();
	char * dir = "records";
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[5000];  // directory specification
	strncpy (DirSpec, dir, strlen(dir)+1);
	strncat (DirSpec, "\\*", 3);
	int i = 0; // indexing
	hFind = FindFirstFile(DirSpec, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		replays_list.AddRow("Error refreshing list.", i++);
		replays_list.AddRow("Make sure records folder exists,", i++);
		replays_list.AddRow("it has recorded files", i++);
		replays_list.AddRow("and folder permissions are ok.", i++);
	} else {
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
			if (strstr(FindFileData.cFileName, ".krec")){
				char FileName[2000];
				wsprintf(FileName,"%s\\%s", dir, FindFileData.cFileName);
				replays_list_add_record(FileName, i++);
			}
		}
		while (FindNextFile(hFind, &FindFileData) != 0) {
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
				if (strstr(FindFileData.cFileName, ".krec")){
					char FileName[2000];
					wsprintf(FileName,"%s\\%s", dir, FindFileData.cFileName);
					replays_list_add_record(FileName, i++);
				}
			}
		}
		FindClose(hFind);
	}
}


void replays_list_delete_selected() {
	int s = replays_list.SelectedRow();
	if (s >=0 && s < replays_list.RowsCount()){
		replays_list.CheckRow(file_name, 2048, 8, s);
		
		DeleteFile(file_name);

		replays_list_repopulate();

		s = max(min(s, replays_list.RowsCount()-1), 0);
		replays_list.SelectRow(s);
				
	}
}


RECT okai_player_pos = {-1};

int CALLBACK OkaiPlayerDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HICON icon02 = 0;
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			PlayerWindowHandle = hDlg;

			SetWindowText(hDlg, "Player");
			modHandler->okaiModulesInitCB(GetDlgItem(hDlg, CMB_MODE));

			replays_list.handle = GetDlgItem(hDlg, LV_GLIST);
			replays_list.AddColumn("Game", 220);
			replays_list.AddColumn("App", 110);
			replays_list.AddColumn("Recorded Time", 80);
			replays_list.AddColumn("Length", 60);
			replays_list.AddColumn("Player", 30);
			replays_list.AddColumn("Comment", 120);
			replays_list.AddColumn("CRC32", 60);
			replays_list.AddColumn("Compressed", 20);
			replays_list.AddColumn("filename", 150);
			replays_list.FullRowSelect();

			replays_list_repopulate();

			SetWindowPos(hDlg, 0, 0, 0, 5, 4, SWP_NOMOVE|SWP_NOZORDER);

			nSettings::Initialize("player");
			nSettings::get_struct("wpos", &okai_player_pos, sizeof(okai_player_pos));
			nSettings::Terminate();

			if (okai_player_pos.left != -1)
				SetWindowPos(hDlg, 0, okai_player_pos.left, okai_player_pos.top, okai_player_pos.right - okai_player_pos.left, okai_player_pos.bottom-okai_player_pos.top, SWP_NOZORDER);

			if (icon02 ==0)
				icon02 = LoadIcon(hx, (LPCSTR)ICON02);

			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon02);
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon02);			

		}
		return TRUE;
		break;
	case WM_CLOSE:
		PlaybackBuffer.cleanup();//memory leaks n stuff

		GetWindowRect(hDlg, &okai_player_pos);
		nSettings::Initialize("player");
		nSettings::set_struct("wpos", &okai_player_pos, sizeof(okai_player_pos));
		nSettings::Terminate();


		EndDialog(hDlg, 0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){ 
		case CMB_MODE:
			if (HIWORD(wParam)==CBN_SELCHANGE) {
				if (modHandler->okaiModuleActivate(SendMessage(GetDlgItem(hDlg, CMB_MODE), CB_GETCURSEL, 0, 0))){
					SendMessage(hDlg, WM_CLOSE, 0, 0);
				}
			}
			break;
		case BTN_DELETE:
			replays_list_delete_selected();
			break;
		case BTN_PLAY:
			replays_list_play_selected();
			break;
		case BTN_REFRESH:
			replays_list_repopulate();
			break;
		case BTN_BROWSE:
			replays_browse();
			break;
		case BTN_SAVE:
			replays_list_save_selected();
			break;
		};
		break;
	case WM_NOTIFY:
		if (((LPNMHDR)lParam)->hwndFrom==replays_list.handle) {
			if (((LPNMHDR)lParam)->code==NM_DBLCLK) {
				replays_list_play_selected();
				return TRUE;
			} else if (((LPNMHDR)lParam)->code==LVN_KEYDOWN) {
				if (((LPNMLVKEYDOWN)lParam)->wVKey==VK_DELETE)
					replays_list_delete_selected();
				else if (((LPNMLVKEYDOWN)lParam)->wVKey==VK_RETURN)
					replays_list_play_selected();
			}
		}
		break;

	case WM_SIZE:
		{

			RECT encrct;

			GetWindowRect(hDlg, &encrct);

			unsigned short width = encrct.right - encrct.left;
			unsigned short height = encrct.bottom - encrct.top;

			kprintf("WM_SIZE %i, %i", width, height);
			if (width < 680) {
				SetWindowPos(hDlg, 0, 0, 0, 680, height, SWP_NOMOVE|SWP_NOZORDER);
				return TRUE;
			}

			if (height < 500) {
				SetWindowPos(hDlg, 0, 0, 0, width, 500, SWP_NOMOVE|SWP_NOZORDER);
				return TRUE;
			}

			unsigned short widthc = LOWORD(lParam);
			unsigned short heightc = HIWORD(lParam);


			SetWindowPos(replays_list.handle, 0, 0, 0, widthc-22, heightc-47, SWP_NOMOVE|SWP_NOZORDER);

			return TRUE;
		}
		break;
	};
	return 0;
}













static void EndGame(){
	//MessageBox(0,0,"11",0);
	if (playing) {
		//MessageBox(0,0,"12",0);
		playing = false;
		//MessageBox(0,0,"13",0);
		mod_dev.kssdfa->input = KSSDFA_END_GAME;
		//MessageBox(0,0,"14",0);
		//mod_dev.okaiGameEndCallback();
		//MessageBox(0,0,"15",0);
	}
}






// =======================

static bool OKAI_CCNV idleModeStep (){
	return false;
}
static void OKAI_CCNV activeteGUI	(){
	INITCOMMONCONTROLSEX ccx;
	ccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	ccx.dwICC = ICC_LISTVIEW_CLASSES | ICC_PROGRESS_CLASS;
	InitCommonControlsEx(&ccx);
	DialogBox(hx, (LPCTSTR)PLAYER_SDLG, 0, OkaiPlayerDialogProc);
}

static int  OKAI_CCNV getSelectedMode(){
	return 0;
}
static int  OKAI_CCNV synchronizeGame(){
	// start game
	return 0;
}
static void OKAI_CCNV endGame(){
	SendNotifyMessage(playingWindowHandle, WM_COMMAND, BTN_STOP, 0);
	//MessageBox(0,0,"endGame",0);
	// stop playbac
}
static void OKAI_CCNV asyncData(const void * value, const int len, const int position){
	//ignore
}
static void OKAI_CCNV sendPlayValue(const void * value, const int len){
	//ignore
}
static int  OKAI_CCNV recvPlayValues(void * value, const int len){
	//kprintf("pl=%i, %i", playing, PlaybackBuffer.left());
	char workingBuffer [1024];
	while (playing && PlaybackBuffer.left() > 3) {

		recorder_chunk_head head;
		PlaybackBuffer.get_bytes(&head, 3);
		PlaybackBuffer.get_bytes(workingBuffer, head.size);

		switch (head.type) {
			case RDATA_VALUES:
				playerFrameNo++;
				memcpy(value, workingBuffer, head.size);
				return head.size;
				break;
			case RDATA_DATA:
				okai->callbacks.asyncDataCallback(workingBuffer, head.size);
				break;
			case RDATA_CHAT:
				mod_dev.okaiChatReceivedCallback(workingBuffer, workingBuffer + (strlen(workingBuffer) + 1));
				break;
			case RDATA_DROP:
				mod_dev.okaiClientDroppedCallback(workingBuffer, *((short*)(workingBuffer + (strlen(workingBuffer) + 1))));
				break;
			case RDATA_EOF:
				EndGame();
				return -1;
				break;
		};
	}
	EndGame();
	return -1; // return data
}
static int  OKAI_CCNV exchangePlayValues(void * value, const int len){
	return recvPlayValues(value, len);
}
static int  OKAI_CCNV getLocalQueueLen(){
	return 0;//ignore
}
static int  OKAI_CCNV playDataCheck(int ms){
	return 0;//ignore
}
static void OKAI_CCNV chatSend(char * text){
	//ignore
}
static int  OKAI_CCNV getBaseDelay(){
	return 0; // ignore
}
static int  OKAI_CCNV getAddedDelay(){
	return 0; //ignore
}
static bool OKAI_CCNV isRecordingEnabled(){
	return false; // no point recording recorded files... hopefully, we'll have an editor to cut/paste/trim stuff
}
static void __cdecl debugCallback(char *, ...){

	// do nothing?
}



okaiModule mod_player = {
	"Player",
	OKAI_GMODE_PLAYBACK,
	idleModeStep,
	activeteGUI,
	getSelectedMode,
	synchronizeGame,
	endGame,
	asyncData,
	sendPlayValue,
	recvPlayValues,
	exchangePlayValues,
	getLocalQueueLen,
	playDataCheck,
	chatSend,
	getBaseDelay,
	getAddedDelay,
	isRecordingEnabled,
	debugCallback
};

