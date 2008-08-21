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
*******************************************************************************
Kaillera interface emulation
Kaillera (c) 2001 Christophe Thibault
Be sure to exclude and remove this part and other kaillera related parts from
your builds if it ever conflicts with kaillera's interests
******************************************************************************/
#define N02_STATIC
#include "n02.h"
#define KAILLERA_DLL
#include "kailleraclient.h"

#include "common.h"
using namespace n02;

namespace kaillera {

	static n02ClientInterface kaillera;

	static kailleraInfos infos;

	static char kInfosAppName[128];

	static bool kailleraGameLoaded = false;

	int  N02CCNV kGameCallback (char *game, int player, int numplayers, int) {
		TRACE();
		kailleraGameLoaded = false;
		return infos.gameCallback(game, player, numplayers);
	}

	void N02CCNV kChatReceivedCallback	(const char *nick, const char *text){
		TRACE();
		if(infos.chatReceivedCallback)
			infos.chatReceivedCallback(nick, text);
	}
	void N02CCNV kClientDroppedCallback	(const char *nick, int playernb){
		TRACE();
		if(infos.clientDroppedCallback)
			infos.clientDroppedCallback(nick, playernb);
	}

	extern "C" {

		DLLEXP kailleraGetVersion(char *version) {
			TRACE();
			memcpy(version, "0.9", 4);
			return 0;
		}

		DLLEXP kailleraInit() {
			TRACE();
			n02ResetInterface(&kaillera, INTERFACE_CLIENT);
			return 0;
		}

		DLLEXP kailleraShutdown() {
			TRACE();
			return 0;
		}

		DLLEXP kailleraSetInfos(kailleraInfos *infosp) {
			TRACE();
			infos = *infosp;
			strncpy(kInfosAppName, infos.appName, 127);

			kaillera.games.clear();

			char * gl = infos.gameList;

			while (strlen(gl) != 0) {
				kaillera.games.add(gl, 8);
				gl += strlen(gl) + 1;
			}

			return 0;
		}

		DLLEXP kailleraSelectServerDialog(HWND) {
			TRACE();
			strncpy(kaillera.app.name, kInfosAppName, 63);
			strcpy(kaillera.app.description, "Kaillera interface converter");
			kaillera.app.framerate = 6000;
			kaillera.app.attributes = 0;
			kaillera.gameplay.callbackChatReceived = kChatReceivedCallback;
			kaillera.gameplay.callbackPlayerDropped = kClientDroppedCallback;
			kaillera.gameplay.callbackGameLoad = kGameCallback;
			kaillera.activate(&kaillera);
			return 0;
		}


		DLLEXP kailleraModifyPlayValues(void *values, int size) {
			TRACE();
			if (kailleraGameLoaded) {
				return kaillera.gameplay.syncData(values, size);
			} else {
				if (((kailleraGameLoaded = (kaillera.gameplay.synchronizeGame(0,0)==0)))==true)
					return kailleraModifyPlayValues(values, size);
			}
			return -1;
		}


		DLLEXP kailleraChatSend(char *text) {
			TRACE();
			kaillera.gameplay.sendChat(text);
			return 0;
		}


		DLLEXP kailleraEndGame() {
			TRACE();
			kailleraGameLoaded = false;
			kaillera.gameplay.endGame();
			return 0;
		}

	};
};
