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

#include "common.h"

#define USE_WINDOWS_INI
// only use for one shot save files
#define USE_CPP_CUSTOM_INI

#ifdef USE_WINDOWS_INI
#if !defined(N02_WIN32) || defined(USE_CPP_CUSTOM_INI)
#undef USE_WINDOWS_INI
#ifndef USE_CPP_CUSTOM_INI
#define USE_CPP_CUSTOM_INI
#endif
#endif
#endif


#ifdef USE_WINDOWS_INI
#include <windows.h>
#endif

#ifdef USE_CPP_CUSTOM_INI
#include <fstream>
using namespace std;
#endif

namespace n02 {

    void ConfigurationManager::loadFromFile(const char * fileName, const char * module)
    {
#ifdef USE_WINDOWS_INI
        char file[2048];
        if (strchr(fileName, '\\')==0) {
            GetCurrentDirectoryA(2048, file);
            strcat(file, "\\");
            strcat(file, fileName);

            OFSTRUCT of;

            if (OpenFile(file, &of, OF_EXIST) == HFILE_ERROR) {
                strcpy(file, fileName);
            }
        } else {
            strcpy(file, fileName);
        }
        for (int x = 0; x < itemsCount(); x++) {
            ConfigurationItem * cnf = getItemPtr(x);
            switch (cnf->type) {
                case ConfigurationItem::INT:
                    *reinterpret_cast<unsigned int*>(cnf->pointer) = GetPrivateProfileIntA(module, cnf->name, reinterpret_cast<int>(cnf->defaultValuePointer), file);
                    break;
                case ConfigurationItem::STRING:
                    GetPrivateProfileStringA(module, cnf->name, reinterpret_cast<LPCSTR>(cnf->defaultValuePointer), reinterpret_cast<LPSTR>(cnf->pointer), cnf->length, file);
                    break;
                case ConfigurationItem::STRUCT:
                    {
                        if (cnf->length < 256) {
                            char bufferL[520];
                            bufferL[0] = 0;
                            GetPrivateProfileStringA(module, cnf->name, "", bufferL, 520, file);
                            int slen = strlen(bufferL) >> 1;
                            if (slen >= cnf->length) {
                                StringUtils::strToBytes(reinterpret_cast<unsigned char*>(cnf->pointer), bufferL, cnf->length);
                            }
                        }
                    }
                    break;
                case ConfigurationItem::STRLIST:
                    {
                        DynamicOrderedArray<char*> * list = (DynamicOrderedArray<char*>*)cnf->pointer;
                        while (list->itemsCount() > 0) {
                            delete list->getItem(itemsCount()-1);
                            list->removeIndex(itemsCount()-1);
                        }
                        char key[44];
                        strcpy(key, cnf->name);
                        strcat(key, "_count");
                        int count = GetPrivateProfileIntA(module, key, 0, file);
                        for (int y = 0; y < count; y++) {
                            char * newValue = new char[cnf->length];
                            strcpy(key, cnf->name);
                            strcat(key, "_");
                            strcat(key, StringUtils::intToAlpha(y));
                            GetPrivateProfileStringA(module, key, "", newValue, cnf->length, file);
                            list->addItem(newValue);
                        }
                    }
                    break;
            };
        }
#endif
#ifdef USE_CPP_CUSTOM_INI
        // first load default values for all items
        for (int x = 0; x < itemsCount(); x++) {
            ConfigurationItem * cnf = getItemPtr(x);
            switch (cnf->type) {
                case ConfigurationItem::INT:
                    *reinterpret_cast<unsigned int*>(cnf->pointer) = reinterpret_cast<int>(cnf->defaultValuePointer);
                    break;
                case ConfigurationItem::STRING:
                    strcpy(reinterpret_cast<char*>(cnf->pointer), reinterpret_cast<char*>(cnf->defaultValuePointer));
                    break;
                case ConfigurationItem::STRLIST:
                    reinterpret_cast<DynamicOrderedArray<char*>*>(cnf->pointer)->clearItems();
                    break;

            };
        }

        ifstream of;
        of.open(fileName);

        char xxx[2048];
        while (of.getline(xxx, 2048)) {

            LOG(%s, xxx);

            if (strchr(xxx, '=') != 0) {
                char * value = strchr(xxx, '=');
                *value = 0;	value++;

                for (int x = 0; x <= itemsCount(); x++) {
                    if (x != itemsCount()) {
                        ConfigurationItem * cnf = getItemPtr(x);
                        
                        if (strcmp(xxx, cnf->name)==0) {
                            switch (cnf->type) {
							case ConfigurationItem::INT:
								*reinterpret_cast<unsigned int*>(cnf->pointer) = StringUtils::alphaToUint(value);
								break;
							case ConfigurationItem::STRING:
								strncpy(reinterpret_cast<char*>(cnf->pointer), value, cnf->length);
								break;
							case ConfigurationItem::STRLIST:
								{
									DynamicOrderedArray<char*> * list = reinterpret_cast<DynamicOrderedArray<char*>*>(cnf->pointer);
									list->addItem(strdup(value));
								}
								break;
                        case ConfigurationItem::STRUCT:
                            {
                                if (cnf->length < 256) {
                                    if ((strlen(value) >> 1) >= cnf->length) {
                                        StringUtils::strToBytes(reinterpret_cast<unsigned char*>(cnf->pointer), value, cnf->length >> 1);
                                    }
                                }
                            }
                            break;
                            };
                            break;
                        }
                    } else {
                        LOG(nokeymatch value %s=%s, xxx, value);						
                    }
                }


            }

            memset(xxx, 0, sizeof(xxx));

        }
        of.close();
#endif
        // TODO: Add c++ file version of setting loading
    }

