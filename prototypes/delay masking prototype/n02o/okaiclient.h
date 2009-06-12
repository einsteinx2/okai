/******************************************************************************

  Open Kaillera Application Programming Interface

---------------------------------
 Kaillera interface is emulated with this interface inside open kaillera
 kailleraclient.dll so never use both in combination
---------------------------------
 Parts of this is unimplemented but they are implementable. If a feature is
 implemented, it is listed here and there are probably examples out there on the
 net. If you would like to use an unimplemented feature in your project, just
 contact us.
 
 Implemented:

 To implement:
 everything else
 anyncData - need to be able to transfer it on server. ill figure
				something out after recoding kaillera section of okai...its a big mess

 

---------------------------------
 Incorporatiion of this interface demands deeper level of integration with
 the emulator and you are advised not to attempt it if you are struggling
 with normal kaillera interface. The original kaillera acts synchronously
 and requires its incorporator to have no networking knowledge. However, this
 one does.
 ---------------------------------
 A good netplaying application comes from good netplay integration with its
 librarys and vice versa. Its reverse is also true.
 ---------------------------------
 Some hints for implementing kaillera in general and implementing open kaillera
 - Kaillera loves and in some cases doesnt work without default 00000000s encoding
 - Never, never and NEVER update your netplay related status based on what you sent
	and always on what the functions return;


******************************************************************************/
#if !defined(OKAI_CLIENT_DEFS)
#define OKAI_CLIENT_DEFS

/* Okai interface version.
Okai Interface Changelog:
	v0x00000000: Initial release
*/

#define OKAI_INTERFACE_VER 0x00000000

// export function and callback calling cpnventions

#ifndef OKAI_CCNV
#define OKAI_CCNV __stdcall
#endif


// building dll or importing dll
#ifdef OKAI_BUILD
#define OKAIEXP __declspec(dllexport) int OKAI_CCNV
#define OKAIEXPC __declspec(dllexport) int __cdecl
#else
#define OKAIEXP __declspec(dllimport) int OKAI_CCNV
#define OKAIEXPC __declspec(dllimport) int __cdecl
#endif


//Flags... description below
#define OAPP_CAP_TRAD		1
#define OAPP_CAP_RSIM		2
#define OAPP_CAP_SRVS		4
#define OAPP_CAP_NOPERF		7
#define OAPP_CAP_PLAYBACK	8

#define OAPPM_AUTO_RESIM	7
#define OAPPM_TRIM_DEL		8
#define OAPPM_INVAR_DATA	16
#define OAPPM_OWNEDREFLEX	32

#define OAPOS_PREGSYNC		0
#define OAPOS_POSGSYNC		1
#define OAPOS_POSGASYNC		2

// 2 polayers only with nodelay, 2 players only delay, 8 players... playback mode
#define OKAI_GMODE_RESIM    0
#define OKAI_GMODE_FIXED	1
#define OKAI_GMODE_PLAYBACK	4
//wont work
#define OKAI_GMODE_2PR		0
#define OKAI_GMODE_2PS		1
#define OKAI_GMODE_8PR		2
#define OKAI_GMODE_8PS		3


