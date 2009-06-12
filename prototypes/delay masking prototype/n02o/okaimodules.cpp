#include "okaimodules.h"
#include "common/nSettings.h"

#include "errr.h"

#define OKAI_MAX_MODS 8
#define OKAI_MAX_SERVICE_MODS 16
// -------------------------------------------------------------------------
// --  okaiModule handling Functions --

// the active module
okaiModule activeMod;


//blank module 
bool OKAI_CCNV blank_blank_idleModeStep	(){n02_TRACE(); return true;}
void OKAI_CCNV blank_activeteGUI		(){n02_TRACE();}
int  OKAI_CCNV blank_getSelectedMode	(){n02_TRACE(); return 0;}
int  OKAI_CCNV blank_synchronizeGame	(){n02_TRACE(); return 0;}
void OKAI_CCNV blank_endGame			(){n02_TRACE();}
void OKAI_CCNV blank_asyncData			(const void * value, const int len, const int position){n02_TRACE();}
void OKAI_CCNV blank_sendPlayValue		(const void * value, const int len){n02_TRACE();}
int  OKAI_CCNV blank_recvPlayValues	(void * value, const int len){n02_TRACE(); return 0;}
int  OKAI_CCNV blank_exchangePlayValues(void * value, const int len){n02_TRACE(); return 0;}
int  OKAI_CCNV blank_getLocalQueueLen	(){n02_TRACE(); return 0;}
int  OKAI_CCNV blank_playDataCheck		(int ms){n02_TRACE(); return 0;}
void OKAI_CCNV blank_chatSend			(char * text){n02_TRACE();}
int  OKAI_CCNV blank_getBaseDelay		(){n02_TRACE(); return 0;}
int  OKAI_CCNV blank_getAddedDelay		(){n02_TRACE(); return 0;}
bool OKAI_CCNV blank_isRecordingEnabled	(){n02_TRACE(); return false;}
void kprintf(char*,...);
okaiModule okaiBlankModule = {
	"blank",
	-1,
	blank_blank_idleModeStep,
	blank_activeteGUI,
	blank_getSelectedMode,
	blank_synchronizeGame,
	blank_endGame,
	blank_asyncData,
	blank_sendPlayValue,
	blank_recvPlayValues,
	blank_exchangePlayValues,
	blank_getLocalQueueLen,
	blank_playDataCheck,
	blank_chatSend,
	blank_getBaseDelay,
	blank_getAddedDelay,
	blank_isRecordingEnabled,
	kprintf
};

//blank service module
void OKAI_CCNV blank_getServiceModuleInterface (void * interf){}
void OKAI_CCNV blank_getServiceModuleInfo (char * str){strcpy(str, "Blank service module, does nothing");}
void OKAI_CCNV blank_configureServiceModule (){}
void OKAI_CCNV blank_testServiceModule (){}

okaiServiceModule okaiBlankServiceModule = {
	"blank",
	"blank",
	blank_getServiceModuleInterface,
	blank_getServiceModuleInfo,
	blank_configureServiceModule,
	blank_testServiceModule,
	-1
};

//module vars

okaiModule * okaiModules[OKAI_MAX_MODS];
okaiServiceModule * okaiServiceModules[OKAI_MAX_SERVICE_MODS];

typedef struct {
	HMODULE h;
	char filename[1000];
} okaiExternModule;

okaiExternModule okaiExternModules[OKAI_MAX_MODS];
int okaiExternModulesTotal = 0;
int okaiModsTotal = 0;
int okaiServiceModsTotal = 0;

int activeModIndex = 0;
int modeActivatedRecently = 0;

// resets all modules to blank
void okaiModulesReset () {
	for (int x = 0; x < OKAI_MAX_MODS;  x++) {
		okaiModules[x] = &okaiBlankModule;
		okaiExternModules[x].h = 0;
	}
	for (int y = 0; y < OKAI_MAX_SERVICE_MODS;  y++) {
		okaiServiceModules[y] = &okaiBlankServiceModule;
	}
	okaiExternModulesTotal = okaiServiceModsTotal = okaiModsTotal = 0;
}


