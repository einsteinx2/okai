#include "okaimodules.h"
#include "common/nSettings.h"
#include "common/nThread.h"
#include "common/k_socket.h"
#include "uihlp.h"
#include "resource.h"
#include "errr.h"

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
OKAI_Context * okai;
HICON icon02 = 0;
HINSTANCE hx;
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/* default callbacks */
int  OKAI_CCNV gameCallback	(char *game, int player, int numplayers, int mode){return 1;}
void OKAI_CCNV gameEndCallback	(){}
void OKAI_CCNV chatReceivedCallback(char *nick, char *text){}
void OKAI_CCNV clientDroppedCallback(char *nick, int playernb){}
//----------------------
/* Other callbacks */
int  OKAI_CCNV gamePreStartCallback(const char *game){return 0;}
void OKAI_CCNV packetWaitCallback(){}
void OKAI_CCNV asyncDataCallback(const void * data, const int size){}
//----------------------
/* auto rollback support callback...for OAPPM_AUTO_RESIM... memory management...application side */
void OKAI_CCNV saveStateCallback(const int no){}
void OKAI_CCNV loadStateCallback(const int no){}
void OKAI_CCNV deleteStateCallback(){}
int  OKAI_CCNV getPlayerInputCallback(void * input){return 0;}
void OKAI_CCNV setPlayerInputCallback(const void * input, const int player){}
void OKAI_CCNV comparePlayerInputCallback(const void * input1, const void * input2){}
void OKAI_CCNV runFrameCallback(int type, int){}
// to be handled by active module
void OKAI_CCNV asyncData(const void * value, const int len, const int position){n02_TRACE();}
void OKAI_CCNV sendPlayValue(const void * value, const int len){n02_TRACE();}
int  OKAI_CCNV recvPlayValues(void * value, const int len){n02_TRACE();return 0;}
int  OKAI_CCNV exchangePlayValues(void * value, const int len){n02_TRACE();return 0;}
int  OKAI_CCNV getLocalQueueLen(){n02_TRACE();return 0;}
int OKAI_CCNV playDataCheck(int ms){n02_TRACE();return 0;}
void OKAI_CCNV sendChat	(char *text) {n02_TRACE();}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////






// -------------------------------------------------------------------------
// --  Reel stuff --


int  OKAI_CCNV synchronizeGame(){
	//ADBFC(in);
	activeMod.debugCallback("Synchronizing players");
	if (!AutoGameLoaded){
		AutoGameLoaded = activeMod.synchronizeGame()==0;
	}
	//ADBFC(out);
	return AutoGameLoaded? 0:1;
}

void OKAI_CCNV changeGameMode(int newMode){
	ADBFC(Game mode change request: %s, (newMode==OKAI_GMODE_2PR||newMode==OKAI_GMODE_2PR)? "Zero Delay Mode":"Traditional Delay mode");
	AutoGameMode = newMode;

	//	if (AutoGameMode != OKAI_GMODE_2PR && AutoGameMode != OKAI_GMODE_8PR)
	//		AutoGameUseVariableDelay = 0;

}



void OKAI_CCNV okaiGameEndCallback	(){
	okai->callbacks.gameEndCallback();
	recorder.recorderTerm();
	//KDBFC(a);
}
void OKAI_CCNV okaiChatReceivedCallback(char *nick, char *text){
	okai->callbacks.chatReceivedCallback(nick, text);
	recorder.recorderAddChat(nick, text);
	//KDBFC(b);
}
void OKAI_CCNV okaiClientDroppedCallback(char *nick, int playernb){
	okai->callbacks.clientDroppedCallback(nick, playernb);
	recorder.recorderAddDrop(nick, playernb);
	//KDBFC(c);
}
void OKAI_CCNV okaiAsyncDataCallback(const void * data, const int size){
	okai->callbacks.asyncDataCallback(data, size);
	recorder.recorderAddData(data, size);
	//KDBFC(d);
}


//bool AutoGameUseVariableDelay = false;



/* end current game */
void OKAI_CCNV endGame(){
	AutoGameLoaded = false;

	int secx = AutoGameFrameNo / 60;
	char xxx[30];
	wsprintf(xxx, "%02i:%02i:%02i", secx/3600, secx/ 60, secx%60);
	recorder.recorderSetComment(xxx);
	recorder.recorderTerm();

	activeMod.endGame();
}



