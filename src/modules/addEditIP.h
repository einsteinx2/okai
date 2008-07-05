#pragma once
#include "common.h"
#include "clientgui.h"
namespace n02 {
	bool AddNewIP(Component * parent);
	bool EditIP(Component * parent, TCHAR * name, TCHAR * IP);
	TCHAR * addEditGetIP();
	TCHAR * addEditGetName();
};
