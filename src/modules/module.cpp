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
#include "module.h"
#include "common.h"
#include "GamesList.h"
#include "n02version.h"

#include "internalmodules.h"

// TODO: Test and review


namespace n02 {

    DynamicArray<ModuleAbstraction02> modules;

#define MODULE(X) extern ModuleAbstraction02 X;
    N02_INTERNAL_MODULES
#undef MODULE

        extern n02ClientInterface * client;

    static int gameListIndex = 0;
    static char * N02CCNV gameListStart() {
        return GamesList::start(&gameListIndex);
    }
    static char * N02CCNV gameListNext() {
        return GamesList::next(&gameListIndex);
    }
    static char * N02CCNV gameListFind(const char * game) {
        return GamesList::find(game);
    }
    static int N02CCNV gameListGetCaps(const char * game) {
        if (GamesList::select(game)) {
            return GamesList::selectedCaps();
        }
        return 0;
    }
    static int N02CCNV gameListGetPlayers(const char * game){
        if (GamesList::select(game)) {
            return GamesList::selectedMaxPlayers();
        }
        return 0;
    }
    static void   N02CCNV gameListReserved(int, int){}

    static GameListInterface02 gameListInterface = {
        gameListStart,
        gameListNext,
        gameListFind,
        gameListGetCaps,
        gameListGetPlayers,
        gameListReserved,
        gameListReserved,
        gameListReserved
    };

    STDMODULE(modGameList, "gamelist", "gamelist", gameListInterface, 0, 0, MOD02_STATUS_WORKING, "Module for working with game list");

    static ModuleAbstraction02 * N02CCNV modulesHandlerGetByName(const char * name){
        LOG (%s, name);
        if (name && strlen(name) > 0 && modules.itemsCount() > 0) {
            for (int x = 0; x < modules.itemsCount();  x++) {
                if (strcmp(modules[x].name, name)==0) {
                    return &modules[x];
                }
            }
        }
        return 0;
    }
    static ModuleAbstraction02 * N02CCNV modulesHandlerGetByIndex(const int index){
        LOG (%i/%i, index, modules.itemsCount());
        if (index >= 0 && index < modules.itemsCount()) {
            return &modules[index];
        }
        return 0;
    }
    static int N02CCNV modulesHandlerAdd(ModuleAbstraction02 * mod) {
        if (mod && client &&
            (mod->appAttributes & client->app.attributes) == mod->appAttributes &&
            (mod->appAttributesC & ~client->app.attributes) == mod->appAttributesC )
        {
            LOG(Registering %s of type %s, mod->name, mod->type);
            mod->index = modules.itemsCount();
            modules.addItem(*mod);
            return 0;
        }
        return 1;
    }
    ModuleAbstraction02 * N02CCNV modulesHandlerFind(char * type){
        if (type && strlen(type) > 0 && modules.itemsCount() > 0) {
            LOG(%s, type);
            for (int x = 0; x < modules.itemsCount();  x++) {
                if (strcmp(modules[x].type, type)==0) {
                    return &modules[x];
                }
            }
        }
        return 0;
    }
    ModuleAbstraction02 * N02CCNV modulesHandlerFindNext(ModuleAbstraction02 * current){
        if (current && current->type && strlen(current->type) > 0 && modules.itemsCount() > current->index + 1) {
            LOG(%s, current->type);
            for (int x = current->index + 1; x < modules.itemsCount();  x++) {
                if (strcmp(modules[x].type, current->type)==0) {
                    return &modules[x];
                }
            }
        }
        return 0;
    }

    ModulesHandler02 modulesHandler = {
        modulesHandlerGetByName,
        modulesHandlerGetByIndex,
        modulesHandlerAdd,
        modulesHandlerFind,
        modulesHandlerFindNext
    };

    STDMODULE(modModulesHandler, "modhandller", "modhandller", modulesHandler, 0, 0, MOD02_STATUS_WORKING, "Module for registering and searching for registered modules");

};
//
///******************************************************************************
//
//
//
//*********************************************************************/

#include "transport.h"
#include "recorder.h"
namespace n02 {

    void N02CCNV interfStartGame (const char * gameP, const int playerNoP, const int numPlayersP);
    void N02CCNV interfEndGame ();
    void N02CCNV interfChatReceived (const char *nick, const char *text);
    void N02CCNV interfPlayerDropped (const char *nick, const int playernb);
    void N02CCNV interfAsyncData (const void * data, const int size);
    void * N02CCNV interfGetExtendedInterface (const n02ClientInterface * clientInterface, const int type);

