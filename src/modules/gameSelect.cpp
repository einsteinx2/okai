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

#include "gameSelect.h"
#include "juceGameSelect.h"
#include "GamesList.h"
#include "juceAbortableStatusWindow.h"

namespace n02 {

    static int selectedGameIndex;
    static int gamesCount;
    static juceGameSelect * newCmp;

    namespace gamelistSel {
        int selectedIndex;
    };

    using namespace gamelistSel;

    char * getSelectedGame(Component* parent) {
        if (GamesList::getCount() > 0) {
            selectedGameIndex = -1;

            int r = DialogWindow::showModalDialog(T("Select game"), newCmp = new juceGameSelect, parent, Colours::whitesmoke, true);
            delete newCmp;

            if (r==0 || selectedIndex == -1)
                return 0;
            else
                return GamesList::current(&selectedIndex);
        } else {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "No games in list");
            return 0;
        }
    }

    int  GameSelectLB::getNumRows () {
        return GamesList::getCount();
    }

    void GameSelectLB::selectedRowsChanged (int lastRowSelected) {
        selectedIndex = lastRowSelected;
    }

    void  GameSelectLB::paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) {
        if (rowIsSelected) {
            g.fillAll (Colour(0xdd,0xdd,0xff));
        }
        selectedGameIndex = rowNumber;
        String text(GamesList::current(&selectedGameIndex));
        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }
    void  GameSelectLB::listBoxItemClicked (int row, const MouseEvent &e){
        GamesList::selectByIndex(selectedIndex = selectedGameIndex = row);

        juce::String status;

        status.printf(T("%i players"), GamesList::selectedMaxPlayers());

        if (GamesList::selectedCaps() & GCAPS_STATES)
            status.append(T(", states"), 42);

        newCmp->updateGameInfo(status);
    }

    void  GameSelectLB::listBoxItemDoubleClicked (int row, const MouseEvent &e){
        selectedIndex = selectedGameIndex = row;
        newCmp->closeUp();
    }
    void  GameSelectLB::returnKeyPressed (int lastRowSelected){
        selectedIndex = selectedGameIndex = lastRowSelected;
        newCmp->closeUp();
    }



    void setupAutorunUIItems(int currentGameCaps, ComboBox * cmbRun, ComboBox * cmbDelay, Label * lblRun, Label * lblDelay) {
        cmbRun->clear();
        cmbDelay->clear();
        if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
            cmbRun->setEnabled(true);
            lblRun->setEditable(true);
            cmbDelay->setEnabled(true);
            lblDelay->setEnabled(true);
            int firstSelIndex = -1;
            for (ModuleAbstraction02 * mod = modHelper.modHandler->find(MTYPE02_RUNALG); mod != 0; mod = modHelper.modHandler->findNext(mod)) {
                RunAlgorithmInterface02 ra; mod->getInterface(&ra);

                if ((ra.attributes & RA_GENERIC)==0)
                    continue;

                if ((currentGameCaps & GAME_CAPS_STATES) == 0 && (ra.attributes & RA_STATES) !=0)
                    continue;

                if (firstSelIndex == -1)
                    firstSelIndex = mod->index;

                cmbRun->addItem(mod->name, mod->index);
            }
            cmbRun->setSelectedId(firstSelIndex);

            for (int x = 0; x < 9; x++) {
                String xx;				
                xx.printf(T("+%i frames"), x);
                cmbDelay->addItem(xx, x+1);
            }
            cmbDelay->setSelectedId(1);
        } else {
            cmbRun->setEnabled(false);
            lblRun->setEnabled(false);
            cmbDelay->setEnabled(false);
            lblDelay->setEnabled(false);
        }

    }


    //TODO: Set this up with a thread

    bool cancelled;
    volatile bool cancelableInitialized;

    SIMPLEWINDOW(CancelableStatusWindow, "...", Colours::whitesmoke, DocumentWindow::closeButton, juceAbortableStatusWindow, 300, 120);

    class CancelableWindowThread: public PosixThread {
    public:
        void run() {
            CancelableStatusWindow::createAndShowModal();
        }

    } cancelableWindowThread;

    void CancelableStatusWindow::OnClose()
    {
        cancelled = true;
    }

    void showCancelable()
    {
        cancelled = false;
        cancelableInitialized = false;

        cancelableWindowThread.start();

        int counter = 100;
        while (cancelableInitialized == false && counter-->0) {
            PosixThread::sleep(10); PosixThread::yield();
        }

        if (counter == 0) {
            if (cancelableWindowThread.isRunning()) {
                cancelableWindowThread.stop();
            }
        }

    }

    bool updateCancelable(char * status)
    {
        if (cancelableInitialized) {
            CancelableStatusWindow::cmponnt->updateStatus(status);
        }
        return cancelled;
    }
    bool isCancelableCanceled()
    {
        return cancelled;
    }
    void hideCancelable()
    {
        delete CancelableStatusWindow::window;
    }


};

