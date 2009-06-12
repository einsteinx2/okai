/******************************************************************************
O                                                           8d8b. Yb  dP Yb dP 
O                                                           8P Y8  YbdP   `8.  
Open Kaillera: Kaillera Server                              8   8   dP   dP Yb 
*******************************************************************dP**********
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

#include "nyx_UsersList.h"


StaticArray<User*, 128> UsersList::users;


int UsersList::getConnectingCount() {
	int count = 0;
	for (int x = 0; x < users.itemsCount(); x++) {
		if (users[x]->state == User::CONNECTING)
			count++;
	}
	return count;
}

void UsersList::step()
{
	for (int x = 0; x < users.itemsCount(); x++) {
		User * u;
		if ((u = users.getItem(x))->idleStep()){
			delete u;
			users.removeIndex(x);
			x--;
		}
	}
}


unsigned short UsersList::id = 56;

void UsersList::writeUsersState(Instruction & i)
{
	for (int x = 0; x < users.itemsCount(); x++) {
		User * u = users[x];
		if (u->state != User::CONNECTING) {
			i.writeString(u->nick);
			i.writeSignedInt32(u->ping);
			i.writeSignedInt8(u->state);
			i.writeSignedInt16(u->id);
			i.writeSignedInt8(u->connection);
		}
	}
}

void UsersList::sendToAll (Instruction & i)
{
	for (int x = 0; x < users.itemsCount(); x++) {
		users[x]->sendGlobal(i);
	}
}


User * UsersList::findUser(unsigned short id)
{
	for (int x = 0; x < users.itemsCount(); x++) {
		if (users[x]->id == id)
			return users[x];
	}
	return 0;
}
