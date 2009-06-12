/******************************************************************************
***********   .d8888b.   .d8888b    *******************************************
**********   d88P  Y88b d88P  Y88b   ******************************************
*            888    888        888   ******************************************
*    8888b.  888    888      .d88P   ******************************************
*   888 "88b 888    888  .od888P"  ********************************************
*   888  888 888    888 d88P"    **********************************************
*   888  888 Y88b  d88P 888"        *******************************************
*   888  888  "Y8888P"  888888888            Open Kaillera: http://okai.sf.net
******************************************************************************/
#include "kailleraclient.h"
#include "okaimodules.h"
#include "common/nSettings.h"
#include "p2p_ui.h"
#include <windows.h>
#include <stdarg.h>
#include "string.h"
#include "common/nSTL.h"
#include "common/nThread.h"
#include "uihlp.h"
#include "resource.h"
#include "errr.h"
#include "common/k_socket.h"
#include "common/nprintf.h"


void __cdecl kprintf(char * arg_0, ...);
#define P2P_DEFAULT_SSRV_STR "209.40.202.72 27889"

#pragma optimize("", off)
#pragma optimize("agt", on)


#ifdef STATS
#include "stats.h"
#endif

extern HINSTANCE hx;
void p2p_ShowWaitingGamesList(HWND);


void OKAI_CCNV p2p_host_game(char * game, int port);
void OKAI_CCNV p2p_connect_peer(char * hostname);
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
HWND p2pchat_hwnd;
#define WM_APP_CHATCHECK_BASE (WM_APP+1000)
#define P2PCHAT_ISLOADED	(WM_APP_CHATCHECK_BASE + 0)
#define P2PCHAT_SETSTATE	(WM_APP_CHATCHECK_BASE + 0)
#define P2PCHAT_SETSTATE_AVAILABLE 0
#define P2PCHAT_SETSTATE_CONNECTED 1
typedef struct p2pchat_msg_emuInfo{
	HWND handle;
	char AppName[128];
} p2pchat_msg_emuInfo;
typedef struct {
	char hostName[65];
	int hostPort;
	int natPort;
} p2pchat_msg_ipInfo;
typedef struct {
	int type; //type = 1  emulator info; type = 2  ip info
	union {
		p2pchat_msg_emuInfo emuInfo;
		p2pchat_msg_ipInfo ipInfo;		
	}body;
} p2pchat_msg;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void outp(char * line);

void __cdecl outpf(char * arg_0, ...) {
	char V8[1024];
	char V88[2084];
	nsprintf(V8, "%s\r\n", arg_0);
	nvsprintf (V88, V8, &arg_0);
	outp(V88);
}


