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

#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerConnection.h"
#include "juceKailleraServerGame.h"
#include "gameSelect.h"
#include "kaillera_ClientCore.h"
#include "kaillera_uiServerWindowLists.h"

namespace n02 {

	namespace kaillera {

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
			return KailleraServerGame::window != 0;
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
			if (KailleraServerGame::window != 0) {
				
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
			sprintf_s(txt, 255, "* %s joined the partyline\r", nick);
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
			sprintf_s(txt, 255, "* %s left the partyline\r", nick);
			sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

			TRACE();
		}
		static void N02CCNV partylineChat (const char * userName, const char * message)
		{
			TRACE();

			char txt[512];
			sprintf_s(txt, 511, "<%s> %s\r", userName, message);
			sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

			TRACE();
		}
		static void N02CCNV gameChat (const char * userName, const char * message)
		{
			TRACE();
			
			char txt[512];
			sprintf_s(txt, 511, "<%s> %s\r", userName, message);
			sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
			modHelper.chatReceived(userName, message);

			TRACE();
		}
		static void N02CCNV MOTD (const char * source, const char * message)
		{
			TRACE();

			char txt[256];
			sprintf_s(txt, 255, "- %s\r", message);
			sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

			TRACE();
		}
		static void N02CCNV clientLoginStatusChange (const char * msg)
		{
			TRACE();

			char txt[256];
			sprintf_s(txt, 255, "%s\r", msg);
			sendCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));

			TRACE();
		}
		static void N02CCNV loggedIn ()
		{
			TRACE();
			clientLoginStatusChange("Logged in");

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
			KailleraServerGame::createAndShowChild(static_cast<Component*>(KailleraServerConnection::window));

			sendGameCommand(LISTCMD_REMALLPLAYERS, 0);

			gameRunning = false;

			TRACE();
		}
		static void N02CCNV gameClosed ()
		{
			TRACE();
			if (KailleraServerGame::window != 0) {
				KailleraServerGame::window->setVisible(false);
				KailleraServerConnection::window->removeChildComponent(KailleraServerGame::window);
				PosixThread::sleep(150); // pray that its no longer active otherwise BANG! crash >_<
				PosixThread::yield();
				delete KailleraServerGame::window;
				KailleraServerGame::window = 0;
			}
			TRACE();
		}
		static void N02CCNV gameJoined ()
		{

			TRACE();
			activeGameCaps = modHelper.gameList->getCaps(lastGame);	
			lastSelectedUserId = 0xFFFF;
			hosting = false;
			KailleraServerGame::createAndShowChild(static_cast<Component*>(KailleraServerConnection::window));

			sendGameCommand(LISTCMD_REMALLPLAYERS, 0);

			gameRunning = false;
			TRACE();
		}
		static void N02CCNV gameKicked ()
		{
			TRACE();
			sendCommand(LISTCMD_APPEND, new String("* You have been kicked out of the game\r"));
			TRACE();
		}
		static void N02CCNV gamePlayerAdd (char *name, int ping, unsigned short id, char connectionSetting)
		{
			TRACE();
			KailleraPlayerT * player = new KailleraPlayerT;

			player->connectionSetting = connectionSetting;
			player->id = id;
			strncpy(player->name, name, 31);
			player->ping = ping;

			sendGameCommand(LISTCMD_ADDPLAYER, player);
			TRACE();
		}

		static void N02CCNV gamePlayerJoined (char * username, int ping, unsigned short uid, char connset)
		{
			TRACE();
			gamePlayerAdd(username, ping, uid, connset);

			if (isGameWindowActive()) {
				char txt[256];
				sprintf_s(txt, 255, "* %s joined the game\r", username);
				sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
			}

			TRACE();
		}
		static void N02CCNV gamePlayerLeft (char * username, unsigned short id)
		{
			TRACE();
			KailleraPlayerT * player = new KailleraPlayerT;
			player->id = id;

			sendGameCommand(LISTCMD_REMPLAYER, player);

			if (isGameWindowActive()) {
				char txt[256];
				sprintf_s(txt, 255, "* %s left the game\r", username);
				sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
			}

			TRACE();
		}
		static void N02CCNV gamePlayerDropped (char * username, int gdpl)
		{
			TRACE();
			if (isGameWindowActive()) {
				char txt[256];
				sprintf_s(txt, 255, "* %s (Player %i) dropped\r", username, gdpl);
				sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8(txt)));
			}
			TRACE();
		}
		static void N02CCNV gameStart (int playerNo, int numPlayers)
		{
			String text;
			if (isGameWindowActive()) {
				sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8("Game started\r")));
			}
			modHelper.startGame(lastGame, playerNo, numPlayers);

			gameRunning = true;
		}
		static void N02CCNV gameEnded ()
		{
			if (gameRunning) {
				gameRunning = false;
				if (isGameWindowActive()) {
					sendGameCommand(LISTCMD_APPEND, new String(FROMUTF8("Game ended\r")));
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
				coreLeave();
				gameClosed();
				return;
			} else {
				sendChat(text.toUTF8());
			}
			editor.setText("");
		}


		/************************************************************
		** entry
		*******************************/
		void ConnectCallback() {

			KailleraServerGame::window = 0;

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
							sprintf_s(stat, 255, "Connecting to %s", toConnect.toString());
							clientLoginStatusChange(stat);
						}

						KailleraServerConnection::window->runModalLoop();
					
						LOGS(======================================================================);

						coreDisconnect(uiQuitMessage);
					}
					coreTerminte();

					delete KailleraServerConnection::window;

				} else {
					AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Error initializing core");
				}
			} else {
				AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Unable to parse IP address");
			}


			////KailleraServerConnection::window->setVisible(false);
			//KailleraServerGame::createAndShowModal();
			////KailleraServerConnection::window->setVisible(true);

		}
	};
};
