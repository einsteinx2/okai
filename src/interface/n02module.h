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
#include "n02.h"

#pragma once


/******************************************************
ModuleAbstraction02
****************************
Module retrival interface
*********************
index - do not modify
name - name of the module
type - module type
description - data about the module
getStatus - get the status of the module. returns:
		MOD02_STATUS_WORKING or a combination of the following flags:
		MOD02_STATUS_NOT_WORKING_MISC
		MOD02_STATUS_NOT_WORKING_DIFFERENT_VERSION
		MOD02_STATUS_NOT_WORKING_ESSENTIALS_MISSING
		MOD02_STATUS_NOT_WORKING_SYSTEM_ESSENTIALS_MISSING
getInterface - Copies the interface struct to the pointer. returns length
    on success
appAttributes - impose conditions on application attribute field
appAttributesC - impose negaative conditions on application attribute field
****************/
struct ModuleAbstraction02 {
public:
	int index;
	char name[32];
	char type[32];
	char description[256];
	int (N02CCNV * getStatus) ();
	int (N02CCNV * getInterface) (void * interf);
	int appAttributes;
	int appAttributesC;
};

#define MOD02_STATUS_WORKING 0
#define MOD02_STATUS_NOT_WORKING_MISC 1
#define MOD02_STATUS_NOT_WORKING_DIFFERENT_VERSION 2
#define MOD02_STATUS_NOT_WORKING_ESSENTIALS_MISSING 4
#define MOD02_STATUS_NOT_WORKING_SYSTEM_ESSENTIALS_MISSING 8

typedef ModuleAbstraction02 ModuleAbstraction02;




/******************************************************
ModulesHandler02
****************************
Modules handler
*********************
getByName - Returns a module by name
getByIndex - Returns a module by index
add - registers a module. It keeps and works with it's own copy of the
    ModuleAbstraction02 interface but it fills the index field of the structure
	pointed	by mod
find - finds module by type name. Put "" as type for traversing through all
findNext - find the next module of the searched type
****************/
struct ModulesHandler02 {
	ModuleAbstraction02 *	(N02CCNV *getByName)	(const char * name);
	ModuleAbstraction02 *	(N02CCNV *getByIndex)	(const int index);
	int						(N02CCNV *add)			(ModuleAbstraction02 * mod);
	ModuleAbstraction02 *	(N02CCNV *find)			(char * type);
	ModuleAbstraction02 *	(N02CCNV *findNext)		(ModuleAbstraction02 * current);
};

typedef ModulesHandler02 ModulesHandler02;




/******************************************************
OptionItemsInterface02
****************************
Signifies a special options dialog
*********************
showConfiguration - 
****************/
struct OptionItemsInterface02 {
	void (N02CCNV * showConfiguration) (void * parent);
};

#define MTYPE02_OPTION		"option"
typedef OptionItemsInterface02 OptionItemsInterface02;



/******************************************************
TransportModule02
****************************
Transport modules. P2P/Kaillera/Player etc
*********************
initialize - 
terminate - 
activete - 
step - 
synchronizeGame - 
endGame - 
sendAsyncData - 
sendSyncData - 
recvSyncData - 
syncData - 
syncDataCheck - 
sendChat - 
activeteGui - 
getSelectedAutorunIndex - 
getSelectedAutorunDelay - 
isRecordingEnabled - 
****************/
struct TransportModule02 {
public:
	void (N02CCNV *initialize)();
	void (N02CCNV *terminate)();
	void (N02CCNV *activete)();
	void (N02CCNV *end)();
	int  (N02CCNV *step)();
	int  (N02CCNV *synchronizeGame)(void * syncData, int len);
	void (N02CCNV *endGame)();
	void (N02CCNV *sendAsyncData)(const void * value, const int len, const int mode);
	void (N02CCNV *sendSyncData)(const void * value, const int len);
	int  (N02CCNV *recvSyncData)(void * value, const int len);
	int  (N02CCNV *syncData)(void * value, const int len);
	int  (N02CCNV *syncDataCheck) (int);
	void (N02CCNV *sendChat) (const char * message);
	/* <gui> */
	void (N02CCNV *activeteGui)();
	void (N02CCNV *endGui)();
	int  (N02CCNV *getSelectedAutorunIndex)();
	int  (N02CCNV *getSelectedAutorunDelay)();
	int  (N02CCNV *isRecordingEnabled)();
	/* </gui> */
	int  (N02CCNV * getGameplayType)();
	void (N02CCNV * reserved2)(int, int);
	void (N02CCNV * reserved3)(int, int);
	void (N02CCNV * reserved4)(int, int);
};

