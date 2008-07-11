#include "uiServerWindowLists.h"
#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerConnection.h"
#include "juceKailleraServerGame.h"
#include "gameSelect.h"
#include "kaillera_ClientCore.h"


namespace n02 {

	namespace kaillera {

		/************************************************************
		** vars
		*******************************/

			/*
			
			
			
			
			for (int x = 0; x < games.itemsCount(); x++) {
				register KailleraGame * g = games.getItemPtr(x);
				if (id == g->id) {
					g->status = status;
					g->players.printf(T("%i/%i"), players, maxplayers);
					KailleraServerConnection::cmponnt->redrawGamesRow(x);
				}
			}
			
			
			*/

		typedef struct {
			unsigned short id;
			juce::String name;
			int ping;
			char connectionSetting;
		} KailleraPlayer;

		typedef struct {
			unsigned short id;
			juce::String username;
			int ping;
			char connectionSetting; // 1 == LAN; 6 == LOW
			char status; // 0 = connecting? 1 = idle ; 2 = playing
		} KailleraUser;


		typedef struct {
			unsigned int id;
			juce::String name;
			juce::String app;
			juce::String owner;
			char status;
			juce::String players;
		} KailleraGame;


		/************************************************************
		** 
		*******************************/
		juce::tchar * connectionSettingTexts [] = {
			0,
			T("LAN"),
			T("Exc"),
			T("Good"),
			T("Avg"),
			T("Bad"),
			T("Low")
		};

		juce::tchar * userStatusTexts [] = {
			T("Conneting"),
			T("Idle"),
			T("Playing")
		};

		juce::tchar * gameStatusTexts [] = {
			T("Waiting"),
			T("Playing"),
			T("Playing")
		};


		// players list
		DynamicOrderedArray<KailleraPlayer, 8> players;

		// players list drawing
		int  KailleraPlayersList::getNumRows ()
		{
			TRACE();
			return players.itemsCount();
		}
		void  KailleraPlayersList::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
		{
			TRACE();
			if (rowIsSelected)
				g.fillAll (Colour(0xdd,0xdd,0xff));
			TRACE();
		}
		void  KailleraPlayersList::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
		{
			TRACE();
			g.setColour (Colours::black);
			if (rowNumber >= 0 && rowNumber < players.itemsCount()) {
				KailleraPlayer k = players[rowNumber];
				switch (columnId) {
					case 1:
						g.drawText (k.name, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 2:
						{
							String text(k.ping);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
					case 3:
						g.drawText (connectionSettingTexts[k.connectionSetting], 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 4:
						{
							String text(((((k.ping  * 60) / (1000 * k.connectionSetting))+2) * k.connectionSetting) -1);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
				};
			}
			g.setColour (Colours::black.withAlpha (0.2f));
			g.fillRect (width - 1, 0, 1, height);
			TRACE();
		}



		/************************************************************
		** users list
		*******************************/

		DynamicOrderedArray<KailleraUserT, 100> users;

		// list drawing
		int   KailleraUsers::getNumRows ()
		{
			TRACE();
			return users.itemsCount();
		}
		void  KailleraUsers::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
		{
			TRACE();
			if (rowIsSelected)
				g.fillAll (Colour(0xdd,0xdd,0xff));
			TRACE();
		}
		void  KailleraUsers::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
		{
			TRACE();
			g.setColour (Colours::black);

			if (rowNumber >= 0 && rowNumber < users.itemsCount()) {
				KailleraUserT k = users[rowNumber];
				switch (columnId) {
					case 1:
						g.drawText (k.username, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 2:
						{
							String text(k.ping);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
					case 3:
						{
							String text(connectionSettingTexts[k.connectionSetting]);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
					case 4:
						{
							String text(userStatusTexts[k.status]);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
				};
			}
			g.setColour (Colours::black.withAlpha (0.2f));
			g.fillRect (width - 1, 0, 1, height);
			TRACE();
		}
		void  KailleraUsers::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e)
		{

		}

		/************************************************************
		** games list
		*******************************/

		DynamicOrderedArray<KailleraGameT, 50> games;

		// list drawing
		int   KailleraGames::getNumRows ()
		{
			return games.itemsCount();
		}
		void  KailleraGames::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
		{
			if (rowIsSelected)
				g.fillAll (Colour(0xdd,0xdd,0xff));
		}
		void  KailleraGames::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
		{
			g.setColour (Colours::black);

			if (rowNumber >= 0 && rowNumber < games.itemsCount()) {
				KailleraGameT k = games[rowNumber];
				switch (columnId) {
					case 1:
						g.drawText (k.name, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 2:
						g.drawText (k.app, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 3:
						g.drawText (k.owner, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
					case 4:
						{
							String text(gameStatusTexts[k.status]);
							g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
						}
						break;
					case 5:
						g.drawText (k.players, 2, 0, width - 4, height, Justification::centredLeft, true);
						break;
				};
			}
			g.setColour (Colours::black.withAlpha (0.2f));
			g.fillRect (width - 1, 0, 1, height);
		}

		void  KailleraGames::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e)
		{
		}


		void processCommand(KailleraListsCommand * cmd) {
			switch(cmd->command) {
				case LISTCMD_ADDGAME:
					{
						KailleraGameT * g = cmd->body.game;

						//KailleraGame game;
						//game.app = g->app;
						//game.id = g->id;
						//game.name = g->name;
						//game.owner = g->owner;
						//game.players = g->players;
						//game.status = g->status;

						//games.addItemPtr(&game);

						games.addItemPtr(g);

						delete g;
					}
					break;
				case LISTCMD_REMGAME:
					{
						KailleraGameT * game = cmd->body.game;

						for (int x = 0; x < games.itemsCount(); x++) {
							register KailleraGameT * g = games.getItemPtr(x);
							if (game->id == g->id) {
								games.removeIndex(x);
								break;
							}
						}
						delete game;
					}
					break;


				case LISTCMD_STATGAME:
					{
						KailleraGameT * game = cmd->body.game;

						for (int x = 0; x < games.itemsCount(); x++) {
							register KailleraGameT * g = games.getItemPtr(x);
							if (game->id == g->id) {
								strcpy(g->players, game->players);
								g->status = game->status;
								break;
							}
						}
						delete game;
					}
					break;

				case LISTCMD_REMALLGAMES:
					games.clearItems();
					break;

				case LISTCMD_ADDUSER:
					{
						KailleraUserT * u = cmd->body.user;
					/*	KailleraUser user;
						user.connectionSetting = u->connectionSetting;
						user.id = u->id;
						user.ping = u->ping;
						user.status = u->status;
						user.username = u->username;
						users.addItemPtr(&user);*/
						users.addItemPtr(u);
						delete u;
					}
					break;
				case LISTCMD_REMUSER:
					for (int x = 0; x < users.itemsCount(); x++) {
						register KailleraUserT * u = users.getItemPtr(x);
						if (cmd->body.user->id == u->id) {
							users.removeIndex(x);
							break;
						}
					}
					delete cmd->body.user;
					break;

				case LISTCMD_REMALLUSERS:
					users.clearItems();
					break;

			};
			delete cmd;
		}
	};
};
