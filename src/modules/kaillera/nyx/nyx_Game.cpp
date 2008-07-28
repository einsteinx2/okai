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

#include "nyx_Game.h"
#include "nyx_GamesList.h"
#include "nyx_UsersList.h"

Game::Game(const char * name_, User * user): state(WAITING)
{
	strncpy(name, name_, 127);

	owner = user;

	maxUsers = 8;
	id = GamesList::getSpareId();

	LOG(%s created a new game: %i %s, user->nick, id, name);
}

void Game::start()
{
}

Game::~Game()
{

}

void Game::writeGameState(Instruction & i)
{
	i.writeSignedInt32(players.itemsCount());
	for (int x = 0; x < players.itemsCount(); x++) {
		User * u = players.getItem(x);
		i.writeString(u->nick);
		i.writeUnsignedInt32(u->ping);
		i.writeUnsignedInt16(u->id);
		i.writeSignedInt8(u->connection);
	}
}


void Game::addUser(User * user)
{
	players.addItem(user);

	Instruction kix(GAMRJOIN);
	kix.writeSignedInt32((int)this);
	kix.writeString(user->nick);
	kix.writeSignedInt32(user->ping);
	kix.writeUnsignedInt16(user->id);
	kix.writeSignedInt8(user->connection);

	sendToAll(kix);
}

void Game::sendToAll(Instruction & i) {
	for (int x = 0; x < players.itemsCount(); x++) {
		players[x]->sendGlobal(i);
	}
}

bool Game::removeUser(User * player)
{
	LOG(%s left game %i, player->nick, id);

	{
		Instruction kix(GAMRLEAV);
		kix.writeSignedInt16(player->id);
		strcpy(kix.user, player->nick);
		sendToAll(kix);
	}

	for (int x = 0; x < players.itemsCount(); x++) {
		if (players[x] == player) {
			players.removeIndex(x);
			break;
		}
	}

	if (players.itemsCount() == 0)
		return true;

	if (owner == player)
		owner = players[0];

	return false;
}


void Game::updateStatus()
{
	Instruction gs(GAMESTAT);
	gs.writeUnsignedInt32(id);
	gs.writeSignedInt8(state);
	gs.writeSignedInt8(players.itemsCount());
	gs.writeSignedInt8(maxUsers);
	UsersList::sendToAll(gs);
}


void Game::kickUser(unsigned short user)
{
}


bool Game::step()
{
	return players.itemsCount() != 0;
}