void __cdecl p2p_core_debug(char * arg_0, ...) {

	char V8[1024];
	char V88[2084];
	wsprintf(V8, "%s\r\n", arg_0);

	nvsprintf (V88, V8, &arg_0);

	outp(V88);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <class _type> 
class FlipBuffer {
private:
	_type _buffer[2];
	unsigned _current: 1;

public:
	inline _type * front () {
		return &_buffer[_current];
	}

	inline _type * back () {
		return &_buffer[_current^1];
	}
	inline void flip () {
		_current++;
	}
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void flash_window(HWND handle) {
	FLASHWINFO fi;
	fi.cbSize = sizeof(fi);
	fi.hwnd = handle;
	fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
	fi.uCount = 0;
	fi.dwTimeout = 0;
	FlashWindowEx(&fi);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
bool HOST;
char IP[128];
char USERNAME[128];
int PORT;


///////////////////////////////////////////////////////////////////////////////
int COREINIT = 0;
///////////////////////////////////////////////////////////////////////////////






void print_p2pm_packet_stack(void (__cdecl * outpf) (char*,...));

///////////////////////////////////////////////////////////////////////////////
void p2p_ui_pcs(){
	outpf("============= Core status begin ===============");
	p2p_print_core_status();
	unsigned int sta =  KSSDFA.state;
	unsigned int inp = KSSDFA.input;
	outpf("KSSDFA { state: %i, input: %i }", sta, inp);
	//outpf("PACKETS { LOSS: %u, MISORDER: %u, IADSC: %u, ICDSC: %u}", PACKETLOSSCOUNT, PACKETMISOTDERCOUNT, PACKETIADSCCOUNT, PACKETINCDSCCOUNT);

	print_p2pm_packet_stack(outpf);

	outpf("============ Core status end =================");
}

extern int KDVAL;
///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// GUI vars
nLVw p2p_sdlg_con_ul;
HWND p2p_cdlg_outp; // output screen
int  p2p_cdlg_last_ping;
int  p2p_cdlg_game_dropped;
int  p2p_cdlg_peer_joined;

///////////////////////////////////////////
void DumpTrace();

static void OKAI_CCNV chatSend(char * xxx){
	if (strcmp(xxx, "/pcs")==0) {
		p2p_ui_pcs();
		return;
	} else if (strcmp(xxx, "/retr")==0) {
		p2p_retransmit();
		return;
	} else if (strcmp(xxx, "/ptrace")==0) {
		DumpTrace();
		return;
	} else if (strncmp(xxx, "/ssrv", 5)==0) {
		char * ptr = xxx + 6;
		char * host = ptr;
		while (*ptr != 0 && *ptr++ != ' ');
		int port = atoi(ptr);
		while (*ptr != 0 && *ptr++ != ' ');
		char * cmd = ptr;
		p2p_send_ssrv_packet(cmd, strlen(cmd)+1, host, port);
		return;
	} else if (strncmp(xxx, "/stats", 6)==0) {
		StatsDisplayThreadBegin();
		return;
	}
	if (p2p_is_connected()) {
		if (*xxx == '/') {
			xxx++;
			if (strcmp(xxx, "ping")==0) {
				p2p_ping();
			} else {
				outpf("Unknown command \"%s\"", xxx);
				p2p_send_chat(xxx-1);
			}
		} else {
			p2p_send_chat(xxx);
		}
	}
}
//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
// Setting vars
char p2p_option_username[32];			// username
int  p2p_option_portNo;					// port no
int  p2p_option_portSysAllocConnect;	// let system allocate port when not hosting?
char p2p_option_ssrvstr[128];			// "<ssrv server address> <ssrv server port>"
char p2p_option_lastconip[32];			// last connected ip address
char p2p_option_selected_game[128];		// selected game
int  p2p_option_sdlg_lastTab;			// last selected sdlg tab
//////////////////////////////////////////////////////////
int  p2p_option_smoothing;
int  p2p_option_agm;
int  p2p_option_recordg;
int  p2p_option_forcePort;
int  p2p_option_auto;
int  p2p_option_enlist;
int  p2p_option_forceCompatibility;

RECT  p2p_option_cdlg_pos = {-1, -1, -1, -1};
RECT  p2p_option_sdlg_pos = {-1, -1, -1, -1};

///////////////////////////////////
#define P2P_OPRIONS						\
	IOPTION("port", p2p_option_portNo, 27886)\
	IOPTION("sac", p2p_option_portSysAllocConnect, 1)\
	IOPTION("sdlg", p2p_option_sdlg_lastTab, 0)\
	IOPTION("agm", p2p_option_agm, 1)\
\
	IOPTION("smooth", p2p_option_smoothing, 0)\
	IOPTION("record", p2p_option_recordg, 0)\
	IOPTION("fport", p2p_option_forcePort, 0)\
\
	SOPTION("nick", p2p_option_username, "")\
	SOPTION("lcip", p2p_option_lastconip, "")\
	SOPTION("sgam", p2p_option_selected_game, "")\
\
	STOPTION("cdps", p2p_option_cdlg_pos)\
	STOPTION("sdps", p2p_option_sdlg_pos)\
\
	IOPTION("auto", p2p_option_auto, 0)\
	IOPTION("enlist", p2p_option_enlist, 0)\
	IOPTION("fcompat", p2p_option_forceCompatibility, 0)\

///////////////////////////////////



void p2p_ssrv_send(char *cmd){
	char xxx[2048];
	nsprintf(xxx, "/ssrv %s %s", p2p_option_ssrvstr, cmd);
	chatSend(xxx);
}
void p2p_ssrv_whatismyip(){
	p2p_ssrv_send("WHATISMYIP");
}


void p2p_ssrv_enlistgame(){
	char buf[500];
	nsprintf(buf, "ENLIST %s|%s|%s", p2p_option_selected_game, okai->appName, p2p_option_username);
	//p2p_core_debug(buf);
	p2p_ssrv_send(buf);
}
void p2p_ssrv_enlistgamef(){
	char buf[500];
	nsprintf(buf, "ENLISP %s|%s|%s|%i", p2p_option_selected_game, okai->appName, p2p_option_username, PORT);
	//p2p_core_debug(buf);
	p2p_ssrv_send(buf);
}
void p2p_ssrv_unenlistgame(){
	p2p_ssrv_send("UNENLIST");
}
void p2p_enlist_game() {
	if (p2p_option_forcePort) {
		p2p_ssrv_enlistgamef();
	} else {
		p2p_ssrv_enlistgame();
	}
}


//////////////////////////////////////////////////////////

void p2p_hosted_game_callback(char * game){

	strcpy(GAME,game);

	if (!gameListFind(game)){
		int x = 10;
		while (x-->0){
			outpf("ERROR: Game not found on your local list");
			chatSend("Game not on my list");
		}
		strcpy(GAME,"ERROR: Game not found on your local list");
	}
}


void p2p_ssrv_packet_recv_callback(char *cmd, int len, void *sadr){
	if (strcmp(cmd, "PINGRQ")==0){ // ping support
		p2p_send_ssrv_packet("xxxxxxxxxx",11, sadr);
		return;
	} else if (strncmp(cmd, "IPIS", 4)==0){ //what si my ip support
		outpf("*** Your IP address is %s", cmd+5);
		return;
	}
	sockaddr_in * s = (sockaddr_in *) sadr;
	p2p_core_debug("SSRV (%s): %s", inet_ntoa(s->sin_addr), cmd);
}

void p2p_ping_callback(int PING){
	p2p_cdlg_last_ping = PING;
}
void p2p_chat_callback(char * nick, char * msg){
	outpf("<%s> %s",nick, msg);
	if (KSSDFA.state==2) {
		okaiChatReceivedCallback(nick, msg);
	}
}
void p2p_packet_delay_callback(){
	okai->callbacks.packetWaitCallback();
}

void p2p_game_callback(char * game, int playernop, int maxplayersp){
	strcpy(GAME, game);
	playerno = playernop;
	numplayers = maxplayersp;
	KSSDFA.input = KSSDFA_START_GAME;
}
void p2p_end_game_callback(){
	KSSDFA.input = KSSDFA_END_GAME;
	KSSDFA.state = 0;
	p2p_cdlg_game_dropped = 1;
}
void p2p_client_dropped_callback(char * nick, int playerno){
	okaiClientDroppedCallback(nick, playerno);
}

void p2p_peer_joined_callback(char * p33rname, char * app){
	MessageBeep(MB_OK);
	if (HOST && p2p_option_enlist)
		p2p_ssrv_unenlistgame();

	p2p_core_debug("Connected to %s (%s)", p33rname, app);
	
	p2p_cdlg_peer_joined = 1;

}

void p2p_peer_left_callback(){
	MessageBeep(MB_OK);
	p2p_core_debug("Peer left");
	if (HOST && p2p_option_enlist)
		p2p_enlist_game();
}

void p2p_change_game_callback(char* game){
	outpf("Game changed to %s", game);
	if(!HOST) {
		strcpy(GAME, game);
	}

	if (!gameListFind(game)){
		int x = 10;
		while (x-->0)
			outpf("ERROR: Game not found on your local list");
		strcpy(GAME,"ERROR: Game not found on your local list");
		chatSend("Game not on my list");
	}
}
bool p2p_allow_incompatible(){
	return p2p_option_forceCompatibility == 0;
}
////////////////////////////////////////////////////////

void outp(char * line){
	//kprintf(line);
	int i = strlen(line);
	CHARRANGE cr;
	GETTEXTLENGTHEX gtx;
	gtx.codepage = CP_ACP;
	gtx.flags = GTL_PRECISE;
	cr.cpMin = GetWindowTextLength(p2p_cdlg_outp);
	cr.cpMax = cr.cpMin;//+i;
	SendMessage(p2p_cdlg_outp, EM_EXSETSEL, 0, (LPARAM)&cr);
	SendMessage(p2p_cdlg_outp, EM_REPLACESEL, FALSE, (LPARAM)line);
	SendMessage(p2p_cdlg_outp, WM_VSCROLL, SB_BOTTOM, 0);
}
//////////////////////////////////////////////////////////////////////



int CALLBACK P2P_CDLG_HOST_CHANGEGAMEDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static char SelectedGame[128];
	switch (uMsg) {
		case WM_INITDIALOG:
			{
				n02_TRACE();
				HWND hxx = GetDlgItem(hDlg, IDC_GAME);n02_TRACE();
				n02_TRACE();
				char * game = gameListStart();n02_TRACE();
				if (game) {
					do {n02_TRACE();
					SendMessage(hxx, CB_ADDSTRING, 0, (WPARAM)game);

					if (strcmp(p2p_option_selected_game, game)==0) {
						SetWindowText(hxx, p2p_option_selected_game);
					}
					n02_TRACE();
					} while (game = gameListNext(game));
				}
			}
			break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
			case IDC_CHANGE:

				GetDlgItemText(hDlg, IDC_GAME, SelectedGame, 128);

				if (gameListFind(SelectedGame)==0) {
					if (MessageBox(hDlg, "Please pick a valid game", "Change game", MB_OKCANCEL | MB_ICONEXCLAMATION)==IDCANCEL)
						EndDialog(hDlg, 0);
				} else {
					EndDialog(hDlg, (INT_PTR)SelectedGame);
				}

				break;
			case IDC_CANCEL:
				EndDialog(hDlg, 0);
				break;
		};
		break;
	case WM_CLOSE:
		EndDialog(hDlg, 0);
		break;
	};
	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////

int CALLBACK P2P_CDLG_HOSTDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_INITDIALOG:
		{



			if (lParam != 0) {
				*((HWND*)lParam) = hDlg;
			}

			// Smooching option
			{
				HWND p2p_ui_cdlg_CMB_DMODE = GetDlgItem(hDlg, CMB_DMODE);
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"None   (+0 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"If near UB (+4ms)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Always (+1 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+2 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+3 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+4 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+5 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+6 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+7 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_ADDSTRING, 0, (WPARAM)"Extra  (+8 frames)");
				SendMessage(p2p_ui_cdlg_CMB_DMODE, CB_SETCURSEL, p2p_option_smoothing, 0);
			}


			SendMessage(GetDlgItem(hDlg, CHK_ENLISTF), BM_SETCHECK, p2p_option_forcePort? BST_CHECKED:BST_UNCHECKED, 0);

			ShowWindow(hDlg, SW_SHOW);

			SendMessage(GetDlgItem(hDlg, CHK_COMPATIBLEONLY), BM_SETCHECK, p2p_option_forceCompatibility? BST_CHECKED:BST_UNCHECKED, 0);
			//

			if (p2p_option_auto){
				SendMessage(GetDlgItem(hDlg, CHK_ENLIST), BM_SETCHECK, p2p_option_enlist? BST_CHECKED:BST_UNCHECKED, 0);
				SendMessage(hDlg, WM_COMMAND, CHK_ENLIST, 0);
			} else {
				p2p_option_enlist = 0;
			}

		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case CMB_DMODE:
			{
				int r = p2p_option_smoothing;
				if (r != (p2p_option_smoothing = SendMessage(GetDlgItem(hDlg, CMB_DMODE), CB_GETCURSEL, 0, 0))) {
					if (p2p_option_smoothing == 0) {
						outpf("* The game will not have any smoothing");
					} else if (p2p_option_smoothing == 1) {						
						outpf("* The game will have 1 frame conditioned delay");
					} else {
						r = p2p_option_smoothing - 1;
						if (okai->appCaps & OAPP_CAP_RSIM == OAPP_CAP_RSIM) {
							outpf("* The game will have %i frames smoothing", r);
						} else {
							outpf("* The game will have %i frames extra delay", r);
						}
					}
				}
			}
			break;
		case CHK_ENLISTF:
			{
				int r = p2p_option_forcePort;
				if (r != (p2p_option_forcePort = SendMessage(GetDlgItem(hDlg,CHK_ENLISTF), BM_GETCHECK, 0, 0)==BST_CHECKED? 1:0)) {
					if (p2p_option_forcePort)
						outpf("* Port will be forced to read %i on waiting game list", p2p_option_portNo);
					else
						outpf("* Force port mode disabled");
				}		
			}
//			break;

		case CHK_ENLIST:
			{
				
				if (p2p_option_enlist=(SendMessage(GetDlgItem(hDlg,CHK_ENLIST), BM_GETCHECK, 0, 0)==BST_CHECKED)) {
					p2p_enlist_game();
				} else {
					p2p_ssrv_unenlistgame();
				}
			}
			break;

		case CHK_COMPATIBLEONLY:
			p2p_option_forceCompatibility = SendMessage(GetDlgItem(hDlg,CHK_COMPATIBLEONLY), BM_GETCHECK, 0, 0) == BST_CHECKED? 1:0;
			break;

		case IDC_SSERV_WHATSMYIP:
			p2p_ssrv_whatismyip();
			break;

		case IDC_CHANGEGAME:
			char * newg = (char*)DialogBox(hx, (LPCTSTR)P2P_CDLG_HOST_CHANGEGAME, hDlg, P2P_CDLG_HOST_CHANGEGAMEDialogProc);			

			if (newg != 0) {
				strcpy(GAME, newg);
				p2p_change_game(GAME);
				if (!p2p_is_connected()) {
					SetDlgItemText(GetParent(hDlg), IDC_GAME, GAME);
				}
			}

			break;
			
		};
		break;
	};
	return 0;
}

