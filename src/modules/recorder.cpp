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
#include "recorder.h"
#include "common.h"

/******************************************************************************



*********************************************************************/

namespace n02 {

	// IDEA: Add multiple recorder suport?

	extern ExternalModuleHelper02 modHelper;

	static int N02CCNV  recorderDefaultStart (const char * gameName, const int playerNo, const int totalPlayers, const char * comments)
	{
		LOG(%s; %i/%i; %s, gameName, playerNo, totalPlayers, comments? comments:"");
		return 0;
	}


	static void N02CCNV  recorderDefaultStop ()
	{
		LOG(0);
	}

	int N02CCNV  recorderDefaultAddMetadata (const char * dataName, const char * data)
	{
		LOG(META %s=%s, dataName? dataName:"", data? data:"");
		return 0;
	}

	static int N02CCNV  recorderDefaultAddComment (const char * comment)
	{
		LOG(%s, comment? comment:"");
		return 0;
	}

	static int N02CCNV  recorderDefaultAddGameSyncData (const void * value, const int len)
	{
		char strBuffer[256];
		if (len > 0) {
			StringUtils::bytesToStr(strBuffer, const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(value)), len);
			LOG(%s; %i; %i, strBuffer, len);
		}
		return len;
	}


	static int N02CCNV  recorderDefaultAddSyncData (const void * value, const int len)
	{ 
		char strBuffer[256];
#if 0
		if (len > 0) {
			StringUtils::bytesToStr(strBuffer, const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(value)), len);
			LOG(%s; %i; %i, strBuffer, len);
		}
#endif
		return len;
	}

	static int N02CCNV  recorderDefaultAddAsyncData (const void * value, const int len)
	{
		char strBuffer[256];
		StringUtils::bytesToStr(strBuffer, const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(value)), len);
		LOG(%s; %i; %i, strBuffer, len);
		return len;
	}


	static int N02CCNV  recorderDefaultAddChat (const char * username, const char * chat)
	{
		//LOG(%s; %s, username, chat);
		return 0;
	}


	static int N02CCNV  recorderDefaultAddDrop (const char * username, int playerNo)
	{
		LOG(%s; %i, username, playerNo);
		return 0;
	}

	static void N02CCNV  recorderDefaultReserved(int, int)
	{
		LOG(0);
	}

	static RecorderInterface02 defaultRecorder = {
		recorderDefaultStart,
		recorderDefaultStop,
		recorderDefaultAddMetadata,
		recorderDefaultAddComment,
		recorderDefaultAddGameSyncData,
		recorderDefaultAddSyncData,
		recorderDefaultAddAsyncData,
		recorderDefaultAddChat,
		recorderDefaultAddDrop,
		recorderDefaultReserved,
		recorderDefaultReserved,
		recorderDefaultReserved,
		recorderDefaultReserved
	};

	RecorderInterface02 recorder;

	void recorderReset(){
		recorder = defaultRecorder;
	}

	void recorderResetCheck() {
		ModuleAbstraction02 * mab;

		recorderReset();

		if ((mab = modHelper.modHandler->find(MTYPE02_RECORDER)) != 0) {
			mab->getInterface(&recorder);
		}
	}

	void recorderInitialize(){
		 recorderResetCheck();
	}
	void recorderTerminate(){
		recorderReset();
	}	

	bool N02CCNV recorderIsLoaded() {
		ModuleAbstraction02 * mab;
		recorderReset();
		if ((mab = modHelper.modHandler->find(MTYPE02_RECORDER))!=0) {
			mab->getInterface(&recorder);
			return true;
		}
		return false;
	}

};