void okaiclientActiveModChangeCallback() {
	okai->functions.asyncData			= activeMod.asyncData;
	okai->functions.sendPlayValue		= activeMod.sendPlayValue;
	okai->functions.recvPlayValues		= activeMod.recvPlayValues;
	okai->functions.exchangePlayValues	= activeMod.exchangePlayValues;
	okai->functions.getLocalQueueLen	= activeMod.getLocalQueueLen;
	okai->functions.playDataCheck		= activeMod.playDataCheck;
	okai->functions.sendChat			= activeMod.chatSend;
}


// ---------------------------------------------------------------------------
// Gui and state transision stuff ---------------------------
int KSSDFAST [16] = {
	0, 0, 1, 1,
	1, 0, 1, 0,
	2, 0, 2, 0,
	3, 3, 3, 3
};

KSSDFA_ KSSDFA;

char GAME[128];
int playerno;
int numplayers;

class GuiThread : public nThread {
public:
	bool running;
	void run(void) {
		KDBFC(in);
		running = true;
		// init controls
		INITCOMMONCONTROLSEX icx;
		icx.dwSize = sizeof(icx);
		icx.dwICC = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
		InitCommonControlsEx(&icx);n02_TRACE();
		HMODULE p2p_riched_hm = LoadLibrary("riched32.dll");

		//gui loop
		__try {
			modeActivatedRecently = 1;
			do {n02_TRACE();
			modeActivatedRecently = 0;
			activeMod.activeteGUI();n02_TRACE();
			} while (modeActivatedRecently);
		} __except(n02ExceptionFilterFunction(GetExceptionInformation())) {
			MessageBox(GetActiveWindow(), "Please reopen kaillera", "GUI thread exception", 0);n02_TRACE();
		}

		FreeLibrary(p2p_riched_hm);
		KSSDFA.state = 3;
		running = false;
		KDBFC(out);
	}

	void starraat() {
		KDBFC(in);
		KSSDFA.state = create()!=0? 0:3;
		KDBFC(out);
	}

} okaiGuiThread;


int  OKAI_CCNV activateContext	(void* context){

	KDBFC(in);

	okai = (OKAI_Context*)context;

	// Initialize stuff

	k_socket::Initialize();

	okaiModulesInit();

	gameListInit(okai->gameList);


	if (icon02==0)
		icon02 = LoadIcon(hx, (LPCTSTR)ICON02);


	KSSDFA.state = 0;
	KSSDFA.input = 0;

	//Start Gui thread

	n02_TRACE();

	okaiGuiThread.starraat();

	// Set up current thread

	nThread game_thread;
	game_thread.capture();


	// Synchronize with GUI thread

	int counter = 5;
	while (!okaiGuiThread.running && counter-->0)
		game_thread.sleep(1);

	
	n02_TRACE();

	if (okaiGuiThread.running) {

		KDBFC(tres(1) %i == noe = %i cdo = %i, timeBeginPeriod(1), TIMERR_NOERROR , TIMERR_NOCANDO );

		// Run our state machine
		do {

			__try {
				while (KSSDFA.state != 3){
					n02_TRACE();
					KSSDFA.state = KSSDFAST[((KSSDFA.state)<<2) | KSSDFA.input];
					KSSDFA.input = 0;
					if (KSSDFA.state == 2){
						MSG message;
						while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)){
							TranslateMessage(&message);
							DispatchMessage(&message);
						}
					} else if (KSSDFA.state == 1) {
						//ADBFC(GameCallBack (game: %s player: %i total players: %i), GAME,playerno, numplayers);
						activeMod.debugCallback("Loading game %s (player %i of %i)", GAME,playerno, numplayers);

						// Set up autogame variables
						AutoGamePlayerFPTR = 0;
						AutoGameFrameNo = 0;
						AutoGamePredComplete = 0;
						AutoGameLoaded = false;
						AutoGamePlayerIndex = playerno -1;
						
						AutoGameMode = activeMod.modeBaseNo;
						AutoGameDelayMode = max(min(activeMod.getSelectedMode(), 2),0);
						AutoGamePlayerDelay = activeMod.getBaseDelay();
						

						KSSDFA.state = 2;

						// initialize recorder
						recorder.recorderTerm();
						if (isRecorderLoaded() && activeMod.isRecordingEnabled()) {
							recorder.recorderInit(GAME, playerno, numplayers, 0, 0);
						}

						okai->callbacks.gameCallback(GAME, playerno, numplayers, ((AutoGameMode==OKAI_BMODE_PB)?OKAI_GMODE_PLAYBACK:((AutoGameDelayMode==OKAI_AGDM_TRAD)?OKAI_GMODE_FIXED:OKAI_GMODE_RESIM)));

					} else if (KSSDFA.state == 0) {
						MSG message;
						while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)){
							TranslateMessage(&message);
							DispatchMessage(&message);
						}
						if (activeMod.idleModeStep()){
							continue;
						}
					} 
					game_thread.msleep(500);
				}
			} __except(n02ExceptionFilterFunction(GetExceptionInformation())) {
				KSSDFA.state = 0;
			}

		} while (KSSDFA.state != 3);

		timeEndPeriod(1);
	}

	/////////////////////////////////////
	//cleanup stuff

	recorder.recorderTerm();

	k_socket::Cleanup();

	gameListTerm();

	okaiModulesTerm();

	if (icon02!=0)
		DestroyIcon(icon02);
	icon02 = 0;

	return 0;


}

