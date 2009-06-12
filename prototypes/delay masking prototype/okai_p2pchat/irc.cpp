/*
* Open Kaillera p2p IRC chat channel
* Most of this work is from libircclient
* libircclient Copyright (C) 2004 Georgy Yunaev tim@krasnogorsk.ru
* LGPL license:
* This example is free, and not covered by LGPL license. There is no 
* restriction applied to their modification, redistribution, using and so on.
* You can study them, modify them, use them in your own program - either 
* completely or partially. By using it you may give me some credits in your
* program, but you don't have to.
* parts of this (some commented info) was also copied from irchelp.org
*/

#define P2P_IRC_VER "v0r1"
#define IRC_P2P_DEFCHAN "#p2ptest"

#include <time.h>

#include <windows.h>
#include "resource.h"
#include "nThread.h"
#include "nSettings.h"
#include "uihlp.h"
#include "include/libircclient.h"
#include "include/libirc_rfcnumeric.h"
#include "nprintf.h"

#include "../okaimodule.h"
#include "../okaiclient.h"
#include "dlist.h"
#include "odlist.h"

#define WM_APP_EDITRETURN		(WM_APP + 100)
#define WM_APP_EDITTAB			(WM_APP_EDITRETURN + 1)

#define WM_APP_CHATCHECK_BASE (WM_APP+1000)
#define P2PCHAT_ISLOADED	(WM_APP_CHATCHECK_BASE + 0)
#define P2PCHAT_SETSTATE	(WM_APP_CHATCHECK_BASE + 0)
#define P2PCHAT_SETSTATE_AVAILABLE 0
#define P2PCHAT_SETSTATE_CONNECTED 1
typedef struct p2pchat_msg_emuInfo{
	HWND handle;
	char AppName[128];
} p2pchat_msg_emuInfo;
typedef struct {
	char hostName[65];
	int hostPort;
	int natPort;
} p2pchat_msg_ipInfo;
typedef struct {
	int type; //type = 1  emulator info; type = 2  ip info
	union {
		p2pchat_msg_emuInfo emuInfo;
		p2pchat_msg_ipInfo ipInfo;		
	}body;
} p2pchat_msg;

p2pchat_msg_emuInfo emuinfo;

HINSTANCE hx;

bool emuSynced = false;
char playingNick[64] = {0};
// -- Service Module interface struct -----------------------------------------

#define RGB2BRG(RGB) (((RGB & 0xFF) << 16) | (RGB & 0xFF00) | ((RGB & 0xFF0000)>>16))

#define C_APP		RGB2BRG(0x700080)
#define C_CCTP		RGB2BRG(0xf00000)
#define C_HILIT		RGB2BRG(0xd06800)
#define C_INFO		RGB2BRG(0x0000a8)
#define C_JOIN		RGB2BRG(0x2b5932)
#define C_KICK		RGB2BRG(0x2b5932)
#define C_MODE		RGB2BRG(0x2b5932)
#define C_NICK		RGB2BRG(0x2b5932)
#define C_NORMAL	RGB2BRG(0x000000)
#define C_NOTICE	RGB2BRG(0x884848)
#define C_OTHER		RGB2BRG(0x700080)
#define C_PART		RGB2BRG(0x7f7f7f)
#define C_TOPIC		RGB2BRG(0x2b5932)
#define C_EVENT		RGB2BRG(0xeeeeee)

static bool connected = false;
static char nick [64] = "0746";
static char channel[64] = "#p2ptest";
static char server[64] = "irc.prison.net";
//static char channel[64] = "#srkkaillera";



// -- Misc helper functions ---------------------------------------------------



void p2p_username(char * xxx){

//	for (int x = 0; x < 500; x++) {
//
//		if (usersl[x].no != -1 && strcmp(xxx, usersl[x].usern)==0) {
//			usersl[x].p2p_user = true;
//			char xxxx[50];
//			wsprintf(xxxx, "[%s]", xxx);
////			users.FillRow(xxxx, 0, users.Find(x));
//
//			break;
//		}
//	}

}
char usern[500];
char * extract_nickname(const char * user) {
	//int len = strstr(user, "|") - user;
	//strncpy(usern, user, len);
	strcpy(usern, user);
	char * un = usern;
	while (*un && *un!= '!') *un++;
	*un = 0;
	return usern;
}

const char * find_base_username(const char * user) {
	if (*user == '@' || *user == '+') 
		return user + 1;
	else
		return user;
}

// -- Users list management ---------------------------------------------------

typedef struct{ 
	char mode;
	char usern [93];
	bool p2pUser;
	void reset(){
		mode = 0;
		usern[0] = 0;
		p2pUser = false;
	}
} userst;




// -- Various event handlers --------------------------------------------------



// -- Error text function -----------------------------------------------------

