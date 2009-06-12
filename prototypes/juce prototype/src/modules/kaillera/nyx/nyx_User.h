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

#define CONNECTING_TIMEOUT_LIMIT 10000
#define PING_CONSIDERATION_RATIO 3
#define PING_LIMIT 4
#define CONNECTION_TIMEOUT 90000

#include "nyx_UserMessaging.h"

class User :
	public UserMessaging
{
public:



public:

	// User class definition

	// user state
	enum {
		CONNECTING=0,
		IDLE=1,
		PLAYING=2,
		DISCONNECTED=3
	} state;


	// general info
	unsigned short id;
	char nick[32];
	char app[128];
	char connection;
	int ping;


	// game info & gameplay data
	int gamePlayerIndex;
	int gamePlayerDelay;
	bool gamePlayerReady;

	void * game;

	DataQueue inBuffer;
	DataQueue outBuffer;

	StaticOrderedArray<void*, 256> inCache;
	StaticOrderedArray<void*, 256> outCache;

	int inputLength;

	// timeout & retransmission vars
	unsigned int lastDataArrival;
	unsigned int lastDataSent;


	// misc
	DynamicOrderedArray<unsigned int, 10> chatTimes;

public:

	// constructor
	User(unsigned short uid);
	~User();

	// called back when a new instruction for the user arrives
	void instructionArrivalCallback(Instruction & ki);
	
	// idle step function
	bool idleStep();

	// when sent a global message
	void sendGlobal(Instruction & i)
	{
		if (state != CONNECTING)
			includeInstruction(i);

		if (state!=PLAYING)
			sendMessage();
	}

	void sendGameData(void *, int);

	void leaveGame();

};
