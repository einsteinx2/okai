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

#include "jucep2pSelect.h"
#include "modP2P.h"
#include "addEditIP.h"
#include "p2p_uiSession.h"

namespace n02 {
	namespace p2p {

		SIMPLEWINDOW(ModP2PSelectWindow, MOD_P2P_FULL_NAME, Colours::whitesmoke, DocumentWindow::allButtons, jucep2pSelect, 600, 344);

		void ModP2PSelectWindow::OnClose() {
			cmponnt->saveConfig();
			Component::getCurrentlyModalComponent()->exitModalState(0);
		}

		void uiModChangeCallback(int index) {
			modHelper.activeTransportByIndex(index);
			ModP2PSelectWindow::window->waitNotifyAndCloseNotify();
		}

		char nick[32];
		char ip[128];
		int port;
		DynamicOrderedArray<char*> ips;
		DynamicOrderedArray<char*> names;
		extern int recordingEnabled;

		// add server button press
		void uiAddServer(){
			if (AddNewIP(ModP2PSelectWindow::window)) {
				names.addItem(strdup(addEditGetName()));
				ips.addItem(strdup(addEditGetIP()));
			}
		}

		// edit server button press
		void uiEditServer(int index){
			if (index >= 0 && index < names.itemsCount()) {
				if (EditIP(ModP2PSelectWindow::window, names[index], ips[index])) {
					delete names[index];
					delete ips[index];
					names[index] = strdup(addEditGetName());
					ips[index] = strdup(addEditGetIP());
				}
			}
		}

		// delete server button press
		void uiDeleteServer(int index){
			if (index >= 0 && index < names.itemsCount()) {
				if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Delete", "Do you want to delete the seleted item", "Yes", "No")) {
					names.removeIndex(index);
					ips.removeIndex(index);
				}
			}
		}


		void uiConnect()
		{
			ModP2PSelectWindow::window->setVisible(false);
			sessionRun(true);
			ModP2PSelectWindow::window->setVisible(true);
		}
		void uiHost()
		{
			ModP2PSelectWindow::window->setVisible(false);
			sessionRun(false);
			ModP2PSelectWindow::window->setVisible(true);
		}



		CONFIG_START(p2pConfig)
			CONFIG_STRVAR("nick", nick, 32, "Ape")
			CONFIG_STRVAR("ip", ip, 128, "127.0.0.1:27886")
			CONFIG_INTVAR("port", port, 27886)
			CONFIG_STRLIST("ips", ips, 128)
			CONFIG_STRLIST("names", names, 128)
			CONFIG_INTVAR("record", recordingEnabled, 1)
			CONFIG_END;

		void N02CCNV activeteGui()
		{
			TRACE(); 
			if (GuiIsJuceThread()) {
				TRACE(); ConfigurationManager config(p2pConfig);
				TRACE(); config.load("p2p");

				TRACE(); ModP2PSelectWindow::createAndShowModal();
				TRACE(); ModP2PSelectWindow::deleteAndZeroWindow();

				TRACE(); config.save("p2p");
			} else {
				GuiJUCEThreadCallbackLock(activeteGui);
			}
		}
		int  N02CCNV getSelectedAutorunIndex()
		{
			return selectedAutorunIndex;
		}
		int  N02CCNV getSelectedAutorunDelay()
		{
			return selectedDelayParam;
		}
		int  N02CCNV isRecordingEnabled()
		{
			return recordingEnabled;
		}

		int  StoredListListboxModel::getNumRows()
		{
			return names.itemsCount();
		}
		void  StoredListListboxModel::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
		{
			if (rowIsSelected)
				g.fillAll (Colour(0xdd,0xdd,0xff));
		}
		void  StoredListListboxModel::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
		{
			g.setColour (Colours::black);
			if (columnId == 2) {
				const String text (FROMUTF8(ips.getItem(rowNumber)));
				g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
			} else {
				const String text (FROMUTF8(names.getItem(rowNumber)));
				g.drawText (text, 4, 0, width - 6, height, Justification::centredLeft, true);
			}
			g.setColour (Colours::black.withAlpha (0.2f));
			g.fillRect (width - 1, 0, 1, height);
		}
		void  StoredListListboxModel::cellClicked (int rowNumber, int columnId, const MouseEvent &e)
		{
			if (ips.itemsCount() > rowNumber) {
				ModP2PSelectWindow::window->postCommandMessage(reinterpret_cast<int>(ips[rowNumber]));
			}
		}
		void  StoredListListboxModel::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e)
		{
			//ServersListListboxModel::returnKeyPressed(rowNumber);
		}
		void  StoredListListboxModel::deleteKeyPressed (int lastRowSelected)
		{
			//uiDeleteServer(lastRowSelected);
			//ModKailleraServerSelect::cmponnt->updateLV();
		}
		void  StoredListListboxModel::returnKeyPressed (int lastRowSelected)
		{
			if (lastRowSelected >= 0 && ips.itemsCount() > lastRowSelected) {
				ModP2PSelectWindow::window->postCommandMessage(reinterpret_cast<int>(ips[lastRowSelected]));
				// String xxx(uiServersIP.getItem(lastRowSelected));
				//ModKailleraServerSelect::cmponnt->updateIP(xxx);
				//ModKailleraServerSelect::cmponnt->saveConfig();
				//uibtnConnectCallback();
			}
		}
	};
};

