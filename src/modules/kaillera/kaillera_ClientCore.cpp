#include "kaillera_ClientCore.h"
#include "kaillera_ClientMessaging.h"


namespace n02 {
	namespace kaillera {
		namespace core {
			// internal core state
			enum CoreState {
				UNINITIALIZED=0,
				INITIALIZED=1,
				DISCONNECTED=2,
				REJECTED=3,
				LOADING=4,
				LOADED=5,
				RUNNING=6,
				CONNECTING=7,
				CONNECTED=8,
				LOGGEDIN=9,
				INGAME=10
			};
			// inputs to translate states
			enum CoreStateInput {
				ACTION_INITIALIZE=0,
				ACTION_TERMINATE=1,
				ACTION_CONNECT=2,
				ACTION_DISCONNECT=3,
				ACTION_JOIN=4,
				ACTION_CREATE=5,
				ACTION_LEAVE=6,
				ACTION_START=7,
				ACTION_END=8,
				ACTION_SYNC=9,
				ACTION_IDLE=10,
				RECVED_HELLODOOD=11,
				RECVED_VER=12,
				RECVED_TOO=13,
				RECVED_ERRONEUS=14,
				RECVED_PING=15,
				RECVED_LOGNSUCC=16,
				RECVED_GAMRLEAV=17,
				RECVED_KICKED=18,
				RECVED_GAMEJOINED=19,
				RECVED_GAMEMADE=20,
				DUMMY
			};

			volatile CoreState state = UNINITIALIZED;

			// flags... since thse things dont need to be optimized, stuff them all togeather
			volatile struct {
				unsigned LEAVE_REQUESTED:1;
				unsigned CREATE_REQUESTED:1;
				unsigned JOIN_REQUESTED:1;
				unsigned USERID_REQUESTED:1;
				unsigned KICKED:1;
				unsigned GAME_OWNER: 1;
				unsigned INSIDE_STEP: 1;
			} flags;

			// callbacks for various events
			ClientCoreCallbacks callbacks;

			// user information
			struct {
				char nick[32];
				char app[128];
				char game[128];
				char connectionSetting;

				unsigned short userId;
				unsigned int gameId;
				unsigned char playerNo;
				unsigned char numPlayers;
			} userInfo;

			unsigned int lastTimeoutSent;

			ClientMessaging connection;

			void send(Instruction & instr) {
				LOG(instruction send %x, instr.type);
				connection.includeInstruction(instr);
				if (instr.type == GAMEDATA || instr.type == GAMCDATA || state == CONNECTED) {
					connection.sendMessage();
				}
			}

			void resetDisconnectTimeout() {
				if (lastTimeoutSent != 0xFFFFFFFF && GlobalTimer::getTime() - lastTimeoutSent > 55000) {
					Instruction timeout(TMOUTRST);
					send(timeout);
					lastTimeoutSent = GlobalTimer::getTime();
				}
			}

