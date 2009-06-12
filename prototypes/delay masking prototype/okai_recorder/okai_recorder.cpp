#define OKAI_BUILD

#include "okai_recorder_common.h"


#include <string.h>
#define _WIN32_WINNT 0x0502
#include <windows.h>
#include <time.h>
#include "odlist.h"




//------- variables n shit ----------------------
OKAI_Context * okai;

static char AppName[128];
char Game[128];
static short PlayerNo;
static short TotalPlayers;
static time_t RecTime;

static HFILE OpenedFile = HFILE_ERROR;

static bool BufsAllocated = false;
static char Comment[239];

//------- buffering control ----------------------

//assume 3 byte player data running at 60 fps for 10 minutes
#define bufflen (3 + 6) * 60 * 60 * 10


typedef struct {
	unsigned char buffer[bufflen]; // 5 kb buffer will so...

	union {
		unsigned char * ucptr;
		unsigned short * usptr;
		unsigned int * uiptr;
	};

	int len;

	inline bool will_overflow(const int l) {
		return ((l + len) > bufflen);
	}

	inline void reset(){
		len = 0;
		ucptr = buffer;
	}

	inline void put_bytes(const void * bytes, const int plen){
		memcpy(ucptr, bytes, plen);
		ucptr += plen;
		len += plen;
	}

} RecordingBufferType;



static odlist<RecordingBufferType*, 32> allocated_bufrers;


static RecordingBufferType * currentBuffer = 0;

static void AllocateNewBuffer(){

	if (currentBuffer != 0)
		allocated_bufrers.add(currentBuffer);

	currentBuffer = new RecordingBufferType;

	currentBuffer->reset();

}





// compress recorded data, save to file and close it
void  OKAI_CCNV recorderTerm		(){

	kprintf(__FILE__ ":%i", __LINE__);

	if (OpenedFile != HFILE_ERROR) {
		//out end of file marker

		static recorder_chunk_head recdata = {RDATA_EOF, 0};

		if (currentBuffer->will_overflow(recdata.size + 3))
			AllocateNewBuffer();
		currentBuffer->put_bytes(&recdata, 3);

		//put current buffer togeather with all the other buffers

		if (currentBuffer != 0)
			allocated_bufrers.add(currentBuffer);
		currentBuffer = 0;

		//calculate total no of bytes in all those buffers

		int TotalLength = 0;

		for (int x = 0; x < allocated_bufrers.size(); x++) {
			TotalLength += allocated_bufrers[x]->len;
		}

		// allocate a big big buffer to contain all of those
		char * ptrBufrer;
		char * bigBufrer = ptrBufrer = (char *)malloc(TotalLength);

		//copy all to the big bufrer
		for (int x = 0; x < allocated_bufrers.size(); x++) {
			memcpy(ptrBufrer, allocated_bufrers[x]->buffer, allocated_bufrers[x]->len);
			ptrBufrer += allocated_bufrers[x]->len;
		}

		// now get rid of the allocated shit
		while (allocated_bufrers.size() > 0) {
			delete allocated_bufrers[0];
			allocated_bufrers.removei(0);
		}

		//make ompression buffer
		uLongf CompressedBufrerLen = (TotalLength * 3) >> 1;
		unsigned char * CompressedBufrer = (unsigned char*)malloc(CompressedBufrerLen);

		//compress
		compress2(CompressedBufrer, &CompressedBufrerLen, (unsigned char*)bigBufrer, TotalLength, 9);

		//is it worth writing the compressed data?
		unsigned char WriteCompressed = CompressedBufrerLen < TotalLength? 1:0;


		//initialize header
		record_file_header recordFile;
		recordFile.VERSION = KREC_VERSION1;
		recordFile.TIMESTAMP = RecTime;
		recordFile.PlayerNo = PlayerNo;
		recordFile.TotalPlayers = TotalPlayers;
		strcpy(recordFile.Game, Game);
		strcpy(recordFile.App, AppName);
		strcpy(recordFile.Comment, Comment);
		recordFile.Compressed = WriteCompressed;
		recordFile.crc = crc32(crc32(0L, Z_NULL, 0), WriteCompressed? CompressedBufrer: (unsigned char *)bigBufrer, WriteCompressed? CompressedBufrerLen: TotalLength);


		//write header
		_lwrite(OpenedFile, (LPCCH)&recordFile, sizeof(recordFile));

		_lwrite(OpenedFile, (LPCCH)&TotalLength, sizeof(TotalLength));			

		//write body
		_lwrite(OpenedFile, WriteCompressed? (char *)CompressedBufrer: bigBufrer, WriteCompressed? CompressedBufrerLen: TotalLength);

		//free bufrers
		free(CompressedBufrer);
		free(bigBufrer);

		//close file
		CloseHandle((HANDLE)OpenedFile);

	}
	kprintf(__FILE__ ":%i", __LINE__);

	OpenedFile = HFILE_ERROR;

}
/*
Do not use recorderInit during a netplay game

filename is NULL fNameReturn is ignored

if filename is not null
- if fNameReturn is false, it'll try to record on the file name specified
- otherwise it will record in its default user configured location and 
copy its location to filename
*/

