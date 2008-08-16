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
#include "modDefStress.h"
#include "common.h"
#include "module.h"
#include "clientgui.h"
#include "juceModDefStressTest.h"



namespace n02 {


    namespace defStressT {


        //GUI
        SIMPLEWINDOW(ModDefStressTestWindow, "Deferment Stress Testing", Colours::whitesmoke, DocumentWindow::closeButton, juceModDefStressTest, 450, 145);

        volatile int guiIsRunning = 0;
        static PosixThread * guiThread = 0;
        volatile int running = 0;
        volatile int frame = 0;
        int delay = 0;
        static n02AutorunInterface * autorun = 0;


        class : public Timer {
            void timerCallback() {

                static int prevFrame = 0;
                static unsigned int prevTime = 0;

                if (running == 0) {
                    ModDefStressTestWindow::cmponnt->updateStatus(0);
                } else {

                    unsigned int dt = GlobalTimer::getTime() - prevTime;
                    unsigned int df = frame - prevFrame;
                    if (dt != 0)
                        ModDefStressTestWindow::cmponnt->updateStatus( 1000 * df / dt);

                    prevFrame = frame;
                    prevTime = GlobalTimer::getTime();
                }
            }
        } timer;


        void uiStartGame(const char * gameName) {
            if (running != 0) {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "A game is already running.", "ok");
            } else {
                if (strlen(gameName) > 0) {
                    modHelper.startGame(gameName, 1, 1);
                    timer.startTimer(2000);
                }
            }
        }
        void uiStopGame() {
            running = 0;
            timer.stopTimer();
        }

        void uiModChangeCallback(int index) {
            uiStopGame();
            modHelper.activeTransportByIndex(index);
            delete ModDefStressTestWindow::window;
            ModDefStressTestWindow::window = 0;
            ModDefStressTestWindow::OnClose();
        }

        void ModDefStressTestWindow::OnClose() {
            uiStopGame();
            guiIsRunning = 0;
            if (guiThread != 0)
                guiThread->notify();
            PosixThread::yield();
        }

        static void N02CCNV initialize()
        {
            delay = 0;
            frame = 0;
            running = 0;

            autorun = reinterpret_cast<n02AutorunInterface*>(modHelper.getExtendedInterface(client, INTERFACE_AUTORUN));

        }
        static void N02CCNV terminate()
        {
            delay = 0;
            frame = 0;
            running = 0;

        }
        static void N02CCNV activete()
        {
        }
        static int  N02CCNV step()
        {
            return 0;
        }
        static int  N02CCNV synchronizeGame(void * syncData, int len)
        {
            running = 1;
            frame = 0;
            delay = 1;
            return len;
        }

        static void N02CCNV endGame()
        {
            running = 0;
            timer.stopTimer();
        }
        static void N02CCNV sendAsyncData(const void *, const int, const int)
        {
            LOGS(0);
        }
        static void N02CCNV sendSyncData(const void *, const int)
        {
            LOGS(0);
        }
        static int  N02CCNV recvSyncData(void *, const int)
        {
            LOGS(0);
            return -1;
        }
        static int  N02CCNV syncData(void *, const int)
        {
            LOGS(0);
            return -1;
        }
        static int  N02CCNV syncDataCheck (int)
        {
            LOGS(0);
            return -1;
        }
        static void N02CCNV sendChat (const char *)
        {
            LOGS(0);
        }
        /* <gui> */
        static void N02CCNV activeteGui()
        {
            guiIsRunning = 1;
            ModDefStressTestWindow::createAndShow();

            guiThread = new PosixThread(true);

            while (guiIsRunning==1)
                guiThread->wait(1000000);

            delete guiThread;

            GuiJUCEDisposeObject(ModDefStressTestWindow::window);

            guiThread = 0;
        }
        static int  N02CCNV getSelectedAutorunIndex();

        static int  N02CCNV getSelectedAutorunDelay()
        {
            return 0;
        }
        static int  N02CCNV isRecordingEnabled()
        {
            return 0;
        }
        /* </gui> */
        static void N02CCNV  reserved(int, int)
        {

        }

        static int N02CCNV getGameplayType() {
            return GAME_TYPE_GAMEPLAY;
        }


        void N02CCNV  rinitialize(int playerNo, int numPlayers, int selectedAutoRunDelay);
        int  N02CCNV  rgetGameplayType();
        void N02CCNV  rterminate();
        void N02CCNV  rrun(int drawFrame);


        static void N02CCNV runInitialize(int, int, int) {
            delay = 1;
            frame = 0;
            running = 1;
        }
        static int  N02CCNV runGetGameplayType() {
            return RA_GT_GAMEPLAY;
        }
        static void N02CCNV runTerminate() {

        }
        static void N02CCNV runRun(int drawFrame) {
            if (running && autorun != 0) {
                frame++;
                if (frame == 1)
                    autorun->callbackSaveState(0);
                unsigned char input[256];
                autorun->callbackLoadState(0);
                autorun->callbackGetPlayerInput(input);
                for (int x = 0; x <= delay; x++) {
                    if (x != 0)
                        autorun->callbackSaveState(x);
                    autorun->callbackSetPlayerInput(input, 0);
                    autorun->callbackRunFrame(x < delay? GAME_RUNFRAME_NODRAW : (drawFrame? GAME_RUNFRAME_DRAW : GAME_RUNFRAME_NODRAW), x < delay? 0 : 1);
                }
                autorun->callbackPurgeStates(1);
            } else {
                modHelper.endGame();
            }
        }





    };


    RunAlgorithmInterface02 raDefermentStressTest = {
        RA_STATES,
        defStressT::runInitialize,
        defStressT::runGetGameplayType,
        defStressT::runTerminate,
        defStressT::runRun
    };


    STDTRANSPORTDEFNNMS(trModDefStress, defStressT);

    STDMODULE(modDefStress, "defStrTest", MTYPE02_TRANSPORT, trModDefStress, APP_ATTRIBUTES_AUTORUN, 0, MOD02_STATUS_WORKING, "Module for stress testing deferred compansation"); 


    STDMODULE(modDefStressRun, "defStrTestRuner", MTYPE02_RUNALG, raDefermentStressTest, APP_ATTRIBUTES_AUTORUN, 0, MOD02_STATUS_WORKING, "Runner module for modDefStressTest");


    namespace defStressT {

        static int  N02CCNV getSelectedAutorunIndex()
        {
            return modDefStressRun.index;
        }

    };




};