void IRCHandleError(int erro){
	/*switch (erro) {
	case LIBIRC_RFC_ERR_NOSUCHNICK:
		outp(0xff, "401 :No such nick/channel");
		break;	
	case LIBIRC_RFC_ERR_NOSUCHSERVER:
		outp(0xFF, " 402 <server name> :No such server");
		break;
	case LIBIRC_RFC_ERR_NOSUCHCHANNEL:
		outp(0xFF, " 403 <channel name> :No such channel");
		break;
	case LIBIRC_RFC_ERR_CANNOTSENDTOCHAN:
		outp(0xFF, " 404 <channel name> :Cannot send to channel");
		break;
	case LIBIRC_RFC_ERR_TOOMANYCHANNELS:
		outp(0xFF, " 405 <channel name> :You have joined too many channels");
		break;
	case LIBIRC_RFC_ERR_WASNOSUCHNICK:
		outp(0xFF, " 406 <nickname> :There was no such nickname");
		break;
	case LIBIRC_RFC_ERR_TOOMANYTARGETS:
		outp(0xFF, " 407 <target> :<error code> recipients. <abort message>");
		break;
	case LIBIRC_RFC_ERR_NOSUCHSERVICE:
		outp(0xFF, " 408 <service name> :No such service");
		break;
	case LIBIRC_RFC_ERR_NOORIGIN:
		outp(0xFF, " 409 :No origin specified");
		break;
	case LIBIRC_RFC_ERR_NORECIPIENT:
		outp(0xFF, " 411 :No recipient given (<command>)");
		break;
	case LIBIRC_RFC_ERR_NOTEXTTOSEND:
		outp(0xFF, " 412 :No text to send");
		break;
	case LIBIRC_RFC_ERR_NOTOPLEVEL:
		outp(0xFF, " 413 <mask> :No toplevel domain specified");
		break;
	case LIBIRC_RFC_ERR_WILDTOPLEVEL:
		outp(0xFF, " 414 <mask> :Wildcard in toplevel domain");
		break;
	case LIBIRC_RFC_ERR_BADMASK:
		outp(0xFF, " 415 <mask> :Bad Server/host mask");
		break;
	case LIBIRC_RFC_ERR_UNKNOWNCOMMAND:
		outp(0xFF, " 421 <command> :Unknown command");
		break;
	case LIBIRC_RFC_ERR_NOMOTD:
		outp(0xFF, " 422 :MOTD File is missing");
		break;
	case LIBIRC_RFC_ERR_NOADMININFO:
		outp(0xFF, " 423 <server> :No administrative info available");
		break;
	case LIBIRC_RFC_ERR_FILEERROR:
		outp(0xFF, " 424 :File error doing <file op> on <file>");
		break;
	case LIBIRC_RFC_ERR_NONICKNAMEGIVEN:
		outp(0xFF, " 431 :No nickname given");
		break;
	case LIBIRC_RFC_ERR_ERRONEUSNICKNAME:
		outp(0xFF, " 432 <nick> :Erroneous nickname");
		break;
	case LIBIRC_RFC_ERR_NICKNAMEINUSE:
		outp(0xFF, " 433 <nick> :Nickname is already in use");
		break;
	case LIBIRC_RFC_ERR_NICKCOLLISION:
		outp(0xFF, " 436 <nick> :Nickname collision KILL from <user>@<host>");
		break;
	case LIBIRC_RFC_ERR_UNAVAILRESOURCE:
		outp(0xFF, " 437 <nick/channel> :Nick/channel is temporarily unavailable");
		break;
	case LIBIRC_RFC_ERR_USERNOTINCHANNEL:
		outp(0xFF, " 441 <nick> <channel> :They aren't on that channel");
		break;
	case LIBIRC_RFC_ERR_NOTONCHANNEL:
		outp(0xFF, " 442 <channel> :You're not on that channel");
		break;
	case LIBIRC_RFC_ERR_USERONCHANNEL:
		outp(0xFF, " 443 <user> <channel> :is already on channel");
		break;
	case LIBIRC_RFC_ERR_NOLOGIN:
		outp(0xFF, " 444 <user> :User not logged in");
		break;
	case LIBIRC_RFC_ERR_SUMMONDISABLED:
		outp(0xFF, " 445 :SUMMON has been disabled");
		break;
	case LIBIRC_RFC_ERR_USERSDISABLED:
		outp(0xFF, " 446 :USERS has been disabled");
		break;
	case LIBIRC_RFC_ERR_NOTREGISTERED:
		outp(0xFF, " 451 :You have not registered");
		break;
	case LIBIRC_RFC_ERR_NEEDMOREPARAMS:
		outp(0xFF, " 461 <command> :Not enough parameters");
		break;
	case LIBIRC_RFC_ERR_ALREADYREGISTRED:
		outp(0xFF, " 462 :Unauthorized command (already registered)");
		break;
	case LIBIRC_RFC_ERR_NOPERMFORHOST:
		outp(0xFF, " 463 :Your host isn't among the privileged");
		break;
	case LIBIRC_RFC_ERR_PASSWDMISMATCH:
		outp(0xFF, " 464 :Password incorrect");
		break;
	case LIBIRC_RFC_ERR_YOUREBANNEDCREEP:
		outp(0xFF, " 465 :You are banned from this server");
		break;
	case LIBIRC_RFC_ERR_YOUWILLBEBANNED:
		outp(0xFF, " 466 :You will be banned from this server");
		break;
	case LIBIRC_RFC_ERR_KEYSET:
		outp(0xFF,	" 467 <channel> :Channel key already set");
		break;
	case LIBIRC_RFC_ERR_CHANNELISFULL:
		outp(0xFF, " 471 <channel> :Cannot join channel (+l)");
		break;
	case LIBIRC_RFC_ERR_UNKNOWNMODE:
		outp(0xFF, " 472 <char> :is unknown mode char to me for <channel>");
		break;
	case LIBIRC_RFC_ERR_INVITEONLYCHAN:
		outp(0xFF, " 473 <channel> :Cannot join channel (+i)");
		break;
	case LIBIRC_RFC_ERR_BANNEDFROMCHAN:
		outp(0xFF, " 474 <channel> :Cannot join channel (+b)");
		break;
	case LIBIRC_RFC_ERR_BADCHANNELKEY:
		outp(0xFF, " 475 <channel> :Cannot join channel (+k)");
		break;
	case LIBIRC_RFC_ERR_BADCHANMASK:
		outp(0xFF, " 476 <channel> :Bad Channel Mask");
		break;
	case LIBIRC_RFC_ERR_NOCHANMODES:
		outp(0xFF, " 477 <channel> :Channel doesn't support modes");
		break;
	case LIBIRC_RFC_ERR_BANLISTFULL:
		outp(0xFF, " 478 <channel> <char> :Channel list is full");
		break;
	case LIBIRC_RFC_ERR_NOPRIVILEGES:
		outp(0xFF, " 481 :Permission Denied- You're not an IRC operator");
		break;
	case LIBIRC_RFC_ERR_CHANOPRIVSNEEDED:
		outp(0xFF, " 482 <channel> :You're not channel operator");
		break;
	case LIBIRC_RFC_ERR_CANTKILLSERVER:
		outp(0xFF, " 483 :You can't kill a server!");
		break;
	case LIBIRC_RFC_ERR_RESTRICTED:
		outp(0xFF, " 484 :Your connection is restricted!");
		break;
	case LIBIRC_RFC_ERR_UNIQOPPRIVSNEEDED:
		outp(0xFF, " 485 :You're not the original channel operator");
		break;
	case LIBIRC_RFC_ERR_NOOPERHOST:
		outp(0xFF, " 491 :No O-lines for your host");
		break;
	case LIBIRC_RFC_ERR_UMODEUNKNOWNFLAG:
		outp(0xFF, " 501 :Unknown MODE flag");
		break;
	case LIBIRC_RFC_ERR_USERSDONTMATCH:
		outp(0xFF, " 502 :Cannot change mode for other users");
		break;
	};//phew*/
}
// ----------------------------------------------------------------------------

