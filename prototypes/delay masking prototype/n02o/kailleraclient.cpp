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

#include "okaimodules.h"
#include "kailleraclient.h"
#include "errr.h"

OKAI_Context kaillera;
kailleraInfos infos;
char kInfosAppName[128];

bool kailleraGameLoaded = true;

int  OKAI_CCNV kGameCallback (char *game, int player, int numplayers, int mode) {
	kailleraGameLoaded = false;
	n02_TRACE();
	if (mode == OKAI_GMODE_2PR || mode == OKAI_GMODE_8PR) {
		okai->functions.changeGameMode(mode+1);
	}
	n02_TRACE();
	return infos.gameCallback(game, player, numplayers);
}

void OKAI_CCNV kChatReceivedCallback	(char *nick, char *text){
	if(infos.chatReceivedCallback)
		infos.chatReceivedCallback(nick, text);
}
void OKAI_CCNV kClientDroppedCallback	(char *nick, int playernb){
	if(infos.clientDroppedCallback)
		infos.clientDroppedCallback(nick, playernb);
}

DLLEXP kailleraGetVersion(char *version){
	n02_TRACE();
	memcpy(version, "0.9", 4);
	return 0;
}

DLLEXP kailleraInit(){
	n02_TRACE();
	return 0;
}

DLLEXP kailleraShutdown(){
	n02_TRACE();
	gameListTerm();
	n02_TRACE();
	return 0;
}

DLLEXP kailleraSetInfos(kailleraInfos *infosp){
	n02_TRACE();
	infos = *infosp;

	strncpy(kInfosAppName, infos.appName, 127);

	gameListInit(infos.gameList);
	
	return 0;
}

DLLEXP kailleraSelectServerDialog(HWND parent){
	n02_TRACE();

	kaillera.stSize = sizeof(kaillera);

	kaillera.interfVer = OKAI_INTERFACE_VER;
	
	okaiContextReset(&kaillera, OKAI_CONTEXT_MAIN);

	strncpy(kaillera.appName, kInfosAppName, 127);

	kaillera.callbacks.chatReceivedCallback = kChatReceivedCallback;

	kaillera.callbacks.clientDroppedCallback = kClientDroppedCallback;
	
	kaillera.callbacks.gameCallback = kGameCallback;

	kaillera.appCaps = OAPP_CAP_TRAD;

	kaillera.functions.activateContext(&kaillera);

	return 0;
}


DLLEXP kailleraModifyPlayValues(void *values, int size){
	n02_TRACE();
	if (kailleraGameLoaded) {
		int r = kaillera.functions.exchangePlayValues(values, size);
		if (kailleraGameLoaded = (r != -1))
			recorder.recorderAddValues(values, r);
		return r;
	} else {
		if ((kailleraGameLoaded = (kaillera.functions.synchronizeGame()==0)))
			return kailleraModifyPlayValues(values, size);
	}
	return -1;
}


DLLEXP kailleraChatSend(char *text){
	n02_TRACE();
	kaillera.functions.sendChat(text);
	return 0;
}


DLLEXP kailleraEndGame(){
	n02_TRACE();
	kailleraGameLoaded = false;
	kaillera.functions.endGame();
	recorder.recorderTerm();
	return 0;
}


BOOL APIENTRY DllMain (HINSTANCE hInst,
	DWORD reason,
	LPVOID){
	if(reason==DLL_PROCESS_ATTACH)
		hx = hInst;
	return TRUE;
}

