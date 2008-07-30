/******************************************************************************
          .d8888b.   .d8888b.  
         d88P  Y88b d88P  Y88b 
         888    888        888 
88888b.  888    888      .d88P 
888 "88b 888    888  .od888P"  
888  888 888    888 d88P"      
888  888 Y88b  d88P 888"       
888  888  "Y8888P"  888888888              Open Kaillera Arcade Netplay Library
*******************************************************************************
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
#pragma once

#include "module.h"
#include "common.h"

#define N02_P2P_CORE_VERSION 10

namespace n02 {
	namespace p2p {
		int  N02CCNV step();
		int  N02CCNV synchronizeGame(void * syncData, int len);
		void N02CCNV endGame();
		void N02CCNV sendAsyncData(const void * value, const int len, const int module);
		void N02CCNV sendSyncData(const void * value, const int len);
		int  N02CCNV recvSyncData(void * value, const int len);
		int  N02CCNV syncData(void * value, const int len);
		int  N02CCNV syncDataCheck (int);
		void N02CCNV sendChat (const char * message);



		typedef struct {
			void (N02CCNV * statusUpdate)(char * status);
			void (N02CCNV * SSRV) (const char *, const int);
			void (N02CCNV * chatReceived) (const char * src, const char * msg);
			void (N02CCNV * connected) ();
			void (N02CCNV * changeGameLocked) ();
			void (N02CCNV * setUserReady) (bool);
			void (N02CCNV * setPeerReady) (bool);
			void (N02CCNV * gameChanged) (char*);
			void (N02CCNV * gameStart) (int, int);
			void (N02CCNV * gameEnded) ();
			int  (N02CCNV * getSelectedSmoothing) ();
		} ClientCoreCallbacks;

#define MASK_INITIAL_FRAMES 60


		bool coreInitialize (const char * userName, ClientCoreCallbacks * callbacks, const int localPort);
		void coreTerminte();
		void coreAddIP(const SocketAddress & ip);
		void coreDisconnect();
		void coreCpr();
		void coreSetReady(bool ready);
		void coreChangeGameLock();
		void coreChangeGameReleaseChange(char*);
		void coreChangeGameReleaseNoChange();

	};
};
