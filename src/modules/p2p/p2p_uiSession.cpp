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
#include "jucep2pSession.h"
#include "p2p_Core.h"
#include "juceModHelpers.h"

namespace n02 {
    namespace p2p {

        SIMPLEWINDOW(ModP2PSessionWindow, "P2P", Colours::whitesmoke, DocumentWindow::closeButton, jucep2pSession, 576, 232);

        void ModP2PSessionWindow::OnClose() {
			coreDisconnect();
            getCurrentlyModalComponent()->exitModalState(0);
        }



		void ChatInput::textEditorReturnKeyPressed(class juce::TextEditor & t) {
			String text = t.getText();
			if (text.length() > 0) {
				sendChat(text.toUTF8());
				t.setText("");
			}
		}


        extern char nick[32];
        extern char ip[128];
        extern int port;
        int selectedAutorunIndex;
        int selectedDelayParam;
        int recordingEnabled;
        int selectedSmoothing;

        int activeGameCaps = 0;

		char game[128];




		static void N02CCNV statusUpdate(char * status) {
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8(status)));


		}



		static void N02CCNV SSRV (const char * a, const int b)
		{

		}

		static void N02CCNV chatReceived (const char * src, const char * msg)
		{
			char buf[512];
			sprintf_s(buf, 512, "<%s> %s", src, msg);
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8(buf)));
			modHelper.chatReceived(src, msg);
		}

		static void N02CCNV connected ()
		{
			*game = 0;
		}
		static void N02CCNV changeGameLocked ()
		{
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_CGLOCK);
		}

		void N02CCNV  setUserReady (bool r)
		{
			char buf[512];
			sprintf_s(buf, 512, "You are %sready", r? "": "not ");
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8(buf)));
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_SET_READY, r? &buf:0);
		}
		void N02CCNV  setPeerReady (bool r)
		{
			char buf[512];
			sprintf_s(buf, 512, "Peer is %sready", r? "": "not ");
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8(buf)));
		}

		void N02CCNV gameChanged (char*g)
		{
			strcpy(game, g);
			char buf[512];
			sprintf_s(buf, 512, "Game changed to %s", game);
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8(buf)));

			if (modHelper.gameList->find(game)==0) {
				ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8("The game is not available on your list")));
			} else {
				ModP2PSessionWindow::cmponnt->sendMessage(MSG_UPDATE_CAPS, 0, modHelper.gameList->getCaps(game));
			}
			
		}

		static bool gameIsRunning = false;

		void N02CCNV gameStart (int playerNo, int numPlayers)
		{
			modHelper.startGame(game, playerNo, numPlayers);
			gameIsRunning = true;
		}
		void N02CCNV gameEnded ()
		{
			statusUpdate("Game endedxxx");
			if (gameIsRunning) {
				gameIsRunning = false;
			}
		}

		int N02CCNV getSelectedSmoothing ()
		{
			return selectedSmoothing;
		}


		static ClientCoreCallbacks callbcaks = {
			statusUpdate,
			SSRV,
			chatReceived,
			connected,
			changeGameLocked,
			setUserReady,
			setPeerReady,
			gameChanged,
			gameStart,
			gameEnded,
			getSelectedSmoothing
		};

        void uiReadynessChange(bool ready)
        {
			if (ready && modHelper.gameList->find(game) ==0) {
				if (*game ==0)
					ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8("No game selected")));
				else
					ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8("The selected game is not available on your list")));

				ModP2PSessionWindow::cmponnt->sendMessage(MSG_SET_READY, 0);
			} else
				coreSetReady(ready);
        }
        void uiGetIP()
        {
			ModP2PSessionWindow::cmponnt->sendMessage(MSG_APPEND, new String(FROMUTF8("Not implemented")));
        }
        void uiDrop()
        {
			endGame();
        }
        void uiChangeGame()
        {
			coreChangeGameLock();
        }
        void uiDisconnect()
        {
			coreDisconnect();
        }
        void uiCpr()
        {
			coreCpr();
        }


		bool isSID(char * dst)
		{
			return false;
		}

		void uiChangeGameCallBack()
		{
			char * g = getSelectedGame(ModP2PSessionWindow::window);
			if (g != 0)
				coreChangeGameReleaseChange(g);
			else
				coreChangeGameReleaseNoChange();

		}

        void sessionRun(bool connect)
        {
			LOGS(0);
			if (!connect || strlen(ip) > 2) {
				// todo: remove that 0 if connecting hack
				if (coreInitialize(nick, &callbcaks, connect? 0 : port)) {
					LOGS(Core initialized);
					ModP2PSessionWindow::createAndShow();
					if (connect) {
						// check if its an ip or sid
						if (isSID(ip)) {
							// retrive sid info
							// fill up core ips
						} else {
							SocketAddress shocker;
							if (shocker.parse(ip)) {
								if (shocker.getPort() ==0)
									shocker.setPort(27886);
								coreAddIP(shocker);
								ModP2PSessionWindow::window->runModalLoop();
								ModP2PSessionWindow::deleteAndZeroWindow();
							}
						}
					} else {
						ModP2PSessionWindow::window->runModalLoop();
						ModP2PSessionWindow::deleteAndZeroWindow();
					}
					coreTerminte();
				}
			}
		}
    };
};
