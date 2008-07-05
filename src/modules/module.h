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
#include "n02module.h"

namespace n02 {
	void modulesInitialize();
	void modulesTerminate();
	extern ExternalModuleHelper02 modHelper;
	extern n02ClientInterface * client;
};


// simple always working modules
#define STDMODULE(N, MN, TN, ST, C, CC, S, D)							\
static int N02CCNV N##GetStatus (){										\
	return S;															\
}																		\
static int N02CCNV N##GetInterface (void * interf){						\
	memcpy(interf, &ST, sizeof(ST));									\
	return 0;															\
}																		\
ModuleAbstraction02 N = {												\
	-1,																	\
	MN,																	\
	TN,																	\
	D,																	\
	N##GetStatus,														\
	N##GetInterface,													\
	C,																	\
	CC																	\
}


#define STDTRANSPORTDEFN(NAME)				\
TransportModule02 NAME = {					\
	initialize,								\
	terminate,								\
	activete,								\
	step,									\
	synchronizeGame,						\
	endGame,								\
	sendAsyncData,							\
	sendSyncData,							\
	recvSyncData,							\
	syncData,								\
	syncDataCheck,							\
	sendChat,								\
	activeteGui,							\
	getSelectedAutorunIndex,				\
	getSelectedAutorunDelay,				\
	isRecordingEnabled,						\
	getGameplayType,						\
	reserved,								\
	reserved,								\
	reserved								\
}

#define STDTRANSPORTDEFNNMS(NAME, NAMESPACE)			\
TransportModule02 NAME = {								\
	NAMESPACE##::initialize,							\
	NAMESPACE##::terminate,								\
	NAMESPACE##::activete,								\
	NAMESPACE##::step,									\
	NAMESPACE##::synchronizeGame,						\
	NAMESPACE##::endGame,								\
	NAMESPACE##::sendAsyncData,							\
	NAMESPACE##::sendSyncData,							\
	NAMESPACE##::recvSyncData,							\
	NAMESPACE##::syncData,								\
	NAMESPACE##::syncDataCheck,							\
	NAMESPACE##::sendChat,								\
	NAMESPACE##::activeteGui,							\
	NAMESPACE##::getSelectedAutorunIndex,				\
	NAMESPACE##::getSelectedAutorunDelay,				\
	NAMESPACE##::isRecordingEnabled,					\
	NAMESPACE##::getGameplayType,						\
	NAMESPACE##::reserved,								\
	NAMESPACE##::reserved,								\
	NAMESPACE##::reserved								\
}



/* STD module start code */

/*




static void N02CCNV initialize()
{

}
static void N02CCNV terminate()
{

}
static void N02CCNV activete()
{

}
static int  N02CCNV step()
{
return 0;
}
static int  N02CCNV synchronizeGame(void * syncData, int len)
{
return 0;
}

static void N02CCNV endGame()
{

}
static void N02CCNV sendAsyncData(const void * value, const int len, const int module)
{

}
static void N02CCNV sendSyncData(const void * value, const int len)
{

}
static int  N02CCNV recvSyncData(void * value, const int len)
{
return 0;
}
static int  N02CCNV syncData(void * value, const int len)
{
return 0;
}
static int  N02CCNV syncDataCheck (int)
{
return 0;
}
static void N02CCNV sendChat (const char * message)
{

}
// <gui>
static void N02CCNV activeteGui()
{

}
static int  N02CCNV getSelectedAutorunIndex()
{
return 0;
}
static int  N02CCNV getSelectedAutorunDelay()
{
return 0;
}
static int  N02CCNV isRecordingEnabled()
{
return 0;
}
// </gui>

static int N02CCNV getGameplayType() {
	return GAME_TYPE_GAMEPLAY;
}

static void N02CCNV  reserved(int, int)
{

}

*/