/////////////////////

int CALLBACK P2P_CDLGDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static HWND cdlghost = 0;
	static UINT_PTR p2p_cdlg_timer;
	switch (uMsg) {
	case WM_INITDIALOG:
		{
			
			SetWindowText(hDlg, "Connection window");

			// set icon
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon02);
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon02);
			
			//output rich edit
			p2p_cdlg_outp = GetDlgItem(hDlg, IDC_RICHEDIT2);

			if (HOST) {
				CreateDialogParam(hx, (LPCTSTR)P2P_CDLG_HOST, hDlg, P2P_CDLG_HOSTDialogProc, (LPARAM)&cdlghost);
			}

			if (p2p_core_initialize(HOST, PORT, okai->appName, GAME, p2p_option_username)){
				if (HOST) {
					outpf("Hosting %s on port %i", GAME, p2p_core_get_port());
					outpf("WARNING: Hosting requires hosting ports to be forwarded and enabled in firewalls.");
					SetDlgItemText(hDlg, IDC_GAME, GAME);
					COREINIT = 1;
				} else {
					COREINIT = 1;
					char * host;
					host = IP;
					int port = 27886;
					while (*++host != ':' && *host != 0);
					if (*host == ':') {
						*host++ = 0x00;
						port = atoi(host);
						port = port==0?27886:port;
					}
					host = IP;
					outpf("Connecting to %s:%i", host, port);
					
					if (!p2p_core_connect(host, port)){
						MessageBox(hDlg, "Error connecting to specified host/port", host, 0);
						p2p_core_cleanup();
						COREINIT = 0;
					}
				}
			} else {
				COREINIT = 0;				
				outpf("Error initializing sockets");
			}

		
			// This is status update timer
			p2p_cdlg_timer = SetTimer(hDlg, 0, 100, 0);

			// if there is no recorder, hide the record game ceckbox
			if (!isRecorderLoaded()) {
				ShowWindow(GetDlgItem(hDlg, CHK_REC), SW_HIDE);
				p2p_option_recordg = 0;
			} else {
				SendMessage(GetDlgItem(hDlg, CHK_REC), BM_SETCHECK, p2p_option_recordg? BST_CHECKED:BST_UNCHECKED, 0);
			}


			if (p2p_option_cdlg_pos.left != -1) {
				SetWindowPos(hDlg, HWND_TOP, p2p_option_cdlg_pos.left, p2p_option_cdlg_pos.top, 0, 0, SWP_NOSIZE);

				//kprintf("%i %i", p2p_option_cdlg_pos.left, p2p_option_cdlg_pos.top);
			}

			
			{
				HWND hxx = GetDlgItem(hDlg, CMB_AGM);
				SendMessage(hxx, CB_ADDSTRING, 0, (LPARAM)"Traditional delay");
				SendMessage(hxx, CB_ADDSTRING, 0, (LPARAM)"Zero delay/Fixed resim");
				//SendMessage(hxx, CB_ADDSTRING, 0, (LPARAM)"Zero delay/Variable resim");
				SendMessage(hxx, CB_SETCURSEL, p2p_option_agm, 0);

				if ((okai->appCaps & (OAPP_CAP_RSIM | OAPP_CAP_TRAD)) == (OAPP_CAP_RSIM | OAPP_CAP_TRAD) && (okai->appExtraMode & OAPPM_AUTO_RESIM)==OAPPM_AUTO_RESIM) { // there is a choice
					ShowWindow(hxx, SW_SHOW);
				}

			}

			p2p_option_cdlg_pos.left = p2p_option_cdlg_pos.top = 0x155;

			if(p2p_option_auto) {
				SendMessage(GetDlgItem(hDlg, IDC_READY),  BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(hDlg, WM_COMMAND, IDC_READY, 0);
			}

		}
		break;
	case WM_CLOSE:

		if (p2p_disconnect()) {
			
			if (!IsIconic(hDlg))
				GetWindowRect(hDlg, &p2p_option_cdlg_pos);
			
			//kprintf("%i %i", p2p_option_cdlg_pos.left, p2p_option_cdlg_pos.top);
			
			if (SendMessage(GetDlgItem(cdlghost, CHK_ENLIST), BM_GETCHECK, 0, 0)==BST_CHECKED) {
				p2p_ssrv_unenlistgame();
			}

			KillTimer(hDlg, p2p_cdlg_timer);

			if (COREINIT == 1) {
				COREINIT = 2;
				//synchronize with game thread
				nThread nx;
				nx.capture();

				outpf("Synchronizing with game thread");

				DWORD start = GetTickCount();
				while (COREINIT == 2) {
					nx.msleep(50);
					if (GetTickCount() - start > 5000)
						ExitProcess(1);
				}

				outpf("Exitting");

				p2p_core_cleanup();
				COREINIT = 0;
			}
			
			KSSDFA.state = 0;
			EndDialog(hDlg, 0);
		}
		break;
	case WM_TIMER:
		{
			static int framenoo = -2;
			static unsigned int p2p_cdlg_timer_step = 0;
			p2p_cdlg_timer_step++;
			if (KSSDFA.state == 0 && (p2p_cdlg_timer_step % 10) == 0) {
				if (p2p_is_connected()){
					SetDlgItemText(hDlg, IDC_GAME, GAME);
					if (p2p_cdlg_game_dropped) {
					
						SendMessage(GetDlgItem(hDlg, IDC_READY), BM_SETCHECK, BST_UNCHECKED, 0);
						EnableWindow(GetDlgItem(hDlg, CMB_AGM), TRUE);
						p2p_cdlg_game_dropped = 0;

					} else if (HOST && p2p_cdlg_peer_joined) {
						p2p_cdlg_peer_joined = 0;
						flash_window(hDlg);
					}
				} else {
					if (p2p_cdlg_timer_step%30==0 && !p2p_is_connected() && HOST && SendMessage(GetDlgItem(cdlghost,CHK_ENLIST), BM_GETCHECK, 0, 0)==BST_CHECKED) {
						p2p_enlist_game();
					}
					if (!HOST && p2p_is_connecting()){
						p2p_retransmit();
					}
				}

				{
					p2p_core_stats pcs;
					p2p_get_core_stats(&pcs);
					char buf[200];
					nsprintf(buf, "Ping :%i ms""\neipm: %i s: %i i: %i o: %i", pcs.ping, pcs.PIPM, pcs.InstsSpare, pcs.InstsInUse, pcs.InstsOutUse);
					SetDlgItemText(hDlg, SA_PST, buf);
				}
			} else if (KSSDFA.state==2) {

				static FlipBuffer<p2p_core_stats> stats;
				stats.flip();
				p2p_get_core_stats(stats.front());

				int fps = (stats.front()->frameno - stats.back()->frameno) * 1000;
				int dt = stats.front()->time - stats.back()->time;
				dt = max(dt, 1);

				fps /= dt;

				char buf[200];

				extern int AutoGamePlayerFPTR;

				nsprintf(buf, "fps: %i "
							  "q: %i | %i f: %i\n"
							  "eipm: %i s: %i i: %i o: %i d: %i",
							  fps, 
							  stats.front()->IncomingLocalQueueLen, stats.front()->OutgoingLocalQueueLen, stats.front()->frameno,
							  stats.front()->PIPM, stats.front()->InstsSpare, stats.front()->InstsInUse, stats.front()->InstsOutUse, AutoGamePlayerFPTR);

				SetDlgItemText(hDlg, SA_PST, buf);

			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case CHK_REC:
			{
				int r = p2p_option_recordg;

				if (r != (p2p_option_recordg = SendMessage(GetDlgItem(hDlg,CHK_REC), BM_GETCHECK, 0, 0)==BST_CHECKED? 1:0)) {
					outpf("* You game will%s be recorded", p2p_option_recordg? "": " not");
				}		

			}
			break;

	
		case IDC_CHAT:
			{
				char xxx[251];
				GetDlgItemText(hDlg,IDC_CHATI, xxx, 251);
				chatSend(xxx);
				SetDlgItemText(hDlg,IDC_CHATI, "");
			}
			break;
		case CMB_AGM:
			p2p_option_agm = SendMessage(GetDlgItem(hDlg, CMB_AGM), CB_GETCURSEL, 0, 0);
			break;
		// for quicker debug/recompiler/rerun only
		case BTN_ESCAPE:
			ExitProcess(0);
			break;

		case IDC_RETR:
			{
				chatSend("/retr");
			}
			break;

		case IDC_PING:
			{
				chatSend("/ping");
			}
			break;

		case IDC_DROPGAME:
			p2p_endGame();
			break;

		case IDC_READY:
			{
				p2p_cdlg_game_dropped = 0;
				bool r;
				p2p_set_ready(r=(SendMessage(GetDlgItem(hDlg, IDC_READY), BM_GETCHECK, 0, 0)==BST_CHECKED));
				EnableWindow(GetDlgItem(hDlg, CMB_AGM), r? FALSE:TRUE);
			}
			break;
		};
		break;
	};	
	return 0;
}































