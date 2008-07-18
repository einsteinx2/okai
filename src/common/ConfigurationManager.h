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
#include <tchar.h>

#include "DynamicOrderedArray.h"

namespace n02 {

    struct ConfigurationItem {
        TCHAR name[31];
        enum {
            INT, // &int
            STRING, // char[len]
            STRUCT, // &struct
            STRLIST // DynamicOrderedArray<TCHAR*>
        } type : 8;
        void * pointer;
        unsigned short length;
        void * defaultValuePointer;
    };

    typedef ConfigurationItem ConfigurationItem;

#define CONFIG_START(NAME) ConfigurationItem NAME[] = {
#define CONFIG_ITEM(NAME, TYPE, PTR, LEN, DEFPTR) {NAME, ConfigurationItem::TYPE, PTR, LEN, DEFPTR},
#define CONFIG_INTVAR(NAME, VER, DEF) CONFIG_ITEM(NAME, INT, &VER, sizeof(VER), (void*)DEF)
#define CONFIG_STRVAR(NAME, VER, LEN, DEF) CONFIG_ITEM(NAME, STRING, VER, LEN, const_cast<TCHAR*>(DEF))
#define CONFIG_STRUCTVAR(NAME, VER) CONFIG_ITEM(NAME, STRUCT, &VER, sizeof(VER), 0)
#define CONFIG_STRLIST(NAME, VER, LEN) CONFIG_ITEM(NAME, STRLIST, &VER, LEN, 0)
#define CONFIG_END	{0}	};

    class ConfigurationManager :
        protected DynamicOrderedArray<ConfigurationItem>
    {
    public:
        ConfigurationManager(ConfigurationItem * configTable);
        ConfigurationManager(ConfigurationItem * configTable, int len);
        ConfigurationManager();
        ~ConfigurationManager();

        void saveToFile(TCHAR * fileName, TCHAR * module = _T("default"));
        void loadFromFile(TCHAR * fileName, TCHAR * module = _T("default"));

        void save(TCHAR * modName);
        void load(TCHAR * modName);

        void add(ConfigurationItem*);
        void remove(TCHAR * name);

        void useConfgTable(ConfigurationItem * configTable);
        void useConfgTable(ConfigurationItem * configTable, int len);

    };

};