#define OKAI_RUNFRAME_SILENT	0
#define OKAI_RUNFRAME_DRAW		1
#define OKAI_RUNFRAME_BLANK		2

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	/* Version & compatibility related stuff */
	int stSize;		// size of the structure
	int interfVer;	// Request interface version

	/* General Information */

	/* name of the application including version. This will be used to
	     differenciate between different clients and client versions */
	char appName[128];

	/* Application capability: combination of one of the following flags
	     indicating what kind of stuff it can handle

		OAPP_CAP_TRAD       - Traditional style kaillera implementation, user
							    experiances fixed frame delay as measured by
							    latency

		OAPP_CAP_RSIM       - Has rapid save/load/frameskip state/frames
								support to give the user 0 input delay experiance

		OAPP_CAP_SRVS       - Has algorithems implemented to play with 0 input
								delay on servers

		OAPP_CAP_PLAYBACK 	- This application recognizes playback mode

	*/
	unsigned int appCaps;

	/* Application mode:	Various flags indicating extra services it wants from
								the client

		OAPPM_AUTO_RESIM    - OAPP_CAP_RSIM must be set in appCaps. Client will
								automatically control gameplay. All save/load/run
								function callbacks must work. 

		OAPPM_TRIM_DEL      - Will trim off delay frames from any recording
								during resim

		OAPPM_INVAR_DATA    - Variable frame data size. Only available with
								OAPP_CAP_NOPERF

		OAPPM_OWNEDREFLEX   - This is going a bit too far but may be reasonable
								in some circumstances. The client will not draw its
								server/playback related gui items. Only p2p.
	*/
	unsigned int appExtraMode;


	/* Max rsim states, for APPM_AUTO_RESIM... specifys the maximum no of states it can handle */
	unsigned int appMaxRsim;

	/* Pointer to a list of games. If OAPP_CAP_NOPERF is set and this is NULL,
				the client will run in single game mode */	
	char *gameList;

	int autoInputSize;

	/* callbacks */
	/* do not touch the ones you dont need */
	struct callbacks {
		/* standard callbacks same as kaillera */
		/* when a game starts */
		/* mode is 0,1 = p2p with/without resim; 2,3 = server with/without resim 4=playback 5 = OAPP_CAP_NOPERF */
		int  (OKAI_CCNV *gameCallback)			(char *game, int player, int numplayers, int mode);
		void (OKAI_CCNV *gameEndCallback)		();
		/* When a chat message is received. NOTE: UNRELIABLE. NEVER SHOULD BE USED FOR TRANSFERRING DATA*/
		void (OKAI_CCNV *chatReceivedCallback)	(char *nick, char *text);
		/* When a client drops out ... Never use this to determine if a game is still running */
		void (OKAI_CCNV *clientDroppedCallback)	(char *nick, int playernb);
		//----------------------
		/* Other callbacks */
		/* before a game starts. May not be called when in server or in playback mode */
		int  (OKAI_CCNV *gamePreStartCallback)	(const char *game);
		/* called when waiting for packets. */
		void (OKAI_CCNV *packetWaitCallback)	();
		/* Asynchronus Data Callback */
		void (OKAI_CCNV *asyncDataCallback)		(const void * data, const int size);
		//----------------------
		/* auto rollback support callback...for OAPPM_AUTO_RESIM... memory management...application side */
		void (OKAI_CCNV *saveStateCallback)		(const int no);
		void (OKAI_CCNV *loadStateCallback)		(const int no);
		void (OKAI_CCNV *deleteStateCallback)	();
		/* maximum input size is 16 bytes... please do not put bullshit inside inputs
		    like dipswitches and gameshark codes */
		int  (OKAI_CCNV *getPlayerInputCallback)(void * input);
		void (OKAI_CCNV *setPlayerInputCallback)(const void * input, const int player);
		/* compare 2 player input, return 0 if same, 1 if different... not used */
		void (OKAI_CCNV *comparePlayerInputCallback)(const void * input1, const void * input2);
		// OKAI_RUNFRAME_BLANK || OKAI_RUNFRAME_SILENT || OKAI_RUNFRAME_DRAW
		void (OKAI_CCNV *runFrameCallback)(int type, int finalFrame);
	} callbacks;

	/* filled when you reset context. This is for YOUR USE. It may help debugging
	kaillera implementations but if you dont know what you are doing => program crash */
	struct functions{
		/* After a game starts and user is loaded, wait for other to load. If it returns nonzero=>game isnt running */
		int  (OKAI_CCNV *synchronizeGame)	();
		/* end current game */
		void (OKAI_CCNV *endGame)			();
		/* send asynchronus data. never, never and never assume that such data was ever sent.
				Only process what asyncDataCallback returns when it returns
		*/
		void (OKAI_CCNV *asyncData)			(const void * value, const int len, const int position);
		/* Send play value */
		void (OKAI_CCNV *sendPlayValue)		(const void * value, const int len);
		/* receive play values */
		int  (OKAI_CCNV *recvPlayValues)	(void * value, const int len);
		/* Do both at one go */
		int  (OKAI_CCNV *exchangePlayValues)(void * value, const int len);
		/* get the ammount of data waiting to be read...can be used to determine if a side is falling behind */
		int  (OKAI_CCNV *getLocalQueueLen)	();
		/* This is for checking if data is available on the network and if it is, bufferize */
		int  (OKAI_CCNV *playDataCheck)		(int ms);
		// send a line of chat message
		void (OKAI_CCNV *sendChat)			(char *text);
		//change game mode
		void (OKAI_CCNV *changeGameMode)	(int newMode);
		// run frame
		void (OKAI_CCNV *runFrameAuto)		(int drawFrame);
		// start okai gui
		int  (OKAI_CCNV *activateContext)	(void*);

	} functions;

} OKAI_Context;

#define OKAI_CONTEXT_MAIN	0
// returns 0 if fuccessful

// This is to fill OKAI_Context and any other buture structures we may have with default values
// context must not be NULL
OKAIEXP okaiContextReset(void * context, int type);

#ifdef OKAIEXPC
OKAIEXPC okaiContextResetC(void * context, int type);
#endif


#ifdef __cplusplus
}
#endif

#ifdef OKAI_BUILD

extern OKAI_Context * okai;
extern void OKAI_CCNV okaiGameEndCallback();
extern void OKAI_CCNV okaiChatReceivedCallback(char*,char *);
extern void OKAI_CCNV okaiClientDroppedCallback(char*,int);
extern void OKAI_CCNV okaiAsyncDataCallback(const void*, const int);

#endif

#endif
