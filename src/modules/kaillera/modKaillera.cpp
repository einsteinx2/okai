#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerSelect.h"
#include "addEditIP.h"
#include "kaillera_ClientCore.h"

namespace n02 {

	namespace kaillera {


		/************************************************************
		** Global sync vars
		*******************************/
		volatile int guiIsRunning = 0;
		PosixThread * gui = 0;

		/************************************************************
		** vars
		*******************************/
		TCHAR uiUsername[32];
		TCHAR uiQuitMessage[128];
		TCHAR uiLastIP[128];
		int uiConnectionSetting;
		int recordingEnabled;
		int selectedAutorunIndex;
		int selectedDelayParam;
		DynamicOrderedArray<TCHAR*> uiServersIP;
		DynamicOrderedArray<TCHAR*> uiServersName;


		/************************************************************
		** Config Table
		*******************************/
		CONFIG_START(kailleraConfig)
		CONFIG_STRVAR(_T("nick"), uiUsername, 32, _T("Ape"))
		CONFIG_STRVAR(_T("qmsg"), uiQuitMessage, 128, _T("Ape Escaped!"))
		CONFIG_STRVAR(_T("ip"), uiLastIP, 128, _T("127.0.0.1:27886"))
		CONFIG_INTVAR(_T("connection"), uiConnectionSetting, 1)
		CONFIG_INTVAR(_T("record"), recordingEnabled, 1)
		CONFIG_STRLIST(_T("servers_ip"), uiServersIP, 128)
		CONFIG_STRLIST(_T("servers_name"), uiServersName, 128)
		CONFIG_END

		/************************************************************
		** Misc Prototypes
		*******************************/
		// after copying IP to uiLastIP, call this to proceed for connecting
		void ConnectCallback();

		/************************************************************
		** GUI Window
		*******************************/

		SIMPLEWINDOW(ModKailleraServerSelect, MOD_KAILLERA_FULL_NAME, Colours::whitesmoke, DocumentWindow::allButtons, juceKailleraServerSelect, 580, 330);

		// Close button press
		void ModKailleraServerSelect::OnClose(){
			ModKailleraServerSelect::cmponnt->saveConfig();
			guiIsRunning = 0;
			if (gui != 0)
				gui->notify();
		}

		// Mod changing
		void uiModChangeCallback(int index) {
			ModKailleraServerSelect::OnClose();
			modHelper.activeTransportByIndex(index);
			delete ModKailleraServerSelect::window;
			ModKailleraServerSelect::window = 0;
		}

		
		/************************************************************
		** GUI Callbacks
		*******************************/
		// Connect button press
		void uibtnConnectCallback() {
			ModKailleraServerSelect::window->setVisible(false);
			ConnectCallback();
			ModKailleraServerSelect::window->setVisible(true);
		}

		// Ping button press
		void uibtnPingCallback(){
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Callback", "Ping");
		}

		// Masters list button press
		void uibtnMastersListCallback(){
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Callback", "List");
		}

		// add server button press
		void uiAddServer(){
			if (AddNewIP(ModKailleraServerSelect::window)) {
				uiServersName.addItem(_tcsdup(addEditGetName()));
				uiServersIP.addItem(_tcsdup(addEditGetIP()));
			}
		}

		// edit server button press
		void uiEditServer(int index){
			if (index >= 0 && index < uiServersName.itemsCount()) {
				if (EditIP(ModKailleraServerSelect::window, uiServersName[index], uiServersIP[index])) {
					delete uiServersName[index];
					delete uiServersIP[index];
					uiServersName[index] = _tcsdup(addEditGetName());
					uiServersIP[index] = _tcsdup(addEditGetIP());
					ModKailleraServerSelect::cmponnt->redrawServersRow(index);
				}
			}
		}

		// delete server button press
		void uiDeleteServer(int index){
			if (index >= 0 && index < uiServersName.itemsCount()) {
				if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Delete server", "Do you want to delete the seleted item", "Yes", "No")) {
					uiServersName.removeIndex(index);
					uiServersIP.removeIndex(index);
				}
			}
		}


		/************************************************************
		** Module interface
		*******************************/

		// Automation... Not planning to do it for kaillera module
		static void N02CCNV initialize(){}
		static void N02CCNV terminate(){}
		static void N02CCNV activete(){}

		// GUI Entry point
		static void N02CCNV activeteGui()
		{

			// Load config
			ConfigurationManager config(kailleraConfig);
			config.load(_T("kaillera"));

			// set up synchronization stuff
			guiIsRunning = 1;

			// draw our window
			ModKailleraServerSelect::createAndShow();

			// well this is preety much it... wait till we're done
			gui = new PosixThread(true);
			while (guiIsRunning == 1) {
				gui->wait(100000);
			}
			delete gui;
			gui = 0;

			// Save config
			config.save(_T("kaillera"));

		}

		// etc...
		static int  N02CCNV getSelectedAutorunIndex()
		{
			return selectedAutorunIndex;
		}
		static int  N02CCNV getSelectedAutorunDelay()
		{
			return selectedDelayParam;
		}
		static int  N02CCNV isRecordingEnabled()
		{
			return recordingEnabled;
		}
		static int N02CCNV getGameplayType()
		{
			return GAME_TYPE_GAMEPLAY;
		}
		static void N02CCNV  reserved(int, int){}



		/************************************************************
		** List box model for servers list...
		*******************************/
		int  ServersListListboxModel::getNumRows()
		{
			return uiServersIP.itemsCount();
		}
		void  ServersListListboxModel::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
		{
			if (rowIsSelected)
				g.fillAll (Colour(0xdd,0xdd,0xff));
		}
		void  ServersListListboxModel::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
		{
			g.setColour (Colours::black);
			if (columnId == 2) {
				const String text (uiServersIP.getItem(rowNumber));
				g.drawText (text, 2, 0, width - 4, height, Justification::centredLeft, true);
			} else {
				const String text (uiServersName.getItem(rowNumber));
				g.drawText (text, 4, 0, width - 6, height, Justification::centredLeft, true);
			}
			g.setColour (Colours::black.withAlpha (0.2f));
			g.fillRect (width - 1, 0, 1, height);
		}
		void  ServersListListboxModel::cellClicked (int rowNumber, int columnId, const MouseEvent &e)
		{
			if (uiServersIP.itemsCount() > rowNumber) {
				String xxx(uiServersIP.getItem(rowNumber));
				ModKailleraServerSelect::cmponnt->updateIP(xxx);
			}
		}
		void  ServersListListboxModel::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e)
		{
			ServersListListboxModel::returnKeyPressed(rowNumber);
		}
		void  ServersListListboxModel::deleteKeyPressed (int lastRowSelected)
		{
			uiDeleteServer(lastRowSelected);
			ModKailleraServerSelect::cmponnt->updateLV();
		}
		void  ServersListListboxModel::returnKeyPressed (int lastRowSelected)
		{
			if (lastRowSelected >= 0 && uiServersIP.itemsCount() > lastRowSelected) {
				String xxx(uiServersIP.getItem(lastRowSelected));
				ModKailleraServerSelect::cmponnt->updateIP(xxx);
				ModKailleraServerSelect::cmponnt->saveConfig();
				uibtnConnectCallback();
			}
		}
	};

	// Transport module definition
	STDTRANSPORTDEFNNMS(trModKaillera, kaillera);

	// abstraction stuff...
	STDMODULE(modKaillera, "n02.kaillera", MTYPE02_TRANSPORT, trModKaillera, 0, 0, MOD02_STATUS_WORKING, "Kaillera module"); 

};
