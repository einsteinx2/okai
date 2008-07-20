/******************************************************************************
O                                                           8d8b. Yb  dP Yb dP 
O                                                           8P Y8  YbdP   `8.  
Open Kaillera: Kaillera Server                              8   8   dP   dP Yb 
*******************************************************************dP**********
Copyright (c) Open Kaillera Team 2003-2008

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice must be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include <signal.h>

#include "common.h"
#include "kaillera_Instruction.h"


using namespace n02;
using namespace n02::kaillera;

GlobalTimer serverTime;

#include "nyx_IncomingConnection.h"
#include "nyx_config.h"

// Incoming connections class
class NyxIncomingConnection : public IncomingConnection {
	void incomingDataCallback(char * data, int length, SocketAddress & source);	
public: NyxIncomingConnection (int port) : IncomingConnection(port) {}
};






bool serverIsRunning;

void __cdecl Signalhandler(int signal_){
	if (signal_ == SIGINT){
		LOGS("SIGINT - Shutting down...");
		serverIsRunning = false;
	} else if (signal_ == SIGTERM) {
		LOGS("SIGTERM - Shutting down...");
		serverIsRunning = false;
	} else if (signal_ == SIGABRT) {
		LOGS("SIGABRT - Abnormal termination");
	} else if (signal_ == SIGILL) {
		LOGS("SIGILL - Illegal instruction");
	} else if (signal_ == SIGSEGV) {
		LOGS("SIGSEGV - Illegal storage access");
	}
}


// incoming connection
void NyxIncomingConnection::incomingDataCallback(char * data, int length, SocketAddress & source) {
	if (length == 5) {
		LOG(ping from %s, source.toString());
		BsdSocket::sendTo("PONG", 5, source);
	} else if (length == 10 && strcmp(data, "HELLO0.83")==0) {
		LOG(con request from %s, source.toString());
		BsdSocket::sendTo("VER", 5, source);
	} else {
		LOG(unknown %s %s, data, source.toString());
	}
}

int main(int argc, char * args[]) {

	// initialize commons
	commonInitialize();

	LOGBASICS("*******************************************************************************");
	LOGBASICS("O                                                           8d8b. Yb  dP Yb dP ");
	LOGBASICS("O                                                           8P Y8  YbdP   `8.  ");
	LOGBASICS("Open Kaillera: Kaillera Server                              8   8   dP   dP Yb ");
	LOGBASICS("*******************************************************************dP**********");
	LOGBASICS("nyx Copyright (c) Open Kaillera Team 2003-2008");
	LOGBASICS("Kaillera Copyright (c) Christophe Thibault 2001, 2002");
	LOGBASIC ("Built on %s %s", __DATE__, __TIME__);

	// load configuration items
	loadConfig();

	// set up some signal handlers
	signal(SIGINT, Signalhandler);
	signal(SIGTERM, Signalhandler);
	signal(SIGABRT, Signalhandler);
	signal(SIGILL, Signalhandler);
	signal(SIGSEGV, Signalhandler);

	serverIsRunning = true;

	LOGBASICS("Initializing communication layer");

	NyxIncomingConnection incoming(config::port);

	if (incoming.isInitialized()) {
		LOGBASICS("Comminication layer initialized");

		while (serverIsRunning == true) {
			for (int x = 0; x < 10 && serverIsRunning; x++) {
				BsdSocket::step(0, 1);
			}
			// step users list
			// step gameslist
		}

	} else {
		LOGBASICS("Failed initializing communication layer");
	}


	LOGBASICS("Cleaning up");
	// save configs
	saveCoinfig();
	// terminate commons
	commonTerminate();
}



const char * n02::n02GetName() {
	return "nyx";
}

