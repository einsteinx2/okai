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
#include "p2p_Core.h"
#include "p2p_Messaging.h"
#include "n02version.h"
#include "recorder.h"
#define REJECT_VER 1
#define REJECT_APP 3
#define REJECT_APPVER 4
namespace n02 {
    namespace p2p {
        namespace core {

            // internal core state
            enum CoreState {
                UNINITIALIZED=0,
                INITIALIZED=1,
                PUNCHTHROUGH=2,
                CONNECTING=3,
                CONNECTED=4,
                LOADING=5,
                LOADED=6,
                ALLLOADED=7,
                RUNNING=8
            };

            // inputs to translate states
            enum CoreStateInput {
                ACTION_INITIALIZE=0,
                ACTION_TERMINATE=1,
                ACTION_ADDIP=2,
                ACTION_DISCONNECT=3,
                ACTION_READY=4,
                ACTION_IDLE=5,
                ACTION_SYNC=6,
                ACTION_END=7,
                RECVED_HELLO=10,
                RECVED_HELLOREJ=11,
                RECVED_REJECTED=12,
                RECVED_ACK=13,
                RECVED_BYE=14,
                RECVED_DROP=15,
                RECVED_LOADED=16,
                RECVED_ECHOREPLY=17,
                RCVD_GSYNC=18
            };

            volatile CoreState state = UNINITIALIZED;

            // callbacks for various events
            ClientCoreCallbacks callbacks;

            unsigned int lastDataSent;
            unsigned int lastDataReceived;

            Messaging connection;

            struct {
                char nick[32];
                char peer[32];
                bool cgLocked;
                bool peerReady;
                bool userReady;
            } usersInfo;

            struct {
                int frame;
                int delay;
                int inputLength;
                int totalInputLength;
                unsigned char playerNo;
                DataQueue myBuffer;
                DataQueue inBuffer;
                unsigned char defaultInput[256];
                unsigned int ping;
            } gameInfo;

            DynamicArray<SocketAddress*, 4> ptIPs;

            void resetLocalReadyness()
            {
                usersInfo.peerReady = usersInfo.userReady = false;
                callbacks.setPeerReady(usersInfo.peerReady);
                callbacks.setUserReady(usersInfo.userReady);
            }

            void send(Instruction & instr) {
                //LOG(instruction send %x, instr.type);
                connection.include(instr);
                if (state != CONNECTED && (state != RUNNING || instr.type == SYNC))
                    connection.sendMessage();
            }