void OKAI_CCNV p2p_host_game(char * game){
	if (gameListFind(game)) {
		HOST = true;
		PORT = p2p_option_portNo;
		strncpy(GAME, game, 127);
		DialogBox(hx, (LPCTSTR)P2P_CDLG, 0, P2P_CDLGDialogProc);
	} else {
		MessageBox(0, "Pick a valid game from the list", "Invalid game speified", 0);
	}
}

void OKAI_CCNV p2p_chat_host_game(){
	if (gameListFind(p2p_option_selected_game)==0) {
		strcpy(p2p_option_selected_game, gameListStart());
	}
	
	p2p_host_game(p2p_option_selected_game);
}

void OKAI_CCNV p2p_connect_peer(char * hostname){

	HOST = false;
	PORT = p2p_option_portSysAllocConnect? 0: p2p_option_portNo;
	
	strncpy(IP, hostname, 127);

	DialogBox(hx, (LPCTSTR)P2P_CDLG, 0, P2P_CDLGDialogProc);
}








////////////////////////////////////////////////////////////////////////////////////////////
// P2P buddylist

//namespace mexico {

typedef struct {
	char username[32];
	char hostname[128];
} p2p_amigo;

static odlist<p2p_amigo, 100> p2p_amigos;


static void p2p_unload_amigos(){
	p2p_sdlg_con_ul.DeleteAllRows();
	for (int x = 0; x< p2p_amigos.length; x++){
		p2p_sdlg_con_ul.AddRow(p2p_amigos[x].username, x);
		p2p_sdlg_con_ul.FillRow(p2p_amigos[x].hostname,1, x);
	}
}

