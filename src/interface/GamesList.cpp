#include "GamesList.h"
#include "DynamicOrderedArray.h"

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