			// state transformation function
			void updateState(CoreStateInput input) {
				LOG(state=%i, state);
				TRACE();
				switch (state) {
					case UNINITIALIZED:
						if (input == ACTION_INITIALIZE) {
							state = INITIALIZED;
							lastTimeoutSent = 0xFFFFFFFF;
							return;
						}
						break;
					case INITIALIZED:
						if (input == ACTION_TERMINATE) {
							state = UNINITIALIZED;
							return;
						} else if (input == ACTION_CONNECT) {
							state = CONNECTING;
							return;
						}
						break;
					case CONNECTING:
						if (input == RECVED_VER) {
							state = REJECTED; callbacks.clientLoginStatusChange("Connection failed: Protocol version mismatch.");
							return;
						} else if (input == RECVED_TOO) {
							state = REJECTED; callbacks.clientLoginStatusChange("Connection failed: Server is full.");
							return;
						} else if (input == RECVED_ERRONEUS) {
							state = REJECTED; callbacks.clientLoginStatusChange("Connection failed: Erroneous response");
							return;
						} else if (input == RECVED_HELLODOOD) {
							state = CONNECTED; callbacks.clientLoginStatusChange("Connected, logging in");
							connection.setState(INSTRMSGS);
							// send login
							lastTimeoutSent = GlobalTimer::getTime();
							{
								Instruction login(USERLOGN);
								strcpy(login.user, userInfo.nick);
								login.writeString(userInfo.app);
								login.writeSignedInt8(userInfo.connectionSetting);
								core::send(login);
#ifdef PING_HACK
								Instruction pong (USERPONG);
								for (int x = 0; x < 4; x++)
									pong.writeSignedInt32(x);

								send(pong);
								send(pong);
								send(pong);
								send(pong);
#endif
							}
							return;
						} else if (input == ACTION_DISCONNECT) {
							state = DISCONNECTED;
						}
						break;
					case DISCONNECTED:
					case REJECTED:
						if (input == ACTION_TERMINATE) {
							state = UNINITIALIZED;
							return;
						}
						break;
					case LOADING:
						if (input == ACTION_INITIALIZE) {
							state = INITIALIZED;
							return;
						}
						break;
					case CONNECTED: // Logging in
						if (input == RECVED_PING) {
							lastTimeoutSent = GlobalTimer::getTime();
							return;
						} else if (input == RECVED_LOGNSUCC) {
							state = LOGGEDIN;
							callbacks.loggedIn();
							return;
						} else if (input == ACTION_IDLE) {
							if (lastTimeoutSent != 0xFFFFFFFF && GlobalTimer::getTime() - lastTimeoutSent > 500) {
								connection.sendMessage();
								lastTimeoutSent = GlobalTimer::getTime();
							}
							return;
						}
						break;
					case LOGGEDIN:
						if (input == ACTION_IDLE) {
							resetDisconnectTimeout();
							return;
						} else if (input == ACTION_DISCONNECT) {
							connection.sendMessage();
							state = DISCONNECTED;
							return;
						} else if (input == RECVED_GAMEJOINED) {
							callbacks.gameJoined();
							state = INGAME;
							flags.GAME_OWNER = 0;
							flags.JOIN_REQUESTED = 0;
							flags.USERID_REQUESTED = 1;
							return;
						} else if (input == RECVED_GAMEMADE) {
							callbacks.gameCreated();
							state = INGAME;
							flags.GAME_OWNER = 1;
							flags.CREATE_REQUESTED = 0;
							flags.USERID_REQUESTED = 1;
							return;
						} else if (input == ACTION_CREATE) {
							flags.CREATE_REQUESTED = 1;
							return;
						} else if (input == ACTION_JOIN) {
							flags.JOIN_REQUESTED = 1;
							return;
						}
						break;
					case INGAME:
						if (input == ACTION_IDLE) {
							resetDisconnectTimeout();
							return;
						} else if (input == ACTION_LEAVE) {
							flags.LEAVE_REQUESTED = 1;
							return;
						} else if (input == RECVED_KICKED) {
							state = LOGGEDIN;
							callbacks.gameKicked();
							callbacks.gameClosed();
							return;
						} else if (input == RECVED_GAMRLEAV) {
							if (flags.LEAVE_REQUESTED != 1)
								callbacks.gameKicked();
							flags.LEAVE_REQUESTED = 0;
							callbacks.gameClosed();
							state = LOGGEDIN;
							return;
						}
						break;
					case LOADED:
						if (input == ACTION_INITIALIZE) {
							state = INITIALIZED;
							return;
						}
						break;

					case RUNNING:
						break;
					default:
						break;
				};
				TRACE();
				LOG(erroneous state %i[%i], state, input);
				char xxx[234];
				wsprintfA(xxx, "erroneous state/input %i[%i]", state, input);
				MessageBoxA(0,xxx,0,0);
				TRACE();
			}

			void dataArrivalCallback(char * data, int len) {
				LOG(len=%i, len);
				TRACE();
				if (len > 0) {
					if (strncmp("HELLOD00D", data, 9)==0 && strlen(data) > 10) {
						TRACE();
						connection.lastAddress.setPort(0xffff & StringUtils::alphaToUint(data + 9));
						connection.setAddress(connection.lastAddress);
						updateState(RECVED_HELLODOOD);
						return;
					} else if (strcmp("VER", data)==0) {
						TRACE();
						updateState(RECVED_VER);
						return;
					} else if (strcmp("TOO", data)==0) {
						TRACE();
						updateState(RECVED_TOO);
						return;
					}
				}
				TRACE();
				updateState(RECVED_ERRONEUS);
			}

