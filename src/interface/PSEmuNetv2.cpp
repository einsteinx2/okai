#include "PSEmuNetv2.h"

#include "n02version.h"

#include "common.h"

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
long CALLBACK NETcompareData(void * blockPointer, int length) { // <- 2 parameters confirmed
	return 0;
}
long CALLBACK NETpadState(tNetPadPSX* p1, tNetPadPSX * p2) {
	return 0;
}
long CALLBACK NETtransferData(char * name, void * blockPointer, int length) {
	return 0;
}

unsigned long CALLBACK PSEgetLibType(void) {
	return PSE_LT_NET;
}

unsigned long CALLBACK PSEgetLibVersion(void) {
	return N02_VERSION_MAJOR << 8 | N02_VERSION_MINOR | 2 << 16;
}

char * CALLBACK PSEgetLibName(void) {
	return N02_NAME;
}


long CALLBACK NETinit(void) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return PSE_INIT_ERR_SUCCESS;
}

long CALLBACK NETshutdown(void) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return PSE_ERR_SUCCESS;
}

long CALLBACK NETconfigure(void) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return 0;
}
long CALLBACK NETtest(void) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return 0;
}
void CALLBACK NETabout(void) {
	MessageBox(0,0,_T(__FUNCTION__),0);
}

/*
opens the connection.
shall return PSE_ERR_SUCCESS on success, else PSE_ERR_FATAL.
PSE_ERR_FATAL is also returned if the user selects offline mode.
*/
long CALLBACK NETopen(void* hWnd) {
	//MessageBox(hWnd,0,_T(__FUNCTION__),0);
	return PSE_ERR_SUCCESS;
}

/*
closes the connection.
shall return 0 on success, else PSE_ERR_FATAL.
*/
long CALLBACK NETclose() {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return PSE_ERR_FATAL;
}



/*
this is called when the user paused the emulator.
*/
void CALLBACK NETpause() {
	MessageBox(0,0,_T(__FUNCTION__),0);
}

/*
this is called when the user resumed the emulator.
*/
void CALLBACK NETresume() {
	MessageBox(0,0,_T(__FUNCTION__),0);
}




/*
returns player number
*/
long CALLBACK NETqueryPlayer() {
	MessageBox(0,0,_T(__FUNCTION__),0);
	return 1;
}




/*
this should be called for the first pad only on each side.
*/

long CALLBACK NETsendPadData(void *pData, int Size) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	// normal send will do
	return PSE_ERR_FATAL;
}




/*
call this for Pad 1/2 to get the data sent by the above func.
*/

long CALLBACK NETrecvPadData(void *pData, int Pad) {
	MessageBox(0,0,_T(__FUNCTION__),0);
	// this comes paired with above function
	return PSE_ERR_FATAL;
}




/*
sends Size bytes from pData to the other side.
*/
long CALLBACK NETsendData(void *pData, int Size, int Mode){
	MessageBox(0,0,_T(__FUNCTION__),0);
	// if mode is blocking, wait for confirmation too
	return PSE_ERR_FATAL;
}



/*
receives Size bytes from pData to the other side.
*/
long CALLBACK NETrecvData(void *pData, int Size, int Mode){
	MessageBox(0,0,_T(__FUNCTION__),0);
	// if mode is blocking, wait for confirmation too
	return PSE_ERR_FATAL;
}



/*
sets the netInfo struct.
*/
void CALLBACK NETsetInfo(netInfo *info) {
	// store this
	MessageBox(0,0,_T(__FUNCTION__),0);
}



/*
(linux only)
key is a XK_?? (X11) keycode.
*/
void CALLBACK NETkeypressed(int key){
	// probably for getting key value
	MessageBox(0,0,_T(__FUNCTION__),0);
}