static p2p_amigo middle_man;


static int CALLBACK p2p_amigo_forge_fakedocs(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg==WM_INITDIALOG){
		if (lParam){
			SetWindowText(GetDlgItem(hDlg, IDC_NAME), middle_man.username);
			SetWindowText(GetDlgItem(hDlg, IDC_IP), middle_man.hostname);
			SetWindowText(hDlg, "Edit");
		} else SetWindowText(hDlg, "Add");
	} else if (uMsg==WM_CLOSE){
		EndDialog(hDlg, 0);
	} else if (uMsg==WM_COMMAND){
		if (LOWORD(wParam)==IDOK){
			GetWindowText(GetDlgItem(hDlg, IDC_NAME), middle_man.username,32);
			GetWindowText(GetDlgItem(hDlg, IDC_IP), middle_man.hostname,128);
			EndDialog(hDlg, 1);
		} else if (LOWORD(wParam)==IDCANCEL)
			EndDialog(hDlg, 0);
	}
	return 0;
}

static void p2p_amigo_prepare(HWND parent){
	int i = p2p_sdlg_con_ul.SelectedRow();
	if (i>=0&&i<p2p_amigos.length) {
		middle_man = p2p_amigos[i];
		if (DialogBoxParam(hx, (LPCTSTR)P2P_ITEM_EDIT, parent, (DLGPROC)p2p_amigo_forge_fakedocs, 1)){
			p2p_amigos.set(middle_man, i);
		}
	}
	p2p_unload_amigos();
}

static void p2p_amigo_load(HWND hDlg){
	memset(&middle_man, 0, sizeof(middle_man));
	if (DialogBoxParam(hx, (LPCTSTR)P2P_ITEM_EDIT, hDlg, (DLGPROC)p2p_amigo_forge_fakedocs, 0)){
		p2p_amigos.add(middle_man);
	}
	p2p_unload_amigos();
}


static void p2p_amigo_check(HWND hDlg){
	int i = p2p_sdlg_con_ul.SelectedRow();
	if (i>=0&&i<p2p_amigos.length)
		SetWindowText(hDlg, p2p_amigos[i].hostname);
}

static void p2p_amigos_slay(){
	int i = p2p_sdlg_con_ul.SelectedRow();
	if (i>=0&&i<p2p_amigos.length)
		p2p_amigos.removei(i);
	p2p_unload_amigos();
}

static void p2p_amigos_uncross_border(){
	p2p_amigos.clear();
	int count = nSettings::get_int("ULISTC", 0);
	for (int x=1;x<=count;x++){
		char idt[32];
		p2p_amigo sx;
		nsprintf(idt, "ULUN%i", x);
		nSettings::get_str(idt,sx.username, "UserName");
		nsprintf(idt, "ULHN%i", x);
		nSettings::get_str(idt,sx.hostname, "127.0.0.1");
		p2p_amigos.add(sx);
	}
}

static void p2p_amigos_cross_border(){
	nSettings::set_int("ULISTC",p2p_amigos.length);
	for (int x=0;x<p2p_amigos.length;x++){
		char idt[32];
		p2p_amigo sx = p2p_amigos[x];
		nsprintf(idt, "ULUN%i", x+1);
		nSettings::set_str(idt,sx.username);
		nsprintf(idt, "ULHN%i", x+1);
		nSettings::set_str(idt,sx.hostname);
	}
}

//};




////////////////////////////////////////////////////////////////////////////////////////////
// Settings




void p2p_load_settings(){
	nSettings::Initialize();

#define IOPTION(S, V, D) V = nSettings::get_int(S, D);
#define SOPTION(S, V, D) nSettings::get_str(S, V, D);
#define STOPTION(K, S) nSettings::get_struct(K, &S, sizeof(S));

	P2P_OPRIONS

	SOPTION("ssrv", p2p_option_ssrvstr, P2P_DEFAULT_SSRV_STR);

#undef STOPTION
#undef IOPTION
#undef SOPTION

	p2p_amigos_uncross_border();

	if (strlen(p2p_option_username) == 0) {
		DWORD size = 64;
		GetUserName(p2p_option_username, &size);
	}

	nSettings::Terminate();
}

void p2p_save_settings(){
	nSettings::Initialize();

#define IOPTION(S, V, D) nSettings::set_int(S, V);
#define SOPTION(S, V, D) nSettings::set_str(S, V);
#define STOPTION(K, S) nSettings::set_struct(K, &S, sizeof(S));

	P2P_OPRIONS

#undef STOPTION
#undef IOPTION
#undef SOPTION

	p2p_amigos_cross_border();

	nSettings::Terminate();
}



///////////////////////////////////////////////////////////////////////////////////////////
// Gui

#define TABCOUNT 5

struct P2PsDlgTabs {
	HWND handle;
	char * name;
	bool active;
	HINSTANCE hx;
} P2PsDlgTabs[TABCOUNT];

int P2PsDlgTabsActivate(int i) {
	//MessageBoxF(0, "Select %i", i); 
	i = max(min(i, 	TABCOUNT-1), 0);
	for(int x = 0 ; x < TABCOUNT; x++) {
		if (P2PsDlgTabs[x].active) {
			//MessageBoxF(0, "hiding %i", x); 
			ShowWindow(P2PsDlgTabs[x].handle, SW_HIDE);
		}
	}
	P2PsDlgTabs[i].active = true;
	//MessageBoxF(0, "showing %i", i); 
	ShowWindow(P2PsDlgTabs[i].handle, SW_SHOW);
	return i;
}


