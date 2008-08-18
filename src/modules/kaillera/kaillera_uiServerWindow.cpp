/******************************************************************************
>         .d8888b.   .d8888b.  
>        d88P  Y88b d88P  Y88b 
>        888    888        888 
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

#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerConnection.h"
#include "juceKailleraServerGame.h"
#include "juceModHelpers.h"
#include "kaillera_ClientCore.h"
#include "kaillera_uiServerWindowLists.h"
#include "locid.h"

namespace n02 {

    namespace kaillera {

		/************************************************************
        ** global vars
        *******************************/

        extern char uiUsername[32];
        extern char uiQuitMessage[128];
        extern char uiLastIP[128];
        char lastGame[128];
        extern int uiConnectionSetting;
        extern int selectedAutorunIndex;
        extern int selectedDelayParam;
        int activeGameCaps;
        bool hosting;
        unsigned short lastSelectedUserId;
        bool gameRunning;
		static bool gameWindowVisible = false;


        /************************************************************
        ** game window
        *******************************/

        SIMPLEWINDOW(KailleraServerGame, "Game Window", Colours::whitesmoke, DocumentWindow::closeButton, juceKailleraServerGame, 650, 250);

        void sendGameCommand(int command, void * param) {
            KailleraListsCommand * cmd = new KailleraListsCommand;
            cmd->command = command;
            cmd->body.ptr = param;
            KailleraServerGame::cmponnt->sendCommand(cmd);
        }

        void KailleraServerGame::OnClose() {
            uiLeaveGameCallback();
        }

        bool isGameWindowActive() {
            return gameWindowVisible;
        }

        // start button callback
        void uiStartGameCallback() {
            if (hosting == true && gameRunning == false) {
                coreStart();
            }
        }

        // leave button callback
        void uiLeaveGameCallback() {
            coreLeave();
        }

        // kick button callback
        void uiKickGameCallback() {
            if (lastSelectedUserId != 0xFFFF) {
                coreKick(lastSelectedUserId);
                lastSelectedUserId = 0xFFFF;
            }
        }

        void uiGameJoinCallback(unsigned int id) {
            coreJoin(id);
        }

        /************************************************************
        ** Server connection window
        *******************************/

        SIMPLEWINDOW(KailleraServerConnection, "Connecting...", Colours::whitesmoke, DocumentWindow::allButtons, juceKailleraServerConnection, 750, 500);
        void KailleraServerConnection::OnClose() {
            if (gameWindowVisible) {
				
            } else  {
                KailleraServerConnection::getCurrentlyModalComponent()->exitModalState(0);
                KailleraServerConnection::window->setVisible(false);
            }
        }

        void sendCommand(int command, void * param) {
            KailleraListsCommand * cmd = new KailleraListsCommand;
            cmd->command = command;
            cmd->body.ptr = param;
            KailleraServerConnection::cmponnt->sendCommand(cmd);
        }

        void uiSetTitleCallback(String & text) {
            KailleraServerConnection::window->setName(text);
        }

        /************************************************************
        ** chat input
        *******************************/

        void KailleraChatInput::textEditorReturnKeyPressed (TextEditor &editor) {
            String text = editor.getText();
            coreSendChat(text.toUTF8());
            editor.setText("");
        }


        /************************************************************
        ** new game button
        *******************************/
        void uiNewGameCallback() {
            char * c = getSelectedGame(KailleraServerConnection::window);
            if (c) {
                strcpy(lastGame, c);
                coreCreate(c);
            }
        }



        /************************************************************
        ** Callbacks from core
        *******************************/

        static void N02CCNV serverUserAdd (const char * nick, const int ping, const unsigned short id, const char connectionSetting, const char status)
        {
            TRACE();
            KailleraUserT * user = new KailleraUserT;

            strncpy(user->username, nick, 31);
            user->ping = ping;
            user->id = id;
            user->connectionSetting =  connectionSetting;
            user->status = status;

            sendCommand(LISTCMD_ADDUSER, user);

            TRACE();
        }
        static void N02CCNV serverGameAdd (const char * name, const unsigned int id, const char * nick, const char * app, const char * users, const char status)
        {
            TRACE();
            KailleraGameT * game = new KailleraGameT;

            strncpy(game->name, name, 127);
            game->id = id;
            strncpy(game->owner, nick, 31);
            strncpy(game->players, users, 15);
            game->status = status;
            strncpy(game->app, app, 127);

            sendCommand(LISTCMD_ADDGAME, game);

            TRACE();
        }

        static void N02CCNV serverUserJoin (const char * nick, const int ping, const unsigned short id, const char connectionSetting)
        {
            TRACE();
            serverUserAdd(nick, ping, id, connectionSetting, 1);

            char txt[256];
			sprintf_s(txt, 255, LUTF8(LID_KAILLERA_J), nick);
            sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

            TRACE();
        }



        static void N02CCNV serverUserLeave (const char * nick, const char * /*quitMessage*/, const unsigned short id)
        {
            TRACE();

            KailleraUserT * user = new KailleraUserT;
            user->id = id;

            sendCommand(LISTCMD_REMUSER, user);

            char txt[256];
            sprintf_s(txt, 255, LUTF8(LID_KAILLERA_L), nick);
            sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

            TRACE();
        }
        static void N02CCNV partylineChat (const char * userName, const char * message)
        {
            TRACE();

            char txt[512];
            sprintf_s(txt, 511, "<%s> %s", userName, message);
            sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

            TRACE();
        }
        static void N02CCNV gameChat (const char * userName, const char * message)
        {
            TRACE();

            char txt[512];
            sprintf_s(txt, 511, "<%s> %s", userName, message);
            sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
            modHelper.chatReceived(userName, message);

            TRACE();
        }
        static void N02CCNV MOTD (const char * source, const char * message)
        {
            TRACE();

            char txt[256];
            sprintf_s(txt, 255, "- %s", message);
            sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

            TRACE();
        }
        static void N02CCNV clientLoginStatusChange (const char * msg)
        {
            TRACE();

            char txt[256];
            sprintf_s(txt, 255, "%s", msg);
            sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

            TRACE();
        }
		
        static void N02CCNV loggedIn ()
        {
            TRACE();
            clientLoginStatusChange(LUTF8(LID_KAILLERA_LI));

            sendCommand(LISTCMD_SETTITLE, uiLastIP);

            //KailleraServerConnection::window->setName(title);
            TRACE();
        }
        static void N02CCNV serverNewGame (const char * gname, const unsigned int id, const char*app, const char*owner)
        {
            TRACE();
            serverGameAdd(gname, id, owner, app, "1/2", 0);
            TRACE();
        }
        static void N02CCNV serverGameStatusChange (unsigned int id, char status, int players, int maxplayers)
        {
            TRACE();
            KailleraGameT * game = new KailleraGameT;
            game->id = id;
            game->status = status;
            sprintf_s(game->players, 10, "%i/%i", players, maxplayers);

            sendCommand(LISTCMD_STATGAME, game);
            TRACE();
        }
        static void N02CCNV serverGameClose (unsigned int id)
        {
            TRACE();
            KailleraGameT * game = new KailleraGameT;
            game->id = id;

            sendCommand(LISTCMD_REMGAME, game);
            TRACE();
        }
        static void N02CCNV gameCreated ()
        {
            TRACE();

			activeGameCaps = modHelper.gameList->getCaps(lastGame);
			lastSelectedUserId = 0xFFFF;
			hosting = true;

			{
				sendCommand(LISTCMD_SHOWGAME, 0);
			}

            sendGameCommand(LISTCMD_REMALLPLAYERS, 0);

            gameRunning = false;

            TRACE();
        }
		
		void uiGameWindowCreateCallback() {
			TRACE(); KailleraServerGame::createAndShow();
			TRACE(); KailleraServerGame::window->setVisible(false);
		}

		void uiGameWindowCreateCloseCallback() {
			if (KailleraServerGame::window != 0) {
				MessageManagerLock ml;
				KailleraServerGame::window->setVisible(false);
				KailleraServerGame::window->removeFromDesktop();
				delete KailleraServerGame::window;
				KailleraServerGame::window = 0;
			}
		}

		void uiGameWindowShowCallback() {
			TRACE(); KailleraServerGame::addToAsChild(KailleraServerConnection::window);
			TRACE(); KailleraServerGame::window->setVisible(true);
			gameWindowVisible = true;
			KailleraServerGame::cmponnt->clearText();
		}

		void uiGameWindowHideCallback() {
			TRACE(); KailleraServerGame::removeFromAsChild(KailleraServerConnection::window);
			TRACE(); KailleraServerGame::window->setVisible(false);
			gameWindowVisible = false;
		}

        static void N02CCNV gameClosed ()
        {
            TRACE();
			sendCommand(LISTCMD_HIDEGAME, 0);
            TRACE();
        }
        static void N02CCNV gameJoined ()
        {

            TRACE();
            activeGameCaps = modHelper.gameList->getCaps(lastGame);	
            lastSelectedUserId = 0xFFFF;
            hosting = false;

			{
				sendCommand(LISTCMD_SHOWGAME, 0);
			}

            sendGameCommand(LISTCMD_REMALLPLAYERS, 0);

            gameRunning = false;
            TRACE();
        }
        static void N02CCNV gameKicked ()
        {
            TRACE();
            sendCommand(LISTCMD_APPEND, new String(LUTF8(LID_KAILLERA_K)));
            TRACE();
        }
        static void N02CCNV gamePlayerAdd (char *name, int ping, unsigned short id, char connectionSetting)
        {
            TRACE();
            if (KailleraServerGame::window != 0) {
                KailleraPlayerT * player = new KailleraPlayerT;

                player->connectionSetting = connectionSetting;
                player->id = id;
                strncpy(player->name, name, 31);
                player->ping = ping;

                sendGameCommand(LISTCMD_ADDPLAYER, player);
            }
            TRACE();
        }

        static void N02CCNV gamePlayerJoined (char * username, int ping, unsigned short uid, char connset)
        {
            TRACE();
            if (KailleraServerGame::window != 0) {
                gamePlayerAdd(username, ping, uid, connset);

                if (isGameWindowActive()) {
                    char txt[256];
                    sprintf_s(txt, 255, LUTF8(LID_KAILLERA_GJ), username);
                    sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
                }
            }

            TRACE();
        }
        static void N02CCNV gamePlayerLeft (char * username, unsigned short id)
        {
            if (KailleraServerGame::window != 0) {
                TRACE();
                KailleraPlayerT * player = new KailleraPlayerT;
                player->id = id;

                sendGameCommand(LISTCMD_REMPLAYER, player);

                if (isGameWindowActive()) {
                    char txt[256];
                    sprintf_s(txt, 255, LUTF8(LID_KAILLERA_GL), username);
                    sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
                }

                TRACE();
            }
        }
        static void N02CCNV gamePlayerDropped (char * username, int gdpl)
        {
            if (KailleraServerGame::window != 0) {
                TRACE();
                if (isGameWindowActive()) {
                    char txt[256];
                    sprintf_s(txt, 255, LUTF8(LID_KAILLERA_PD), username, gdpl);
                    sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
                }
                TRACE();
            }
        }
        static void N02CCNV gameStart (int playerNo, int numPlayers)
        {
            if (KailleraServerGame::window != 0) {
                String text;
                if (isGameWindowActive()) {
                    sendGameCommand(LISTCMD_APPEND, new String(LUTF16(LID_KAILLERA_GS)));
                }
                modHelper.startGame(lastGame, playerNo, numPlayers);

                gameRunning = true;
            }
        }
        static void N02CCNV gameEnded ()
        {
            if (gameRunning) {
                gameRunning = false;
                if (KailleraServerGame::window != 0) {
                    if (isGameWindowActive()) {
                        sendGameCommand(LISTCMD_APPEND, new String(LUTF16(LID_KAILLERA_GE)));
                    }
                }
            }
        }

        static ClientCoreCallbacks callbacks = {
            serverUserJoin,
            serverUserLeave,
            loggedIn,
            serverUserAdd,
            serverGameAdd,
            partylineChat,
            gameChat,
            MOTD,
            clientLoginStatusChange,
            serverNewGame,
            serverGameStatusChange,
            serverGameClose,
            gameCreated,
            gameClosed,
            gameJoined,
            gameKicked,
            gamePlayerAdd,
            gamePlayerJoined,
            gamePlayerLeft,
            gamePlayerDropped,
            gameStart,
            gameEnded
        };


        // game chat input callback
        void KailleraGameChatInput::textEditorReturnKeyPressed (TextEditor &editor) {
            String text = editor.getText();
            if (text.equalsIgnoreCase(T("/close"))) {
				coreLeaveForce();
            } else {
                sendChat(text.toUTF8());
            }
            editor.setText("");
        }


        /************************************************************
        ** entry
        *******************************/
        void ConnectCallback() {

			gameWindowVisible = false;

            if (strlen(uiUsername) == 0)
                strcpy(uiUsername, "Ape");

            uiConnectionSetting = common_min(common_max(1, uiConnectionSetting), 6);

            SocketAddress toConnect;

            if (toConnect.parse(uiLastIP)) {

                if (toConnect.getPort() == 0)
                    toConnect.setPort(27888);

                if (coreInitialize(uiUsername, 0x0F & uiConnectionSetting, &callbacks, 0)) {

                    KailleraServerConnection::createAndShow();

                    {
                        sendCommand(LISTCMD_REMALLUSERS, 0);
                    }
                    {
                        sendCommand(LISTCMD_REMALLGAMES, 0);
                    }

                    if (coreConnect(toConnect)) {
                        {
                            char stat[256];
                            sprintf_s(stat, 255, LUTF8(LID_KAILLERA_CT), toConnect.toString());
                            clientLoginStatusChange(stat);
                        }

                        KailleraServerConnection::window->runModalLoop();

                        coreDisconnect(uiQuitMessage);
                    }
                    coreTerminte();

                    KailleraServerConnection::window->setVisible(false);
                    KailleraServerConnection::window->removeFromDesktop();
                    delete KailleraServerConnection::window;
					KailleraServerConnection::window = 0;
                } else {
                    AlertWindow::showMessageBox(AlertWindow::WarningIcon, LUTF8(LID_ERR1), LUTF16(LID_KAILLERA_EI));
                }
            } else {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, LUTF8(LID_ERR1), LUTF16(LID_KAILLERA_EP));
            }


            ////KailleraServerConnection::window->setVisible(false);
            //KailleraServerGame::createAndShowModal();
            ////KailleraServerConnection::window->setVisible(true);

        }
    };
};
