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

#include "n02.h"

#include "common.h"
#include "module.h"
#include "recorder.h"
#include "transport.h"
#include "gameplay.h"
#include "autorun.h"
#include "clientgui.h"

//*****************************************************************************
#include "n02version.h"
namespace n02 {
    const static n02ClientInfoInterface clientInfoInterface = {
        N02_NAME,
        N02_VERSION_STR,
        N02_DESCRIPTION,
        __DATE__,
        __TIME__,
        N02_API_VERSION
    };
};

//*****************************************************************************
#include "GamesList.h"
namespace n02 {
    static void N02CCNV gameListReset() {
        GamesList::reset();
    }
    static void (N02CCNV gameListAdd)(char * name, int maxPlayers) {
        GamesList::add(name, maxPlayers);
    }
    static void (N02CCNV gameListAddCaps)(char * name, int maxPlayers, int caps) {
        GamesList::add(name, maxPlayers & 0xff, caps & 0xff);
    }
    const static n02GamelistInterface gamelistInterf = {
        gameListReset,
        gameListAdd,
        gameListAddCaps
    };
};

//*****************************************************************************
namespace n02 {

    typedef struct {
        int type;
        void * ptr;
        int size;
    } ClientInterfaceExtension;

    n02ClientInterface * client = 0;
    n02AutomationInterface * autom = 0;

    static int numPlayers;
    static int playerNo;
    static char game[128];
    static int gamePlayType;

    typedef struct {
        unsigned state:2;
        unsigned input:2;
    } Gtsandfa;

    int gtsandfaTT [16] = {
        0, 0, 1, 1,
        1, 0, 1, 0,
        2, 0, 2, 0,
        3, 3, 3, 3
    };

    Gtsandfa gtsandfa;

#define GTSADFA_START_GAME 2
#define GTSADFA_END_GAME 1