            void endGame(int reason) {
                Instruction drop(DROP, reason);
                send(drop);
				connection.sendMessage(60);
                callbacks.gameEnded();
                resetLocalReadyness();
                //resetGameInfo();
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
                            return;
                        }
                        break;
                    case INITIALIZED:
                        if (input == ACTION_ADDIP) {
                            state = PUNCHTHROUGH;

                            require(ptIPs.itemsCount() > 0);

                            connection.setAddress(*ptIPs[(gameInfo.inputLength++ % ptIPs.itemsCount())]);

                            if (ptIPs.itemsCount() == 1) {

                                Instruction hello(HELLO, P2P_INSTF_HELLO_HELLO);
                                hello.writeSignedInt8(N02_P2P_CORE_VERSION);
                                hello.writeString(client->app.name);
                                hello.writeString(client->app.version);
                                hello.writeString(usersInfo.nick);
                                hello.writeString(N02_NAME);
                                hello.writeString(N02_VERSION_STR);
                                send(hello);

                            }

                            lastDataSent = GlobalTimer::getTime();
                            return;
                        } else if (input == RECVED_HELLO) {
                            state = CONNECTING;

                            Instruction hello(HELLO, P2P_INSTF_HELLO_ACK);
                            hello.writeSignedInt8(N02_P2P_CORE_VERSION);
                            hello.writeString(client->app.name);
                            hello.writeString(client->app.version);
                            hello.writeString(usersInfo.nick);
                            hello.writeString(N02_NAME);
                            hello.writeString(N02_VERSION_STR);
                            send(hello);

                            lastDataSent = GlobalTimer::getTime();

                            callbacks.statusUpdate("Connection requested");

                            return;
                        } else if (input == ACTION_TERMINATE) {
                            state = UNINITIALIZED;
                            return;
                        } else if (input == ACTION_IDLE) {
                            connection.resetState();							
                            return;
                        }
                        break;
                    case PUNCHTHROUGH:
                        if (input == RECVED_HELLO) {
                            state = CONNECTING;

                            Instruction hello(HELLO, P2P_INSTF_HELLO_ACK);
                            hello.writeSignedInt8(N02_P2P_CORE_VERSION);
                            hello.writeString(client->app.name);
                            hello.writeString(client->app.version);
                            hello.writeString(usersInfo.nick);
                            hello.writeString(N02_NAME);
                            hello.writeString(N02_VERSION_STR);
                            send(hello);

                            lastDataSent = GlobalTimer::getTime();

                            callbacks.statusUpdate("Connection requested");

                            return;
                        } else if (input == RECVED_ACK) {
                            state = CONNECTED;
                            usersInfo.cgLocked = false;

                            Instruction hello(HELLO, P2P_INSTF_HELLO_ACK);
                            hello.writeSignedInt8(N02_P2P_CORE_VERSION);
                            hello.writeString(client->app.name);
                            hello.writeString(client->app.version);
                            hello.writeString(usersInfo.nick);
                            hello.writeString(N02_NAME);
                            hello.writeString(N02_VERSION_STR);
                            send(hello);

                            callbacks.statusUpdate("Connection confirmed");
                            callbacks.connected();

                            gameInfo.playerNo = 0;

                            lastDataSent = GlobalTimer::getTime();
                            return;
                        } else if (input == ACTION_DISCONNECT) {
                            Instruction dc(BYE, P2P_INSTF_BYE_USER);
                            send(dc);
                            connection.sendMessage();
                            state = INITIALIZED;
                            return;
                        } 
                        // TIMEOUT => Disconnect, INITIALIZED
                        break;
                    case CONNECTING:
                        if (input == RECVED_ACK) {
                            state = CONNECTED;
                            usersInfo.cgLocked = false;

                            callbacks.statusUpdate("Connection confirmed");
                            callbacks.connected();
                            gameInfo.playerNo = 0;

                            lastDataSent = GlobalTimer::getTime();
                            return;
                        } else if (input == ACTION_DISCONNECT) {
                            Instruction dc(BYE, P2P_INSTF_BYE_USER);
                            send(dc);
                            connection.sendMessage();
                            state = INITIALIZED;
                            callbacks.statusUpdate("Disconnected");
                            return;
                        } else if (input == RECVED_BYE) {
                            state = INITIALIZED;
                            callbacks.statusUpdate("Peer Disconnected");
                            return;
                        } else if (input == RECVED_HELLOREJ) {
                            callbacks.statusUpdate("Peer Disconnected");
                            state = INITIALIZED;
                        }
                        // TIMEOUT => Disconnect, INITIALIZED
                        break;
                    case CONNECTED:
                        if (input == ACTION_IDLE) {
                            if (GlobalTimer::getTime() - lastDataSent > 5000) {
                                connection.sendMessage();
                                lastDataSent = GlobalTimer::getTime();
                            }
                            if (usersInfo.userReady == true && usersInfo.peerReady == true) {
                                callbacks.statusUpdate("Both players are ready...starting game");
                                state = LOADING;
                                callbacks.gameStart(gameInfo.playerNo, 2);
                            }
                            return;
                        } else if (input == ACTION_DISCONNECT) {
                            Instruction dc(BYE, P2P_INSTF_BYE_USER);
                            send(dc);
                            connection.sendMessage();
                            state = INITIALIZED;
                            callbacks.statusUpdate("Disconnected");
                            return;
                        } else if (input == RECVED_BYE) {
                            state = INITIALIZED;
                            callbacks.statusUpdate("Peer Disconnected");
                            return;
                        }
                        break;
                        // TIMEOUT, BYE => Disconnect, INITIALIZED
                    case LOADING:
                        if (input == ACTION_SYNC) {
                            state = LOADED;

                            Instruction loaded(LOAD);
                            send(loaded);

                            lastDataSent = GlobalTimer::getTime();
                            return;
                        } else if (input == RECVED_DROP || input == ACTION_END) {

                            endGame(1);

                            state = CONNECTED;
                            return;
                        }
                        break;
                    case LOADED:
                        if (input == RECVED_DROP || input == ACTION_END) {

                            endGame(1);

                            state = CONNECTED;
                            return;
                        } else if (input == RECVED_LOADED) {
                            state = ALLLOADED;

                            Instruction ping(ECHO, 0);
                            send(ping);
                            gameInfo.ping = GlobalTimer::getTime();

                            lastDataSent = GlobalTimer::getTime();

                            return;
                        } else if (input == ACTION_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (time - lastDataSent > 5000) {
                                lastDataSent = time;
                                if (time - lastDataReceived > 12000) {
                                    Instruction drop(DROP, 2);
                                    send(drop);
                                    callbacks.gameEnded();
                                    resetLocalReadyness();
                                    state = CONNECTED;
                                } else {
                                    connection.sendMessage(60);
                                }
                            }
                            return;
                        }
                        break;
                    case ALLLOADED:
                        if (input == RECVED_DROP || input == ACTION_END) {
                            endGame(1);
                            state = CONNECTED;
                            return;
                        } else if (input == ACTION_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (time - lastDataSent > 5000) {
                                lastDataSent = time;
                                if (time - lastDataReceived > 12000) {
                                    Instruction drop(DROP, 2);
                                    send(drop);
                                    callbacks.gameEnded();
                                    resetLocalReadyness();
                                    state = CONNECTED;
                                } else {
                                    connection.sendMessage(60);
                                }
                            }
                            return;
                        } else if (input == RECVED_ECHOREPLY) {

                            if (gameInfo.totalInputLength < 4) {
                                gameInfo.totalInputLength++;
                                Instruction ping(ECHO, 0);
                                send(ping);
                                connection.sendMessage();

                                lastDataReceived = lastDataSent = GlobalTimer::getTime();

                                //LOG(>ping is %i, (GlobalTimer::getTime() - gameInfo.ping) / gameInfo.totalInputLength);

                                //callbacks.statusUpdate("Ping");

                            } else if (gameInfo.totalInputLength == 4) {
                                gameInfo.ping = (GlobalTimer::getTime() - gameInfo.ping) / gameInfo.totalInputLength;
                                gameInfo.delay = (int) gameInfo.ping * client->app.framerate / 100000;

                                gameInfo.delay /= 2;

                                Instruction gsnc(GSYNC);

                                gsnc.writeSignedInt8(0xff & gameInfo.delay);
                                gsnc.writeSignedInt8(0xff & callbacks.getSelectedSmoothing());
                                gsnc.writeSignedInt8(0xff & gameInfo.inputLength);
                                if (gameInfo.inputLength != 0) {
                                    gsnc.writeBytes(gameInfo.defaultInput + ((gameInfo.playerNo - 1) * gameInfo.inputLength), gameInfo.inputLength);
                                }
                                send(gsnc);

                                LOG(ping is %i, gameInfo.ping);
                                LOG(delay is %i, gameInfo.delay);

                            }
                            return;
                        } else if (input == RCVD_GSYNC) {
                            callbacks.statusUpdate("Game synchronized... starting game");
                            state = RUNNING;
                            return;
                        }
                        break;
                    case RUNNING:
                        if (input == ACTION_IDLE) {
                            unsigned int time = GlobalTimer::getTime();
                            if (time - lastDataSent > 100) {
                                lastDataSent = time;
                                if (time - lastDataReceived > 12000) {
                                    endGame(2);
                                    state = CONNECTED;
                                } else {
                                    connection.sendMessage(60);
                                }
                            }
                            return;
                        } else if (input == RECVED_DROP || input == ACTION_END) {
							callbacks.statusUpdate("end or drop");
                            endGame(1);
                            state = CONNECTED;
                            return;
						} else if (input == ACTION_DISCONNECT) {
							endGame(1);
							state = CONNECTED;
							updateState(ACTION_DISCONNECT);
						}
                        break;
                    default:
                        break;
                };
                LOG(erroneous state %i[%i], state, input);
                TRACE();
            }

