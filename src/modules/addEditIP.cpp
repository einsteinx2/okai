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
#include "common.h"
#include "juceAddEditIPDialog.h"
#include "clientgui.h"
#include "locid.h"

namespace n02 {

    namespace addedit {
        int returnValue;
        char name[128];
        char ip[128];
    };

    bool AddNewIP(Component * parent)
	{
		TRACE(); const MessageManagerLock ml;
		require(ml.lockWasGained());
        TRACE(); addedit::returnValue = 0;
        TRACE(); juceAddEditIPDialog * newCmp;
		TRACE(); DialogWindow::showModalDialog(LUTF16(LID_ADD1), newCmp = new juceAddEditIPDialog, parent, Colours::whitesmoke, true);
		TRACE(); delete newCmp;
		TRACE(); return addedit::returnValue == 1;
    }

    bool EditIP(Component * parent, char * NAME, char * IP)
	{
		TRACE(); const MessageManagerLock ml;
		require(ml.lockWasGained());
        TRACE(); strcpy(addEditGetIP(), IP);
        TRACE(); strcpy(addEditGetName(), NAME);
        TRACE(); addedit::returnValue = 1;
        TRACE(); juceAddEditIPDialog * newCmp;
		TRACE(); DialogWindow::showModalDialog(LUTF16(LID_EDT1), newCmp = new juceAddEditIPDialog, parent, Colours::whitesmoke, true);
		TRACE(); delete newCmp;
		TRACE(); return addedit::returnValue == 1;
    }
    char * addEditGetIP()
	{
        return addedit::ip;
    }
    char * addEditGetName()
	{
        return addedit::name;
    }
};

