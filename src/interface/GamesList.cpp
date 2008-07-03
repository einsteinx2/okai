/******************************************************************************
          .d8888b.   .d8888b.  
         d88P  Y88b d88P  Y88b 
         888    888        888 
88888b.  888    888      .d88P 
888 "88b 888    888  .od888P"  
888  888 888    888 d88P"      
888  888 Y88b  d88P 888"       
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

#include "GamesList.h"
#include "DynamicOrderedArray.h"
#include <string.h>

namespace n02 {

	struct Game {
		char name[128];
		unsigned char maxPlayers;
		unsigned char gameCaps;

		void initialize(char * game, unsigned char max, unsigned char caps)
		{
			strcpy(name, game);
			maxPlayers = max;
			gameCaps = caps;
		}
	};

	typedef Game Game;

	static DynamicOrderedArray<Game, 128> games;
	static int selectedGame;

	int GamesList::getCount() {
		return games.itemsCount();		
	}

	void GamesList::initialize()
	{
		reset();
	}

	void GamesList::terminate()
	{
		reset();
	}

	void GamesList::reset()
	{
		selectedGame = 0;
		games.clearItems();
	}

	void GamesList::add(char * name, int maxPlayers, int caps)
	{

		Game game;
		game.initialize(name, 0xFF & maxPlayers, 0xFF & caps);
		games.addItem(game);

	}
	void GamesList::addCaps(char * name, int caps)
	{
		add(name, 2, caps);
	}


	char * GamesList::start(int * index)
	{
		if (games.itemsCount()){
			if (index) {
				*index = 0;
			}
			return games[0].name;
		}
		if (index) {
			*index = -1;
		}
		return 0;
	}

	char * GamesList::next(int * curIndex)
	{
		if (curIndex) {
			*curIndex = *curIndex + 1;
			if (*curIndex >= 0 && *curIndex < games.itemsCount()) {
				return games[*curIndex].name;
			}
		}
		return 0;	
	}

	char * GamesList::find(const char * game)
	{
		if (game) {
			int curIndex = -1;

			char * gamep = start(&curIndex);
			if (*gamep) {
				do {
					if(strcmp(gamep, game)==0) {
						return gamep;
					}

				} while ((gamep=next(&curIndex))!=0);
			}
		}
		return 0;
	}

	bool GamesList::select(const char * game)
	{
		int curIndex = -1;
		if (game) {
			char * gamep = start(&curIndex);
			if (*gamep) {
				do {
					if (strcmp(gamep, game)==0) {
						selectedGame = curIndex;
						return true;
					}
				} while ((gamep=next(&curIndex))!=0);
			}
		}
		return false;
	}

	bool GamesList::selectByIndex(const int index)
	{
		if (index >= 0 && index < games.itemsCount()) {
			selectedGame = index;
			return true;
		}
		return false;
	}

	int GamesList::selectedMaxPlayers()
	{
		return games[selectedGame].maxPlayers;	
	}
	int GamesList::selectedCaps()
	{
		return games[selectedGame].gameCaps;
	}

};
