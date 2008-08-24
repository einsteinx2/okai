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

#include "juceModHelpers.h"
#include "juceGameSelect.h"
#include "GamesList.h"
#include "juceAbortableStatusWindow.h"
#include "locid.h"

namespace n02 {

	/*****************************************************************************
	Games List Selection
	*******************************************/

	namespace gameListSelect {

		static int selected;

		static juceGameSelect * gameSelComponent;

		void gameSelectInitialize()
		{
			LOGS(Game selection window initialization);
			gameSelComponent = new juceGameSelect(0);
		}

		void gameSelectTerminate()
		{
			LOGS(Game selection window termination);
			delete gameSelComponent;
		}

		bool gameSelectChangeToNew(Component* parent)
		{
			if (GamesList::getCount() > 0) {
				if (GamesList::getCount() == 1) {
					selected = 0;
					return true;
				} else {
					int r = DialogWindow::showModalDialog(LUTF16(LID_SELG), gameSelComponent, parent, Colours::whitesmoke, true);
					if (r > 0) {
						selected = r - 1;
						return true;
					}
				}
			} else {
				TRACE(); AlertWindow::showMessageBox(AlertWindow::WarningIcon, LUTF16(LID_ERR1), LUTF16(LID_NGOL));
			}
			return false;
		}

		bool gameSelectChange(const char * game)
		{
			return true;
		}

		char * gameSelectGetSelectedName()
		{
			if (GamesList::selectByIndex(selected)) {
				return GamesList::current(&selected);
			}
			return 0;
		}

		int gameSelectGetSelectedCaps()
		{
			if (GamesList::selectByIndex(selected)) {
				return GamesList::selectedCaps();
			}
			return 0;
		}

		int gameSelectGetSelectedPlayers()
		{
			if (GamesList::selectByIndex(selected)) {
				return GamesList::selectedMaxPlayers();
			}
			return 0;
		}

	}

    int  GameSelectLB::getNumRows ()
	{
        return GamesList::getCount();
    }

    void GameSelectLB::selectedRowsChanged (int lastRowSelected)
	{
        //selectedIndex = lastRowSelected;
    }

