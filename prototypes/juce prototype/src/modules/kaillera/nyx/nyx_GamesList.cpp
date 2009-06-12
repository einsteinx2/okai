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

#include "nyx.h"

StaticArray<Game*, 64> GamesList::games;

void GamesList::writeGamesState(Instruction & instr)
{
	for (int x = 0; x < games.itemsCount(); x++) {
		Game * g = games.getItem(x);
		instr.writeString(g->name);
		instr.writeUnsignedInt32(g->id);
		instr.writeString(g->owner->app);
		instr.writeString(g->owner->nick);
		char vc[20];
		sprintf(vc, "%i/%i", g->players.itemsCount(), g->maxUsers);
		instr.writeString(vc);
		instr.writeSignedInt8(g->state);
	}
}

Game * GamesList::findGame(unsigned int id)
{
	for (int x = 0; x < games.itemsCount(); x++) {
		if (id == games.getItem(x)->id)
			return games.getItem(x);
	}
	return 0;
}

void GamesList::step()
{
	for (int x = 0; x < games.itemsCount(); x++) {
		games.getItem(x)->step();
	}
}

void GamesList::removeGame(Game *gr) {
	for (int x = 0; x < games.itemsCount(); x++) {
		Game * g;
		if ((g = games.getItem(x))->id == gr->id) {
			
			LOG(Closing game %i, g->id);

			Instruction kix (GAMESHUT);
			kix.writeUnsignedInt32(gr->id);
			UsersList::sendToAll(kix);

			delete g;
			games.removeIndex(x);
			return;
		}
	}
}

unsigned int GamesList::id = 0;
