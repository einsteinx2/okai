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

namespace n02 {
    namespace deferment {

        // basic variables needed across all modules
        int delay;
        int delayParam;
        int frame;
        int playerIndex;
        int numPlayers;
        int inputLength;

        n02AutorunInterface * autorun = 0;

        static void N02CCNV initialize(int playerNo, int numPlayersP, int delayParamP)
        {
            playerIndex = playerNo - 1;
            numPlayers = numPlayersP;
            delayParam = delayParamP;
            delay = frame = 0;

            autorun = reinterpret_cast<n02AutorunInterface*>(modHelper.getExtendedInterface(client, INTERFACE_AUTORUN));

        }

        static int  N02CCNV getGameplayType()
        {
            return RA_GT_GAMEPLAY;
        }
        static void N02CCNV terminate() {

        }


        static void N02CCNV run(int drawFrame) {
            frame++;
            unsigned char buffer[256];
            TRACE(); inputLength = autorun->callbackGetPlayerInput(buffer);
            int r = transport.syncData(buffer, inputLength);
            if (r > 0) {
                recorder.addSyncData(buffer, numPlayers * inputLength);
                for (int x =0; x < numPlayers; x++) {
                    TRACE(); autorun->callbackSetPlayerInput(buffer + (x * inputLength), x);
                }
                TRACE(); autorun->callbackRunFrame(drawFrame? GAME_RUNFRAME_DRAW: GAME_RUNFRAME_NODRAW, 1);
            } else if (r ==0) {
                delay++;
                recorder.addSyncData(buffer, r);
                for (int x =0; x < numPlayers; x++) {
                    TRACE(); autorun->callbackSetPlayerInput(buffer + (x * inputLength), x);
                }
                TRACE(); autorun->callbackRunFrame(drawFrame? GAME_RUNFRAME_DRAW: GAME_RUNFRAME_NODRAW, 1);
            } else {
                //TRACE(); modHelper.endGame();
            }
            TRACE();
        }
    };

    using namespace deferment;

    static RunAlgorithmInterface02 raTradDelay = {
		RA_GENERIC | RA_TRAD,
        initialize,
        getGameplayType,
        terminate,
        run
    };

    STDMODULE(modTradDelay, "traditional", MTYPE02_RUNALG, raTradDelay, APP_ATTRIBUTES_AUTORUN, 0, MOD02_STATUS_WORKING, "Traditional run module");

};