// Host tab
int CALLBACK P2P_SDLG_HOSTDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {n02_TRACE();
	switch (uMsg) {
		case WM_INITDIALOG:
		{n02_TRACE();
			P2PsDlgTabs[0].name = "Host";
			P2PsDlgTabs[0].handle = hDlg;
			P2PsDlgTabs[0].active = false;

			{n02_TRACE();
				HWND hxx = GetDlgItem(hDlg, IDC_GAME);n02_TRACE();
				n02_TRACE();
				char * game = gameListStart();n02_TRACE();
				if (game) {
					do {n02_TRACE();
						SendMessage(hxx, CB_ADDSTRING, 0, (WPARAM)game);
						
						if (strcmp(p2p_option_selected_game, game)==0) {
							SetWindowText(hxx, p2p_option_selected_game);
						}
n02_TRACE();
					} while (game = gameListNext(game));
				}
				n02_TRACE();
			}n02_TRACE();
		}
		break;
	case WM_COMMAND:n02_TRACE();
		switch(LOWORD(wParam)){
		case IDC_HOST:
			GetDlgItemText(hDlg, IDC_GAME, p2p_option_selected_game, 127);
			ShowWindow(GetParent(hDlg), SW_HIDE);
			p2p_host_game(p2p_option_selected_game);
			ShowWindow(GetParent(hDlg), SW_SHOW);
			k_socket::Zero();
			break;
		};
		break;
	case (WM_APP + 1):
		SendMessage(hDlg, WM_COMMAND, IDC_HOST, 0);
		break;
	case WM_NOTIFY:
		break;
	case WM_DESTROY:
		GetDlgItemText(hDlg, IDC_GAME, p2p_option_selected_game, 127);
		break;
	case WM_APP + 5:
		{
			RECT r = *((LPRECT)wParam);
			SetWindowPos(hDlg, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER);

			//convert to local coordinates i guess
			r.bottom -= r.top; r.top = 0;
			r.right -= r.left; r.left = 0;

			SetWindowPos(GetDlgItem(hDlg, IDC_GAME), 0, 0, 0, r.right - 20, r.bottom - 70, SWP_NOZORDER|SWP_NOMOVE);
			SetWindowPos(GetDlgItem(hDlg, IDC_HOST), 0, r.right - 65, r.bottom - 35, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
		}
		break;
	};
	return 0;
}


// Connect tab
int CALLBACK P2P_SDLG_CONNECTDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {n02_TRACE();
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			P2PsDlgTabs[1].name = "Connect";
			P2PsDlgTabs[1].handle = hDlg;
			P2PsDlgTabs[1].active = false;

			p2p_sdlg_con_ul.handle = GetDlgItem(hDlg, IDC_ULIST);
			p2p_sdlg_con_ul.AddColumn("Name", 180);
			p2p_sdlg_con_ul.AddColumn("IP", 160);
			p2p_sdlg_con_ul.FullRowSelect();

			p2p_unload_amigos();

			SetDlgItemText(hDlg, IDC_IP, p2p_option_lastconip);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case IDC_ADD:
			p2p_amigo_load(hDlg);
			break;
		case IDC_EDIT:
			p2p_amigo_prepare(hDlg);
			break;
		case IDC_DELETE:
			p2p_amigos_slay();
			break;
		case IDC_CONNECT:
			{
				char ipadd [128];
				GetDlgItemText(hDlg, IDC_IP, ipadd, 127);
				ShowWindow(GetParent(hDlg), SW_HIDE);
				p2p_connect_peer(ipadd);
				ShowWindow(GetParent(hDlg), SW_SHOW);
				k_socket::Zero();
			}
			break;
		};
		break;
	case (WM_APP + 1):
		SendMessage(hDlg, WM_COMMAND, IDC_CONNECT, 0);
		break;
	case WM_NOTIFY:
		{
			NMHDR * nParam = (NMHDR*)lParam;
			if (nParam->hwndFrom == p2p_sdlg_con_ul.handle){
				if (nParam->code == NM_CLICK) {
					p2p_amigo_check(GetDlgItem(hDlg, IDC_IP));
				}
			}
		}
		break;
	case WM_DESTROY:
		GetDlgItemText(hDlg, IDC_IP, p2p_option_lastconip, 127);
		break;
	case WM_APP + 5:
		{
			RECT r = *((LPRECT)wParam);
			SetWindowPos(hDlg, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER);

			//convert to local coordinates i guess
			r.bottom -= r.top; r.top = 0;
			r.right -= r.left; r.left = 0;
			
			SetWindowPos(p2p_sdlg_con_ul.handle, 0, 0, 0, r.right - 73, r.bottom - 55, SWP_NOZORDER|SWP_NOMOVE);
			SetWindowPos(GetDlgItem(hDlg, IDC_CONNECT), 0, r.right - 73, r.top + 20, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
			SetWindowPos(GetDlgItem(hDlg, IDC_IP), 0, 0, 0, r.right - 140, 22, SWP_NOZORDER|SWP_NOMOVE);

		}
		break;
	};
	return 0;
}

// Waiting games tab
int CALLBACK p2p_WaitingGamesListProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK P2P_SDLG_WGAMESDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {n02_TRACE();
	if (uMsg == WM_INITDIALOG) {
		P2PsDlgTabs[2].name = "Waiting games";
		P2PsDlgTabs[2].handle = hDlg;
		P2PsDlgTabs[2].active = false;
	}
	return p2p_WaitingGamesListProc(hDlg, uMsg, wParam, lParam);
}


