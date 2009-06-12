
#include "okaiclient.h"

#pragma once
#define _WIN32_WINNT 0x0502
#include <windows.h>
// -- internal modules n shit

#include "okaimodule.h"

extern okaiModule activeMod;
extern int modeActivatedRecently;

void okaiModulesInit();
void okaiModulesTerm();
void OKAI_CCNV okaiModulesInitCB(HWND hDlg);
bool OKAI_CCNV okaiModuleActivate(int mode);

// -- game list
extern char * gameList;
void gameListInit(char * gamelist);
void gameListTerm();
char * OKAI_CCNV gameListStart();
char * OKAI_CCNV gameListNext(char * current);
char * OKAI_CCNV gameListFind(char * game);

extern HICON icon02;
extern HINSTANCE hx;


// open kaillera's traditional and confusing state DFA

extern KSSDFA_ KSSDFA;



/* KSSDFAST: transision table
 + | 00 | 01 | 11 | 10 |
---+----+----+----+----|
00 | 00 | 00 | 01 | 01 |
01 | 01 | 00 | 00 | 01 |
11 | 11 | 11 | 11 | 11 |
10 | 10 | 00 | 00 | 10 |
*/

extern int KSSDFAST [16];

extern char GAME[128];
extern int playerno;
extern int numplayers;

void __cdecl kprintf(char*,...);

#define ADBFC(X, ...)\
	activeMod.debugCallback("DEBUG: %s:%i#%s " #X "",__FILE__, __LINE__, __FUNCTION__ , __VA_ARGS__);

#define KDBFC(X, ...)\
	kprintf("DEBUG: %s:%i#%s (" #X ")",__FILE__, __LINE__, __FUNCTION__ , __VA_ARGS__);

extern modDevPackInterface modDevPack;
extern recorderInterface recorder;
bool isRecorderLoaded();

int OKAI_CCNV getModule (const char * searchStr, okaiModule * mod);
int OKAI_CCNV getServiceModule (const char * searchStr, okaiServiceModule * mod);
int OKAI_CCNV registerModule (okaiModule * mod);
int OKAI_CCNV registerServiceModule (okaiServiceModule * mod);
okaiServiceModule * OKAI_CCNV findServiceModule (char * type);
okaiServiceModule * OKAI_CCNV findNextServiceModule (okaiServiceModule * current);

// --  Reel stuff --
extern int AutoGameMode;
extern int AutoGameDelayMode;
extern bool AutoGameLoaded;
extern int AutoGamePlayerIndex;
extern int AutoGamePlayerDelay;
extern int AutoGamePlayerFPTR;
extern int AutoGameFrameNo;
extern int AutoGamePredComplete;
void OKAI_CCNV runFrameAuto(int drawFrame);
