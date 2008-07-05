#include "modKaillera.h"
#include "module.h"
#include "common.h"
#include "clientgui.h"
#include "juceKailleraServerSelect.h"
#include "addEditIP.h"
namespace n02 {

	namespace kaillera {

	
		SIMPLEWINDOW(ModKailleraServerSelect, MOD_KAILLERA_FULL_NAME, Colours::whitesmoke, DocumentWindow::allButtons, juceKailleraServerSelect, 580, 330);

		volatile int guiIsRunning = 0;
		PosixThread * gui = 0;

		TCHAR uiUsername[32];
		TCHAR uiQuitMessage[128];
		TCHAR uiLastIP[128];
		int uiConnectionSetting;
		DynamicOrderedArray<TCHAR*> uiServersIP;
		DynamicOrderedArray<TCHAR*> uiServersName;

		CONFIG_START(kailleraConfig)
		CONFIG_STRVAR(_T("nick"), uiUsername, 32, _T("Ape"))
		CONFIG_STRVAR(_T("qmsg"), uiQuitMessage, 128, _T("Ape Escaped!"))
		CONFIG_STRVAR(_T("ip"), uiLastIP, 128, _T("127.0.0.1:27886"))
		CONFIG_INTVAR(_T("connection"), uiConnectionSetting, 1)
		CONFIG_STRLIST(_T("servers_ip"), uiServersIP, 128)
		CONFIG_STRLIST(_T("servers_name"), uiServersName, 128)
		CONFIG_END

		void ModKailleraServerSelect::OnClose(){
			ModKailleraServerSelect::cmponnt->saveConfig();
			guiIsRunning = 0;
			if (gui != 0)
				gui->notify();
		}

		void uiModChangeCallback(int index) {
			ModKailleraServerSelect::OnClose();
			modHelper.activeTransportByIndex(index);
			delete ModKailleraServerSelect::window;
			ModKailleraServerSelect::window = 0;
		}

		void uibtnConnectCallback(){
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Callback", "Connect");
		}
		void uibtnPingCallback(){
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Callback", "Ping");
		}
		void uibtnMastersListCallback(){
			AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Callback", "List");
		}

		void uiAddServer(){
			if (AddNewIP(ModKailleraServerSelect::window)) {
				uiServersName.addItem(_tcsdup(addEditGetName()));
				uiServersIP.addItem(_tcsdup(addEditGetIP()));
			}
		}

		void uiEditServer(int index){
			if (index >= 0 && index < uiServersName.itemsCount()) {
				if (EditIP(ModKailleraServerSelect::window, uiServersName[index], uiServersIP[index])) {
					delete uiServersName[index];
					delete uiServersIP[index];
					uiServersName[index] = _tcsdup(addEditGetName());
					uiServersIP[index] = _tcsdup(addEditGetIP());
				}
			}
		}

		void uiDeleteServer(int index){
			if (index >= 0 && index < uiServersName.itemsCount()) {
				if (AlertWindow::showOkCancelBox(AlertWindow::WarningIcon, "Delete server", "Do you want to delete the seleted item", "Yes", "No")) {
					uiServersName.removeIndex(index);
					uiServersIP.removeIndex(index);
				}
			}
		}

		static void N02CCNV initialize()
		{

		}
		static void N02CCNV terminate()
		{

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
			return 0;
		}

		static void N02CCNV endGame()
		{

		}
		static void N02CCNV sendAsyncData(const void * value, const int len, const int module)
		{

		}
		static void N02CCNV sendSyncData(const void * value, const int len)
		{

		}
		static int  N02CCNV recvSyncData(void * value, const int len)
		{
			return 0;
		}
		static int  N02CCNV syncData(void * value, const int len)
		{
			return 0;
		}
		static int  N02CCNV syncDataCheck (int)
		{
			return 0;
		}
		static void N02CCNV sendChat (const char * message)
		{

		}
		// <gui>
		static void N02CCNV activeteGui()
		{

			ConfigurationManager config(kailleraConfig);
			config.load(_T("kaillera"));

			guiIsRunning = 1;
			ModKailleraServerSelect::createAndShow();

			gui = new PosixThread(true);

			while (guiIsRunning == 1) {
				gui->wait(100000);
			}


			delete gui;
			gui = 0;

			config.save(_T("kaillera"));

		}
		static int  N02CCNV getSelectedAutorunIndex()
		{
			return 0;
		}
		static int  N02CCNV getSelectedAutorunDelay()
		{
			return 0;
		}
		static int  N02CCNV isRecordingEnabled()
		{
			return 0;
		}
		// </gui>

		static int N02CCNV getGameplayType() {
			return GAME_TYPE_GAMEPLAY;
		}

		static void N02CCNV  reserved(int, int)
		{

		}

	};

	STDTRANSPORTDEFNNMS(trModKaillera, kaillera);

	STDMODULE(modKaillera, "n02.kaillera", MTYPE02_TRANSPORT, trModKaillera, 0, 0, MOD02_STATUS_WORKING, "Kaillera module"); 

};

using namespace n02;
using namespace n02::kaillera;


int  juceKailleraServerSelect::ServersListListboxModel::getNumRows (){
	return uiServersIP.itemsCount();
}
void  juceKailleraServerSelect::ServersListListboxModel::paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected){
	if (rowIsSelected)
		g.fillAll (Colour(0xdd,0xdd,0xff));
}
void  juceKailleraServerSelect::ServersListListboxModel::paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
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
void  juceKailleraServerSelect::ServersListListboxModel::cellClicked (int rowNumber, int columnId, const MouseEvent &e){
	if (uiServersIP.itemsCount() > rowNumber) {
		String xxx(uiServersIP.getItem(rowNumber));
		ModKailleraServerSelect::cmponnt->updateIP(xxx);
	}
}
void  juceKailleraServerSelect::ServersListListboxModel::cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e){
	juceKailleraServerSelect::ServersListListboxModel::returnKeyPressed(rowNumber);
}
void  juceKailleraServerSelect::ServersListListboxModel::deleteKeyPressed (int lastRowSelected){
	uiDeleteServer(lastRowSelected);
	ModKailleraServerSelect::cmponnt->updateLV();
}
void  juceKailleraServerSelect::ServersListListboxModel::returnKeyPressed (int lastRowSelected){
	if (lastRowSelected >= 0 && uiServersIP.itemsCount() > lastRowSelected) {
		String xxx(uiServersIP.getItem(lastRowSelected));
		ModKailleraServerSelect::cmponnt->updateIP(xxx);
		ModKailleraServerSelect::cmponnt->saveConfig();
		uibtnConnectCallback();
	}
}

