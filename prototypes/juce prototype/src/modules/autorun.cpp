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
#include "autorun.h"
#include "common.h"

namespace n02 {

    RunAlgorithmInterface02 autorun;
    extern ExternalModuleHelper02 modHelper;

    static void N02CCNV  callbackSaveState		(const int slotNo)
    {
        LOG(%i, slotNo);
    }
    static void N02CCNV  callbackLoadState		(const int slotNo)
    {
        LOG(%i, slotNo);
    }
    static void N02CCNV  callbackPurgeStates	(int count)
    {
        LOG(%i, count);
    }
    static void N02CCNV  callbackSetPlayerInput(const void *, const int player)
    {
        LOG(%i, player);
    }
    static int  N02CCNV  callbackGetPlayerInput(void *) {
        LOGS(0);
        return 0;
    }
    static void N02CCNV  callbackRunFrame		(int type, int finalFrame)
    {
        LOG(%i;%i, type, finalFrame);
    }
    static int  N02CCNV  callbackRetriveState	(void **, int slotNo)
    {
        LOG(%i, slotNo);
        return -1;
    }
    static void N02CCNV  callbackForceState	(void *, int , int slotNo)
    {
        LOG(%i, slotNo);
    }

    static void N02CCNV  runFrameAuto			(int drawFrame) {
        //LOG(%i, drawFrame);
        autorun.run(drawFrame);
    }
    static void N02CCNV  reserved				(int, int) {
        LOGS(0);
    }

    n02AutorunInterface defaultAutorunInterface = {
        runFrameAuto,
        reserved,
        reserved,
        reserved,
        callbackSaveState,
        callbackLoadState,
        callbackPurgeStates,
        callbackSetPlayerInput,
        callbackGetPlayerInput,
        callbackRunFrame,
        callbackRetriveState,
        callbackForceState,
        reserved,
        reserved,
        reserved,
        reserved
    };

    void N02CCNV defaultRunAlgInitialize(int playerNo, int numPlayers, int inputLagDefermentSmoothing)
    {
        LOG(%i; %i; %i, playerNo, numPlayers, inputLagDefermentSmoothing);
    }
    int N02CCNV defaultRunAlgGetGameplayType()
    {
        return RA_GT_GAMEPLAY;
    }
    void N02CCNV defaultRunAlgTerminate()
    {
        LOGS(0);
    }
    void N02CCNV defaultRunAlgRun(int drawFrame)
    {
        LOG(%i, drawFrame);
    }

    RunAlgorithmInterface02 defaultRunAlg = {
        RA_TRAD | RA_STATES | RA_GENERIC,
        defaultRunAlgInitialize,
        defaultRunAlgGetGameplayType,
        defaultRunAlgTerminate,
        defaultRunAlgRun
    };



    void autorunInitialize()
    {
        autorun = defaultRunAlg;
    }

    void autorunTerminate()
    {
    }

    //TODO: Debug this code

    static bool autorunActivate(ModuleAbstraction02 * mod)
    {
        if (mod != 0) {
            if (strcmp(mod->type, MTYPE02_RUNALG)==0) {
                if (mod->getInterface(&autorun)==0) {
                    LOGS(Interface retrival successful);
                    return true;
                } else {
                    LOGS(Failed to retrive the module interface structure);
                }
            } else {
                LOG(Invalid type of module specified %s, mod->type);
            }
        } else {
            LOGS(Could not find specified autorun module index);
        }
        return false;
    }

    bool autorunActivateByName(char * name)
    {
        LOG(%s, name);
        if (name != 0 && strlen(name) > 0) {
            return autorunActivate(modHelper.modHandler->getByName(name));
        } else {
            LOGS(Invalid autorun module name specified);
            return false;
        }
    }

    bool autorunActivateByIndex(int index)
    {
        LOG(%i, index);
        if (index >= 0) {
            return autorunActivate(modHelper.modHandler->getByIndex(index));
        } else {
            LOGS(Invalid autorun module index specified);
            return false;
        }
    }

};