            void instructionArrivalCallback(Instruction & instr)
            {
                lastDataReceived = GlobalTimer::getTime();
                switch(instr.type) {
            case HELLO:
                require(state < CONNECTED && state > UNINITIALIZED);
                connection.setLastAddress();
                if (instr.has(P2P_INSTF_HELLO_REJECT)) {
                    updateState(RECVED_REJECTED);
                } else {
                    if (instr.readUnsignedInt8() != N02_P2P_CORE_VERSION) {
                        Instruction hello(HELLO, P2P_INSTF_HELLO_REJECT);
                        hello.writeSignedInt16(REJECT_VER);
                        send(hello);
                        updateState(RECVED_HELLOREJ);
                    } else {
                        if (strcmp(client->app.name, instr.getCurrentStringPtr())!=0) {
                            Instruction hello(HELLO, P2P_INSTF_HELLO_REJECT);
                            hello.writeSignedInt16(REJECT_APP);
                            send(hello);
                            updateState(RECVED_HELLOREJ);
                        } else {
                            instr.skipString();
                            if (strcmp(client->app.version, instr.getCurrentStringPtr())!=0) {
                                Instruction hello(HELLO, P2P_INSTF_HELLO_REJECT);
                                hello.writeSignedInt16(REJECT_APPVER);
                                send(hello);
                                updateState(RECVED_HELLOREJ);
                            } else {
                                instr.skipString();
                                instr.readString(usersInfo.peer, 32);

                                if (instr.has(P2P_INSTF_HELLO_ACK)) {
                                    updateState(RECVED_ACK);
                                } else {
                                    updateState(RECVED_HELLO);
                                }

                                // dont need the rest
                            }
                        }
                    }
                }
                break;
            case BYE:
                updateState(RECVED_BYE);
                break;
            case CHAT:
                callbacks.chatReceived(usersInfo.peer, instr.getCurrentStringPtr());
                break;
            case CHANGE:
                // locking request
                if (instr.flags == P2P_INSTF_CHANGE_LOCK)
                {
                    Instruction lock(CHANGE, usersInfo.cgLocked? P2P_INSTF_CHANGE_LOCKFAIL: P2P_INSTF_CHANGE_LOCKED);
                    send(lock);
                    callbacks.statusUpdate("Lock request");
                } else if (instr.flags == P2P_INSTF_CHANGE_LOCKED && usersInfo.cgLocked) {
                    callbacks.statusUpdate("Lock success");
                    callbacks.changeGameLocked();
                } else if (instr.flags == P2P_INSTF_CHANGE_CHANGE) {
                    callbacks.gameChanged(instr.getCurrentStringPtr());
                    gameInfo.playerNo = 2;
                    resetLocalReadyness();
                } else if (instr.flags == P2P_INSTF_CHANGE_LOCKFAIL) {
                    callbacks.statusUpdate("Lock failed");
                }
                break;
            case READY:
                callbacks.setPeerReady(usersInfo.peerReady = instr.flags== P2P_INSTF_READY_READY);
                break;
            case LOAD:
                updateState(RECVED_LOADED);
                break;
            case ECHO:
                //callbacks.statusUpdate("adf");
                if (instr.flags == 0) {
                    instr.flags = instr.flags = P2P_INSTF_ECHO_ACK;
                    send(instr);
                } else
                    updateState(RECVED_ECHOREPLY);
                break;
            case GSYNC:
                {
                    int delay = instr.readUnsignedInt8();

                    LOG(Local delay %i, gameInfo.delay);
                    LOG(Received delay %i, delay);

                    int smoothing = instr.readUnsignedInt8();

                    LOG(Local smoothing %i, callbacks.getSelectedSmoothing());
                    LOG(Received smoothing %i, smoothing);

                    int inlen = instr.readSignedInt8();

                    gameInfo.delay = (delay + gameInfo.delay) / 2;

                    gameInfo.delay += 1;

                    smoothing += callbacks.getSelectedSmoothing();
                    smoothing /= 2;

                    gameInfo.delay += smoothing;

                    LOG(FInal delay %i, gameInfo.delay);

                    if (inlen == gameInfo.inputLength && inlen > 0) {
                        instr.readBytes(gameInfo.defaultInput + (((gameInfo.playerNo - 1) ^ 1) * gameInfo.inputLength), inlen);
                    }
                    updateState(RCVD_GSYNC);
                }
                break;
            case SYNC:
                gameInfo.inBuffer.push(instr.getCurrentBinaryPtr(), gameInfo.inputLength);
                break;
            case DROP:
                updateState(RECVED_DROP);
                break;
            default:
                instr.log();
                break;
                };
            }