			void instructionArrivalCallback(Instruction & ki) {
				LOG(instruction arrival %x, ki.type);
				TRACE();
				switch (ki.type) {
				case USERJOIN:
					{
						TRACE();
						unsigned short id = ki.readUnsignedInt16();
						int ping = ki.readSignedInt32();
						char conn = ki.readSignedInt8();
						callbacks.serverUserJoin(ki.user, ping, id, conn);
						break;
					}
				case USERLEAV:
					{
						TRACE();
						unsigned short id = ki.readSignedInt16();
						callbacks.serverUserLeave(ki.user, ki.getCurrentStringPtr(), id);
						break;
					}
				case PARTCHAT:
					TRACE();
					callbacks.partylineChat(ki.user, ki.getCurrentStringPtr());
					break;
				case GAMECHAT:
					TRACE();
					callbacks.gameChat(ki.user, ki.getCurrentStringPtr());
					break;
				case MOTDLINE:
					TRACE();
					callbacks.MOTD(ki.user, ki.getCurrentStringPtr());
					break;
				case LONGSUCC:
					{
						TRACE();
						int usersCount = ki.readSignedInt32();
						int gamesCount = ki.readSignedInt32();
						while (usersCount-->0 && ki.getSpaceLeft() > 9) {
							char * name  = ki.getCurrentStringPtr(); ki.skipString();
							int ping = ki.readSignedInt32();
							char status = ki.readSignedInt8();
							unsigned short id = ki.readUnsignedInt16();
							char conn = ki.readSignedInt8();
							callbacks.serverUserAdd(name, ping, id, conn, status);
						}
						while (gamesCount-->0 && ki.getSpaceLeft() > 12) {
							char * gname = ki.getCurrentStringPtr(); ki.skipString();
							unsigned int id = ki.readUnsignedInt32();
							char * app = ki.getCurrentStringPtr(); ki.skipString();
							char * owner = ki.getCurrentStringPtr(); ki.skipString();
							char * users = ki.getCurrentStringPtr(); ki.skipString();
							char status = ki.readSignedInt8();
							callbacks.serverGameAdd(gname, id, owner, app, users, status);
						}
						updateState(RECVED_LOGNSUCC);
						break;
					}
				case SERVPING:
					{
						TRACE();
#ifndef PING_HACK
						Instruction pong (USERPONG);
						for (int x = 0; x < 4; x++)
							pong.writeSignedInt32(x);
						send(pong);
#endif
						break;
					}
				case LOGNSTAT:
					{
						TRACE();
						ki.readUnsignedInt16();
						callbacks.clientLoginStatusChange(ki.getCurrentStringPtr());
						break;
					}
				case GAMEMAKE:
					{
						TRACE();
						char * gname = ki.getCurrentStringPtr(); ki.skipString();
						char * app = ki.getCurrentStringPtr(); ki.skipString();
						unsigned int id = ki.readSignedInt32();
						callbacks.serverNewGame(gname, id, app, ki.user);

						if (flags.CREATE_REQUESTED == 1
							&& strcmp(userInfo.app, app)==0
							&& strcmp(ki.user, userInfo.nick)==0
							&& strcmp(gname, userInfo.game)==0)
						{
							userInfo.gameId = id;
							updateState(RECVED_GAMEMADE);
						}
						break;
					}
				case GAMESTAT:
					{
						TRACE();
						unsigned int id = ki.readUnsignedInt32();
						char status = ki.readSignedInt8();
						int players = ki.readSignedInt8();
						int maxplayers = ki.readSignedInt8();
						TRACE();
						callbacks.serverGameStatusChange(id, status, players, maxplayers);
						break;
					}
				case GAMESHUT:
					{
						TRACE();
						unsigned int id = ki.readUnsignedInt32();
						callbacks.serverGameClose(id);
						break;
					}
				case GAMRJOIN:
					{
						TRACE();
						ki.readUnsignedInt32();
						char * username = ki.getCurrentStringPtr(); ki.skipString();
						int ping = ki.readUnsignedInt32();					
						unsigned short uid = ki.readUnsignedInt16();
						char connset = ki.readSignedInt8();
						callbacks.gamePlayerJoined(username, ping, uid, connset);

						if (flags.USERID_REQUESTED==1
							&& userInfo.connectionSetting == connset
							&& strcmp(userInfo.nick, username)==0) {
							userInfo.userId = uid;
							flags.USERID_REQUESTED = 0;
						}
						if (state != INGAME) {
							coreKick(uid);
						}
						break;
					}
				case GAMRLEAV:
					{
						TRACE();
						unsigned short id = ki.readSignedInt16();
						callbacks.gamePlayerLeft(ki.user, id);
						if (id==userInfo.userId) {
							updateState(RECVED_GAMRLEAV);
						}
						break;
					}
				case GAMRSLST:
					{
						TRACE();
						updateState(RECVED_GAMEJOINED);
						int numplayers = ki.readUnsignedInt32();
						for (int x=0; x < numplayers; x++) {
							char name[32];
							ki.readString(name, 32);
							int ping = ki.readUnsignedInt32();
							unsigned short id = ki.readUnsignedInt16();
							char conn = ki.readSignedInt8();
							callbacks.gamePlayerAdd(name, ping, id, conn);
						}
						break;
					}
//				case GAMEDATA:
//					{
//						unsigned short length = ki.readUnsignedInt16();
//						unsigned short reqLen = 0xFFFF & (totalInputLength * connectionSetting);
//						void * newBuffer;
//						if (inCache.itemsCount() == 256) {
//							newBuffer = inCache[0];
//							inCache.removeIndex(0);
//						} else {
//							newBuffer = malloc(reqLen);
//						}
//						inCache.addItem(newBuffer);
//						if (length != reqLen) {
//							int srcDividor = length / connectionSetting;
//							memset(newBuffer, 0, reqLen);
//							for (int x = 0; x < connectionSetting; x++) {
//								memcpy(((char*)newBuffer)+(x * inputLength), ki.getCurrentStringPtr() + (x * srcDividor), min(srcDividor, inputLength));
//							}
//						} else {
//							memcpy(newBuffer, ki.getCurrentStringPtr(), reqLen);
//						}
//						inBuffer.addData(newBuffer, reqLen);
//					}
//					break;
//				case GAMCDATA:
//					{
//						unsigned char index = ki.readUnsignedInt8();
//						if (index < inCache.itemsCount()) {
//							inBuffer.addData(inCache[index], totalInputLength * connectionSetting);
//						}
//					}
//					break;
//				case GAMEBEGN:
//					{
//						LOG(Starting game);
//
//						unsigned short cDelay = ki.readSignedInt16();
//						delay = (cDelay + 1) * connectionSetting;
//						frameNo = 0;
//						LOG(Delay is %i frames, delay-1);
//						gamePlayerNo = ki.readSignedInt8();
//						gameNumPlayers = ki.readSignedInt8();
//						callbacks.gameStart(gamePlayerNo, gameNumPlayers);
//					}
//					break;
//				case GAMRDROP:
//					{
//						int gdpl = ki.readSignedInt8();
//						callbacks.gamePlayerDropped(ki.user, gdpl);
//						if (gamePlayerNo == gdpl) {
//							stopGame();
//							callbacks.gameEnded();
//						}
//					}
//					break;
//				case GAMRSRDY:
//					{
//						LOG(All players are ready);
//						runGame();
//					}
//					break;
				default:
					TRACE();
					LOG("Unhandled instruction %i", ki.type);
//					ki->log();					
//					break;
				};
				TRACE();
			}

			

