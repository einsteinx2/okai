/******************************************************************************
>         .d8888b.   .d8888b.                                                 <
>        d88P  Y88b d88P  Y88b                                                <
>        888    888        888                                                <
88888b.  888    888      .d88P                                                <
888 "88b 888    888  .od888P"                                                 <
888  888 888    888 d88P"                                                     <
888  888 Y88b  d88P 888"                                                      <
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

#include "kaillera_ClientCore.h"
#include "kaillera_ClientMessaging.h"
#include "recorder.h"
// This flag is crucial for synchronization in some applications
#define MASK_INITIAL_FRAMES 60


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
                ACTION_SYNC_IDLE=23,
                ACTION_RUNNING_IDLE=24,
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
                RECVED_BEGIN=21,
                RECVED_ALLREADY=22,
                DUMMY=25
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
            } userInfo;

            unsigned int lastTimeoutSent;

            ClientMessaging connection;

            struct {
                int frame;
                int delay;
                int inputLength;
                int totalInputLength;
                unsigned char playerNo;
                unsigned char numPlayers;
                DataQueue outBuffer;
                DataQueue inBuffer;
                StaticOrderedArray<void*, 256> inCache;
#ifndef DISABLE_OUTGOING_CACHE
                StaticOrderedArray<void*, 256> outCache;
#endif
                unsigned char defaultInput[256];
                unsigned int lastDataSentTime;
                int syncDataTransfer;
            } gameInfo;

#define ASYNCDATA_GAMESYNC 0

            void send(Instruction & instr) {
                //LOG(instruction send %x, instr.type);
                connection.includeInstruction(instr);
                if (instr.type == GAMEDATA || instr.type == GAMCDATA || state == CONNECTED || state == LOADING || state == LOADED) {
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


            void resetGameplay() {
                // caches
#ifndef DISABLE_OUTGOING_CACHE
                for (int x = 0; x < gameInfo.outCache.itemsCount(); x++) {
                    delete gameInfo.outCache[x];
                }
                gameInfo.outCache.clearItems();
#endif
                for (int x = 0; x < gameInfo.inCache.itemsCount(); x++) {
                    delete gameInfo.inCache[x];
                }
                gameInfo.inCache.clearItems();
                // buffers
                gameInfo.outBuffer.reset();
                gameInfo.inBuffer.reset();
                // misc
                gameInfo.delay = 0;
                gameInfo.frame = 0;
                gameInfo.inputLength = 0;
            }



            // state transformation function
            void updateState(CoreStateInput input) {
                //LOG(state=%i[%i], state, input);
                TRACE();
                switch (state) {
                    case UNINITIALIZED:
                        TRACE();
                        if (input == ACTION_INITIALIZE) {
                            state = INITIALIZED;
                            lastTimeoutSent = 0xFFFFFFFF;
                            return;
                        }
                        break;
                    case INITIALIZED:
                        TRACE();
                        if (input == ACTION_TERMINATE) {
                            state = UNINITIALIZED;
                            return;
                        } else if (input == ACTION_CONNECT) {
                            state = CONNECTING;
                            return;
                        }
                        break;
                    case CONNECTING:
                        TRACE();
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
                            }
                            return;
                        } else if (input == ACTION_DISCONNECT) {
                            state = DISCONNECTED;
                            return;
                        } else if (input == ACTION_IDLE) {
                            return;
                        }
                        break;
                    case DISCONNECTED:
                    case REJECTED:
                        TRACE();
                        if (input == ACTION_TERMINATE) {
                            state = UNINITIALIZED;
                            return;
                        } else if (input == ACTION_IDLE) {
                            LOGS(THIS ONE);
                            return;
                        }
                        break;

                    case CONNECTED: // Logging in
                        TRACE();
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
                        } else if (input == ACTION_DISCONNECT) {
                            state = DISCONNECTED;
                            return;
                        } 
                        break;
                    case LOGGEDIN:
                        TRACE();
                        if (input == ACTION_IDLE) {
                            resetDisconnectTimeout();
                            return;
                        } else if (input == ACTION_DISCONNECT) {
                            connection.sendMessage();
                            resetGameplay();
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
                        TRACE();
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
                        } else if (input == ACTION_START) {
                            Instruction begin(GAMEBEGN);
                            begin.writeSignedInt32(-1);
                            send(begin);
                            return;
                        } else if (input == RECVED_BEGIN) {
                            callbacks.gameStart(gameInfo.playerNo, gameInfo.numPlayers);
                            state = LOADING;
                            return;
                        }
                        break;
                    case LOADING:
                        TRACE();
                        if (input == ACTION_SYNC) {
                            Instruction ready(GAMRSRDY);
                            send(ready);
                            state = LOADED;
                            lastTimeoutSent = gameInfo.lastDataSentTime = GlobalTimer::getTime();
                            return;
                        } else if (input == ACTION_END) {
                            Instruction drop(GAMRDROP);
                            drop.writeSignedInt8(0);
                            send(drop);
                            callbacks.gameEnded();
							modHelper.endGame();
                            state = INGAME;
                            return;
                        } else if (input == RECVED_GAMRLEAV) {
                            callbacks.gameEnded();
                            if (flags.LEAVE_REQUESTED != 1)
                                callbacks.gameKicked();
                            flags.LEAVE_REQUESTED = 0;
                            callbacks.gameClosed();
                            state = LOGGEDIN;
                            return;
                        } else if (input == ACTION_LEAVE) {
                            flags.LEAVE_REQUESTED = 1;
                            return;
                        } else if (input == ACTION_SYNC_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (time - gameInfo.lastDataSentTime > 5000) {
                                gameInfo.lastDataSentTime = time;
                                if (time - lastTimeoutSent > 12000) {
                                    Instruction drop(GAMRDROP);
                                    drop.writeSignedInt8(0);
                                    send(drop);
                                    callbacks.gameEnded();
                                    state = INGAME;
                                } else {
                                    connection.sendMessage();
                                }
                            }
                            return;
                        }
                        break;
                    case LOADED:
                        TRACE();
                        if (input == ACTION_END) {
                            Instruction drop(GAMRDROP);
                            drop.writeSignedInt8(0);
                            send(drop);
                            callbacks.gameEnded();
							modHelper.endGame();
                            state = INGAME;
                            return;
                        } else if (input == RECVED_ALLREADY) {
                            state = RUNNING;
                            return;
                        } else if (input == ACTION_SYNC_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (time - gameInfo.lastDataSentTime > 5000) {
                                gameInfo.lastDataSentTime = time;
                                if (time - lastTimeoutSent > 12000) {
                                    Instruction drop(GAMRDROP);
                                    drop.writeSignedInt8(0);
                                    send(drop);
                                    callbacks.gameEnded();
                                    state = INGAME;
                                } else {
                                    connection.sendMessage();
                                }
                            }
                            return;
                        } else if (input == RECVED_GAMRLEAV) {
                            callbacks.gameEnded();
                            if (flags.LEAVE_REQUESTED != 1)
                                callbacks.gameKicked();
                            flags.LEAVE_REQUESTED = 0;
                            callbacks.gameClosed();
                            state = LOGGEDIN;
                            return;
                        } else if (input == ACTION_LEAVE) {
                            flags.LEAVE_REQUESTED = 1;
                            return;
                        }
                        break;
                    case RUNNING:
                        TRACE();
                        if (input == ACTION_END) {
                            Instruction drop(GAMRDROP);
                            drop.writeSignedInt8(0);
                            send(drop);
                            callbacks.gameEnded();
							modHelper.endGame();
                            state = INGAME;
                            return;
                        } else if (input == ACTION_RUNNING_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (gameInfo.inBuffer.length() < gameInfo.totalInputLength) {
                                if((time - gameInfo.lastDataSentTime) > 100) {
                                    gameInfo.lastDataSentTime = time;
                                    if (time - lastTimeoutSent > 10000) {
                                        Instruction drop(GAMRDROP);
                                        drop.writeSignedInt8(0);
                                        send(drop);
                                        callbacks.gameEnded();
                                        state = INGAME;
                                    } else {
                                        connection.sendMessage(((gameInfo.delay + 6)/userInfo.connectionSetting) + 1);
                                    }
                                }
                            } else {
                                lastTimeoutSent = time;
                            }
                            return;
                        } else if (input == RECVED_GAMRLEAV) {
                            callbacks.gameEnded();
                            if (flags.LEAVE_REQUESTED != 1)
                                callbacks.gameKicked();
                            flags.LEAVE_REQUESTED = 0;
                            callbacks.gameClosed();
                            state = LOGGEDIN;
                            return;
                        } else if (input == ACTION_LEAVE) {
                            flags.LEAVE_REQUESTED = 1;
                            return;
                        }
                        break;
                    default:
                        break;
                };
                TRACE();
                LOG(erroneous state %i[%i], state, input);
                char xxx[234];
                sprintf(xxx, "erroneous state/input %i[%i]", state, input);
                callbacks.clientLoginStatusChange(xxx);
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
                //LOG(instruction arrival %x, ki.type);
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
                    {
                        TRACE();
                        if (ki.getSpaceLeft() > 4) {
                            if (ki.readUnsignedInt16() == 0xffff) {
                                char buffer[1024];
                                ki.readString(buffer, 1024);
                                LOG("Received %s", buffer);
                                unsigned char decodedBuffer[256];
                                int len = StringUtils::base64decode(decodedBuffer, buffer);
                                unsigned char type = (decodedBuffer[0] & 0xf0) >> 4;
                                unsigned char playerIndex = decodedBuffer[0] & 0x0f;

                                if (type == ASYNCDATA_GAMESYNC) {
                                    LOGS(game sync data arrival);

                                    memcpy(gameInfo.defaultInput + (playerIndex * gameInfo.inputLength), &decodedBuffer[1], len -1);
                                    gameInfo.syncDataTransfer--;
                                }

                                break;
                            }
                            ki.seek(-2);
                        }
                        callbacks.gameChat(ki.user, ki.getCurrentStringPtr());
                    }
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
                        Instruction pong (USERPONG);
                        for (int x = 0; x < 4; x++)
                            pong.writeSignedInt32(x);
                        send(pong);
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
                case GAMEDATA:
                    {
                        register unsigned short length = ki.readUnsignedInt16();
                        register unsigned short reqLen = 0xFFFF & (gameInfo.totalInputLength * userInfo.connectionSetting);
                        register void * newBuffer;
                        if (gameInfo.inCache.itemsCount() == 256) {
                            newBuffer = gameInfo.inCache[0];
                            gameInfo.inCache.removeIndex(0);
                        } else {
                            newBuffer = new char[reqLen];
                        }
                        gameInfo.inCache.addItem(newBuffer);
                        //  data size adjustments
                        if (length != reqLen) {
                            register int srcDividor = length / userInfo.connectionSetting;
                            memcpy(newBuffer, gameInfo.defaultInput, reqLen);
                            for (int x = 0; x < userInfo.connectionSetting; x++) {
                                memcpy(((char*)newBuffer)+(x * gameInfo.inputLength), ki.getCurrentStringPtr() + (x * srcDividor), common_min(srcDividor, gameInfo.inputLength));
                            }
                        } else {
                            memcpy(newBuffer, ki.getCurrentStringPtr(), reqLen);
                        }
                        gameInfo.inBuffer.push(newBuffer, reqLen);
                    }
                    break;
                case GAMCDATA:
                    {
                        register unsigned char index = ki.readUnsignedInt8();
                        if (index < gameInfo.inCache.itemsCount()) {
                            gameInfo.inBuffer.push(gameInfo.inCache[index], gameInfo.totalInputLength * userInfo.connectionSetting);
                        }
                    }
                    break;
                case GAMEBEGN:
                    {
                        resetGameplay();
                        gameInfo.delay = (ki.readSignedInt16() + 1) * userInfo.connectionSetting;
                        gameInfo.frame = 0;
                        LOG(Delay is %i frames, gameInfo.delay-1);
                        gameInfo.playerNo = ki.readSignedInt8();
                        gameInfo.numPlayers  = ki.readSignedInt8();
                        updateState(RECVED_BEGIN);
                    }
                    break;
                case GAMRDROP:
                    {
                        //						int gdpl = ki.readSignedInt8();
                        //						callbacks.gamePlayerDropped(ki.user, gdpl);
                        //						if (gamePlayerNo == gdpl) {
                        //							stopGame();
                        //							callbacks.gameEnded();
                        //						}
                    }
                    break;
                case GAMRSRDY:
                    {
                        updateState(RECVED_ALLREADY);
                    }
                    break;
                default:
                    TRACE();
                    LOG("Unhandled instruction %i", ki.type);
                    ki.log();
                    break;
                };
                TRACE();
            }


            void stepSync()
            {
                TRACE();
                updateState(ACTION_SYNC_IDLE);
                connection.step(100);
                TRACE();
            }

            void stepRunning()
            {
                TRACE();
                connection.step(1);
                updateState(ACTION_RUNNING_IDLE);
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
            if (state != CONNECTING) {
                Instruction leave(USERLEAV);
                leave.writeSignedInt16(-1);
                leave.writeString(quitMsg);
                send(leave);
            }

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

		void coreLeaveForce()
		{
			coreLeave();
			updateState(RECVED_GAMRLEAV);
		}

        // start game
        void coreStart()
        {
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
        int  N02CCNV synchronizeGame(void * data, int len)
        {
            if (state == LOADING) {
                if (data != 0 && len > 0) {
                    gameInfo.syncDataTransfer = common_min(gameInfo.numPlayers, 8);
                    if (gameInfo.playerNo <= 8) {
                        // send data

                        gameInfo.inputLength = len;

                        Instruction cedata(GAMECHAT);
                        cedata.writeSignedInt16(-1); // starting sequence

                        // encode rest
                        unsigned char buffer[256];
                        buffer[0] = (ASYNCDATA_GAMESYNC << 4) | (gameInfo.playerNo - 1);
                        memcpy(buffer+1, data, len);
                        char encBuffer[1024];
                        StringUtils::base64encode(encBuffer, buffer, len + 1);

                        cedata.writeString(encBuffer);
                        send(cedata);

                        LOGS(Waiting for game sync data);

                        gameInfo.lastDataSentTime = lastTimeoutSent = GlobalTimer::getTime();

                        while (gameInfo.syncDataTransfer > 0 && state == LOADING) {
                            stepSync();						
                        }

                        LOG(sync %i %i, gameInfo.syncDataTransfer,state);

                        LOG("Sending %s", encBuffer);

                    }
                }

                updateState(ACTION_SYNC);

                while (state == LOADED)
                    stepSync();

                if (state == RUNNING) {
                    if (data != 0 && len != 0) {
                        int l = gameInfo.inputLength * gameInfo.numPlayers;
                        gameInfo.inputLength = 0;
                        memcpy(data, gameInfo.defaultInput, l);
                        return l;
                    }
                    return 0;
                }
            }
            return -1;
        }

        // end game
        void N02CCNV endGame()
        {

            updateState(ACTION_END);
        }

        // send asynchronous data
        void N02CCNV sendAsyncData(const void * /*value*/, const int /*len*/, const int /*mode*/)
        {
            LOGS(Asynchronous data transfer in this module is not supported yet);
        }

        // send input data
        void N02CCNV sendSyncData(const void * value, const int len)
        {
            TRACE();
            if (state == RUNNING) {

                // set up default input on the first frame
                if (gameInfo.frame ==0) {
                    gameInfo.inputLength=len;
                    gameInfo.totalInputLength = gameInfo.numPlayers * len;
                    if (client->gameplay.defaultInput != 0) {
                        for (int x = 0; x < sizeof(gameInfo.defaultInput); x+=len) {
                            memcpy(gameInfo.defaultInput + x, client->gameplay.defaultInput, common_min(len, (int)sizeof(gameInfo.defaultInput)-x));
                        }
                    } else {
                        for (int x = 0; x < sizeof(gameInfo.defaultInput); x+=len) {
                            memcpy(gameInfo.defaultInput + x, value, common_min(len, (int)sizeof(gameInfo.defaultInput)-x));
                        }
                    }
					recorder.setDefaultInput(gameInfo.defaultInput, gameInfo.inputLength);
                }

#ifdef MASK_INITIAL_FRAMES
                register int totalBufferedLen;
                if (gameInfo.frame + gameInfo.delay <= MASK_INITIAL_FRAMES) {
                    totalBufferedLen = gameInfo.outBuffer.push(gameInfo.defaultInput, len);
                } else {
                    totalBufferedLen = gameInfo.outBuffer.push(value, len);
                }
#else		
                register int totalBufferedLen = gameInfo.outBuffer.push(value, len);
#endif
                register int requiredBufferLen = len * userInfo.connectionSetting;

                if (totalBufferedLen >= requiredBufferLen) {
                    // see if it was send within last 256 data packets
#ifndef DISABLE_OUTGOING_CACHE
                    for (int x = 0; x < gameInfo.outCache.itemsCount(); x++) {
                        if (memcmp(gameInfo.outCache[x], gameInfo.outBuffer.front(), requiredBufferLen)==0) {
                            Instruction data(GAMCDATA);
                            data.writeUnsignedInt8(x & 0xFF);
                            send(data);
                            gameInfo.outBuffer.pop(requiredBufferLen);
                            //LOG(Sent index %i, x);
                            return;
                        }
                    }
                    // make sure we have space for a new packet
                    if (gameInfo.outCache.itemsCount() == 256) {
                        void * newBuffer = gameInfo.outCache[0];
                        gameInfo.outCache.removeIndex(0);
                        memcpy(newBuffer, gameInfo.outBuffer.front(), requiredBufferLen);
                        gameInfo.outCache.addItem(newBuffer);
                    } else {
                        void * newBuffer = new unsigned char[requiredBufferLen];
                        memcpy(newBuffer, gameInfo.outBuffer.front(), requiredBufferLen);
                        gameInfo.outCache.addItem(newBuffer);
                    }
#endif
                    // if not, send a full packet
                    Instruction data(GAMEDATA);
                    data.writeUnsignedInt16((unsigned short)requiredBufferLen);
                    data.writeBytes(gameInfo.outBuffer.front(), requiredBufferLen);
                    send(data);
                    gameInfo.outBuffer.pop(requiredBufferLen);
                }
            } else {
                LOGS(erroneous action);
            }
        }

        // recv input data
        int  N02CCNV recvSyncData(void * value, const int /*len*/)
        {
            if (state==RUNNING) {
                if (++gameInfo.frame >= gameInfo.delay) {
                    while (gameInfo.inBuffer.length() < gameInfo.totalInputLength && state == RUNNING) {
                        stepRunning();
                    }

#ifdef MASK_INITIAL_FRAMES
                    if (gameInfo.frame <= MASK_INITIAL_FRAMES) {
                        memcpy(value, gameInfo.defaultInput, gameInfo.totalInputLength);
                        gameInfo.inBuffer.pop(gameInfo.totalInputLength);
                        return gameInfo.totalInputLength;
                    }
#endif
					if (state == RUNNING)
						return gameInfo.inBuffer.pop(value, gameInfo.totalInputLength);
                } else {
#ifdef MASK_INITIAL_FRAMES
                    memcpy(value, gameInfo.defaultInput, gameInfo.totalInputLength);
#endif
                    return 0;
                }
			}
			LOGS(Ending it here);
			//updateState(ACTION_END);
			//if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) == 0)
			modHelper.endGame();
			return -1;
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

