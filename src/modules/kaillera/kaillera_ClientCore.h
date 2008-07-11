#pragma once
#include "common.h"
#include "module.h"

namespace n02 {
	namespace kaillera {


		int  N02CCNV step();
		int  N02CCNV synchronizeGame(void * syncData, int len);
		void N02CCNV endGame();
		void N02CCNV sendAsyncData(const void * value, const int len, const int module);
		void N02CCNV sendSyncData(const void * value, const int len);
		int  N02CCNV recvSyncData(void * value, const int len);
		int  N02CCNV syncData(void * value, const int len);
		int  N02CCNV syncDataCheck (int);
		void N02CCNV sendChat (const char * message);


		typedef struct {
			void (N02CCNV * serverUserJoin) (const char * nick, const int ping, const unsigned short id, const char connectionSetting);
			void (N02CCNV * serverUserLeave) (const char * nick, const char * quitMessage, const unsigned short id);
			void (N02CCNV * loggedIn) ();
			void (N02CCNV * serverUserAdd) (const char * nick, const int ping, const unsigned short id, const char connectionSetting, const char status);
			void (N02CCNV * serverGameAdd) (const char * name, const unsigned int id, const char * nick, const char * app, const char * users, const char status);
			void (N02CCNV * partylineChat) (const char * userName, const char * message);
			void (N02CCNV * gameChat) (const char * userName, const char * message);
			void (N02CCNV * MOTD) (const char * source, const char * message);
			void (N02CCNV * clientLoginStatusChange) (const char * msg);
			void (N02CCNV * serverNewGame) (const char * gname, const unsigned int id, const char*app, const char*owner);
			void (N02CCNV * serverGameStatusChange) (unsigned int id, char status, int players, int maxplayers);
			void (N02CCNV * serverGameClose) (unsigned int id);
			void (N02CCNV * gameCreated) ();
			void (N02CCNV * gameClosed) ();
			void (N02CCNV * gameJoined) ();
			void (N02CCNV * gameKicked) ();
			void (N02CCNV * gamePlayerAdd) (char *name, int ping, unsigned short id, char conn);
			void (N02CCNV * gamePlayerJoined) (char * username, int ping, unsigned short uid, char connset);
			void (N02CCNV * gamePlayerLeft) (char * user, unsigned short id);
			void (N02CCNV * gamePlayerDropped) (char * user, int gdpl);
			void (N02CCNV * gameStart) (int, int);
			void (N02CCNV * gameEnded) ();
		} ClientCoreCallbacks;




		bool coreInitialize (const char * userName, const char connectionSetting, ClientCoreCallbacks * callbacks, const int localPort);
		void coreTerminte();
		bool coreConnect(const SocketAddress & server);
		void coreDisconnect(const char * quitMsg);
		void coreSendChat(const char * message);
		void coreKick(const unsigned short id);
		void coreJoin(const unsigned int id);
		void coreCreate(const char * name);
		void coreLeave();
		void coreStart();

	};
};