// Chat tab
int CALLBACK P2P_SDLG_CHATDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {n02_TRACE();
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			P2PsDlgTabs[3].name = "Chat";
			P2PsDlgTabs[3].handle = hDlg;
			P2PsDlgTabs[3].active = false;

			okaiServiceModule * chat;

			n02_TRACE();

			OFSTRUCT of;
			ShowWindow(GetDlgItem(hDlg, OpenFile("modules\\okai_p2pchat.exe", &of, OF_EXIST)==HFILE_ERROR?ST_DELAY:BTN_ACTIVATE), SW_SHOW);

			return TRUE;
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam)==BTN_ACTIVATE){
			char AbsPath[2048];
			GetCurrentDirectory(2048, AbsPath);
			strcat(AbsPath, "\\modules\\okai_p2pchat.exe");
			WinExec(AbsPath, TRUE);
		} else if (LOWORD(wParam)==BTN_CONNECT) {
			nSettings::Initialize("p2pchat", true);
			HWND hxx = (HWND)nSettings::get_int("hwnd", -1);
			nSettings::Terminate();
			if (hxx != (HWND)-1 && IsWindow(hxx)) {
				if (SendMessage(hxx, P2PCHAT_ISLOADED, 0, 0)) {
					//send application info
					p2pchat_msg pcm;
					pcm.type = 1;
					pcm.body.emuInfo.handle = hDlg;
					strcpy(pcm.body.emuInfo.AppName, okai->appName);

					COPYDATASTRUCT cds;
					cds.dwData = 5687;
					cds.lpData = &pcm;
					cds.cbData = sizeof(pcm);

					SendMessage(hxx, WM_COPYDATA, (WPARAM)hDlg, (LPARAM)&cds);
				}
			}
		}
		break;
	case WM_APP: //activate
		//ShowWindow((HWND)wParam, SW_HIDE);
		//MessageBoxF("Chat", "activates now");
		//ShowWindow((HWND)wParam, SW_SHOW);
		//k_socket::Zero();
		//SendMessage(GetParent(hDlg), WM_APP+10, (WPARAM)hDlg, 0);
		break;

	case WM_APP + 3: //unloading
		break;
	case WM_APP + 5: //resize
		{
			LPRECT r = LPRECT(wParam);
			SetWindowPos(hDlg, 0, r->left, r->top, r->right - r->left, r->bottom - r->top, SWP_NOZORDER);

			RECT lr = *r;
			lr.bottom -= lr.top; lr.top = 0;
			lr.right -= lr.left; lr.left = 0;

		}
		break;

	case WM_APP + 10: //
	case WM_APP + 11:
		break;
	case WM_NOTIFY:
		break;
	};
	return 0;
}
// Options tab
odlist<simpleGuiItemInterface, 5> P2P_SDLG_OPTIONSPorts;
odlist<okaiServiceModule*, 5> P2P_SDLG_OPTIONSPortsSrv;
int CALLBACK P2P_SDLG_OPTIONSDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {n02_TRACE();
	switch (uMsg) {
		case WM_INITDIALOG:
		{
			P2PsDlgTabs[4].name = "Options";
			P2PsDlgTabs[4].handle = hDlg;
			P2PsDlgTabs[4].active = false;

			SetDlgItemInt(hDlg, IDC_PORT, p2p_option_portNo, FALSE);

			SendMessage(GetDlgItem(hDlg, IDC_CLIENTRANDOM), BM_SETCHECK, p2p_option_portSysAllocConnect? BST_CHECKED:BST_UNCHECKED, 0);
			P2P_SDLG_OPTIONSPorts.clear();

			okaiServiceModule * s = findServiceModule ("portForward");
			while (s) {
				P2P_SDLG_OPTIONSPortsSrv.add(s);
				simpleGuiItemInterface sg;
				s->getServiceModuleInterface(&sg);
				P2P_SDLG_OPTIONSPorts.add(sg);
				s = findNextServiceModule (s);
			}
			if (P2P_SDLG_OPTIONSPortsSrv.size()){
				HWND hxx = GetDlgItem(hDlg, IDC_PORTS);
				for (int x = 0; x < P2P_SDLG_OPTIONSPortsSrv.size(); x++) {
					SendMessage(hxx, CB_ADDSTRING, 0, (LPARAM)P2P_SDLG_OPTIONSPortsSrv[x]->moduleName);
				}
				PostMessage(hxx, CB_SETCURSEL, 0, 0);				
			} else {
				ShowWindow(GetDlgItem(hDlg, IDC_PORTS), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, BTN_ACTIVATE), SW_HIDE);
				ShowWindow(GetDlgItem(hDlg, ST_PORTS), SW_HIDE);
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_PORT:
			if (HIWORD(wParam)==EN_CHANGE) {
				p2p_option_portNo = GetDlgItemInt(hDlg, IDC_PORT, 0, FALSE);
			}
			break;
		case IDC_CLIENTRANDOM:
			p2p_option_portSysAllocConnect = SendMessage(GetDlgItem(hDlg, IDC_CLIENTRANDOM), BM_GETCHECK, 0, 0)==BST_CHECKED? 1:0;
			break;
		case BTN_ACTIVATE:
			{
				int sel = SendMessage(GetDlgItem(hDlg, IDC_PORT), CB_GETCURSEL, 0, 0);
				if (sel >= 0 && sel < P2P_SDLG_OPTIONSPortsSrv.size()){
					P2P_SDLG_OPTIONSPorts[sel].activateGUI(GetParent(hDlg));
				}
			}
			break;
		case BTN_HELP:
			{
				ShellExecute(0, "open", "http://kaillera.movsq.net/help.php#options", 0, 0, 1);
			}
			break;
		};
		break;
	case WM_APP + 5:
		{
			LPRECT r = LPRECT(wParam);
			SetWindowPos(hDlg, 0, r->left, r->top, r->right - r->left, r->bottom - r->top, SWP_NOZORDER);
		}
		break;
	case WM_APP + 3: //unloading
		
		break;
	case WM_NOTIFY:
		break;
	};
	return 0;
}


DLGPROC P2P_SDLGTabDPs[TABCOUNT] = {P2P_SDLG_HOSTDialogProc, P2P_SDLG_CONNECTDialogProc, P2P_SDLG_WGAMESDialogProc, P2P_SDLG_CHATDialogProc, P2P_SDLG_OPTIONSDialogProc};
int P2P_SDLGTabIDs[TABCOUNT] = {P2P_SDLG_HOST, P2P_SDLG_CONNECT, P2P_SDLG_WLIST, P2P_SDLG_CHAT, P2P_SDLG_OPTIONS};

