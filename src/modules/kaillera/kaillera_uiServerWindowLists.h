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
#define LISTCMD_ADDPLAYER 8
#define LISTCMD_REMPLAYER 9
#define LISTCMD_REMALLPLAYERS 10
#define LISTCMD_LISTSLIMIT 20
#define LISTCMD_APPEND 50
#define LISTCMD_SETTITLE 51


		typedef struct {
			int command;
			union  {
				KailleraPlayerT * player;
				KailleraGameT * game;
				KailleraUserT * user;
				void * ptr;
			} body;
		} KailleraListsCommand;


		void processCommand(KailleraListsCommand * cmd);
		void sendCommand(KailleraListsCommand * cmd);

		void uiGameJoinCallback(unsigned int id);

	};
};

