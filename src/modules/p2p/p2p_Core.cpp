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
#include "p2p_Core.h"
#include "p2p_Messaging.h"
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
				RUNNING=7
			};

			// inputs to translate states
			enum CoreStateInput {
				ACTION_INITIALIZE=0,
				ACTION_TERMINATE=1,
				ACTION_ADDIP=2,
				ACTION_DISCONNECT=3,
				ACTION_READY=4,
				ACTION_IDLE=5,
				RECVED_HELLO=10,
				RECVED_HELLOREJ=11,
				RECVED_REJECTED=12,
				RECVED_ACK=13
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
				unsigned char numPlayers;
				DataQueue myBuffer;
				DataQueue inBuffer;
				unsigned char defaultInput[256];
				unsigned int lastDataSentTime;
			} gameInfo;

			DynamicArray<SocketAddress*, 4> ptIPs;

			void send(Instruction & instr) {
				//LOG(instruction send %x, instr.type);
				connection.include(instr);
				connection.sendMessage();
			}

			// state transformation function
			void updateState(CoreStateInput input) {
				LOG(state=%i[%i], state, input);
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

							Instruction hello(HELLO, P2P_INSTF_HELLO_HELLO);
							hello.writeSignedInt8(N02_P2P_CORE_VERSION);
							hello.writeString(client->app.name);
							hello.writeString(client->app.version);
							hello.writeString(usersInfo.nick);
							send(hello);

							lastDataSent = GlobalTimer::getTime();
							return;
						} else if (input == RECVED_HELLO) {
							state = CONNECTING;
						} else if (input == ACTION_TERMINATE) {
							state = UNINITIALIZED;
							return;
						} else if (input == ACTION_IDLE) {
							return;
						}
						break;
					case PUNCHTHROUGH:
						// HELLO => CONNECTING
						// ACK => CONNECTED

						break;
					case CONNECTING:
						// ACK = > CONNECTED
						// TIMEOUT => INITIALIZED
					default:
						break;
				};
				LOG(erroneous state %i[%i], state, input);
				TRACE();
			}

            void instructionArrivalCallback(Instruction & instr)
			{
				instr.log();				
			}

			void ssrvCallback(char *, int)
			{
			}
        };

		using namespace core;

		// punchthrough;

		
			/*void add_punthrough_ip(char * ip) {
				LOG(%s state=%i, ip, state);
				init();
				if (state==NC) {
					state=PT;
				}
				n_socket_addr nsa;
				nsa.parse(ip);
				punch.add(nsa);

				}*/





		
		bool coreInitialize (const char * userName, ClientCoreCallbacks * callbacks, const int localPort)
		{
			memset(&usersInfo, 0, sizeof(usersInfo));

			core::callbacks = *callbacks;

			while (ptIPs.itemsCount() > 0) {
				delete ptIPs[0];
				ptIPs.removeIndex(0);
			}

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
		}
		void coreCpr()
		{
		}
		void coreSetReady(bool ready)
		{
		}
		void coreChangeGameLock()
		{
		}
		void coreChangeGameReleaseChange(char*)
		{
		}
		void coreChangeGameReleaseNoChange()
		{
		}

		int  N02CCNV step()
		{
			if (state > core::UNINITIALIZED && state < core::LOADING) {
				BsdSocket::step(0, 500);
				updateState(ACTION_IDLE);
				return 1;
			} else
				return 0;
		}
		int  N02CCNV synchronizeGame(void * syncData, int len)
		{
			return 0;
		}

		void N02CCNV endGame()
		{

		}
		void N02CCNV sendAsyncData(const void * value, const int len, const int module)
		{

		}
		void N02CCNV sendSyncData(const void * value, const int len)
		{

		}
		int  N02CCNV recvSyncData(void * value, const int len)
		{
			return 0;
		}
		int  N02CCNV syncData(void * value, const int len)
		{
			return 0;
		}
		int  N02CCNV syncDataCheck (int)
		{
			return 0;
		}
		void N02CCNV sendChat (const char * message)
		{

		}
	};
};