    void N02CCNV interfStartGame (const char * gameP, const int playerNoP, const int numPlayersP){
        LOG(%s; %i / %i, gameP, playerNoP, numPlayersP);
        strcpy(game, gameP);
        playerNo = playerNoP;
        numPlayers = numPlayersP;

        gamePlayType = 0;
        if (autom==0) {
            if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
                if (autorunActivateByIndex(transport.getSelectedAutorunIndex())) {
                    autorun.initialize(playerNo, numPlayers, transport.getSelectedAutorunDelay());
                    gamePlayType = autorun.getGameplayType();
                } else {
                    LOGS(Could not initialize autorun);
                    return;
                }
            } else {
                gamePlayType = transport.getGameplayType();
            }
        } else {
            if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
                if (autorunActivateByName(autom->algorithm)) {
                    autorun.initialize(playerNoP, numPlayers, autom->algoParam1);
                    gamePlayType = autorun.getGameplayType();
                } else {
                    LOGS(Could not initialize autorun);
                    return;
                }
            } else {
                gamePlayType = transport.getGameplayType();
            }
        }

        LOG(Loading game %s (player %i of %i) gtype=%i, game, playerNo, numPlayers, gamePlayType);

        recorder.stop();
        if (modHelper.isRecorderLoaded() && ( (autom == 0 && transport.isRecordingEnabled()) || (autom != 0 && autom->recordingEnabled != 0) )) {
            recorder.start(game, playerNo, numPlayers, "");
        }

        if (autom == 0) {
            gtsandfa.input = GTSADFA_START_GAME;
        } else {
            client->gameplay.callbackGameLoad(game, playerNo, numPlayers, gamePlayType);			
        }
    }
    void N02CCNV  interfEndGame (){
        LOGS(0);
        TRACE(); recorder.stop();

        if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
            TRACE(); autorun.terminate();
        }

        if (autom == 0) {
            gtsandfa.input = GTSADFA_END_GAME;
        }

        TRACE(); client->gameplay.callbackGameEnd();	
    }
    void N02CCNV  interfChatReceived (const char *nick, const char *text){
        LOG(%s; %s, nick, text);
        if (gtsandfa.state==2) {
            TRACE(); recorder.addChat(nick, text);
            TRACE(); client->gameplay.callbackChatReceived(nick, text);
        }
    }
    void N02CCNV  interfPlayerDropped (const char *nick, const int playernb) {
        LOG(%s; %i, nick, playernb);
        TRACE(); recorder.addDrop(nick, playernb);
        TRACE(); client->gameplay.callbackPlayerDropped(nick, playernb);
    }
    void N02CCNV  interfAsyncData (const void * data, const int size) {
        LOG(%i, size);
        TRACE(); recorder.addAsyncData(data, size);
        TRACE(); client->gameplay.callbackAsyncData(data, size);
    }

    void N02CCNV  interfGuiThreadEndCallback() {
        TRACE(); gtsandfa.state = 3;
    }


    void * N02CCNV interfGetExtendedInterface (const n02ClientInterface * clientInterface, const int type) {
        if (type != 0 && clientInterface->internal0 != 0) {
            DynamicArray<ClientInterfaceExtension, 8> * list = reinterpret_cast<DynamicArray<ClientInterfaceExtension, 8>*>(clientInterface->internal0);
            for (int x = 0; x < list->itemsCount(); x++) {
                if (list->getItemPtr(x)->type == type) {
                    return list->getItemPtr(x)->ptr;
                }
            }
        }
        return 0;
    }

    int  N02CCNV extend (void * clientInterfPtr, void * interfStruct, int interfStructLen, int interfType) {
        if (clientInterfPtr != 0) {
            n02ClientInterface * interf = reinterpret_cast<n02ClientInterface*>(clientInterfPtr);
            if (interf->internal0 == 0) {
                interf->internal0 = new DynamicArray<ClientInterfaceExtension, 8>();
            }
            if (interf->internal0!=0 && interfStruct != 0 && interfStructLen > 0 && interfType > 0) {
                DynamicArray<ClientInterfaceExtension, 8> * list = reinterpret_cast<DynamicArray<ClientInterfaceExtension, 8>*>(interf->internal0);
                ClientInterfaceExtension cie;
                cie.ptr = interfStruct;
                cie.size = interfStructLen;
                cie.type = interfType;
                list->addItemPtr(&cie);
            }
        }
        return 0;
    }

    int GuiShowLicenseDialog();

    int N02CCNV activate (void * v) {

        if (v==0)
            return -1;

        client = reinterpret_cast<n02ClientInterface*>(v);

        // set up attributes
        {
            client->app.attributes = 0;

            if (GamesList::getCount() == 1) {
                client->app.attributes |= APP_ATTRIBUTES_SINGLEGAME;
            }

            if (interfGetExtendedInterface(client, INTERFACE_AUTOMATION)) {
                client->app.attributes |= APP_ATTRIBUTES_AUTOMATE;
            }

            if (client->app.framerate == 6000) {
                client->app.attributes |= APP_ATTRIBUTES_60FPS;
            }

            if (interfGetExtendedInterface(client, INTERFACE_AUTORUN)) {
                client->app.attributes |= APP_ATTRIBUTES_AUTORUN;
            }


        }

        // initialize stuff
        commonInitialize();
        modulesInitialize();
        transportInitialize();
        recorderInitialize();
        gameplayInitialize();
        autorunInitialize();
        GuiInitialize();

        LOG(Activate context in %x %i, v, PosixThread::getCurrentThreadId());
        LOG(%s  %s %s, N02_DESCRIPTION, __DATE__, __TIME__);

        if (GuiShowLicenseDialog() ==0) {

            {// automation
                if ((client->app.attributes & APP_ATTRIBUTES_AUTOMATE) != 0) {
                    autom = reinterpret_cast<n02AutomationInterface*>(interfGetExtendedInterface(client, INTERFACE_AUTOMATION));

                    transportSetActiveByName(autom->transport);
                    transportResetActivation();
                } else {
                    autom = 0;
                }
            }

            if (autom==0) {
                if (GuiStartSync()) {
                    //				__try {
                    bool exit = false;
                    do {
                        TRACE();
                        gtsandfa.state = gtsandfaTT[((gtsandfa.state)<<2)|gtsandfa.input];
                        gtsandfa.input = 0;
                        switch (gtsandfa.state) {
                            case 0: // Normal
                                {
                                    TRACE();
#ifdef N02_WIN32
                                    MSG message;
                                    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)){
                                        TranslateMessage(&message);
                                        DispatchMessage(&message);
                                    }
#endif
                                    TRACE();
                                    if (transport.step()){
                                        continue;
                                    }
                                }
                                break;
                            case 1: // Load game
                                {
                                    LOGS(Game load entry point);
                                    gtsandfa.state = 2;
                                    client->gameplay.callbackGameLoad(game, playerNo, numPlayers, gamePlayType);
                                }
                                break;
                            case 2: // In game
                                {
#ifdef N02_WIN32
                                    MSG message;
                                    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)){
                                        TranslateMessage(&message);
                                        DispatchMessage(&message);
                                    }
#endif
                                }
                                break;
                            case 3: // quitting
                                exit = true;
                                break;
                        };
                        TRACE();
                        PosixThread::sleep(500); // clock
                    } while (!exit); // Exit state

                    //				} __except(seh_exception_filter_function(GetExceptionInformation())) {
                    //					LOG(Main thread exception);
                    //					MessageBox(GetActiveWindow(), _T("Please reopen the client"), _T("Main thread exception"),0);
                    //				}
                } else {
                    LOGS(Failed to synchronize with GUI thread);
                }
            } else { // Automation is set so we'll run in single threaded GUI less mode
                transport.initialize();
                transport.activete();
                transport.terminate();
            }
        }

        GuiCleanup();
        autorunTerminate();
        gameplayTerminate();
        recorderTerminate();
        transportTerminate();		
        modulesTerminate();
        commonTerminate();

        if (client->internal0 != 0)
            delete client->internal0;
        client->internal0 = 0;

        return 0;
    }
};