    ExternalModuleHelper02 modHelper = {
        0,
        &modulesHandler,
        &gameListInterface,
        interfStartGame,
        interfEndGame,
        interfChatReceived,
        interfPlayerDropped,
        interfAsyncData,
        interfGetExtendedInterface,
        recorderIsLoaded,
        transportSetActiveByName,
        transportSetActiveByIndex
    };

    STDMODULE(modExternalHelper, MTYPE02_DEVINTERFACE, MTYPE02_DEVINTERFACE, modHelper, 0, 0, MOD02_STATUS_WORKING, "Helper module for accessing necessary internal functions for external modules development.");

};




#ifdef N02_WIN32
#include <windows.h>
#endif

namespace n02 {
#ifdef N02_WIN32

    typedef struct {
        HMODULE handle;
        TCHAR fileName[1024];
    } ExternalLibrary02;




    class ExternalModulesHandler:
        public DynamicArray<ExternalLibrary02>
    {

        typedef void (N02CCNV * ModuleInit02FCNT)(ExternalModuleHelper02 * modHelper);


    public:

        void loadExternalLibrary(TCHAR * fileName)
        {
            LOG(Loading %s, fileName);

            HMODULE hDll = LoadLibrary(fileName);

            if (hDll != NULL) {

                ModuleInit02FCNT modInitialize = (ModuleInit02FCNT)GetProcAddress(hDll, "_moduleInit02@4");

                if (modInitialize != NULL) {

                    modInitialize(&modHelper);

                    ExternalLibrary02 ex;
                    _tcscpy(ex.fileName, fileName);
                    ex.handle = hDll;

                    addItem(ex);

                } else {

                    LOG(Entry point locating failed);
                    FreeLibrary(hDll);

                }				
            } else {

                LOG(Load failed; error = %i, GetLastError());

            }
        }

        TCHAR * fileext(TCHAR * fn){
            fn += _tcslen(fn);
            int extc = 0;
            while (*fn != '.' && ++extc < 6)
                fn--;
            return fn;
        }

        void checkFilenName(TCHAR * fileName, TCHAR * dirName) {
            if (_tcsstr(fileName, _T("02e.dll")) && _tcsstr(fileext(fileName), _T(".dll"))) {
                TCHAR cfilename[2000];
                unsigned char cfilenameAnsi[2000];
                memset(cfilename, 0, sizeof(cfilename));
                _tcsncpy(cfilename, dirName, _tcslen(dirName)-1);
                _tcscat(cfilename, fileName);
                StringUtils::TCHARToUTF8(cfilenameAnsi, cfilename);
                LOG(Search found %s, cfilenameAnsi);
                loadExternalLibrary(cfilename);
            }
        }

        void searchDirectory(TCHAR * directory){

            unsigned char directoryAnsi[2000];
            StringUtils::TCHARToUTF8(directoryAnsi, directory);
            LOG(Searching %s, directoryAnsi);

            TCHAR currentDir[2048];
            GetCurrentDirectory(2048, currentDir);

            WIN32_FIND_DATA FindFileData;
            HANDLE hFind = INVALID_HANDLE_VALUE;

            TCHAR DirSpec[2055];
            _tcsncpy(DirSpec, directory, _tcslen(directory)+1);
            SetCurrentDirectory(DirSpec);
            _tcsncat (DirSpec, _T("\\*"), 3);


            hFind = FindFirstFile(DirSpec, &FindFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
                    checkFilenName(FindFileData.cFileName, DirSpec);
                }
                while (FindNextFile(hFind, &FindFileData) != 0) {
                    if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
                        checkFilenName(FindFileData.cFileName, DirSpec);
                    }
                }
                FindClose(hFind);
            } else {
                LOG(Invalid directory);
            }
            SetCurrentDirectory(currentDir);
        }

        void unloadExternalLibraries(){
            if (itemsCount()) {
                for (int x = 0; x < itemsCount(); x++) {
                    LOG(Unloading external module lib: %s, items[x].fileName);
                    FreeLibrary(items[x].handle);
                }
            }
            clearItems();
        }

        void search() {

            LOG(Searching for okai modules);

            TCHAR currentDir[2048];

            GetSystemDirectory(currentDir, 2048);
            searchDirectory(currentDir);

            GetCurrentDirectory(2048, currentDir);
            searchDirectory(currentDir);

            _tcscat(currentDir, _T("\\modules"));
            searchDirectory(currentDir);

            GetCurrentDirectory(2048, currentDir);
            _tcscat(currentDir, _T("\\n02"));
            searchDirectory(currentDir);
        }


    } externalModules;


