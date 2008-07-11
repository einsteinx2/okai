#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerConnection.h"
#include "juceKailleraServerGame.h"
#include "gameSelect.h"
#include "kaillera_ClientCore.h"
#include "uiServerWindowLists.h"

namespace n02 {

	namespace kaillera {

		extern TCHAR uiUsername[32];
		extern TCHAR uiQuitMessage[128];
		extern TCHAR uiLastIP[128];
		extern int uiConnectionSetting;
		extern int selectedAutorunIndex;
		extern int selectedDelayParam;
		int activeGameCaps;
		bool hosting;


		/************************************************************
		** game window
		*******************************/

		SIMPLEWINDOW(KailleraServerGame, "Game:...", Colours::whitesmoke, DocumentWindow::closeButton, juceKailleraServerGame, 650, 250);


		void KailleraServerGame::OnClose() {
			KailleraServerGame::getCurrentlyModalComponent()->exitModalState(0);
			delete KailleraServerGame::window;
		}

		// start button callback
		void uiStartGameCallback() {
		}

		// leave button callback
		void uiLeaveGameCallback() {
		}

		// kick button callback

		void uiKickGameCallback() {

		}

		// game chat input callback

		void KailleraGameChatInput::textEditorReturnKeyPressed (TextEditor &editor) {
			String text = editor.getText();
			text.append(T("\r"), 1);
			KailleraServerGame::cmponnt->appendText(text);
			editor.setText("");
		}


		/************************************************************
		** Server connection window
		*******************************/

		SIMPLEWINDOW(KailleraServerConnection, "Connecting...", Colours::whitesmoke, DocumentWindow::allButtons, juceKailleraServerConnection, 750, 500);
		void KailleraServerConnection::OnClose() {
			KailleraServerConnection::getCurrentlyModalComponent()->exitModalState(0);
			KailleraServerConnection::window->setVisible(false);
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
				AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Game", c);
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

			KailleraListsCommand * cmd = new KailleraListsCommand;
			cmd->command = LISTCMD_ADDUSER;
			cmd->body.user = user;
			KailleraServerConnection::cmponnt->sendCommand(cmd);
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

			KailleraListsCommand * cmd = new KailleraListsCommand;
			cmd->command = LISTCMD_ADDGAME;
			cmd->body.game = game;
			KailleraServerConnection::cmponnt->sendCommand(cmd);

			TRACE();
		}
		static void N02CCNV serverUserJoin (const char * nick, const int ping, const unsigned short id, const char connectionSetting)
		{
			TRACE();
			serverUserAdd(nick, ping, id, connectionSetting, 1);
			juce::String notification;
			notification << "* " << nick << " joined the partyline\r";
			KailleraServerConnection::cmponnt->appendText(notification);
			TRACE();
		}
		static void N02CCNV serverUserLeave (const char * nick, const char * /*quitMessage*/, const unsigned short id)
		{
			TRACE();

			KailleraUserT * user = new KailleraUserT;
			user->id = id;

			KailleraListsCommand * cmd = new KailleraListsCommand;
			cmd->command = LISTCMD_REMUSER;
			cmd->body.user = user;
			KailleraServerConnection::cmponnt->sendCommand(cmd);

			juce::String notification;
			notification << "* " << nick << " left the partyline\r";
			KailleraServerConnection::cmponnt->appendText(notification);
			TRACE();
		}
		static void N02CCNV partylineChat (const char * userName, const char * message)
		{
			TRACE();
			juce::String chat;
			chat << "<" << userName << "> " << message << "\r";
			KailleraServerConnection::cmponnt->appendText(chat);
			TRACE();
		}
		static void N02CCNV gameChat (const char * userName, const char * message)
		{
			TRACE();
			juce::String chat;
			chat << "<" << userName << "> " << message << "\r";
			KailleraServerGame::cmponnt->appendText(chat);
			TRACE();
		}
		static void N02CCNV MOTD (const char * source, const char * message)
		{
			TRACE();
			juce::String chat;
			chat << " - " << message << "\r";
			KailleraServerConnection::cmponnt->appendText(chat);
			TRACE();
		}
		static void N02CCNV clientLoginStatusChange (const char * msg)
		{
			TRACE();
			juce::String mesg;
			mesg << msg << "\r";
			KailleraServerConnection::cmponnt->appendText(mesg);
			TRACE();
		}
		static void N02CCNV loggedIn ()
		{
			TRACE();
			clientLoginStatusChange("Logged in");
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
			sprintf(game->players, "%i/%i", players, maxplayers);

			KailleraListsCommand * cmd = new KailleraListsCommand;
			cmd->command = LISTCMD_STATGAME;
			cmd->body.game = game;
			KailleraServerConnection::cmponnt->sendCommand(cmd);

			TRACE();
		}
		static void N02CCNV serverGameClose (unsigned int id)
		{
			TRACE();
			KailleraGameT * game = new KailleraGameT;

			game->id = id;

			KailleraListsCommand * cmd = new KailleraListsCommand;
			cmd->command = LISTCMD_REMGAME;
			cmd->body.game = game;
			KailleraServerConnection::cmponnt->sendCommand(cmd);
			TRACE();
		}
		static void N02CCNV gameCreated ()
		{
		}
		static void N02CCNV gameClosed ()
		{
		}
		static void N02CCNV gameJoined ()
		{
		}
		static void N02CCNV gameKicked ()
		{
		}
		static void N02CCNV gamePlayerAdd (char *name, int ping, unsigned short id, char conn)
		{
		}
		static void N02CCNV gamePlayerJoined (char * username, int ping, unsigned short uid, char connset)
		{
		}
		static void N02CCNV gamePlayerLeft (char * user, unsigned short id)
		{
		}
		static void N02CCNV gamePlayerDropped (char * user, int gdpl)
		{
		}
		static void N02CCNV gameStart (int, int)
		{
		}
		static void N02CCNV gameEnded ()
		{
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


		/************************************************************
		** entry
		*******************************/
		void ConnectCallback() {

			char user[32];
			StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(user), uiUsername);

			char ip[128];
			StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(ip), uiLastIP);

			uiConnectionSetting = min(max(1, uiConnectionSetting), 6);

			SocketAddress toConnect;

			if (toConnect.parse(ip)) {

				if (toConnect.getPort() == 0)
					toConnect.setPort(27888);

				if (coreInitialize(user, 0x0F & uiConnectionSetting, &callbacks, 0)) {

					KailleraServerConnection::createAndShow();

					{
						KailleraListsCommand * cmd = new KailleraListsCommand;
						cmd->command = LISTCMD_REMALLUSERS;
						KailleraServerConnection::cmponnt->sendCommand(cmd);
					}
					{
						KailleraListsCommand * cmd = new KailleraListsCommand;
						cmd->command = LISTCMD_REMALLGAMES;
						KailleraServerConnection::cmponnt->sendCommand(cmd);
					}

					if (coreConnect(toConnect)) {

						juce::String stat;
						stat << "Connecting to " << toConnect.toString() << "\r";
						KailleraServerConnection::cmponnt->appendText(stat);

						KailleraServerConnection::window->runModalLoop();
					

						LOG(======================================================================);

						char quit[128];
						StringUtils::TCHARToUTF8(reinterpret_cast<unsigned char*>(quit), uiQuitMessage);

						coreDisconnect(quit);
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
