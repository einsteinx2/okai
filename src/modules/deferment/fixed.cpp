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

#include "common.h"
#include "module.h"
#include "transport.h"
#include "recorder.h"
#include "defermentInputManager.h"

namespace n02 {
    namespace deferment {

        // basic variables needed across all modules
        extern int delay;
        extern int delayParam;
        extern int frame;
        extern int playerIndex;
        extern int numPlayers;
        extern int inputLength;
        static int delayAdjusted;

        extern n02AutorunInterface * autorun;

        static void N02CCNV initialize(int playerNo, int numPlayersP, int delayParamP)
        {
            playerIndex = playerNo - 1;
            numPlayers = numPlayersP;
            delayParam = delayParamP;
            delay = frame = 0;

            autorun = reinterpret_cast<n02AutorunInterface*>(modHelper.getExtendedInterface(client, INTERFACE_AUTORUN));

            inputInitialize();
        }

        static int  N02CCNV getGameplayType()
        {
            return RA_GT_GAMEPLAY;
        }

        static void N02CCNV terminate() {
            inputTerminate();
        }

        static void N02CCNV run(int drawFrame) {
            //LOG(Frame %i =======================================, frame);
            frame++;
            //LOG(Sending input);
            {
                if (inputLength == 0) {
                    TRACE(); inputLength = autorun->callbackGetPlayerInput(getReserveInput());
                }
                register void * inpl = getInput(delay, playerIndex);
                TRACE(); transport.sendSyncData(inpl, autorun->callbackGetPlayerInput(inpl));
            }
            //LOG(Updaing prediction);
            {
                for (int x = 0; x < numPlayers; x++) {
                    if (x != playerIndex) {
                        if (delay == 0) {
                            memcpy(getInput(delay, x), getInput(delay, playerIndex), inputLength);
                        } else {
                            memcpy(getInput(delay, x), getInput(delay-1, x), inputLength);
                        }
                    }
                }
            }
            //LOG(Receiving);
            void * rsBuf = getReserveInput();
            int r = transport.recvSyncData(rsBuf, inputLength);
            if (r == -1) {
                modHelper.endGame();
                return;
            } else if (r==0) {
                TRACE(); recorder.addSyncData(rsBuf, numPlayers * inputLength);
                delay++;
                for (int x = 0; x < numPlayers; x++) {
                    TRACE(); autorun->callbackSetPlayerInput(getReserveInput(x), x);
                }
                TRACE(); autorun->callbackRunFrame(drawFrame? GAME_RUNFRAME_DRAW : GAME_RUNFRAME_NODRAW, 1);
            } else {
                TRACE(); recorder.addSyncData(rsBuf, r);
                if (delay == (frame - 1)) {
                    delayAdjusted = common_max(0, delay - delayParam);
                    for (int x = 0; x <	delay; x++) {
                        TRACE(); autorun->callbackSaveState(x);
                        for (int y = 0; y < numPlayers; y++) {
                            TRACE(); autorun->callbackSetPlayerInput(getInput(x, y), y);
                        }
                        autorun->callbackRunFrame(GAME_RUNFRAME_NODRAW, 0);
                    }
                }
                if (delayAdjusted > 0) {
                    if (memcmp(rsBuf, getInput(0, 0), numPlayers * inputLength) != 0) {
                        {
                            for (int x = 0; x <= delay; x++) {
                                memcpy(getReserveInput(playerIndex), getInput(x, playerIndex), inputLength);
                                memcpy(getInput(x, 0), getReserveInput(), r);
                            }
                        }
                        {
                            TRACE(); autorun->callbackLoadState(0);
                            for (int x = 0; x <= delayAdjusted; x++) {
                                for (int y = 0; y < numPlayers; y++) {
                                    TRACE(); autorun->callbackSetPlayerInput(getInput(x, y), y);
                                }
                                if (x != 0) {
                                    TRACE(); autorun->callbackSaveState(x);
                                }
                                TRACE(); autorun->callbackRunFrame(x < delayAdjusted? GAME_RUNFRAME_NODRAW : (drawFrame? GAME_RUNFRAME_DRAW : GAME_RUNFRAME_NODRAW), x < delayAdjusted? 0 : 1);
                            }
                            TRACE(); autorun->callbackPurgeStates(1);
                            TRACE(); purgeInputBase();
                        }
                    } else {
                        TRACE(); autorun->callbackPurgeStates(1);
                        TRACE(); purgeInputBase();
                        TRACE(); autorun->callbackSaveState(delayAdjusted - 1);
                        for (int y = 0; y < numPlayers; y++) {
                            TRACE(); autorun->callbackSetPlayerInput(getInput(delayAdjusted - 1, y), y);
                        }
                        TRACE(); autorun->callbackRunFrame(drawFrame? GAME_RUNFRAME_DRAW : GAME_RUNFRAME_NODRAW, 1);
                    }
                } else {
                    for (int y = 0; y < numPlayers; y++) {
                        TRACE(); autorun->callbackSetPlayerInput(getReserveInput(y), y);
                    }
                    TRACE(); autorun->callbackRunFrame(drawFrame? GAME_RUNFRAME_DRAW : GAME_RUNFRAME_NODRAW, 1);
                    TRACE(); purgeInputBase();
                }
            }
            TRACE();
        }
    };

    using namespace deferment;

    static RunAlgorithmInterface02 raFixed = {
        RA_GENERIC | RA_STATES,
        initialize,
        getGameplayType,
        terminate,
        run
    };

    STDMODULE(modFixedDelay, "fixed", MTYPE02_RUNALG, raFixed, APP_ATTRIBUTES_AUTORUN, 0, MOD02_STATUS_WORKING, "Traditional run module");

};
