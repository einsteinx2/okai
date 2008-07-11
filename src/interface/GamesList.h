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

#pragma once
// TODO : Review
// TODO : Test
namespace n02 {

    // All games should have this
#define GCAPS_NORMAL		1
    // Can play around with sates
#define GCAPS_STATES		8

    class GamesList {
	public:
        // Initialization
        static void initialize();
        static void terminate();
        static void reset();

        // Incrementing
        static void add(char * name, int maxPlayers = 2, int caps = GCAPS_NORMAL);
        static void addCaps(char * name, int caps);

        // Iteration
        static char * start(int * index);
        static char * next(int * curIndex);
		static char * current(int * curIndex);
        static char * find(const char * game);
        static bool select(const char * game);
		static bool selectByIndex(const int index);
        static int selectedMaxPlayers();
        static int selectedCaps();
		static int getCount();

    };

};
