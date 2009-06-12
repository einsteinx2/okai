#include "okaimodules.h"

// -------------------------------------------------------------------------
// --  Game List Functions --
char * gameList = 0;
void gameListInit(char * gamelist) {
	if (gameList==0) {
		char * ptr = gamelist;
		int len = 0;
		if (ptr != 0) {
			while (*ptr!= 0){
				len += strlen(ptr) + 1;
				ptr = gamelist + len;
			}
			len += 1;
			gameList = (char*)malloc(len);
			memcpy(gameList, gamelist, len);
		}
	}
}

void gameListTerm() {
	if (gameList != 0)
		free(gameList);
	gameList = 0;
}

char * OKAI_CCNV gameListStart() {
	return gameList;
}
char * OKAI_CCNV gameListNext(char * current) {
	current += strlen(current) + 1;
	return *current?current:0;
}

char * OKAI_CCNV gameListFind(char * game){
	//kprintf("gameListFind %s", game);
	char * gamep = gameListStart();
	if (*gamep) {
		do {
			//kprintf(" - - %s", gamep);
			if(strcmp(gamep, game)==0)
				return gamep;
		} while (gamep=gameListNext(gamep));
	}
	return 0;
}