int  OKAI_CCNV recorderInit (char * game, int playerno, int totalPlayers, char * filename, bool fNameReturn) {
	char fn[4000];
	//kprintf(__FILE__ ":%i", __LINE__);
	recorderTerm();

	//kprintf(__FILE__ ":%i", __LINE__);
	strncpy(Game, game, 128);

	PlayerNo = playerno;
	TotalPlayers = totalPlayers;

	//kprintf(__FILE__ ":%i", __LINE__);

	if (filename == 0) {
		CreateDirectory("records", 0);
		int p = PlayerNo;
		int t = time(0);
		wsprintf(fn, "records\\%08x%02x%02x.krec", t, p, TotalPlayers);
		filename = fn;
	}

	OFSTRUCT of;

	OpenedFile = OpenFile(filename, &of, OF_CREATE | OF_WRITE);

	//kprintf(__FILE__ ":%i", __LINE__);

	if (OpenedFile != HFILE_ERROR) {
		AllocateNewBuffer();
		RecTime = time(0);
	}

	return OpenedFile == HFILE_ERROR? 1: 0;	
}
// add data to recorder
int   OKAI_CCNV recorderAddData	(const void * value, const int len){
	if (OpenedFile == HFILE_ERROR)
		return 1;

	static recorder_chunk_head recdata = {RDATA_DATA};

	recdata.size = len;

	if (currentBuffer->will_overflow(recdata.size + 3))
		AllocateNewBuffer();
	
	currentBuffer->put_bytes(&recdata, 3);
	currentBuffer->put_bytes(value, len);

	return 0;	
}
// add key value to recorded
int   OKAI_CCNV recorderAddValues(const void * value, const int len){
	
	//kprintf(__FILE__ ":%i", __LINE__);

	if (OpenedFile == HFILE_ERROR && len >= 0)
		return 1;

	//kprintf(__FILE__ ":%i", __LINE__);

	static recorder_chunk_head recdata = {RDATA_VALUES};

	//kprintf(__FILE__ ":%i", __LINE__);

	recdata.size = len;

	if (currentBuffer->will_overflow(recdata.size + 3))
		AllocateNewBuffer();
	
	currentBuffer->put_bytes(&recdata, 3);
	currentBuffer->put_bytes(value, len);

	//kprintf(__FILE__ ":%i", __LINE__);

	return 0;	
}
// add chat to recorder
int   OKAI_CCNV recorderAddChat	(const char * username, const char * chat){
	
	if (OpenedFile == HFILE_ERROR)
		return 1;

	static recorder_chunk_head recdata = {RDATA_CHAT};

	int ull = strlen(username) + 1;
	int chl = strlen(chat) + 1;

	recdata.size = ull + chl;

	if (currentBuffer->will_overflow(recdata.size + 3))
		AllocateNewBuffer();
	
	currentBuffer->put_bytes(&recdata, 3);
	currentBuffer->put_bytes(username, ull);
	currentBuffer->put_bytes(chat, chl);

	return 0;
}
void  OKAI_CCNV recorderSetComment(const char * comment){
	if (OpenedFile == HFILE_ERROR)
		return;

	memset(Comment, 0, sizeof(Comment));
	strcpy(Comment, comment);
	//MessageBox(0, Comment, "Comment set", 0);

}
// add player drop notification to recorder
int   OKAI_CCNV recorderAddDrop	(const char * username, int playerNo){
if (OpenedFile == HFILE_ERROR)
		return 1;

	static recorder_chunk_head recdata = {RDATA_DROP};

	int ull = strlen(username) + 1;

	recdata.size = ull + 2;

	if (currentBuffer->will_overflow(recdata.size + 3))
		AllocateNewBuffer();
	
	currentBuffer->put_bytes(&recdata, 3);
	currentBuffer->put_bytes(username, ull);
	currentBuffer->put_bytes(&playerNo, 2);

	return 0;
}


