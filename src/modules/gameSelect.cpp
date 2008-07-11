#include "gameSelect.h"
#include "juceGameSelect.h"
#include "GamesList.h"
#include "juceAbortableStatusWindow.h"

namespace n02 {

	static int selectedGameIndex;
	static int gamesCount;
	static juceGameSelect * newCmp;

	char * getSelectedGame(Component* parent) {
		if (GamesList::getCount() > 0) {
			selectedGameIndex = -1;

			int r = DialogWindow::showModalDialog(T("Select game"), newCmp = new juceGameSelect, parent, Colours::whitesmoke, true);
			delete newCmp;

			if (r==0)
				return 0;
			else
				return GamesList::current(&selectedGameIndex);
		} else {
			AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "No games in list");
			return 0;
		}
	}

	int  GameSelectLB::getNumRows () {
		return GamesList::getCount();
	}
	void  GameSelectLB::paintListBoxItem (int rowNumber, Graphics &g, int width, int height, bool rowIsSelected) {
		if (rowIsSelected)
			g.fillAll (Colour(0xdd,0xdd,0xff));
		selectedGameIndex = rowNumber;
		String text(GamesList::current(&selectedGameIndex));
		g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
	}
	void  GameSelectLB::listBoxItemClicked (int row, const MouseEvent &e){
		GamesList::selectByIndex(selectedGameIndex = row);

		juce::String status;

		status.printf(T("%i players"), GamesList::selectedMaxPlayers());

		if (GamesList::selectedCaps() & GCAPS_STATES)
			status.append(T(", states"), 42);

		newCmp->updateGameInfo(status);
	}

	void  GameSelectLB::listBoxItemDoubleClicked (int row, const MouseEvent &e){
		selectedGameIndex = row;
		newCmp->closeUp();
	}
	void  GameSelectLB::returnKeyPressed (int lastRowSelected){
		selectedGameIndex = lastRowSelected;
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
				if ((currentGameCaps & GAME_CAPS_STATES) != 0 && (ra.attributes & RA_STATES) ==0)
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

