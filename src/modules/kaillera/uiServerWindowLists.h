#pragma once

#include "common.h"

namespace n02 {

	namespace kaillera {

		typedef struct {
			unsigned short id;
			char name[32];
			int ping;
			char connectionSetting;
		} KailleraPlayerT;

		typedef struct {
			unsigned short id;
			char username[32];
			int ping;
			char connectionSetting; // 1 == LAN; 6 == LOW
			char status; // 0 = connecting? 1 = idle ; 2 = playing
		} KailleraUserT;

		typedef struct {
			unsigned int id;
			char name[128];
			char app[128];
			char owner[32];
			char status;
			char players[20];
		} KailleraGameT;


#define LISTCMD_ADDUSER 1
#define LISTCMD_REMUSER 2
#define LISTCMD_REMALLUSERS 3
#define LISTCMD_ADDGAME 4
#define LISTCMD_REMGAME 5
#define LISTCMD_STATGAME 6
#define LISTCMD_REMALLGAMES 7


		typedef struct {
			int command;
			union  {
				KailleraPlayerT * player;
				KailleraGameT * game;
				KailleraUserT * user;
			} body;
		} KailleraListsCommand;


		void processCommand(KailleraListsCommand * cmd);
		void sendCommand(KailleraListsCommand * cmd);

	};
};