recorderInterface rec_interf = {
	recorderInit,
	recorderAddData,
	recorderAddValues,
	recorderSetComment,
	recorderAddChat,
	recorderAddDrop,
	&recorderTerm
};



void OKAI_CCNV recorder_getServiceModuleInterface (void * interf){
	((recorderInterface*)interf)->recorderInit = recorderInit;
	((recorderInterface*)interf)->recorderAddData = recorderAddData;
	((recorderInterface*)interf)->recorderAddValues = recorderAddValues;
	((recorderInterface*)interf)->recorderSetComment = recorderSetComment;
	((recorderInterface*)interf)->recorderAddChat = recorderAddChat;
	((recorderInterface*)interf)->recorderAddDrop = recorderAddDrop;
	((recorderInterface*)interf)->recorderTerm = recorderTerm;
}
void OKAI_CCNV recorder_getServiceModuleInfo (char * str){
	strcpy(str, "Records files into compressed KREC format");
}
void OKAI_CCNV recorder_configureServiceModule (){
	MessageBox(0, "Recorder config dialog goes here", "KREC1 recorder", 0);
}
void OKAI_CCNV recorder_testServiceModule (){
	MessageBox(0, "Nothing to test", "KREC1 recorder", 0);
}

okaiServiceModule okaiRecorderModule = {
	"recorder",
	"recorder",
	recorder_getServiceModuleInterface,
	recorder_getServiceModuleInfo,
	recorder_configureServiceModule,
	recorder_testServiceModule,
	-1
};








extern okaiModule mod_player;

modDevPackInterface mod_dev;
gameListInterface gamelist;


extern "C" {
	void __declspec(dllexport) __stdcall okaiModuleInit(okaiModulesHandler * mod_handler){
		okaiServiceModule xx;
		okai = 0;
		if (mod_handler && mod_handler->getServiceModule("mdp", &xx) == 0) {
			//
			xx.getServiceModuleInterface(&mod_dev);
			//MessageBox(0,0,0,0);
			okai = *mod_dev.okai;

			mod_handler->getServiceModule("gamelist", &xx);
			xx.getServiceModuleInterface(&gamelist);

			//MessageBox(0,0,0,0);
			if (okai != 0) {
				modHandler = mod_handler;
				//MessageBox(0,0,0,0);
				mod_handler->registerServiceModule(&okaiRecorderModule);
				strncpy(AppName, okai->appName, 128);

				mod_handler->registerModule(&mod_player);
			}
		}
	}
};

HINSTANCE hx;

BOOL APIENTRY DllMain (HINSTANCE hInst,
	DWORD reason,
	LPVOID){
	if(reason==DLL_PROCESS_ATTACH)
		hx = hInst;
	return TRUE;
}


