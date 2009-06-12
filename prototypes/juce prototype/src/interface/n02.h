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

#ifndef N02_H
#define N02_H

#if !defined(N02_WIN32) && !defined(N02_LINUX) && !defined(N02_MAC)
#define N02_WIN32
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef N02_WIN32
#define N02CCNV __stdcall
#else
#define N02CCNV
#endif

#define N02_API_VERSION 3

/*****************************************************
README
***************************
Things may prove to be not obvious if someone jumped straight into coding
so to make things more obvious, I thought I should write this. The API more
or less consists of data structure types with function pointers. You will
need to declare variables with those data types in your own code space and
you can perform only operations on them:
reset - Fill in the values of function pointers with proper functions
     and dummy callbacks and initialize its other variables
Once the interface has been filled with appropriate values for its function
pointers, the rest will comes from there.
On an additional note: Do not use this API in combination with other supported
interfaces such as kaillera or PSENet or multiple instances of the same
interface. The internals of the client is highly modular and any attempts at
those may cause the client to not function perperly
For static compilation, define N02_STATIC and use the function suffixed with S
make sure N02_LINUX or N02_MAC is defined for non-windows platform linkage
*************/

	///////////////////////////////////////////////////////////////////////////
	// Basic interfaces

	/******************************************************
	AppInfoInterface
	****************************
	This is used to give information about the host application to the client
	*********************
	name - Application Name. 64 ascii charecters max
	version - Application version in string. 32 chars max e.g. "2.0"
	description - Application Description. Not mandetory
	framerate - Application framerate per second multiplied by 100.
		   e.g. 30 fps => framerate = 3000	
		   ** can change during game load callback.
		   ** Make sure its the same for a particular game for all players.
    attributes - Attributes, do not modify. It's calculated on client
	       activation based on values provided and interfaces used.
	****************/

	typedef struct {
		char name[64];
		char version[32];
		char description[256];
		int framerate;
		int attributes;
	} n02AppInfoInterface;

#define APP_ATTRIBUTES_SINGLEGAME	1
#define APP_ATTRIBUTES_60FPS		2
#define APP_ATTRIBUTES_AUTOMATE		4
#define APP_ATTRIBUTES_HASSTATES	8
#define APP_ATTRIBUTES_AUTORUN		16

#define INTERFACE_APPINFO 1

	/******************************************************
	ClientInfoInterface
	****************************
	This is used for getting information about the client
	*********************
	Metadata
	API version
	****************/

	typedef struct {
		char name[32];
		char version[16];
		char description[128];
		char buildDate[16];
		char buildTime[16];
		int apiVersion;
	} n02ClientInfoInterface;

#define INTERFACE_CLIENTINFO 2

	/******************************************************
	GamelistInterface
	****************************
	For giving a list of games to client. Well, instead of maintaining a
	seperate list both client side and application side, this adds games
	directly to the internal list the client works with.
	*********************
	clear - 
	add - 
	addCaps - 
	*********************
	GAME_CAPS_NORMAL - every game has this flag
	GAME_CAPS_AUTORUN_HASSTATES - if it san support saveing/loading states
	****************/
	typedef struct {
		void (N02CCNV *clear)();
		void (N02CCNV *add)(char * name, int maxPlayers);
		void (N02CCNV *addCaps)(char * name, int maxPlayers, int caps);
	} n02GamelistInterface;

#define GAME_CAPS_NORMAL 1
#define GAME_CAPS_STATES 8

#define INTERFACE_GAMELIST 3

	/******************************************************
	GameplayInterface
	****************************
	*********************
	Do not modify;
		synchronizeGame - 
		endGame - 
		sendAsyncData - 
		sendSyncData - 
		recvSyncData - 
		syncData - do both send and recv in one go. Those familier to kaillera
		    may find this useful
		syncDataCheck - 
		sendChat -
	*********************
	Modifiable:
		defaultInput - 
		callbackGameLoad - 
		callbackGameEnd - 
		callbackPlayerDropped - 
		callbackAsyncData - 
		callbackChatReceived - 
	****************/

	typedef struct {
		/* <do not modify these values> */
		/* Functions */
		int  (N02CCNV * synchronizeGame)	(void * syncData, int len);
		void (N02CCNV * endGame)			();
		void (N02CCNV * sendAsyncData)		(const void * value, const int len, const int mode);
		void (N02CCNV * sendSyncData)		(const void * value, const int len);
		int  (N02CCNV * recvSyncData)		(void * value, const int len);
		int  (N02CCNV * syncData)			(void * value, const int len);
		int  (N02CCNV * syncDataCheck)		(void);
		void (N02CCNV * sendChat)			(char * message);
		void (N02CCNV * reservedAction1)	(int, int);
		void (N02CCNV * reservedAction2)	(int, int);
		void (N02CCNV * reservedAction3)	(int, int);
		void (N02CCNV * reservedAction4)	(int, int);
		/* </do not modify these values> */

		/* Callbacks */
		void * defaultInput;
		int  (N02CCNV * callbackGameLoad) (char *game, int player, int numPlayers, int gameplayType);
		void (N02CCNV * callbackGameEnd) ();
		void (N02CCNV * callbackPlayerDropped) (const char * nick, int playerNumber);
		void (N02CCNV * callbackAsyncData) (const void * data, const int size);
		void (N02CCNV * callbackChatReceived) (const char * sourceUser, const char * message);
		void (N02CCNV * callbackReserved1) (int, int);
		void (N02CCNV * callbackReserved2) (int, int);
		void (N02CCNV * callbackReserved3) (int, int);
	} n02GameplayInterface;