    void  GameSelectLB::paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected)
	{
        if (rowIsSelected) {
            g.fillAll (Colour(0xdd,0xdd,0xff));
        }
		String text(FROMUTF8(GamesList::current(&rowNumber)));
        g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
    }

	void  GameSelectLB::listBoxItemClicked (int row, const MouseEvent &e)
	{
        GamesList::selectByIndex(row);
        juce::String status;
        status.printf(LUTF16(LID_PLRS), GamesList::selectedMaxPlayers());
        if (GamesList::selectedCaps() & GCAPS_STATES)
            status.append(LUTF16(LID_STS1), 42);
        gameSelComponent->updateGameInfo(status);
    }

    void  GameSelectLB::listBoxItemDoubleClicked (int row, const MouseEvent &e)
	{
        Component::getCurrentlyModalComponent()->exitModalState(row + 1);
    }

	void  GameSelectLB::returnKeyPressed (int lastRowSelected)
	{
		Component::getCurrentlyModalComponent()->exitModalState(lastRowSelected + 1);
    }


	/*****************************************************************************
	Autorun Options Select
	*******************************************/


	static class auiComboBoxListener
		: public ComboBoxListener,
		public ButtonListener
	{
		void  comboBoxChanged (ComboBox *comboBoxThatHasChanged);
		void buttonClicked (Button* button);
	} * cmbListener;

	namespace autorunUiManagement {

		static ComboBox * cmbMode;
		static ComboBox * cmbModeParam;
		static Label * lblMode;
		static Label * lblModeParam;

		static int selectedAutorunIndex = 0;
		static int selectedDelayParam = 0;

		void autoUiSetActiveItems(ComboBox * cmbRun, ComboBox * cmbDelay, Label * lblRun, Label * lblDelay)
		{
			cmbMode = cmbRun;
			cmbModeParam = cmbDelay;
			lblMode = lblRun;
			lblModeParam = lblDelay;

			lblModeParam->setText(LUTF16(LID_DELA), true);
			lblMode->setText(LUTF16(LID_MODE), true);


			if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
				cmbMode->setEnabled(true);
				lblMode->setEnabled(true);
				cmbModeParam->setEnabled(true);
				lblModeParam->setEnabled(true);

				cmbMode->addListener(cmbListener);
				cmbModeParam->addListener(cmbListener);

			} else {
				cmbMode->setEnabled(false);
				lblMode->setEnabled(false);
				cmbModeParam->setEnabled(false);
				lblModeParam->setEnabled(false);
			}

			autoUiUpdateItems();

		}

		void autoUiUpdateItems()
		{
			if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {

				cmbMode->clear();
				cmbModeParam->clear();

				int currentGameCaps = gameSelectGetSelectedCaps();
				int firstSelIndex = -1;

				for (ModuleAbstraction02 * mod = modHelper.modHandler->find(MTYPE02_RUNALG); mod != 0; mod = modHelper.modHandler->findNext(mod)) {
					RunAlgorithmInterface02 ra; mod->getInterface(&ra);

					if ((ra.attributes & RA_GENERIC)==0)
						continue;

					if ((currentGameCaps & GAME_CAPS_STATES) == 0 && (ra.attributes & RA_STATES) !=0)
						continue;

					if (firstSelIndex == -1)
						firstSelIndex = mod->index;

					cmbMode->addItem(mod->name, mod->index);
				}
				cmbMode->setSelectedId(firstSelIndex);

				for (int x = 0; x < 9; x++) {
					String xx;				
					xx.printf(LUTF16(LID_FRMP), x);
					cmbModeParam->addItem(xx, x+1);
				}
				cmbModeParam->setSelectedId(1);

				selectedAutorunIndex = cmbMode->getSelectedId();
				selectedDelayParam = cmbModeParam->getSelectedId() - 1;

			}
		}

		void autoUiUnset() {
			if ((client->app.attributes & APP_ATTRIBUTES_AUTORUN) != 0) {
				cmbMode->removeListener(cmbListener);
				cmbModeParam->removeListener(cmbListener);
			}
		}

		int autoUiSelectedMode()
		{
			return selectedAutorunIndex;
		}

		int autoUiSelectedModeParam()
		{
			return selectedDelayParam;
		}

	};

	using namespace autorunUiManagement;




	/*****************************************************************************
	Switch mode
	*******************************************/
	
	static modSwitchedCBType * endCB;
	static ComboBox * cmbSwitch;


	void initSwitchModeCB(ComboBox * cmbSwitchp)
	{
		cmbSwitch = cmbSwitchp;
		cmbSwitch->addListener(cmbListener);


		cmbSwitch->setTextWhenNothingSelected (n02::LUTF16(LID_SWIT));
		cmbSwitch->clear();

		{ // modes
			ModuleAbstraction02 * mod = n02::modHelper.modHandler->find(MTYPE02_TRANSPORT);
			if (mod != 0) {
				do {
					cmbSwitch->addItem(mod->name, mod->index);
				} while ((mod = n02::modHelper.modHandler->findNext(mod))!=0);
			}
		}
	}

	void setSwitchModeCB(modSwitchedCBType * cb)
	{
		endCB = cb;
	}



	static int recordingEnabled = 1;
	static ToggleButton * chkRecord;


	void recorderCBInitialize(ToggleButton * chkRecorder)
	{
		chkRecord = chkRecorder;
		chkRecord->setButtonText (n02::LUTF16(LID_KAILLERA_REC));

		if (modHelper.isRecorderLoaded()) {
			chkRecord->setToggleState(recordingEnabled!=0, false);
		} else {
			chkRecord->setToggleState((recordingEnabled=0)!=0, false);
			chkRecord->setEnabled(false);
		}

		chkRecord->addButtonListener (cmbListener);

	}


	int recorderCBGetRecordingEnabled(){
		return recordingEnabled;
	}



	void  auiComboBoxListener::comboBoxChanged (ComboBox *comboBoxThatHasChanged) {
		if (comboBoxThatHasChanged == cmbMode)
		{
			selectedAutorunIndex = cmbMode->getSelectedId();
		}
		else if (comboBoxThatHasChanged == cmbModeParam)
		{
			selectedDelayParam = cmbModeParam->getSelectedId() -1;
		}
		else if (comboBoxThatHasChanged == cmbSwitch)
		{
			modHelper.activeTransportByIndex(cmbSwitch->getSelectedId());
			cmbSwitch->removeListener(cmbListener);
			endCB();			
		}
	}
	void auiComboBoxListener::buttonClicked (Button* buttonThatWasClicked)
	{
		if (buttonThatWasClicked == chkRecord)
		{
			recordingEnabled = chkRecord->getToggleState()? 1:0;
		}
	}





	/*****************************************************************************
	Global shit
	*******************************************/

	void uiHelpersInitialize()
	{
		cmbListener = new auiComboBoxListener;
		gameSelectInitialize();
	}

	void uiHelpersTerminate()
	{
		delete cmbListener;
		gameSelectTerminate();
	}


};

