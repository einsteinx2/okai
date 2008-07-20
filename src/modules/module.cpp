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
        char fileName[1024];
    } ExternalLibrary02;


    class ExternalModulesHandler:
        public DynamicArray<ExternalLibrary02>
    {
        typedef void (N02CCNV * ModuleInit02FCNT)(ExternalModuleHelper02 * modHelper);
    public:
        void loadExternalLibrary(char * fileName)
        {
            LOG(Loading %s, fileName);
            HMODULE hDll = LoadLibraryA(fileName);
            if (hDll != NULL) {
                ModuleInit02FCNT modInitialize = (ModuleInit02FCNT)GetProcAddress(hDll, "_moduleInit02@4");
                if (modInitialize != NULL) {
                    modInitialize(&modHelper);
                    ExternalLibrary02 ex;
                    strcpy(ex.fileName, fileName);
                    ex.handle = hDll;
                    addItem(ex);
                } else {
                    LOGS(Entry point locating failed);
                    FreeLibrary(hDll);
                }
            } else {
                LOG(Load failed; error = %i, GetLastError());
            }
        }

        char * fileext(char * fn){
            fn += strlen(fn);
            int extc = 0;
            while (*fn != '.' && ++extc < 6)
                fn--;
            return fn;
        }

        void checkFilenName(char * fileName, char * dirName) {
            if (strstr(fileName, "02e.dll") && strstr(fileext(fileName), ".dll")) {
                char cfilename[2000];
                strncpy(cfilename, dirName, strlen(dirName)-1);
                strcat(cfilename, fileName);
                LOG(Search found %s, fileName);
                loadExternalLibrary(cfilename);
            }
        }

        void searchDirectory(char * directory){

            LOG(Searching %s, directory);

            char currentDir[2048];
            GetCurrentDirectoryA(2048, currentDir);

            WIN32_FIND_DATAA FindFileData;
            HANDLE hFind = INVALID_HANDLE_VALUE;

            char DirSpec[2055];
            strncpy(DirSpec, directory, strlen(directory)+1);
            SetCurrentDirectoryA(DirSpec);
			strncat (DirSpec, "\\*", 3);


            hFind = FindFirstFileA(DirSpec, &FindFileData);

            if (hFind != INVALID_HANDLE_VALUE) {
                if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
                    checkFilenName(FindFileData.cFileName, DirSpec);
                }
                while (FindNextFileA(hFind, &FindFileData) != 0) {
                    if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)==0){
                        checkFilenName(FindFileData.cFileName, DirSpec);
                    }
                }
                FindClose(hFind);
            } else {
                LOG(Invalid directory);
            }
            SetCurrentDirectoryA(currentDir);
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

            char currentDir[2048];

            GetSystemDirectoryA(currentDir, 2048);
            searchDirectory(currentDir);

            GetCurrentDirectoryA(2048, currentDir);
            searchDirectory(currentDir);

            strcat(currentDir, "\\modules");
            searchDirectory(currentDir);

            GetCurrentDirectoryA(2048, currentDir);
            strcat(currentDir, "\\n02");
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