            void ssrvCallback(char * c, int l)
            {
				callbacks.SSRV(c, l);
            }
        };

        using namespace core;


        void resetGameInfo()
        {
            gameInfo.frame = 0;
            gameInfo.delay = 0;
            gameInfo.inputLength = 0;
            gameInfo.totalInputLength = 0;
            gameInfo.myBuffer.reset();
            gameInfo.inBuffer.reset();
        }

        bool coreInitialize (const char * userName, ClientCoreCallbacks * callbacks, const int localPort)
        {
            memset(&usersInfo, 0, sizeof(usersInfo));

            core::callbacks = *callbacks;

            while (ptIPs.itemsCount() > 0) {
                delete ptIPs[0];
                ptIPs.removeIndex(0);
            }

            resetGameInfo();

            strcpy(usersInfo.nick, userName);

            if (connection.initialize(localPort)) {
                updateState(ACTION_INITIALIZE);
                return true;
            } else {
                return false;
            }
        }
        void coreTerminte()
        {
            connection.terminate();
            updateState(ACTION_TERMINATE);
        }
        void coreAddIP(const SocketAddress & ip)
        {
            ptIPs.addItem(new SocketAddress(ip.toString()));
            updateState(ACTION_ADDIP);
        }
        void coreDisconnect()
        {
            updateState(ACTION_DISCONNECT);
        }
        void coreCpr()
        {
        }
        void coreSetReady(bool ready)
        {
            require(state == CONNECTED);
            usersInfo.userReady = ready;
            Instruction rdy(READY, ready? P2P_INSTF_READY_READY: P2P_INSTF_READY_NOTREADY);
            send(rdy);
            callbacks.setUserReady(ready);
        }
        void coreChangeGameLock()
        {
            usersInfo.cgLocked = true;
            Instruction lock(CHANGE, P2P_INSTF_CHANGE_LOCK);
            send(lock);
        }
        void coreChangeGameReleaseChange(char* g)
        {
            require(usersInfo.cgLocked);
            usersInfo.cgLocked = false;
            Instruction change(CHANGE, P2P_INSTF_CHANGE_CHANGE);
            change.writeString(g);
            send(change);

            resetLocalReadyness();

            gameInfo.playerNo = 1;

            callbacks.gameChanged(g);

        }
        void coreChangeGameReleaseNoChange()
        {
            require(usersInfo.cgLocked);
            usersInfo.cgLocked = false;
        }

        int  N02CCNV step()
        {
            if (state > core::UNINITIALIZED && state < core::LOADING) {
                BsdSocket::step(0, 500);
                updateState(ACTION_IDLE);
                if (connection.hasNewInstructions())
                    connection.sendMessage();
                return 1;
            } else
                return 0;
        }
        void  N02CCNV stepc(int ms)
        {
            updateState(ACTION_IDLE);
            BsdSocket::step(0, ms);
        }
        int  N02CCNV synchronizeGame(void * syncData, int len)
        {
            updateState(ACTION_SYNC);

            if (state == LOADED) {
                callbacks.statusUpdate("Game loaded, waiting for peer");
                resetGameInfo();

                if (len != 0) {
                    memcpy(gameInfo.defaultInput + ((gameInfo.playerNo - 1) * len), syncData, gameInfo.inputLength = len);
                }

                while (state == LOADED) {
                    stepc(600);
                }

                if (state == ALLLOADED) {

                    callbacks.statusUpdate("Peer loaded, synchronizing game");

                    while (state == ALLLOADED) {
                        stepc(2);
                    }
                    if (state == RUNNING) {
                        if (len != 0) {
                            memcpy(syncData, gameInfo.defaultInput, len * 2);
                            gameInfo.inputLength = 0;
                            return len * 2;
                        }
                        return 0;
                    }
                }
            }
            return -1;
        }

        void N02CCNV endGame()
        {
            updateState(ACTION_END);
        }
        void N02CCNV sendAsyncData(const void * value, const int len, const int module)
        {

        }
        void N02CCNV sendSyncData(const void * value, const int len)
        {
            if (state == RUNNING) {

                lastDataSent = GlobalTimer::getTime();

#ifdef MASK_INITIAL_FRAMES
                if (gameInfo.frame ==0) {
                    gameInfo.inputLength=len;
                    gameInfo.totalInputLength = 2 * len;
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

                if (gameInfo.frame + gameInfo.delay <= MASK_INITIAL_FRAMES) {
                    //printf("M");
                    Instruction data(SYNC);
                    data.writeBytes(gameInfo.defaultInput, len);
                    send(data);
                    gameInfo.myBuffer.push(gameInfo.defaultInput, len);

                    //printf("{%08x/",*reinterpret_cast<const int*>(value));
                    //for (int x = 0; x < gameInfo.myBuffer.length(); x+=4) {
                    //	printf("%08x", *(reinterpret_cast<int*>(gameInfo.myBuffer.front())+x/4));
                    //}

                    //printf(":%i}", gameInfo.myBuffer.length());


                    return;
                }
#endif

                Instruction data(SYNC);
                data.writeBytes(value, gameInfo.inputLength);
                gameInfo.myBuffer.push(value, gameInfo.inputLength);
                send(data);

                //printf("{%08x/",*reinterpret_cast<const int*>(value));
                //for (int x = 0; x < gameInfo.myBuffer.length(); x+=4) {
                //	printf("%08x", *(reinterpret_cast<int*>(gameInfo.myBuffer.front())+x/4));
                //}

                //printf(":%i}", gameInfo.myBuffer.length());

            }
        }

        int  N02CCNV recvSyncData(void * value, const int len)
        {
            if (state==RUNNING) {
                if (++gameInfo.frame > gameInfo.delay) {
                    while (gameInfo.inBuffer.length() == 0 && state == RUNNING) {
                        stepc(2);
                    }

                    if (gameInfo.inBuffer.length() > 0) {

                        //printf("{%i/%i/%08x}",gameInfo.inBuffer.length(), gameInfo.myBuffer.length(), *reinterpret_cast<int*>(gameInfo.myBuffer.front()));

                        gameInfo.inBuffer.pop(reinterpret_cast<unsigned char*>(value) + (1 ^ (gameInfo.playerNo -1)) * gameInfo.inputLength, gameInfo.inputLength);
                        gameInfo.myBuffer.pop(reinterpret_cast<unsigned char*>(value) + (0 ^ (gameInfo.playerNo -1)) * gameInfo.inputLength, gameInfo.inputLength);
#ifdef MASK_INITIAL_FRAMES
                        if (gameInfo.frame <= MASK_INITIAL_FRAMES) {
                            memcpy(value, gameInfo.defaultInput, gameInfo.totalInputLength);
                        }
#endif
                        return gameInfo.totalInputLength;
                    }
                } else {
#ifdef MASK_INITIAL_FRAMES
                    memcpy(value, gameInfo.defaultInput, gameInfo.totalInputLength);
#endif
                    return 0;
                }
            }
			LOGS(Ending here);
			modHelper.endGame();
            return -1;
        }
        int  N02CCNV syncData(void * value, const int len)
        {
            sendSyncData(value, len);
            return recvSyncData(value, len);
        }
        int  N02CCNV syncDataCheck (int)
        {
            return 0;
        }
        void N02CCNV sendChat (const char * message)
        {
            if (state >= CONNECTED) {
                Instruction chat(CHAT);
                chat.writeString(message);
                send(chat);
                callbacks.chatReceived(usersInfo.nick, message);
            } else {
                callbacks.statusUpdate("Noone to send chat to");
            }
        }




		void coreSendSSRV(const char * buffer, const int len, const SocketAddress & address)
		{
			core::connection.sendSSRV(buffer, len, address);
		}




    };
};

