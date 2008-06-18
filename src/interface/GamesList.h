#pragma once

namespace n02 {

    // All games should have this
#define GCAPS_TRAD		1
    // Can play around with sates
#define GCAPS_STATES	2

    class GamesList {

        // Initialization
        static void initialize();
        static void terminate();
        static void reset();

        // Incrementing
        static void add(char * name, int maxPlayers = 2, int caps = GCAPS_TRAD);
        static void addCaps(char * name, int caps);

        // Iteration
        static char * start(int * index);
        static char * next(int * curIndex);
        static char * find(const char * game);
        static bool select(const char * game);
		static bool selectByIndex(const int index);
        static int selectedMaxPlayers();
        static int selectedCaps();

    };

};
