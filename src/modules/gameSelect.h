#pragma once
#include "common.h"
#include "module.h"
#include "clientgui.h"

namespace n02 {

	char * getSelectedGame(Component*);

	void setupAutorunUIItems(int currentGameCaps, ComboBox * cmbRun, ComboBox * cmbDelay, Label * lblRun, Label * lblDelay);

	void showCancelable();
	bool updateCancelable(char * status);
	bool isCancelableCanceled();
	void hideCancelable();


};