//*****************************************************************************

using namespace n02;

static int n02ResetInterfaceInternal(void * interfPtr, int type) {
    switch (type) {
        case INTERFACE_APPINFO:
            {
                n02AppInfoInterface * interf = reinterpret_cast<n02AppInfoInterface*>(interfPtr);
                memset(interf, 0, sizeof(n02AppInfoInterface));
                interf->framerate = 6000;
            }
            break;
        case INTERFACE_CLIENTINFO:
            {
                *reinterpret_cast<n02ClientInfoInterface*>(interfPtr) = clientInfoInterface;
            }
            break;
        case INTERFACE_GAMELIST:
            {
                *reinterpret_cast<n02GamelistInterface*>(interfPtr) = gamelistInterf;
            }
            break;
        case INTERFACE_GAMEPLAY:
            {
                *reinterpret_cast<n02GameplayInterface*>(interfPtr) = defaultGameplayInterface;
            }
            break;
        case INTERFACE_CLIENT:
            {
                n02ClientInterface * interf = reinterpret_cast<n02ClientInterface*>(interfPtr);
                n02ResetInterface(&interf->app, INTERFACE_APPINFO);
                n02ResetInterface(&interf->games, INTERFACE_GAMELIST);
                n02ResetInterface(&interf->gameplay, INTERFACE_GAMEPLAY);
                interf->activate = activate;
                interf->extend = extend;
                interf->internal0 = new DynamicArray<ClientInterfaceExtension, 8>();
            }
            break;
        case INTERFACE_AUTOMATION:
            {
                memset(reinterpret_cast<n02AutomationInterface*>(interfPtr), 0, sizeof(n02AutomationInterface));
            }
            break;
        case INTERFACE_AUTORUN:
            {
                *reinterpret_cast<n02AutorunInterface*>(interfPtr) = defaultAutorunInterface;
            }
            break;
        default:
            return 1;
    };
    return 0;
}

#ifdef N02_WIN32

extern "C" {

    __declspec(dllexport) int N02CCNV n02ResetInterface(void * interfPtr, int type) {
        return n02ResetInterfaceInternal(interfPtr, type);
    }
    __declspec(dllexport) int __cdecl n02ResetInterfaceC(void * context, int type) {
        return n02ResetInterface(context, type);
    }
    int N02CCNV n02ResetInterfaceS(void * context, int type) {
        return n02ResetInterface(context, type);
    }
};



#else

extern "C" {

    int N02CCNV n02ResetInterface(void * interfPtr, int type) {
        return n02ResetInterfaceInternal(interfPtr, type);
    }
    int n02ResetInterfaceC(void * context, int type) {
        return n02ResetInterface(context, type);
    }

    int N02CCNV n02ResetInterfaceS(void * context, int type) {
        return n02ResetInterface(context, type);
    }

};


#endif


namespace n02 {

	const char * n02GetName() {
		return "n02";
	}

};