#define MTYPE02_TRANSPORT	"transport"
typedef TransportModule02 TransportModule02;





/******************************************************
RecorderInterface02
****************************
*********************
start - starts a new record file
srop - cleans up, compresses stuff and writes to file
addMetadata - adds metafata. They are stored at the top of the files
addComment - adds a comment in current file position
addGameSyncData - adds synchronizeGame data
addSyncData - add game synchronization data
addAsyncData - add asynchronous data
addChat - add chat message
addDrop - add player drop event
****************/
struct RecorderInterface02 {
	int (N02CCNV * start) (const char * gameName, const int playerNo, const int totalPlayers, const char * comments);
	void (N02CCNV * stop) ();
	int (N02CCNV * addMetadata) (const char * dataName, const char * data);
	int (N02CCNV * addComment) (const char * comment);
	int (N02CCNV * addGameSyncData) (const void * value, const int len);
	int (N02CCNV * addSyncData) (const void * value, const int len);
	int (N02CCNV * addAsyncData) (const void * value, const int len);
	int (N02CCNV * addChat) (const char * username, const char * chat);
	int (N02CCNV * addDrop) (const char * username, int playerNo);
	void (N02CCNV * setDefaultInput)(void *, int len);
	void (N02CCNV * reserved1)(int, int);
	void (N02CCNV * reserved2)(int, int);
	void (N02CCNV * reserved3)(int, int);
};

#define MTYPE02_RECORDER	"recorder"
typedef RecorderInterface02 RecorderInterface02;



/******************************************************
RunAlgorithmInterface02
****************************
Autorun modules
*********************

****************/
struct RunAlgorithmInterface02 {
	int attributes;
	void (N02CCNV * initialize)(int playerNo, int numPlayers, int selectedAutoRunDelay);
	int  (N02CCNV * getGameplayType)();
	void (N02CCNV * terminate)();
	void (N02CCNV * run)(int drawFrame);
};

#define RA_TRAD		1
#define RA_STATES	2
#define RA_GENERIC	128

#define RA_GT_GAMEPLAY  1
#define RA_GT_PLAYBACK  2

#define MTYPE02_RUNALG		"autorun"
typedef RunAlgorithmInterface02 RunAlgorithmInterface02;



/******************************************************
GameListInterface02
****************************
*********************

****************/
typedef struct {
	char * (N02CCNV *start)();
	char * (N02CCNV *next)();
	char * (N02CCNV *find)(const char * game);
	int    (N02CCNV *getCaps)(const char * game);
	int    (N02CCNV *getPlayers)(const char * game);
	void   (N02CCNV *reserved2)(int, int);
	void   (N02CCNV *reserved3)(int, int);
	void   (N02CCNV *reserved4)(int, int);
} GameListInterface02;

#define GCAPS_TRAD		1
#define GCAPS_STATES	8

#define MTYPE02_GAMELIST "gamelist"





/******************************************************
ExternalModuleHelper02
****************************
*********************

getExtendedInterface - returns:
    -1 on length error
	0 if interface wasn't registered or if parameter is 
	length on success
****************/
struct ExternalModuleHelper02{
	n02ClientInterface * client;
	ModulesHandler02 * modHandler;
	GameListInterface02 * gameList;
	// instead of calling the callbacks in client interface directly, these must be called
	void (N02CCNV * startGame)		(const char * game, const int playerNo, const int numPlayers);
	void (N02CCNV * endGame)		();
	void (N02CCNV * chatReceived)	(const char *nick, const char *text);
	void (N02CCNV * playerDropped)	(const char *nick, const int playernb);
	void (N02CCNV * asyncData)		(const void * data, const int size);
	void*(N02CCNV * getExtendedInterface) (const n02ClientInterface*, const int type);
	bool (N02CCNV * isRecorderLoaded)();
	void (N02CCNV * activeTransportByName)(char*);
	void (N02CCNV * activeTransportByIndex)(int);
};

#define MTYPE02_DEVINTERFACE	"dev"
typedef ExternalModuleHelper02 ExternalModuleHelper02;