//module handler interface
//get a particular module
int OKAI_CCNV getModule (const char * searchStr, okaiModule * mod){
	for (int x = 0; x < okaiModsTotal;  x++) {
		if (strcmp(okaiModules[x]->modeName, searchStr)==0) {
			*mod = *okaiModules[x];
			return 0;
		}
	}
	return 1;
}
//get a particular service module
int OKAI_CCNV getServiceModule (const char * searchStr, okaiServiceModule * mod){
	KDBFC(%s, searchStr);
	for (int x = 0; x < okaiServiceModsTotal;  x++) {
		KDBFC(%s==%s?, searchStr, okaiServiceModules[x]->moduleName);
		if (strcmp(okaiServiceModules[x]->moduleName, searchStr)==0) {
			*mod = *okaiServiceModules[x];
			return 0;
		}
	}
	return 1;
	return 0;
}
// add a module to the list
int OKAI_CCNV registerModule (okaiModule * mod){
	KDBFC(%s, mod->modeName);
	if (okaiModsTotal <= OKAI_MAX_MODS) {
		okaiModules[okaiModsTotal++] = mod;
		return 0;
	}
	return 1;
}
//add a service module to the list
int OKAI_CCNV registerServiceModule (okaiServiceModule * mod){
	KDBFC(%s / %s, mod->moduleName, mod->moduleClass);
	if (okaiServiceModsTotal <= OKAI_MAX_SERVICE_MODS) {
		okaiServiceModules[(mod->index = okaiServiceModsTotal++)] = mod;
		return 0;
	}
	return 1;
}
okaiServiceModule * OKAI_CCNV findServiceModule (char * type){
	for (int x = 0; x < okaiServiceModsTotal;  x++) {
		if (strcmp(okaiServiceModules[x]->moduleClass, type)==0) {
			return okaiServiceModules[x];
		}
	}
	return 0;
}
okaiServiceModule * OKAI_CCNV findNextServiceModule (okaiServiceModule * current){
	if (current && current->index + 1 < okaiServiceModsTotal) {
		for (int x = current->index+1; x < okaiServiceModsTotal;  x++) {
			if (strcmp(okaiServiceModules[x]->moduleClass, current->moduleClass)==0) {
				return okaiServiceModules[x];
			}
		}
	}
	return 0;
}


void OKAI_CCNV okaiModulesInitCB(HWND hDlg){
	for (int x = 0 ; x < okaiModsTotal; x++) {
		SendMessage(hDlg, CB_ADDSTRING, 0, (WPARAM)okaiModules[x]->modeName);
	}
	SendMessage(hDlg, CB_SETCURSEL, activeModIndex, 0);
}

void okaiclientActiveModChangeCallback();

bool OKAI_CCNV okaiModuleActivate(int mode){
	modeActivatedRecently = mode != activeModIndex? 1:0;
	activeModIndex = min(max(mode, 0), okaiModsTotal-1);
	activeMod = *okaiModules[activeModIndex];
	okaiclientActiveModChangeCallback();
	return modeActivatedRecently==1;
}


okaiModulesHandler mod_handler = {
	getModule,
	getServiceModule,
	registerModule,
	registerServiceModule,
	findServiceModule,
	findNextServiceModule,
	(void (__stdcall *)(void *))okaiModulesInitCB,
	okaiModuleActivate
};



//=============================================
//internal modules

// -----------------------
//game list interface
gameListInterface gInterf = {
	gameListStart,
	gameListNext,
	gameListFind
};


void OKAI_CCNV gInterf_getServiceModuleInterface (void * interf){
	*((gameListInterface*)interf) = gInterf;
}
void OKAI_CCNV gInterf_getServiceModuleInfo (char * str){
	strcpy(str, "Gamelist retrival service for external modules.");
}

okaiServiceModule okaiGameListInterfaceServiceModule = {
	"gamelist",
	"gamelist",
	gInterf_getServiceModuleInterface,
	gInterf_getServiceModuleInfo,
	blank_configureServiceModule,
	blank_testServiceModule,
	-1
};

// -----------------------
// context interface

void OKAI_CCNV context_getServiceModuleInterface (void * interf){
	*((OKAI_Context**)interf) = okai;
}
void OKAI_CCNV context_getServiceModuleInfo (char * str){
	strcpy(str, "Open Kaillera Context retrival service module for external modules");
}

okaiServiceModule okaiContextServiceModule = {
	"context",
	"context",
	context_getServiceModuleInterface,
	context_getServiceModuleInfo,
	blank_configureServiceModule,
	blank_testServiceModule,
	-1
};

// -----------------------
// module essencials 

modDevPackInterface modDevPack = {
	&KSSDFA,
	GAME,
	&playerno,
	&numplayers,
	&okai,
	okaiGameEndCallback,
	okaiChatReceivedCallback,
	okaiClientDroppedCallback,
	okaiAsyncDataCallback
};

void OKAI_CCNV mdp_getServiceModuleInterface (void * interf){
	*((modDevPackInterface*)interf) = modDevPack;
}
void OKAI_CCNV mdp_getServiceModuleInfo (char * str){
	strcpy(str, "Open Kaillera internal vars needed for module development");
}

okaiServiceModule okaiMDPServiceModule = {
	"mdp",
	"mdp",
	mdp_getServiceModuleInterface,
	mdp_getServiceModuleInfo,
	blank_configureServiceModule,
	blank_testServiceModule,
	-1
};
//===============================================



//===============================================


#define OKAI_INTERNAL_MODULES \
	O_MODULE(modP2P)\
	O_S_MODULE(okaiGameListInterfaceServiceModule)\
	O_S_MODULE(okaiContextServiceModule)\
	O_S_MODULE(okaiMDPServiceModule)

