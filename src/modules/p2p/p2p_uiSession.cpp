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


namespace n02 {
    namespace p2p {

        SIMPLEWINDOW(ModP2PSessionWindow, "P2P", Colours::whitesmoke, DocumentWindow::closeButton, jucep2pSession, 576, 232);

        void ModP2PSessionWindow::OnClose() {
			coreDisconnect();
            getCurrentlyModalComponent()->exitModalState(0);
        }




        extern char nick[32];
        extern char ip[128];
        extern int port;
        int selectedAutorunIndex;
        int selectedDelayParam;
        int recordingEnabled;
        int selectedSmoothing;

        int activeGameCaps = 0;







		static void N02CCNV SSRV (const char * a, const int b)
		{

		}







		static ClientCoreCallbacks callbcaks = {
			SSRV
		};





        void uiReadynessChange(bool ready)
        {
        }
        void uiGetIP()
        {
        }
        void uiDrop()
        {
        }
        void uiChangeGame()
        {
        }
        void uiDisconnect()
        {
        }
        void uiCpr()
        {
        }


		bool isSID(char * dst) {
			return false;
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
								delete ModP2PSessionWindow::window;
							}
						}
					} else {
						ModP2PSessionWindow::window->runModalLoop();
						delete ModP2PSessionWindow::window;
					}
					coreTerminte();
				}
			}
		}
    };
};