OKAIEXP okaiContextReset(void * context, int type){

	KDBFC(in);

	if (OKAI_CONTEXT_MAIN == type) {
		OKAI_Context * okaicontext = (OKAI_Context *) context;

		int size = okaicontext->stSize;
		int interf = okaicontext->interfVer;

		if (size == sizeof(OKAI_Context) && interf == OKAI_INTERFACE_VER) { //version check :D

			memset(okaicontext, 0, size);

			okaicontext->stSize = size;
			okaicontext->interfVer = interf;

			okaicontext->callbacks.gameCallback = gameCallback;
			okaicontext->callbacks.gameEndCallback = gameEndCallback;
			okaicontext->callbacks.chatReceivedCallback = chatReceivedCallback;
			okaicontext->callbacks.clientDroppedCallback = clientDroppedCallback;
			okaicontext->callbacks.gamePreStartCallback = gamePreStartCallback;
			okaicontext->callbacks.packetWaitCallback = packetWaitCallback;
			okaicontext->callbacks.asyncDataCallback = asyncDataCallback;
			okaicontext->callbacks.saveStateCallback = saveStateCallback;
			okaicontext->callbacks.loadStateCallback = loadStateCallback;
			okaicontext->callbacks.deleteStateCallback = deleteStateCallback;
			okaicontext->callbacks.getPlayerInputCallback = getPlayerInputCallback;
			okaicontext->callbacks.setPlayerInputCallback = setPlayerInputCallback;
			okaicontext->callbacks.comparePlayerInputCallback = comparePlayerInputCallback;
			okaicontext->callbacks.runFrameCallback = runFrameCallback;

			okaicontext->functions.synchronizeGame = synchronizeGame;
			okaicontext->functions.endGame = endGame;
			okaicontext->functions.asyncData = asyncData;
			okaicontext->functions.sendPlayValue = sendPlayValue;
			okaicontext->functions.recvPlayValues = recvPlayValues;
			okaicontext->functions.exchangePlayValues = exchangePlayValues;
			okaicontext->functions.getLocalQueueLen = getLocalQueueLen;
			okaicontext->functions.playDataCheck = playDataCheck;
			okaicontext->functions.sendChat = sendChat;
			okaicontext->functions.changeGameMode = changeGameMode;
			okaicontext->functions.runFrameAuto = runFrameAuto;
			okaicontext->functions.activateContext = activateContext;

		}
	}

	KDBFC(out);

	return 0;
}

OKAIEXPC okaiContextResetC(void * context, int type){
	return okaiContextReset(context, type);
}

//OKAIEXP okaiActivate(void * context){
//	return ((OKAI_Context*)context)->functions.activateContext(context);
//}
