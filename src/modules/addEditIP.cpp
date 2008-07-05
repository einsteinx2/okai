
#include "addEditIP.h"
#include "common.h"
#include "juceAddEditIPDialog.h"
#include "clientgui.h"

namespace n02 {

	namespace addedit {
		int returnValue;
		TCHAR name[128];
		TCHAR ip[128];
	};

	bool AddNewIP(Component * parent) {
		addedit::returnValue = 0;
		juceAddEditIPDialog * newCmp;
		DialogWindow::showModalDialog(T("Add Item"), newCmp = new juceAddEditIPDialog, parent, Colours::whitesmoke, true);
		return addedit::returnValue == 1;
	}
	bool EditIP(Component * parent, TCHAR * NAME, TCHAR * IP) {
		_tcscpy(addEditGetIP(), IP);
		_tcscpy(addEditGetName(), NAME);

		addedit::returnValue = 1;

		juceAddEditIPDialog * newCmp;

		DialogWindow::showModalDialog(T("Edit Item"), newCmp = new juceAddEditIPDialog, parent, Colours::whitesmoke, true);

		return addedit::returnValue == 1;
	}
	TCHAR * addEditGetIP() {
		return addedit::ip;
	}
	TCHAR * addEditGetName() {
		return addedit::name;
	}
};

