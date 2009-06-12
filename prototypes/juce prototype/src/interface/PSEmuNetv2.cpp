#include "PSEmuNetv2.h"

#include "n02version.h"

#include "common.h"

#define N02_STATIC
#include "n02.h"

unsigned long CALLBACK PSEgetLibType(void) {
	return PSE_LT_NET;
}

unsigned long CALLBACK PSEgetLibVersion(void) {
	return N02_VERSION_MAJOR << 8 | N02_VERSION_MINOR | 2 << 16;
}

char * CALLBACK PSEgetLibName(void) {
	return N02_NAME;
}





static n02ClientInterface netplay;


volatile int gameStarted = 0;
volatile int closing = 0;
int playerNo = 0;


static int  N02CCNV gameplayCallbackGameLoad(char *, int playerNpo, int, int){

	playerNo = playerNpo;
	gameStarted = 1;

	return 0;
}


static class PSENetGuiThread: public n02::PosixThread {
	
	void run() {

		strcpy(netplay.app.name, "PSENetv1 Interface");

		netplay.games.clear();
		netplay.games.add("PSENetv1 game", 2);

		netplay.gameplay.callbackGameLoad = gameplayCallbackGameLoad;

		netplay.activate(&netplay);
		closing = 1;
	}

} psenetThread;











long CALLBACK NETinit(void) {
	n02ResetInterface(&netplay, INTERFACE_CLIENT);
	return PSE_INIT_ERR_SUCCESS;
}

long CALLBACK NETshutdown(void) {
	MessageBoxA(0,0,"Shutdown",0);
	return PSE_ERR_SUCCESS;
}

long CALLBACK NETconfigure(void) {
	MessageBoxA(0,"Nothing to configure.", "Configure", 0);
	return 0;
}
long CALLBACK NETtest(void) {
	MessageBoxA(0,"Nothing to test.", "Test", 0);
	return 0;
}
void CALLBACK NETabout(void) {
	MessageBoxA(0,"http://okai.movsq.net.", "About", 0);
}




/*
opens the connection.
shall return PSE_ERR_SUCCESS on success, else PSE_ERR_FATAL.
PSE_ERR_FATAL is also returned if the user selects offline mode.
*/
long CALLBACK NETopen(void* hWnd) {

	psenetThread.start();

	gameStarted = 0;
	closing = 0;

	while (!gameStarted && !closing) {
		n02::PosixThread::sleep(50);
	}

	if (!closing) {
		return PSE_ERR_SUCCESS;
	} else {
		return PSE_ERR_FATAL;
	}
}

/*
closes the connection.
shall return 0 on success, else PSE_ERR_FATAL.
*/
long CALLBACK NETclose() {
	MessageBoxA(0,0,__FUNCTION__,0);
	return PSE_ERR_FATAL;
}



/*
this is called when the user paused the emulator.
*/
void CALLBACK NETpause() {
	// can not be implemented
	//MessageBoxA(0,0,__FUNCTION__,0);
}

/*
this is called when the user resumed the emulator.
*/
void CALLBACK NETresume() {
	// can not be implemented
	//MessageBoxA(0,0,__FUNCTION__,0);
}


/*
returns player number
*/
long CALLBACK NETqueryPlayer() {
	//MessageBoxA(0,0,__FUNCTION__,0);
	return playerNo;
}

/*
returns 0 on match
*/
long CALLBACK NETcompareData(void * /*blockPointer*/, int /*length*/) { // <- 2 parameters confirmed
	return 0;
}
//return value irrelevent
static tNetPadPSX pads[8];
long CALLBACK NETpadState(tNetPadPSX* p1, tNetPadPSX * p2) {
	pads[0] = *p1;
	int r = netplay.gameplay.syncData(pads, sizeof(tNetPadPSX));
	*p1 = pads[0];
	*p2 = pads[1];
	return 0;
}
//return value irrelevent
long CALLBACK NETtransferData(char * /*name*/, void * /*blockPointer*/, int /*length*/) {
	return 0;
}

/*
Example:

if ((ret = NETCOMPAREDATA(get_memcard_pointer(1),0x20000)) != 0) {
// Interchange memcard 1
NETTRANSFERDATA("Transfer Memcard 1",get_memcard_pointer(1),0x20000);
}

struct tnetpadpsx netpadpsx1;
struct tnetpadpsx netpadpsx2;

NETPADSTATE((struct tnetpadpsx *)&netpadpsx1,(struct tnetpadpsx *)&netpadpsx2);

Regards,
Carlos.
*/




































///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
this should be called for the first pad only on each side.
*/

long CALLBACK NETsendPadData(void * /*pData*/, int /*Size*/) {
	MessageBoxA(0,0,__FUNCTION__,0);
	// normal send will do
	return PSE_ERR_FATAL;
}




/*
call this for Pad 1/2 to get the data sent by the above func.
*/

long CALLBACK NETrecvPadData(void * /*pData*/, int /*Pad*/) {
	MessageBoxA(0,0,__FUNCTION__,0);
	// this comes paired with above function
	return PSE_ERR_FATAL;
}




/*
sends Size bytes from pData to the other side.
*/
long CALLBACK NETsendData(void * /*pData*/, int /*Size*/, int /*Mode*/){
	MessageBoxA(0,0,__FUNCTION__,0);
	// if mode is blocking, wait for confirmation too
	return PSE_ERR_FATAL;
}



/*
receives Size bytes from pData to the other side.
*/
long CALLBACK NETrecvData(void * /*pData*/, int /*Size*/, int /*Mode*/){
	MessageBoxA(0,0,__FUNCTION__,0);
	// if mode is blocking, wait for confirmation too
	return PSE_ERR_FATAL;
}



/*
sets the netInfo struct.
*/
void CALLBACK NETsetInfo(netInfo * /*info*/) {
	// store this
	MessageBoxA(0,0,__FUNCTION__,0);
}



/*
(linux only)
key is a XK_?? (X11) keycode.
*/
void CALLBACK NETkeypressed(int /*key*/){
	// probably for getting key value
	MessageBoxA(0,0,__FUNCTION__,0);
}