#define O_MODULE(X) extern okaiModule X;
#define O_S_MODULE(X) extern okaiServiceModule X;
OKAI_INTERNAL_MODULES
#undef O_S_MODULE
#undef O_MODULE

typedef void (OKAI_CCNV * okaiModuleInitFunctionType)(okaiModulesHandler * mod_handler);

//=====================================================

void okaiExternalModuleLoad(char * filename) {
	
	//KDBFC(Loading %s, filename);

	HMODULE hhh = LoadLibrary(filename);

	if (hhh != NULL) {

		//KDBFC(Locating entry point);

		okaiModuleInitFunctionType okaiModuleInitFunction = (okaiModuleInitFunctionType)GetProcAddress(hhh, "_okaiModuleInit@4");

		if (okaiModuleInitFunction != NULL) {
			//KDBFC(Entry point located);

			okaiModuleInitFunction(&mod_handler);
			
			strcpy(okaiExternModules[okaiExternModulesTotal].filename, filename);
			
			okaiExternModules[okaiExternModulesTotal++].h = hhh;

			return;
		} else {
			//KDBFC(Entry point locating failed);
		}
		FreeLibrary(hhh);
	} //else 
		//KDBFC(Load failed);
}


char * fileext(char * fn){
	fn += strlen(fn);
	while (*fn != '.')
		fn--;
	return fn;
}

void okaiExternalModuleSeatchDirectory(char * directory){
	char currentDir[2048];

	GetCurrentDirectory(2048, currentDir);

	KDBFC(Searching %s, directory);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	char DirSpec[5000];
	strncpy (DirSpec, directory, strlen(directory)+1);
	SetCurrentDirectory(DirSpec);
	strncat (DirSpec, "\\*", 3);
	hFind = FindFirstFile(DirSpec, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE) {
		if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
			if (strncmp(FindFileData.cFileName, "okai_",5)==0 && strstr(FindFileData.cFileName, ".dll") && strstr(fileext(FindFileData.cFileName), ".dll")) {
				char cfilename[2000];
				memset(cfilename, 0, 2000);				
				strncpy(cfilename, DirSpec, strlen(DirSpec)-1);
				//SetDllDirectory(cfilename);
				strcat(cfilename, FindFileData.cFileName);
				KDBFC(Search found %s, cfilename);
				okaiExternalModuleLoad(cfilename);
			}
		}
		while (FindNextFile(hFind, &FindFileData) != 0) {
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
				
				if (strncmp(FindFileData.cFileName, "okai_",5)==0 && strstr(FindFileData.cFileName, ".dll") && strstr(fileext(FindFileData.cFileName), ".dll")) {
					char cfilename[2000];
					memset(cfilename, 0, 2000);
					strncpy(cfilename, DirSpec, strlen(DirSpec)-1);
					//SetDllDirectory(cfilename);
					strcat(cfilename, FindFileData.cFileName);
					KDBFC(Search found %s, cfilename);
					okaiExternalModuleLoad(cfilename);
				}
			}
		}
		FindClose(hFind);
	}
	SetCurrentDirectory(currentDir);
}







void okaiModulesSearchAdd(){

	KDBFC(Searching for okai modules);

	okaiExternalModuleSeatchDirectory("C:\\windows\\system32");
	char currentDir[1000];
	DWORD cds = 5000;
	GetCurrentDirectory(cds, currentDir);
	okaiExternalModuleSeatchDirectory(currentDir);
	strcat(currentDir,"\\modules");
	okaiExternalModuleSeatchDirectory(currentDir);
	//search for modules, load and and add them
	//search current directory
	//search current directory/n02
	//search current directory/okai
	//search windows/system32
}

void recorderCheck();

void okaiModulesInit(){


	//load settings
	nSettings::Initialize("okai");
	activeModIndex = nSettings::get_int("AM", 0);
	nSettings::Terminate();




	//reset everything to blank
	okaiModulesReset ();

	//add internal modules
#define O_MODULE(X) registerModule(&X);
#define O_S_MODULE(X) registerServiceModule(&X);
OKAI_INTERNAL_MODULES
#undef O_S_MODULE
#undef O_MODULE

	//add external modules
	okaiModulesSearchAdd();

	//activate last mod
	okaiModuleActivate(activeModIndex);


	//check for recorder
	recorderCheck();

}
void okaiModulesTerm(){
	//save settings
	nSettings::Initialize("okai");
	nSettings::set_int("AM", activeModIndex);
	nSettings::Terminate();
	//unload external librarys
	for (int x = 0; x < okaiExternModulesTotal; x++) {
		KDBFC(Unloading external module lib: %s, okaiExternModules[x].filename);
		FreeLibrary(okaiExternModules[x].h);
	}
	okaiExternModulesTotal = okaiServiceModsTotal = okaiModsTotal = 0;
}