int CALLBACK P2P_SDLGDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	static nTab p2p_sdlg_modeseltab;
	switch (uMsg) {
		case WM_INITDIALOG:
		{

			n02_TRACE();
			// No sockets should be active at this point
			k_socket::Zero();

			// Set window icons & titles
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon02);
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon02);		

			SetWindowText(hDlg, "n02.p2p " P2P_VERSION);

			p2p_load_settings();

			// Draw tabs
			p2p_sdlg_modeseltab.handle = GetDlgItem(hDlg, IDC_TAB1);

			memset(P2PsDlgTabs, 0, sizeof(P2PsDlgTabs));

			{
				for(int x = 0 ; x < TABCOUNT; x++) {
					P2PsDlgTabs[x].hx = hx;
				}
			}

			
			for(int x = 0 ; x < TABCOUNT; x++) {
				CreateDialog(P2PsDlgTabs[x].hx, (LPCTSTR)P2P_SDLGTabIDs[x], hDlg, P2P_SDLGTabDPs[x]);
				p2p_sdlg_modeseltab.AddTab(P2PsDlgTabs[x].name, 0);
			}


			p2p_sdlg_modeseltab.SelectTab(4-p2p_option_sdlg_lastTab);
			P2PsDlgTabsActivate(p2p_option_sdlg_lastTab);
			SendMessage(P2PsDlgTabs[p2p_option_sdlg_lastTab].handle, WM_APP, (WPARAM)hDlg, 0);

			// Initialize other controls

			SetDlgItemText(hDlg, IDC_USRNAME, p2p_option_username);

			okaiModulesInitCB(GetDlgItem(hDlg, CMB_MODE));


			// automation stuff
			if (p2p_option_auto) {
				SendMessage(P2PsDlgTabs[p2p_option_sdlg_lastTab].handle, WM_APP + 1, 0, 0);
			}

			// window positioning
			if (p2p_option_sdlg_pos.left != -1) {
				SetWindowPos(hDlg, HWND_TOP, p2p_option_sdlg_pos.left, p2p_option_sdlg_pos.top, p2p_option_sdlg_pos.right-p2p_option_sdlg_pos.left, p2p_option_sdlg_pos.bottom-p2p_option_sdlg_pos.top, 0);
			}
		}
		return TRUE;
		break;
	case WM_CLOSE:
		n02_TRACE();
		if (!IsIconic(hDlg))
			GetWindowRect(hDlg, &p2p_option_sdlg_pos);
		GetDlgItemText(hDlg, IDC_USRNAME, p2p_option_username, 32);
		{
			for(int x = 0 ; x < TABCOUNT; x++) {
				SendMessage(P2PsDlgTabs[x].handle, WM_APP+3, 0, 0);
				DestroyWindow(P2PsDlgTabs[x].handle);
			}
		}
		p2p_save_settings();
		EndDialog(hDlg, 0);
		break;
	case WM_COMMAND:
		n02_TRACE();
		switch (LOWORD(wParam)){
		case CMB_MODE:
			if (HIWORD(wParam)==CBN_SELCHANGE) {
				if (okaiModuleActivate(SendMessage(GetDlgItem(hDlg, CMB_MODE), CB_GETCURSEL, 0, 0))){
					SendMessage(hDlg, WM_CLOSE, 0, 0);
				}
			}
			break;
		case IDC_USRNAME:
			if (HIWORD(wParam)==EN_CHANGE) {
				GetDlgItemText(hDlg, IDC_USRNAME, p2p_option_username, 32);
			}
			break;
		case BTN_SET:

			break;
		};
		break;
	case WM_NOTIFY:
		{
			n02_TRACE();

			NMHDR * nParam = (NMHDR*)lParam;
			if (nParam->hwndFrom == p2p_sdlg_modeseltab.handle && nParam->code == TCN_SELCHANGE){

				//send distress signal first
				SendMessage(P2PsDlgTabs[p2p_option_sdlg_lastTab].handle, WM_APP + 11, 0, 0);

				//get the selected tab
				P2PsDlgTabsActivate(p2p_option_sdlg_lastTab = (4-p2p_sdlg_modeseltab.SelectedTab()));

				//Send activation signal
				SendMessage(P2PsDlgTabs[p2p_option_sdlg_lastTab].handle, WM_APP, (WPARAM)hDlg, 0);

			}
		}
		break;
	case WM_APP+10:

		n02_TRACE();

		p2p_option_sdlg_lastTab = 0;				
		p2p_sdlg_modeseltab.SelectTab(4-p2p_option_sdlg_lastTab);
		P2PsDlgTabsActivate(p2p_option_sdlg_lastTab);

		break;
	case WM_SIZE:
		{
			n02_TRACE();

			RECT encrct;

			GetWindowRect(hDlg, &encrct);

			unsigned short width = encrct.right - encrct.left;
			unsigned short height = encrct.bottom - encrct.top;

			if (width < 515) {
				SetWindowPos(hDlg, 0, 0, 0, 515, height, SWP_NOMOVE|SWP_NOZORDER);
				return TRUE;
			}

			if (height < 413) {
				SetWindowPos(hDlg, 0, 0, 0, width, 413, SWP_NOMOVE|SWP_NOZORDER);
				return TRUE;
			}

			unsigned short widthc = LOWORD(lParam);
			unsigned short heightc = HIWORD(lParam);

			//kprintf("WM_SIZE %i, %i", width, height);

			unsigned short tempw;
			unsigned short temph;

			//set up the tab control

			tempw = widthc - 18;
			temph = heightc - 54;

			//kprintf("TAB WM_SIZE %i, %i", tempw, temph);

			HWND tab = GetDlgItem(hDlg, IDC_TAB1);

			SetWindowPos(tab, 0, 0, 0, tempw, temph, SWP_NOMOVE|SWP_NOZORDER);

			//get tab's rectangle
			RECT tabRect;
			GetWindowRect(tab, &tabRect);

			ScreenToClient(hDlg, (LPPOINT)&tabRect);
			ScreenToClient(hDlg, (LPPOINT)&tabRect.right);

			//kprintf("TAB RECT %i, %i", tabRect.left, tabRect.top);

			TabCtrl_AdjustRect(tab, FALSE, &tabRect);


			//kprintf("TAB RECT %i, %i", tabRect.left, tabRect.top);

			for(int x = 0 ; x < TABCOUNT; x++) {
				SendMessage(P2PsDlgTabs[x].handle, WM_APP + 5, (WPARAM)&tabRect, 0);
			}
			

			return TRUE;
		}
		break;
	};
	return 0;
}



#pragma optimize("", on)




#undef TABCOUNT



///////////////////////////////////////////////////////////////////////////////////////////////


static int  OKAI_CCNV getSelectedMode(){
	n02_TRACE();
	return p2p_option_agm;
}
int  OKAI_CCNV p2p_getSelectedDelay(){
	return p2p_option_smoothing;
}

bool OKAI_CCNV p2p_isRecordingEnabled(){
	if (isRecorderLoaded())
		return p2p_option_recordg == 1;
	else
		return false;
}


static bool OKAI_CCNV idleModeStep(){
	n02_TRACE();
	if (COREINIT == 1) {
		p2p_step();
		return true;
	} else {
		if (COREINIT == 2)
			COREINIT = 3;
	}
	return false;
}

static void OKAI_CCNV activeteGUI(){
	n02_TRACE();
	DialogBox(hx, (LPCTSTR)P2P_SDLG, 0, P2P_SDLGDialogProc);
	n02_TRACE();
}


static int OKAI_CCNV getAddedDelay(){
	return 0;
}



// -----------------


okaiModule modP2P = 
{
	"P2P",
	OKAI_BMODE_2P,
	idleModeStep,
	activeteGUI,
	getSelectedMode,
	p2p_synchronizeGame,
	p2p_endGame,
	p2p_asyncData,
	p2p_sendPlayValue,
	p2p_recvPlayValues,
	p2p_exchangePlayValues,
	p2p_getLocalQueueLen,
	p2p_playDataCheck,
	chatSend,
	p2p_getBaseDelay,
	getAddedDelay,
	p2p_isRecordingEnabled,
	p2p_core_debug
};

