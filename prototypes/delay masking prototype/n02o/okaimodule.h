// okai module developmental include library
#ifndef OKAI_MODULE_H
#define OKAI_MODULE_H

#include "okaiclient.h"

// main module structure
typedef struct {
	// Name of the module
	char modeName[32];
	// base mode no. See OKAI_BMODE_*
	int	 modeBaseNo;
	// Called when not in gameplay mode
	bool (OKAI_CCNV *idleModeStep)		();
	// Called when mode is activated
	void (OKAI_CCNV *activeteGUI)		();
	// see OKAI_AGDM_*
	int  (OKAI_CCNV *getSelectedMode)	();
	// called after a game is loaded
	int  (OKAI_CCNV *synchronizeGame)	();
	// end game
	void (OKAI_CCNV *endGame)			();
	// asynchronus data
	void (OKAI_CCNV *asyncData)			(const void * value, const int len, const int position);
	// synchronous send
	void (OKAI_CCNV *sendPlayValue)		(const void * value, const int len);
	// synchronous revb
	int  (OKAI_CCNV *recvPlayValues)	(void * value, const int len);
	// synchronous send/recv combo
	int  (OKAI_CCNV *exchangePlayValues)(void * value, const int len);
	// should return no of data frames it has in it's queue
	int  (OKAI_CCNV *getLocalQueueLen)	();
	// poll sockets or whatever for that many ms
	int  (OKAI_CCNV *playDataCheck)		(int ms);
	// send a line of chat
	void (OKAI_CCNV *chatSend)			(char * text);
	// get base delay
	int  (OKAI_CCNV *getBaseDelay)		();
	// get smoothing delay (for variable delay mode mainly)
	int  (OKAI_CCNV *getAddedDelay)		();
	// User record option
	bool (OKAI_CCNV *isRecordingEnabled)();
	// for sending debug messages
	void (__cdecl   *debugCallback)     (char *, ...);
} okaiModule;

// Auto game mode: Fixed frame delay
#define OKAI_AGDM_TRAD	0
// Auto game mode: Zero delay game on fixed frame delay base
#define OKAI_AGDM_ZERO	1
// Auto game mode: Zero delay game on variable frame delay base
#define OKAI_AGDM_VARI	2

// Base mode values
// 2 players mode ... equal delay on both ends
#define OKAI_BMODE_2P	0
// 8 players mode
#define OKAI_BMODE_8P	2
// playback mode
#define OKAI_BMODE_PB	4

typedef struct {
	char moduleName[16]; //name of the module e.g. p2pIrc or NPMP
	char moduleClass[16]; //class of the module e.g. autoPortForward
	void (OKAI_CCNV *getServiceModuleInterface) (void * interf);
	void (OKAI_CCNV *getServiceModuleInfo) (char * str);//returns a string with description
	void (OKAI_CCNV *configureServiceModule) ();
	void (OKAI_CCNV *testServiceModule) ();
	int index; //internal, index of the modile in the list...do not touch
} okaiServiceModule;

typedef struct {
	int  (OKAI_CCNV *getModule)				(const char * searchStr, okaiModule * mod);
	int  (OKAI_CCNV *getServiceModule)		(const char * searchStr, okaiServiceModule * mod);
	int  (OKAI_CCNV *registerModule)		(okaiModule * mod);
	int  (OKAI_CCNV *registerServiceModule)	(okaiServiceModule * mod);
	okaiServiceModule * (OKAI_CCNV *findServiceModule)		(char * type);
	okaiServiceModule * (OKAI_CCNV *findNextServiceModule)	(okaiServiceModule * current); //returns NULL if no more is found, looks at its type to find the next one
	void (OKAI_CCNV * okaiModulesInitCB)(void* hDlg);
	bool (OKAI_CCNV * okaiModuleActivate)(int mode);
} okaiModulesHandler;

typedef struct {
	int  (OKAI_CCNV *activateGUI) (void * parent);
} simpleGuiItemInterface;

typedef struct {
	char * (OKAI_CCNV *gameListStart)();
	char * (OKAI_CCNV *gameListNext)(char * current);
	char * (OKAI_CCNV *gameListFind)(char * game);
} gameListInterface;


/* recorder interface. Default recording format is compressed krec1 format */
typedef struct  {
	/*
	Do not use recorderInit during a netplay game

	filename is NULL fNameReturn is ignored

	if filename is not null
	- if fNameReturn is false, it'll try to record on the file name specified
	- otherwise it will record in its default user configured location and 
	copy its location to filename
	*/
	int  (OKAI_CCNV *recorderInit)		(char * game, int playerno, int totalPlayers, char * filename, bool fNameReturn);
	// add data to recorder
	int   (OKAI_CCNV *recorderAddData)	(const void * value, const int len);
	// add key value to recorded
	int   (OKAI_CCNV *recorderAddValues)(const void * value, const int len);
	// add comment to recorder
	void  (OKAI_CCNV *recorderSetComment)(const char * comment);
	// add chat to recorder
	int   (OKAI_CCNV *recorderAddChat)	(const char * username, const char * chat);
	// add player drop notification to recorder
	int   (OKAI_CCNV *recorderAddDrop)	(const char * username, int playerNo);
	// compress recorded data, save to file and close it
	void  (OKAI_CCNV *recorderTerm)		();
} recorderInterface;



// ----------------- misc --------------

typedef struct {
	unsigned state:2;
	unsigned input:2;
} KSSDFA_;

#define KSSDFA_START_GAME 2
#define KSSDFA_END_GAME 1


typedef struct {
	KSSDFA_ * kssdfa;
	char * game;
	int * playerNo;
	int * numPlayers;
	OKAI_Context ** okai;
	void (OKAI_CCNV * okaiGameEndCallback)();
	void (OKAI_CCNV * okaiChatReceivedCallback)(char *nick, char *text);
	void (OKAI_CCNV * okaiClientDroppedCallback)(char *nick, int playernb);
	void (OKAI_CCNV * okaiAsyncDataCallback)(const void * data, const int size);
} modDevPackInterface;




#endif