#define GAME_TYPE_GAMEPLAY 1
#define GAME_TYPE_PLAYBACK 2
#define ASYNC_DATA_MODE_BLOCKING 1
#define ASYNC_DATA_MODE_NBLOCKING 2
#define INTERFACE_GAMEPLAY 4

	/******************************************************
	ClientInterface
	****************************
	This is the main API interface.
	*********************
	app - the AppInfoInterface used by the client
	games - Copy of GamelistInterface
	gameplay - the GameplayInterface used by the client
	activate - call this to activate the client.
	    Something like structVar.activate(&structVar);
	extendInterface - function for registering additional interfaces before
	    activation. e.g. client.extendInterface(&automation, sizeof(automation), INTERFACE_AUTOMATION);
	internal0 - do not modify under any circumstances
	****************/
	typedef struct {
		n02AppInfoInterface app;
		n02GamelistInterface games;
		n02GameplayInterface gameplay;
		int  (N02CCNV *activate)	(void*thisStruct);
		int  (N02CCNV *extend)	(void * clientInterfPtr, void*interfStruct, int interStructLen, int interfType);
		void * internal0;
	} n02ClientInterface;

#define INTERFACE_CLIENT 5



	/* dll/ reset function */

#ifdef N02_WIN32

#ifdef N02_EXPORTS

	__declspec(dllexport) int N02CCNV n02ResetInterface(void * Interface, int type);
	__declspec(dllexport) int __cdecl n02ResetInterfaceC(void * Interface, int type);

#else

#ifndef N02_STATIC

	__declspec(dllimport) int N02CCNV n02ResetInterface(void * Interface, int type);
	__declspec(dllimport) int __cdecl n02ResetInterfaceC(void * Interface, int type);

	typedef int (__cdecl * n02ResetInterfaceCT)(void *, int);

#else

	int N02CCNV n02ResetInterfaceS(void * Interface, int type);

#endif 

#endif

#else

	int N02CCNV n02ResetInterface(void * Interface, int type);
	int n02ResetInterfaceC(void * Interface, int type);
	int N02CCNV n02ResetInterfaceS(void * Interface, int type);

#endif


	/* beyond this point are optional/extendable interfaces/extensions. The
	client does not impose any requirements that an application should use
	them. They are there if you happen to need any of them. */

	///////////////////////////////////////////////////////////////////////////
	// Extendable interfaces

	/******************************************************
	AutomationInterface
	****************************
	This will stop gui loading and use these values provided to make choices.
	It should also be obvious that not enough information is held inside these
	structures to completely automate the client all the way. Thats because any
	further information is transport module specific. e.g. the playback transport
	module would only require a file name to load or file descriptor to load data
	from or the memory address of encoded record file and its length to play back
	the record file whereas network type transport modules like p2p would need to
	know the local port it'll use and the peer's ip address etc. Even among
	network type transport modules, the information they need is different and is
	not worth generalizing.
	*********************
		transport - 
		gameName - 
		algorithm - 
		algoParam1 - 
		algoParam2 - 
		recordingEnabled - 0 for disabled
	****************/
	typedef struct {
		char transport[32];
		char gameName[128];
		char algorithm[32];
		int algoParam1;
		int algoParam2;
		int recordingEnabled;
	} n02AutomationInterface;

#define TRANSPORT_DUMMY "dummy"
#define TRANSPORT_KAILLERA "n02.kaillera"
#define TRANSPORT_P2P "n02.p2p"
#define TRANSPORT_SC "n02.sc"
#define TRANSPORT_PLAYBACK "krec.player"

#define INTERFACE_AUTOMATION 11

	/******************************************************

	****************************
	*********************

	****************/

	///////////////////////////////////////////////////////////////////////////
	// Other extensions

	/******************************************************
	AutorunInterface
	****************************
	Interface to layer for normal guided application running and latency
	compensation stuff. Note its meant to sit on top of application.
	*********************
	runFrameAuto - 
	callbackSaveState - 
	callbackLoadState - 
	callbackPurgeStates - 
	callbackSetPlayerInput - 
	callbackGetPlayerInput - 
	callbackRunFrame - 
	callbackRetriveState - make sure these are session independent
	callbackForceState - make sure these are session independent
	callbackReserved1 - 
	callbackReserved2 - 
	callbackReserved3 - 
	callbackReserved4 - 
	****************/
	typedef struct {
		/* <do not modify> */
		void (N02CCNV * runFrameAuto)			(int drawFrame);
		void (N02CCNV * reservedAction1)		(int, int);
		void (N02CCNV * reservedAction2)		(int, int);
		void (N02CCNV * reservedAction3)		(int, int);
		/* </do not modify> */
		void (N02CCNV * callbackSaveState)		(const int slotNo);
		void (N02CCNV * callbackLoadState)		(const int slotNo);
		void (N02CCNV * callbackPurgeStates)	(int count);
		void (N02CCNV * callbackSetPlayerInput)	(const void * input, const int player);
		int  (N02CCNV * callbackGetPlayerInput)	(void * input);
		void (N02CCNV * callbackRunFrame)		(int type, int finalFrame);
		int  (N02CCNV * callbackRetriveState)	(void **bufPtrPtr, int slotNo);
		void (N02CCNV * callbackForceState)		(void *bufPtr, int size, int slotNo);
		void (N02CCNV * callbackReserved1)		(int, int);
		void (N02CCNV * callbackReserved2)		(int, int);
		void (N02CCNV * callbackReserved3)		(int, int);
		void (N02CCNV * callbackReserved4)		(int, int);
	} n02AutorunInterface;

#define GAME_RUNFRAME_NODRAW 1
#define GAME_RUNFRAME_DRAW 2
#define INTERFACE_AUTORUN 21

	/******************************************************

	****************************
	*********************

	****************/

#ifdef __cplusplus
};
#endif

#endif

