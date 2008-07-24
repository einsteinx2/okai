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

#pragma once

#include "nyx_User.h"


// game class
class Game
{
	// id
	unsigned int id;

	//state
	enum {
		WAITING,
		LOADING,
		RUNNING,
		PURGED
	} state;

	// owner
	User * owner;

	// players
	DynamicOrderedArray<User*, 8> players;

	// misc info
	char name[128];
	char app[128];
	int maxUsers;

	// gameplay info

	unsigned int frame;

	// constructor
	Game(const char * name, User & user);

	// starts game
	void start();

	// game state for when a new player joins
	void writeGameState(Instruction &);

	// add and remove players
	void addUser(User & user);
	bool removeUser(User & player);

	// sed status update
	void updateStatus();
	// kick an user out of the game
	void kickUser(unsigned short user);

	bool step();

};