#endif

    void modulesInitialize(){
        LOGS(Initializing modules);
        modHelper.client = client;
        modules.clearItems();

#define MODULE(X) modHelper.modHandler->add(&X);
        N02_INTERNAL_MODULES
#undef MODULE

#ifdef N02_WIN32
            externalModules.search();
#endif

    }

    void modulesTerminate(){
        LOGS(Cleaning up modules);
        modules.clearItems();
#ifdef N02_WIN32
        externalModules.unloadExternalLibraries();
#endif
    }
};


//	/******************************************************************************
//
//
//
//	*********************************************************************/
//
//	static INT_PTR CALLBACK ModulesDialogProc(HWND hwndDlg,
//		UINT uMsg,
//		WPARAM /* wParam */,
//		LPARAM /* lParam */
//		)
//	{
//		switch(uMsg) {
//			case WM_INITDIALOG:
//				{
//					// Initialize modules LV
//					ListViewControl lvc;
//					lvc.handle = GetDlgItem(hwndDlg, LV_MODS);
//					lvc.addColumn(_T("Index"), 3 * 12);
//					lvc.addColumn(_T("Name"), 8 * 12);
//					lvc.addColumn(_T("Type"), 8 * 12);
//					lvc.addColumn(_T("Description"), 20 * 12);
//					lvc.addColumn(_T("Condition1"), 4 * 12);
//					lvc.addColumn(_T("Status"), 4 * 12);
//					lvc.fullRowSelect();
//
//					int i = 0;
//					ModuleAbstraction02 * m = 0;
//
//					// fill it
//					while ((m = modulesHandlerGetByIndex(i))!= 0) {
//						
//						TCHAR temp[256];
//						wsprintf(temp, _T("%i"), i);
//
//						int index = lvc.addRow(temp, i);
//
//						lvc.fillRow(ANSIToTCHAR(m->name, 0, 0), 1, index);
//						lvc.fillRow(ANSIToTCHAR(m->type, 0, 0), 2, index);
//
//						char desc[256];
//						m->getDescription(desc, 256);
//
//						lvc.fillRow(ANSIToTCHAR(desc, 0, 0), 3, index);
//						
//						wsprintf(temp, _T("%x"), m->appAttributesIntegrationConditioning);
//						lvc.fillRow(temp, 4, index);
//
//
//						int status = m->getStatus();
//						if (status == 0) {
//							lvc.fillRow(_T("Working"), 5, index);
//						} else {
//							*desc = 0;
//							if (status & MOD02_STATUS_NOT_WORKING_DIFFERENT_VERSION) {
//								_tcscat(temp, _T("version mismatch, "));
//							}
//							if (status & MOD02_STATUS_NOT_WORKING_ESSENTIALS_MISSING) {
//								_tcscat(temp, _T("essentials missing, "));
//							}
//							if (status & MOD02_STATUS_NOT_WORKING_SYSTEM_ESSENTIALS_MISSING) {
//								_tcscat(temp, _T("system requirement not met, "));
//							}
//							lvc.fillRow(temp, 5, index);
//						}
//
//						i++;
//					}
//
//					// Initialize the dlls LV
//					lvc.handle = GetDlgItem(hwndDlg, LV_DLLS);
//
//					// Fill it
//					for (int x = 0; x < externalModules.itemsCount(); x++) {
//						lvc.addRow(externalModules[x].fileName);
//					}
//
//					// initialize the window
//					SetWindowText(hwndDlg, _T("n02 v") N02_VER_STR _T(" modules"));
//					SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)icon02);
//					SendMessage(hwndDlg, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)icon02);		
//
//
//				}
//
//				return TRUE;
//
//			case WM_CLOSE:
//				EndDialog(hwndDlg, 0);
//				break;
//		};
//		return 0;
//	}
//
//	void N02CCNV modulesConfigShowConfiguration(void * parent) {
//		DialogBox(hInstance, MAKEINTRESOURCE(DLG_MODULES), (HWND)parent, ModulesDialogProc);
//	}
//
//	OptionItemsInterface02 modulesConfig = {
//		modulesConfigShowConfiguration
//	};
//
//	STDMODULE(modModulesConfig, "modulesConfig", MTYPE02_OPTION, modulesConfig, 0, MOD02_STATUS_WORKING, "Configure modules item")