    void ConfigurationManager::saveToFile(const char * fileName, const char * module)
    {
#ifdef USE_WINDOWS_INI
        char file[2048];
        if (strchr(fileName, '\\')==0) {
            GetCurrentDirectoryA(2048, file);
            strcat(file, "\\");
            strcat(file, fileName);

            OFSTRUCT of;

            if (OpenFile(file, &of, OF_EXIST) == HFILE_ERROR) {
                strcpy(file, fileName);
            }
        } else {
            strcpy(file, fileName);
        }
        for (int x = 0; x < itemsCount(); x++) {
            ConfigurationItem * cnf = getItemPtr(x);
            switch (cnf->type) {
                case ConfigurationItem::INT:
                    {
                        WritePrivateProfileStringA(module, cnf->name, StringUtils::uintToAlpha(*reinterpret_cast<unsigned int*>(cnf->pointer)), file);
                    }
                    break;
                case ConfigurationItem::STRING:
                    WritePrivateProfileStringA(module, cnf->name, (LPSTR)cnf->pointer, file);
                    break;
                case ConfigurationItem::STRUCT:
                    {
                        if (cnf->length < 256) {
                            char bufferL[520];
                            bufferL[0] = 0;
                            StringUtils::bytesToStr(bufferL, reinterpret_cast<unsigned char*>(cnf->pointer), cnf->length, 512);
                            WritePrivateProfileStringA(module, cnf->name, bufferL, file);
                        }
                    }
                    break;
                case ConfigurationItem::STRLIST:
                    {
                        DynamicOrderedArray<char*> * list = reinterpret_cast<DynamicOrderedArray<char*>*>(cnf->pointer);
                        char key[44];
                        strcpy(key, cnf->name);
                        strcat(key, "_count");
                        WritePrivateProfileStringA(module, key, StringUtils::intToAlpha(list->itemsCount()), file);
                        for (int y = 0; y < list->itemsCount(); y++) {
                            strcpy(key, cnf->name);
                            strcat(key, "_");
                            strcat(key, StringUtils::intToAlpha(y));
                            WritePrivateProfileStringA(module, key, list->getItem(y), file);
                        }
                    }
                    break;
            };
        }
#endif

#ifdef USE_CPP_CUSTOM_INI
        ofstream of;
        of.open(fileName);

        for (int x = 0; x < itemsCount(); x++) {
            ConfigurationItem * cnf = getItemPtr(x);
            switch (cnf->type) {
                case ConfigurationItem::INT:
                    {
                        char key[1024];
						sprintf(key, "%s=%u\n", cnf->name, *reinterpret_cast<unsigned int*>(cnf->pointer));
                        of.write(key, strlen(key));
                    }
                    break;
                case ConfigurationItem::STRING:
                    {
                        char key[1024];
						sprintf(key, "%s=%s\n", cnf->name, cnf->pointer);
                        of.write(key, strlen(key));
                    }
                    break;
                case ConfigurationItem::STRUCT:
                    {
                        if (cnf->length < 256) {
                            char key[1024];
							sprintf(key, "%s=", cnf->name);
                            StringUtils::bytesToStr(key + strlen(key), reinterpret_cast<unsigned char*>(cnf->pointer), cnf->length, 512);
                            strcat(key,"\n");
                            of.write(key, strlen(key));
                        }
                    }
                    break;
                case ConfigurationItem::STRLIST:
                    {
                        DynamicOrderedArray<char*> * list = ((DynamicOrderedArray<char*>*)cnf->pointer);
                        for (int y = 0; y < list->itemsCount(); y++) {
                            char key[1024];
							sprintf(key, "%s=%s\n", cnf->name, list->getItem(y));
                            of.write(key, strlen(key));
                        }
                    }
                    break;
            };
        }

        of.close();
#endif
    }