irc_session_t * s = 0;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------












///////////////////////////////////////////////////////////////////////////////
// Helper functions



// ----------------------------------------------------------------------------




// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------



#include "nprintf.h"


typedef struct irc_window {
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//global window management 2345678901234567890123456789012345678901234567890

	//78901234567890123456789012345678901234567890123456789012345678901234567890	
	static dlist<irc_window*, 32> windows;
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	static void unload(irc_window*w);
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	static irc_window* loadNew(char * target);
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	static irc_window* find(char * target);
	static irc_window* find(HWND target);

	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//78901234567890123456789012345678901234567890123456789012345678901234567890
	//Local struct related 89012345678901234567890123456789012345678901234567890

	//78901234567890123456789012345678901234567890123456789012345678901234567890
	char target[64];
	HWND wndHandle;
	HWND chatHandle;
	HWND outpHandle;
	nLVw usersList;

	COLORREF outputColor;


	odlist<userst*, 100> users;

	userst* findUser(const char * nick) {
		if (users.size()) {
			for (int x = 0; x < users.size(); x++) {
				if (strcmp(nick, users[x]->usern)==0) {
					return users[x];
				}
			}
		}
		return 0;
	}


	void addUser(const char * xxx) {
		
		const char * b = find_base_username(xxx);
		//check if Nickname is already there
		if (findUser(b))
			return;

		//outputLine("addUser %s", xxx);

		userst * user = new userst;
		if (user) {
			user->reset();
			if (*xxx == '@' || *xxx == '+')
				user->mode = *xxx;
			strcpy(user->usern, b);
			users.add(user);
		}
	}
	
	void modeUser(const char * xxx, char moa, char mo) {
		userst * user;
		if (user = findUser(xxx)) {
			if (mo == 'o') {
				if (moa == '+')
					user->mode = '@';
				else
					user->mode = 0;
			}
		}
	}

	void p2pUser(const char * xxx) {
		userst * user;
		if (user = findUser(xxx)) {
			user->p2pUser = true;
		}
	}

	void changeNick(const char * old, const char * new_) {
		userst * user;
		if (user = findUser(old)) {
			strcpy(user->usern, new_);
		}
	}

	void removeUser(const char * xxx) {
		userst * user;
		if (user = findUser(xxx)) {
			users.remove(user);
			delete user;
		}
	}

	void removeAllUsers(){
		if (users.size()){
			for (int x = 0; x < users.size(); x++){
				delete users[x];
			}
		}
		users.clear();
	}

	void sortUsers() {
		/*output("Pre sort:");
		flushUsers();*/
		if (users.size()>1) {
			bool sorted = false;
			do {
				sorted = true;
				for (int x = 0; x < users.size()-1; x++) {
					if (_stricmp(users[x]->usern, users[x+1]->usern)>0){
						userst * temp = users[x];
						users.set(users[x+1], x);
						users.set(temp, x+1);
						sorted = false;
					}
				}
			} while (sorted==false);
			sorted = false;
			do {
				sorted = true;
				for (int x = 0; x < users.size()-1; x++) {
					if (!users[x]->mode && users[x+1]->mode){
						userst * temp = users[x];
						users.set(users[x+1], x);
						users.set(temp, x+1);
						sorted = false;
					}
				}
			} while (sorted==false);
		}
		/*output("\npost sort\n");
		flushUsers();*/
	}


	void flushUsersList() {
		usersList.DeleteAllRows();

		if (users.size()) {
			for (int x = 0; x < users.size(); x++) {
				userst * user = users[x];
				char buf[64];
				*buf = 0;
				if (user->mode) {
					*buf = user->mode;
					buf[1] = 0;
				}
				
				if (user->p2pUser)
					strcat(buf, "%");

				strcat(buf, user->usern);
				usersList.AddRow(buf);
				
				//output(user->usern);

			}
		}
	}

	
	void flushUsers() {
		if (users.size()) {
			for (int x = 0; x < users.size(); x++) {
				userst * user = users[x];
				char buf[64];
				*buf = 0;
				if (user->mode) {
					*buf = user->mode;
					buf[1] = 0;
				}

				strcat(buf, user->usern);
				strcat(buf, ",");
				output(buf);
			}
		}

	}

	void getSelectedNickName(char * buf){
		int s = usersList.SelectedRow();
		if (s >= 0 && s < usersList.RowsCount()){
			usersList.CheckRow(buf, 64, 0, s);
		}
	}


	void SortFlashUsersList(){
		//output("SortFlashUsersList %i", users.size());
		sortUsers();
		flushUsersList();
	}


	void __cdecl outp(char * buffer) {
		{
			CHARFORMAT2 crf;
			crf.dwMask = CFM_COLOR;
			crf.cbSize = sizeof(crf);
			crf.crTextColor = outputColor;

			int l = GetWindowTextLength(outpHandle);
			SendMessage(outpHandle, EM_SETSEL, l, l);
			SendMessage(outpHandle, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&crf);
			SendMessage(outpHandle, EM_REPLACESEL, FALSE, (LPARAM)buffer);	
			SendMessage(outpHandle, WM_VSCROLL, SB_BOTTOM, 0);
		}
	}


	void __cdecl output(const char * arg_0, ...) {
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		outp(V8);
	}

	void __cdecl outputLine(const char * arg_0, ...) {
		char V8[1024];		
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl appMessage(const char * arg_0, ...) {
		outputColor = C_APP;
		char V8[1024];		
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl debugMessage(const char * arg_0, ...) {
		outputColor = 0xEEEEEE;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl noticeMessage(const char * arg_0, ...) {
		outputColor = C_NOTICE;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl modeMessage(const char * arg_0, ...) {
		outputColor = C_MODE;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl chatMessage(const char * arg_0, ...) {
		outputColor = C_NORMAL;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl infoMessage(const char * arg_0, ...) {
		outputColor = C_INFO;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

	void __cdecl ctcpMessage(const char * arg_0, ...) {
		outputColor = C_CCTP;
		char V8[1024];
		nvsprintf(V8, arg_0, &arg_0);
		strcat(V8, "\n");
		outp(V8);
	}

} irc_window_v;

irc_window irc_window_main = {IRC_P2P_DEFCHAN, 0};

dlist<irc_window*, 32> irc_window::windows;

irc_window* irc_window::find(char * target) {
	if (strcmp(target, irc_window_main.target)){
		if (windows.length > 0) {
			for (int x = 0; x < windows.length; x++) {
				if (strcmp(target, windows[x]->target)==0){
					return windows[x];
				}
			}
		} return 0;
	} else {
		return &irc_window_main;
	}
}

irc_window* irc_window::find(HWND target) {
	if (target != irc_window_main.wndHandle){
		if (windows.length > 0) {
			for (int x = 0; x < windows.length; x++) {
				if (target == windows[x]->wndHandle){
					return windows[x];
				}
			}
		} return 0;
	} else {
		return &irc_window_main;
	}
}

#define	IS_MAIN_WINDOW(W)\
	(W==&irc_window_main)
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#define outp(x, y, ...) irc_window_main.appMessage(y, _VA_ARGS_)

void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count, int line)
{
	char buf[5120];
	int cnt; buf[0] = '\0'; for ( cnt = 0; cnt < count; cnt++ ) {
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}
	irc_window_main.debugMessage ("%i: Event %s, origin: %s, params: %i [%s]", line, event, origin ? origin : "", cnt, buf);
}
void dump_event (irc_session_t * session, const int event, const char * origin, const char ** params, unsigned int count, int line)
{
	char buf[12];
	sprintf(buf, "%i", event);
	dump_event(session, buf, origin, params, count, line);
}
static void event_umode (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event(session, event, origin, params, count, __LINE__);
}

static void event_invite (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event(session, event, origin, params, count, __LINE__);
}


static void event_pm(const char * origin, const char * msg) {
	if (strstr(origin, "!~p2pc"P2P_IRC_VER) && strstr(msg, "!p2pc"P2P_IRC_VER)){
		irc_window_main.p2pUser(extract_nickname(origin));
		irc_window_main.SortFlashUsersList();
		irc_window_main.noticeMessage("*%s* %s", origin, msg, 0, 0);
	} else
		irc_window_main.noticeMessage("*%s* %s", origin, msg, 0, 0);


}

void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count){
	

	switch (event) {
	case LIBIRC_RFC_RPL_WELCOME:
	case LIBIRC_RFC_RPL_YOURHOST:
	case LIBIRC_RFC_RPL_CREATED:
	case LIBIRC_RFC_RPL_MYINFO:
		{
			char buf[512];
			buf[0] = 0;
			for (int cnt = 1; cnt < count; cnt++ ) {
				if (cnt!=1) strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.noticeMessage(buf);
		}
		break;
		//names inside channel
	case LIBIRC_RFC_RPL_NAMREPLY:
		{
			// params[0] = your nick
			// params[1] = channel mode ... @ for secret and * for private
			// params[2] = Channel name
			// params[3] = ([@\+]?<nick>)(" " ([@\+]?<nick>)+)

			char * nl = _strdup(params[3]);

			char * rl = nl;

			while (*nl != 0) {
				char * name = nl;
				while (*nl != ' ' && *nl != 0)
					nl++;
				if (*nl) *nl++ = 0;

				irc_window_main.addUser(name);
			}

			delete rl;
		}
		break;
	case LIBIRC_RFC_RPL_ENDOFNAMES:
		//End of names
		irc_window_main.SortFlashUsersList();
		break;


		//topic
	case LIBIRC_RFC_RPL_NOTOPIC:
		irc_window_main.appMessage("No topic is set");
		SetWindowText(irc_window_main.wndHandle, "No topic is set");
		break;

	case LIBIRC_RFC_RPL_TOPIC:
		irc_window_main.appMessage("The topic is \"%s\"", params[2]);
		SetWindowText(irc_window_main.wndHandle, params[2]);
		break;

	case 333:
		irc_window_main.appMessage("Set by %s", params[2], 0, 0);
		break;


		// Server status
	case 251://irc.Prison.NET>O0746|There are 11942 users and 49835 invisible on 58 servers
	case 252://irc.Prison.NET>O0746|424|IRC Operators online
	case 253://irc.Prison.NET>O0746|14|unknown connection(s)
	case 254://irc.Prison.NET>O0746|29027|channels formed
	case 255://irc.Prison.NET>O0746|I have 2602 clients and 1 servers
	case 250://irc.Prison.NET>O0746|Highest connection count: 3588 (3587 clients) (1720577 
		/*
		{
		char buf[512];
		buf[0] = 0;
		for (int cnt = 1; cnt < count; cnt++ ) {
		if (cnt) strcat (buf, " ");
		strcat (buf, params[cnt]);
		}
		//outp(C_NORMAL, buf);
		}//*/
		break;
	case 265://irc.Prison.NET>O0746|2602|3587|Current local users 2602, max 3587
	case 266://irc.Prison.NET>O0746|61777|72577|Current global users 61777, max 72577
		
		break;


		//server MOTD
	case LIBIRC_RFC_RPL_MOTDSTART:
	case LIBIRC_RFC_RPL_MOTD:
	case LIBIRC_RFC_RPL_ENDOFMOTD:
		////outp(C_NORMAL, params[1]);
		break;


		//channel infos n shit
	

		//nickname boolshit
	case LIBIRC_RFC_ERR_NICKCOLLISION:
	case LIBIRC_RFC_ERR_NICKNAMEINUSE:
		{
			*nick = 1 + *nick;
			//first clean up the nick
			for (int x = 0; x < strlen(nick); x++){
				if (!isalpha(nick[x]) && !isalnum(nick[x]) && nick[x]!='_') {
					for (int y = x; y < strlen(nick); y++){
						nick[y] = nick[y + 1];
					}
					x--;
				}
			}

			if (!isalpha(*nick)){
				*nick = 'n';
			}
			while (strlen(nick) < 5) {
				int l = strlen(nick);
				nick[l] = '0' + ((nick[l-1]*GetTickCount())%8);
				nick[l+1] = 0;
			}
		}

		irc_window_main.appMessage("Your nickname is in use, trying a different one: %s", nick);


		irc_cmd_nick(session, nick);
		break;
	case LIBIRC_RFC_ERR_NONICKNAMEGIVEN:
	case LIBIRC_RFC_ERR_ERRONEUSNICKNAME:
		{
			*nick = 1 + *nick;
			//first clean up the nick
			for (int x = 0; x < strlen(nick); x++){
				if (!isalpha(nick[x]) && !isalnum(nick[x]) && nick[x]!='_') {
					for (int y = x; y < strlen(nick); y++){
						nick[y] = nick[y + 1];
					}
					x--;
				}
			}

			if (!isalpha(*nick)){
				*nick = 'n';
			}
			while (strlen(nick) < 5) {
				int l = strlen(nick);
				nick[l] = '0' + ((nick[l-1]*GetTickCount())%8);
				nick[l+1] = 0;
			}
		}

		irc_window_main.appMessage("Error with specified nickname, trying a different one: %s", nick);

		irc_cmd_nick(session, nick);

		break;

		//whois
	case LIBIRC_RFC_RPL_WHOISUSER://	311
		{dump_event(session, event, origin, params, count, __LINE__);
			char buf[512];
			strcpy(buf, params[1]);
			strcat(buf, " is");
			for (int cnt = 2; cnt < count; cnt++ ) {
				strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.infoMessage(buf, 0, 0);
		}
		break;

	case LIBIRC_RFC_RPL_WHOISCHANNELS://	319
		{dump_event(session, event, origin, params, count, __LINE__);
			char buf[512];
			strcpy(buf, params[1]);
			strcat(buf, " is on");
			for (int cnt = 2; cnt < count; cnt++ ) {
				strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.infoMessage(buf, 0, 0);
		}
		break;

	case LIBIRC_RFC_RPL_WHOISSERVER://	312
		{dump_event(session, event, origin, params, count, __LINE__);
			char buf[512];
			strcpy(buf, params[1]);
			strcat(buf, " is using");
			for (int cnt = 2; cnt < count; cnt++ ) {
				strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.infoMessage(buf, 0, 0);
		}
		break;
	case LIBIRC_RFC_RPL_WHOISOPERATOR://	313
	case LIBIRC_RFC_RPL_WHOISIDLE://	317
	case LIBIRC_RFC_RPL_ENDOFWHOIS://	318
		{dump_event(session, event, origin, params, count, __LINE__);
			char buf[512];
			strcpy(buf, params[1]);
			for (int cnt = 2; cnt < count; cnt++ ) {
				strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.infoMessage(buf, 0, 0);
		}
		break;
	case 338: //RPL_WHOISACTUALLY
		{dump_event(session, event, origin, params, count, __LINE__);
			char buf[512];
			strcpy(buf, params[1]);
			for (int cnt = 2; cnt < count; cnt++ ) {
				strcat (buf, " ");
				strcat (buf, params[cnt]);
			}
			irc_window_main.infoMessage(buf, 0, 0);
		}
		break;


	default:
		dump_event(session, event, origin, params, count, __LINE__);
		break;
	};
}


// ----------------------------------------------------------------------------
static void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_window_main.noticeMessage("* Joins: %s", extract_nickname(origin), C_JOIN, 0, 0);
	char * en;
	irc_window_main.addUser(en=extract_nickname(origin));

	if (strstr(origin, "~p2pc"P2P_IRC_VER)) {		
		irc_cmd_msg(session, en, "!p2pc" P2P_IRC_VER);
		irc_window_main.p2pUser(en);
	}
	irc_window_main.SortFlashUsersList();
}
// ----------------------------------------------------------------------------
static void event_part (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_window_main.noticeMessage("* Parts: %s", extract_nickname(origin));
	irc_window_main.removeUser(extract_nickname(origin));
	irc_window_main.SortFlashUsersList();
}
// ----------------------------------------------------------------------------
static void event_notice (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	if (strcmp(event, "NOTICE")==0) {
		if (origin)
			irc_window_main.noticeMessage("-%s- %s", extract_nickname(origin), params[1]);
		else
			irc_window_main.noticeMessage(params[1]);
	} else {
		dump_event(session, event, origin, params, count, __LINE__);
	}
}
// ----------------------------------------------------------------------------
static void event_topic (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_window_main.noticeMessage("%s changes topic to '%s'", extract_nickname(origin), params[1]);
	SetWindowText(irc_window_main.wndHandle, params[1]);
}

// ----------------------------------------------------------------------------
static void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	connected = true;
	irc_window_main.removeAllUsers();
	irc_cmd_join (session, channel, 0);
}
// ----------------------------------------------------------------------------
static void event_nick (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_window_main.changeNick(extract_nickname(origin), params[0]);
	irc_window_main.noticeMessage("* %s is now known as %s", extract_nickname(origin), params[0], 0, 0);
}
// ----------------------------------------------------------------------------
static void event_quit (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	irc_window_main.noticeMessage("* Quits: %s", extract_nickname(origin), 0, 0, 0);
	irc_window_main.removeUser(extract_nickname(origin));
}
// ----------------------------------------------------------------------------
static void event_mode (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	char buff[300];
	*buff = 0;
	for (int c = 2; c < count; c++) {
		if (c != 2)
			strcat(buff, " ");
		strcat(buff, params[c]);
	}

	if (count > 2) {
		char * para = (char*)params[1];
		char moa = *para++;
		int x = 0;
		while (*para) {
			irc_window_main.modeUser(params[2+x], moa, *para);
			x++;
			para++;
		}
		irc_window_main.SortFlashUsersList();
	} //else dump_event(session, event, origin, params, count, __LINE__);

	irc_window_main.modeMessage("%s sets mode %s %s", extract_nickname(origin), params[1], buff);
}
// ----------------------------------------------------------------------------
static void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	//dump_event(session, event, origin, params, count, __LINE__);
	if (strcmp(event, "PRIVMSG")==0) {
		if (strcmp(channel, params[0])==0) {//global shit
			irc_window_main.chatMessage("<%s> %s", extract_nickname(origin), params[1], 0, 0);
		} else {
			event_pm(origin, params[1]);
		}
	} else {
		dump_event(session, event, origin, params, count, __LINE__);		
	}
}
// ----------------------------------------------------------------------------
static void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	//dump_event(session, event, origin, params, count, __LINE__);
	if (strcmp(channel, params[0])==0) {//global shit
		irc_window_main.chatMessage("<%s> %s", extract_nickname(origin), params[1], 0, 0);
	} else {
		event_pm(origin, params[1]);
	}
}
// ----------------------------------------------------------------------------
static void event_kick (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	if (count >= 3 && strcmp(params[1], params[2])) {
		irc_window_main.noticeMessage("%s was kicked by %s (%s)", params[1], extract_nickname(origin), params[2]);
	} else {
		irc_window_main.noticeMessage("%s was kicked by %s", params[1], extract_nickname(origin));
	}
	irc_window_main.removeUser(params[1]);
	irc_window_main.SortFlashUsersList();
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static void event_dcc_chat_req (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid) {
//	outp(C_EVENT, "%i: event_dcc_chat_req", __LINE__);
}
// ----------------------------------------------------------------------------
static void event_dcc_send_req (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid){
	//outp(C_EVENT, "%i: event_dcc_send_req", __LINE__);
}
// ----------------------------------------------------------------------------
static void event_ctcp_action (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event(session, event, origin, params, count, __LINE__);	
}
// ----------------------------------------------------------------------------
static void event_ctcp_req (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	if (strncmp(params[0], "VERSION", 7)==0){
		char bufrer[256];
		strcpy(bufrer, "VERSION p2pIRC " P2P_IRC_VER "/");

		irc_cmd_ctcp_reply(session, extract_nickname(origin), bufrer);
	} else if (strncmp(params[0], "PING", 4)==0){
		irc_cmd_ctcp_reply(session, extract_nickname(origin), params[0]);
	} else if (strncmp(params[0], "TIME", 4)==0){
		time_t rawtime;
		struct tm * timeinfo;
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		char bufrer[50];
		sprintf(bufrer, "TIME %s", asctime (timeinfo));
		irc_cmd_ctcp_reply(session, extract_nickname(origin), bufrer);
	} else {
		dump_event(session, event, origin, params, count, __LINE__);
	}

	irc_window_main.ctcpMessage("[%s %s]", extract_nickname(origin), params[0]);

}
// ----------------------------------------------------------------------------
static void event_unknown (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event(session, event, origin, params, count, __LINE__);
}
// ----------------------------------------------------------------------------
static void event_ctcp_rep (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count) {
	dump_event(session, event, origin, params, count, __LINE__);
}















// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
void Nick (const char * nickp){
	strcpy(nick, nickp);
	if (s) {
		irc_cmd_nick(s, nickp);
	}
}

// ----------------------------------------------------------------------------
void SendChat(char * dest, char * msg) {
	int e;
	switch (e = irc_cmd_msg(s, dest, msg)) {
	case 0:
		if (dest == channel) {
			irc_window_main.chatMessage("<%s> %s", nick, msg);
		} else {
			irc_window_main.chatMessage("-> *%s* %s", nick, msg);
		}
		break;
	default:
		IRCHandleError(e);
		break;
	};
}
// ----------------------------------------------------------------------------
void SendNotice(char * dest, char * msg) {
	int e;
	switch (e = irc_cmd_notice(s, dest, msg)) {
	case 0:
		if (dest == channel) {
			irc_window_main.noticeMessage(0, "-%s- %s", nick, msg);
		} else {
			irc_window_main.chatMessage(0, "-> -%s- %s", nick, msg);
		}
		break;
	default:
		IRCHandleError(e);
		break;
	};
}
// ----------------------------------------------------------------------------

static void SendCommand(char * cmd) {

	#define term_space(ptr, nexp) {char * _ptr = ptr; while (*_ptr != ' ' && *_ptr != 0) _ptr++; if (*_ptr) *_ptr++ = 0; nexp = _ptr;}

	char * args;
	term_space(cmd, args);

	if (!strcmp(cmd, "me")) {
		irc_cmd_me (s, channel, args);
		irc_window_main.appMessage("* %s %s", nick, args);
	} else if (!strcmp(cmd, "msg")) {
		char * mesg;
		term_space(args, mesg);
		SendChat(args, mesg);
	} else if (!strcmp(cmd, "nick")) {
		Nick(args);
	} else if (!strcmp(cmd, "notice")) {
		char * mesg;
		term_space(args, mesg);
		SendNotice(args, mesg);
	} else if (!strcmp(cmd, "part")) {
		irc_window_main.removeAllUsers();
		if (*args)
			irc_cmd_part (s, args);
		else
			irc_cmd_part (s, channel);
	} else if (!strcmp(cmd, "join")) {
		irc_window_main.removeAllUsers();
		char * key;
		term_space(args, key);
		strcpy(channel, args);
		irc_cmd_join (s, args, key);
	} else if (!strcmp(cmd, "quit")) {
		irc_cmd_quit (s, args);
	} else if (!strcmp(cmd, "mode")) {
		irc_cmd_channel_mode(s, channel, args);
	} else if (!strcmp(cmd, "kick")) {
		char * reason;
		term_space(args, reason);
		irc_cmd_kick(s, args, channel, *reason? reason:0);
	} else if (!strcmp(cmd, "whois")) {
		irc_cmd_whois(s, args);
	} else if (!strcmp(cmd, "clear")) {
		SetWindowText(irc_window_main.outpHandle, "");
		irc_window_main.appMessage("Window cleared");
	} else if (!strcmp(cmd, "ctcp")) {
		char * req;
		term_space(args, req);
		irc_cmd_ctcp_request(s, args, req);
	}

	#undef term_space

}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static irc_callbacks_t n02_irc_cbs = 
{ 
	event_connect,
	event_nick,
	event_quit,
	event_join,
	event_part,
	event_mode,
	event_umode,
	event_topic,
	event_kick,
	event_channel,
	event_privmsg,
	event_notice,
	event_invite,
	event_ctcp_req,
	event_ctcp_rep,
	event_ctcp_action,
	event_unknown,
	event_numeric,
	event_dcc_chat_req,
	event_dcc_send_req
};
// ----------------------------------------------------------------------------
class IRCThread: public nThread {

public:
	bool running;
	void run (void) {

		if (running)
			return;

		running = true;
		connected = false;

		__try {
			irc_window_main.appMessage("Creating IRC session", 0);
			if (s = irc_create_session (&n02_irc_cbs)) {
				irc_window_main.appMessage("Connecting to server %s", server);		
				if (!irc_connect (s, server, 6667, 0, nick, "p2pc"P2P_IRC_VER, 0))	{
					irc_run (s);
				} else {
					irc_window_main.appMessage("Could not connect: %s\n", irc_strerror (irc_errno(s)));
				}
				irc_window_main.appMessage("Destroying IRC session", 0);
				irc_destroy_session (s);
			} else {
				irc_window_main.appMessage("Could not create session", 0);
			}
			s = 0;
		} __except(EXCEPTION_EXECUTE_HANDLER) {
			irc_window_main.appMessage("IRC THRERAD EXCAPTION");
			running = false;
			return;
		}

		running = false;
	}
} IRCThread;














static INT_PTR CALLBACK irc_settings_window_proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	HICON icon02 = 0;
	switch (uMsg) {
	case WM_INITDIALOG:
		{

			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)lParam);
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)lParam);	
			SetDlgItemText(hDlg, TXT_NICK, nick);
			SetDlgItemText(hDlg, TXT_CHAN, channel);
			SetDlgItemText(hDlg, TXT_SERVER, server);

			return TRUE;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case IDOK:
			{
				GetDlgItemText(hDlg, TXT_NICK, nick, 11);
				GetDlgItemText(hDlg, TXT_CHAN, channel, 63);
				GetDlgItemText(hDlg, TXT_SERVER, server, 63);
				EndDialog(hDlg, 0);
			}
			break;
		case IDCANCEL:
			{
				EndDialog(hDlg, 1);
			}
			break;
		};
		break;
	case WM_CLOSE: 
		{
			EndDialog(hDlg, 1);
		}
		break;
	};
	return FALSE;

}





WNDPROC bpatch = 0;
LRESULT CALLBACK EditControlBetrayal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (uMsg==WM_KEYUP && wParam == VK_RETURN) {
		SendMessage(GetParent(hwnd), WM_APP_EDITRETURN, 0, 0);
	}
	return CallWindowProcA(bpatch, hwnd, uMsg, wParam, lParam);
}
static INT_PTR CALLBACK irc_window_proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam){
	static HICON icon02 = 0;

	static RECT iwPos = {-1};

	switch (uMsg) {
	case WM_INITDIALOG:
		{
			irc_window * w = (irc_window *) lParam;

			
			w->wndHandle = hDlg;
			w->outpHandle = GetDlgItem(hDlg, IDC_CHAT);
			w->usersList.handle = GetDlgItem(hDlg, IDC_USERS);
			w->chatHandle = GetDlgItem(hDlg, IDC_CHATINP);
			
			//set up patch for the edit control
			if (!bpatch) {
				bpatch = (WNDPROC)GetWindowLong(w->chatHandle, GWL_WNDPROC);
				SetWindowLong(w->chatHandle, GWL_WNDPROC, (long)EditControlBetrayal);
			}

			w->usersList.AddColumn("Nick", 100);
			w->usersList.FullRowSelect();

			PostMessage(hDlg, WM_SIZE, 0, 0);

			if (!icon02)
				icon02 = LoadIcon(hx, (LPCSTR)ICON02);

			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon02);
			SendMessage(hDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon02);

			if (IS_MAIN_WINDOW(w)){
				char uuu[64];
				DWORD xxx = 64;
				GetUserName(uuu, &xxx);
				nSettings::Initialize("p2pirc");
				nSettings::get_str("server", server, "irc.prison.net");
				nSettings::get_str("nick", nick, uuu);
				nSettings::get_str("channel", channel, "#channelNameGoesHere");
				nSettings::get_struct("mpos", &iwPos, sizeof(iwPos));
				nSettings::Terminate();

				if (iwPos.left != -1){
					SetWindowPos(hDlg, 0, iwPos.left, iwPos.top, iwPos.right-iwPos.left, iwPos.bottom-iwPos.top, SWP_NOZORDER);
				}
			}

			if (DialogBoxParam(hx, (LPCTSTR)DLG_SETTINGS, hDlg, irc_settings_window_proc, (LPARAM)icon02)) {
				EndDialog(hDlg, 1);
			} else {
				if (IS_MAIN_WINDOW(w)){
					IRCThread.running = false;
					IRCThread.create();

					nSettings::Initialize("p2pchat", true);
					nSettings::set_int("hwnd", (int)hDlg);
					nSettings::Terminate();
					emuSynced = false;
					*playingNick = 0;
				}
			}

			return TRUE;
		}
		break;
	case P2PCHAT_ISLOADED:
		{
			ReplyMessage(TRUE);
			irc_window_main.appMessage("p2pchat existance check");
		}
		break;
	case WM_COPYDATA:
		{
			COPYDATASTRUCT * cd = (COPYDATASTRUCT *)lParam;
			if (cd->dwData == 5687 && cd->cbData==sizeof(p2pchat_msg)) {
				p2pchat_msg * pm = (p2pchat_msg *)cd->lpData;
				if (pm->type == 1) {
					emuSynced = true;
					emuinfo = pm->body.emuInfo;
					irc_window_main.appMessage("Application data received: %s", emuinfo.AppName);
				} else {
					irc_window_main.appMessage("Unhandled data");
				}
			} else {
				irc_window_main.appMessage("Invalid resync data");
			}
		}
		break;
	case WM_NOTIFY:
		{
			irc_window * w;
			NMHDR * nParam = (NMHDR*)lParam;
			
			if (w = irc_window::find(hDlg)) {
				if (nParam->hwndFrom == w->usersList.handle){
					if (nParam->code == NM_DBLCLK) {
						char selNick[64];
						*selNick = 0;
						w->getSelectedNickName(selNick);

						if (*selNick) {
							char xxxx[1000];
							GetWindowText(w->chatHandle, xxxx, 1000);

							if (xxxx[strlen(xxxx)-1] != ' ')
								strcat(xxxx, " ");

							strcat(xxxx, selNick);
							SetWindowText(w->chatHandle, xxxx);
						}
					}
				}
			}
		}
		break;
	case WM_APP_EDITRETURN: 
		{
			char xxxx[1000];
			GetWindowText(irc_window_main.chatHandle, xxxx, 1000);
			if (strlen(xxxx) > 0) {
				if (*xxxx == '/') {
					SendCommand(xxxx + 1);
				} else {
					SendChat(channel, xxxx);
				}
				SetWindowText(irc_window_main.chatHandle, "");
			}
		}
		break;
	case WM_SIZE: //resize
		{

			irc_window * w;
			
			if (w = irc_window::find(hDlg)) {
				RECT r;
				GetClientRect(hDlg, &r);

				//SetWindowPos(hDlg, 0, r.left, r.top, r.right - r.left, r.bottom - r.top, SWP_NOZORDER);

				//convert to local coordinates i guess
				r.bottom -= r.top; r.top = 0;
				r.right -= r.left; r.left = 0;

				SetWindowPos(w->usersList.handle, 0, r.right - 120, 0, 120, r.bottom - 20, SWP_NOZORDER);
				SetWindowPos(w->outpHandle, 0, 0, 0, r.right - 121, r.bottom - 20, SWP_NOZORDER);
				SetWindowPos(GetDlgItem(hDlg, IDC_CHATINP), 0, 0, r.bottom - 19, r.right, 18, SWP_NOZORDER);
			}

		}
		break;
	case WM_CLOSE: 
		{
			irc_window * w;
			if (w = irc_window::find(hDlg)) {
				// unloading ... dont have the time to wait for it to disconnet
				if (IRCThread.running) {
					irc_cmd_quit (s, "WM_CLOSE handler");
					IRCThread.destroy();
					irc_disconnect(s);
					irc_destroy_session(s);
					IRCThread.running = false;
				}

				if (IS_MAIN_WINDOW(w)){
					GetWindowRect(hDlg, &iwPos);
					nSettings::Initialize("p2pirc");
					nSettings::set_str("server", server);
					nSettings::set_str("nick", nick);
					nSettings::set_str("channel", channel);
					nSettings::set_struct("mpos", &iwPos, sizeof(iwPos));
					nSettings::Terminate();
				}
			}

			EndDialog(hDlg, 0);
		}
		break;
	};
	return FALSE;

}





int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	hx = hInstance;

	//this is to make sure no more than 1 irc module is loaded
	HANDLE hm = CreateMutex(0, TRUE, "Global\\P2PIRC");
	//if (hm == NULL || GetLastError() == ERROR_ALREADY_EXISTS) 
	//{
	//	return 1;
	//}

	HMODULE richard = LoadLibrary("riched32.dll");
	InitCommonControlsEx(0);
	DialogBoxParam(hx, (LPCTSTR)DLG_IRC, 0, irc_window_proc, (LPARAM)&irc_window_main);
	return 0;
}