			void step()
			{
				TRACE();
				flags.INSIDE_STEP = 1;
				connection.step(100);
				TRACE();
				if (state > RUNNING) {
					updateState(ACTION_IDLE);
					TRACE();if (connection.hasNewInstructions()) {
						TRACE();connection.sendMessage();
					}
				}
				TRACE();
				flags.INSIDE_STEP = 0;
			}
		};

		using namespace core;

		// setting core parameters
		bool coreInitialize (const char * userName, const char connectionSetting, ClientCoreCallbacks * callbacks, const int localPort)
		{
			memset(&userInfo, 0, sizeof(userInfo));

			strcpy(userInfo.nick, userName);

			// encode appname and appversion to kaillera format
			strcpy(userInfo.app, client->app.name);
			strcat(userInfo.app, " ");
			strcat(userInfo.app, client->app.version);
			StringUtils::trim(userInfo.app);

			userInfo.connectionSetting = connectionSetting;

			core::callbacks = *callbacks;

			flags.LEAVE_REQUESTED=0;
			flags.CREATE_REQUESTED=0;
			flags.JOIN_REQUESTED=0;
			flags.KICKED=0;
			flags.GAME_OWNER=0;
			flags.INSIDE_STEP=0;
			flags.USERID_REQUESTED=0;

			if (connection.initialize(localPort)) {

				updateState(ACTION_INITIALIZE);
				return true;
			} else {

				return false;
			}
		}