    void ConfigurationManager::save(const char * modName)
    {
#ifdef USE_CPP_CUSTOM_INI
        char fileName[128];
        *fileName = 0;
		strcat(fileName, n02GetName());
        strcat(fileName, ".");
        strcat(fileName, modName);
        strcat(fileName, ".conf");
        saveToFile(fileName, modName);
#endif
#ifdef USE_WINDOWS_INI
		char fileName[128];
        *fileName = 0;
		strcat(fileName, n02GetName());
		strcat(fileName, ".ini");
        saveToFile(fileName, modName);
#endif
    }

    void ConfigurationManager::load(const char * modName)
    {
#ifdef USE_CPP_CUSTOM_INI
        char fileName[128];
        *fileName = 0;
		strcat(fileName, n02GetName());
        strcat(fileName, ".");
        strcat(fileName, modName);
        strcat(fileName, ".conf");
        loadFromFile(fileName, modName);
#endif
#ifdef USE_WINDOWS_INI
		char fileName[128];
		*fileName = 0;
		strcat(fileName, n02GetName());
		strcat(fileName, ".ini");
        loadFromFile(fileName, modName);
#endif
    }

    ConfigurationManager::~ConfigurationManager()
    {
        for (int x = 0; x < itemsCount(); x++) {
            ConfigurationItem * cnf = getItemPtr(x);
            if (cnf->type == ConfigurationItem::STRLIST) {
                DynamicOrderedArray<char*> * list = reinterpret_cast<DynamicOrderedArray<char*>*>(cnf->pointer);
                while (list->itemsCount() > 0) {
                    delete list->getItem(list->itemsCount()-1);
                    list->removeIndex(list->itemsCount()-1);
                }
            }
        }
    }

    ConfigurationManager::ConfigurationManager(const ConfigurationItem * configTable)
    {
        useConfgTable(configTable);
    }

    ConfigurationManager::ConfigurationManager(const ConfigurationItem * configTable, const int len)
    {
        useConfgTable(configTable, len);
    }

    ConfigurationManager::ConfigurationManager() {}

    void ConfigurationManager::add(const ConfigurationItem* item)
    {
//TODO: Investigate... althou there is no need for this  bit of code, it was causing ini writing messup
        //for (int x = 0; x < itemsCount(); x++) {
        //    if (strcmp(item->name, getItemPtr(x)->name) < 0) {
        //        insertItemPtr(const_cast<ConfigurationItem*>(item), x);
        //        return;
        //    }
        //}
        addItemPtr(const_cast<ConfigurationItem*>(item));
    }

    void ConfigurationManager::remove(const char * name)
    {
        for (int x = 0; x < itemsCount(); x++) {
            if (strcmp(name, getItemPtr(x)->name) == 0) {
                removeIndex(x);						
                break;
            }
        }
    }

    void ConfigurationManager::useConfgTable(const ConfigurationItem * configTable)
    {
        int count = 0;
        ConfigurationItem * configPtr = const_cast<ConfigurationItem *>(configTable);
        while (configPtr->name[0] != 0) {
            configPtr++;
            count++;
        }
        useConfgTable(configTable, count);
    }

    void ConfigurationManager::useConfgTable(const ConfigurationItem * configTable, const int len)
    {
        clearItems();
		int length = len;
        while (length-->0) {
            add(configTable);
            configTable++;
        }
    }

};
