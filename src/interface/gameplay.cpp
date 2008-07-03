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
#include "gameplay.h"
#include "transport.h"
#include "recorder.h"
#include "common.h"


namespace n02 {
	static int  N02CCNV gameplayCallbackGameLoad(char *, int, int, int){
		LOG(0);
		return 0;
	}
	static void N02CCNV gameplayCallbackGameEnd(){
		LOG(0);
	}
	static void N02CCNV gameplayCallbackPlayerDropped(const char *, int ){
		LOG(0);
	}
	static void N02CCNV gameplayCallbackAsyncData(const void * , const int ){
		LOG(0);
	}
	
	static void N02CCNV gameplayCallbackChatReceived(const char * , const char * ){
		LOG(0);
	}
	static void N02CCNV gameplayFcnReserved(int, int){
		LOG(0);
	}

	static int  N02CCNV gameplaySynchronizeGame(void * syncData, int len){
		int return_val = transport.synchronizeGame(syncData, len);
		{
			char strBuffer[256];
			StringUtils::bytesToStr(strBuffer, (unsigned char*)syncData, len);
			LOG(%s; %i; ret=%i, strBuffer, len, return_val);
		}
		recorder.addGameSyncData(syncData, return_val);
		return return_val;
	}
	static void N02CCNV gameplayEndGame(){
		LOG(0);
		transport.endGame();
	}
	static void N02CCNV gameplaySendAsyncData(const void * value, const int len, const int position) {
		char strBuffer[256];
		StringUtils::bytesToStr(strBuffer, (unsigned char*)value, len);
		LOG(%s; %i, strBuffer, len);
		transport.sendAsyncData(value, len, position);	
	}
	static void N02CCNV gameplaySendSyncData(const void * value, const int len){
		char strBuffer[256];
		StringUtils::bytesToStr(strBuffer, (unsigned char*)value, len);
		LOG(%s; %i, strBuffer, len);
		transport.sendSyncData(value, len);
	}
	static int  N02CCNV gameplayRecvSyncData(void * value, const int len){
		int return_val = transport.recvSyncData(value, len);
		recorder.addSyncData(value, return_val);
		{
			char strBuffer[256];
			StringUtils::bytesToStr(strBuffer, (unsigned char*)value, len);
			LOG(%s; %i; ret=%i, strBuffer, len, return_val);
		}

		return return_val;
	}
	static int  N02CCNV gameplaySyncData(void * value, const int len){
		int return_val = transport.syncData(value, len);
		recorder.addSyncData(value, return_val);
		{
			char strBuffer[256];
			StringUtils::bytesToStr(strBuffer, (unsigned char*)value, len);
			LOG(%s; %i; ret=%i, strBuffer, len, return_val);
		}

		return return_val;
	}
	static int N02CCNV gameplaySyncDataCheck () {
		LOG(0);
		return transport.syncDataCheck(0);
	}
	static void N02CCNV gameplaySendChat (char * message){
		LOG(%s, message);
		transport.sendChat(message);
	}

	n02GameplayInterface defaultGameplayInterface = {
		gameplaySynchronizeGame,
		gameplayEndGame,
		gameplaySendAsyncData,
		gameplaySendSyncData,
		gameplayRecvSyncData,
		gameplaySyncData,
		gameplaySyncDataCheck,
		gameplaySendChat,
		gameplayFcnReserved,
		gameplayFcnReserved,
		gameplayFcnReserved,
		gameplayFcnReserved,
		0,
		gameplayCallbackGameLoad,
		gameplayCallbackGameEnd,
		gameplayCallbackPlayerDropped,
		gameplayCallbackAsyncData,
		gameplayCallbackChatReceived,
		gameplayFcnReserved,
		gameplayFcnReserved,
		gameplayFcnReserved
	};

	void gameplayInitialize(){}
	void gameplayTerminate(){}

};