		// cleanup
		void coreTerminte()
		{
			connection.terminate();
			updateState(ACTION_TERMINATE);
		}

		// connect
		bool coreConnect(const SocketAddress & server)
		{
			core::connection.setAddress(server);

			core::connection.sendData("HELLO0.83", 10);

			updateState(ACTION_CONNECT);

			return true;
		}

		// disconnect
		void coreDisconnect(const char * quitMsg)
		{
			Instruction leave(USERLEAV);
			leave.writeSignedInt16(-1);
			leave.writeString(quitMsg);
			send(leave);
			updateState(ACTION_DISCONNECT);
		}

		// send chat message
		void coreSendChat(const char * message)
		{
			Instruction chat(PARTCHAT);
			chat.writeString(message);
			send(chat);
		}

		// kick user
		void coreKick(const unsigned short id)
		{
			if (flags.GAME_OWNER == 1) {
				Instruction kick(GAMRKICK);
				kick.writeUnsignedInt16(id);
				send(kick);
			}
		}

		// join game
		void coreJoin(const unsigned int id)
		{
			updateState(ACTION_JOIN);
			userInfo.gameId = id;
			Instruction join(GAMRJOIN);
			join.writeUnsignedInt32(id);
			join.writeSignedInt8(0);
			join.writeSignedInt32(0);
			join.writeSignedInt16(-1);
			join.writeSignedInt8(userInfo.connectionSetting);
			send(join);
		}

		// create game
		void coreCreate(const char * name)
		{
			if (name != 0 && strlen(name) > 0) {
				strcpy(userInfo.game, name);
				updateState(ACTION_CREATE);
				Instruction newg(GAMEMAKE);
				newg.writeString(name);
				newg.writeSignedInt8(0);
				newg.writeSignedInt32(-1);
				send(newg);
			}
		}

		// leave game
		void coreLeave()
		{

			Instruction leave(GAMRLEAV);
			leave.writeSignedInt16(-1);
			send(leave);

			updateState(ACTION_LEAVE);
		}

		// start game
		void coreStart()
		{
			/*Instruction begin(GAMEBEGN);
			begin.writeSignedInt32(-1);
			sendInclude(&begin);*/


			updateState(ACTION_START);
		}

		// step filter
		int  N02CCNV step()
		{
			if (state > RUNNING) {
				core::step();
				return 1;
			}
			return 0;
		}

		// game load complete
		int  N02CCNV synchronizeGame(void * syncData, int len)
		{
			return 0;
		}

		// end game
		void N02CCNV endGame()
		{

			updateState(ACTION_END);
		}

		// send asynchronous data
		void N02CCNV sendAsyncData(const void * value, const int len, const int mode)
		{

		}

		// send input data
		void N02CCNV sendSyncData(const void * value, const int len)
		{

		}

		// recv input data
		int  N02CCNV recvSyncData(void * value, const int len)
		{
			return 0;
		}

		// do both send and recv
		int  N02CCNV syncData(void * value, const int len)
		{
			sendSyncData(value, len);
			return recvSyncData(value, len);
		}

		// check for new data, return no of frames of data avilable
		int  N02CCNV syncDataCheck (int)
		{
			return 0;
		}

		// send in game chat message
		void N02CCNV sendChat (const char * message)
		{
			Instruction gameChat(GAMECHAT);
			gameChat.writeString(message);
			send(gameChat);
		}

	};
};

