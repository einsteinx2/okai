/******************************************************************************
>         .d8888b.   .d8888b.                                                 <
>        d88P  Y88b d88P  Y88b                                                <
>        888    888        888                                                <
88888b.  888    888      .d88P                                                <
888 "88b 888    888  .od888P"                                                 <
888  888 888    888 d88P"                                                     <
888  888 Y88b  d88P 888"                                                      <
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

#include "clientgui.h"
#include "locid.h"

/**
Localisation support. Designed to be loaded at load time
*/

#include <fstream>

namespace n02 {

    typedef struct {
        bool allocated;
        unsigned short id;
        char * string;
    } localisationEntry;

#define _ENTRY(ID, STR) {false, ID, STR}

    static localisationEntry entries[] = {
        _ENTRY(LID_NAME, "Open Kaillera"),
        _ENTRY(LID_DEL1, "Delete item"),
        _ENTRY(LID_DEL2, "Do you want to delete the seleted item"),
        _ENTRY(LID_YES1, "Yes"),
        _ENTRY(LID_NO01, "No"),
        _ENTRY(LID_ADD1, "Add Item"),
        _ENTRY(LID_EDT1, "Edit Item"),
        _ENTRY(LID_SELG, "Select game"),
        _ENTRY(LID_ERR1, "Error"),
        _ENTRY(LID_NGOL, "No games in list"),
        _ENTRY(LID_PLRS, "%i players"),
        _ENTRY(LID_STS1, ", states"),
        _ENTRY(LID_FRMP, "+%i frames"),
        _ENTRY(LID_ADD2, "Add"),
        _ENTRY(LID_CAN1, "Cancel"),
        _ENTRY(LID_EDT2, "Edit"),
        _ENTRY(LID_OK01, "ok"),
        _ENTRY(LID_NM01, "Name:"),
        _ENTRY(LID_IP01, "IP:"),
        _ENTRY(0xffff, "")
    };
    static int count = 0;

    extern char primary[128];


    bool dumpLocalisationFile()
    {
        LOGS("Writing localisation file");

        std::ofstream of("localisation02");
        if (of.is_open()) {
#define _WRITE(X) of.write(X, strlen(X))
            _WRITE("# n02 Localisation file\n");
            _WRITE("##################################\n");
            _WRITE("# Its a per line thing. Example:\n");
            _WRITE("# IDNo=String\n");
            _WRITE("# Also allowed is \"font.\" Example:\n");
            _WRITE("# font=SimSun\n");
            _WRITE("# ID numbers are available from https://okai.svn.sourceforge.net/svnroot/okai/n02/src/ui/locid.h\n");
            _WRITE("# Their Corresponding strings are available from https://okai.svn.sourceforge.net/svnroot/okai/n02/src/ui/localisation.cpp\n");
            _WRITE("# Remember to save files as UTF8\n");
            _WRITE("\n");

            if (strlen(primary) > 0) {
                char xxx[2048];
                sprintf(xxx, "font=%s\n", primary);
                _WRITE(xxx);
                _WRITE("\n");
            }

            for (int x = 0; x < count; x++) {
                char xxx[2048];
                sprintf(xxx, "%i=%s\n", entries[x].id, entries[x].string);
                _WRITE(xxx);
            }
#undef _WRITE
            of.close();
            return true;
        } else {
            LOGS("Failed to open localisation file for writing");
            return false;
        }

    }



#undef _ENTRY


    localisationEntry & getLocalisationEntry(int id)
    {
        int first = 0;
        int last = count;
        int mid = (first + last) / 2;
        int _count = 20;
        while (_count-->0) {
            if (entries[mid].id == id) {
                return entries[mid];
            } else if (entries[mid].id > id) {
                last = mid;
                mid = (first + last) / 2;
            } else {
                first = mid;
                mid = (first + last) / 2;
            }
            //LOG(x);
        };

        return entries[count];
    }

    static void loadLocalisationFile() {

        LOGS(Loading Localisation File);

        std::ifstream of;
        of.open("localisation02");

        if (of.is_open()) {
            char xxx[2048];
            while (of.getline(xxx, sizeof(xxx))) {
                LOG(%s, xxx);

                if (*xxx == '#')
                    continue;

                if (strchr(xxx, '=') != 0) {
                    char * value = strchr(xxx, '=');
                    *value = 0;	value++;
                    if (strcmp(xxx, "font")==0) {
                        strcpy(primary, value);
                    } else {
                        int id = StringUtils::alphaToInt(xxx);
                        localisationEntry & l = getLocalisationEntry(id);
                        if (l.allocated)
                            delete l.string;
                        l.string = strdup(value);
                        l.allocated = true;
                    }
                }

                memset(xxx, 0, sizeof(xxx));

            }
            of.close();
        } else {
            LOGS(No Localisation file present);
            //dumpLocalisationFile();
        }
    }

    void initializeLocalisation()
    {
        count = 0;
        while (entries[count].id != 0xffff)
            count++;

        loadLocalisationFile();
    }
    void terminateLocalisation()
    {
    }
    void showLocalisationConfig()
    {
    }

    char * getLocalisedUTF8(int id)
    {
        return getLocalisationEntry(id).string;
    }
    juce_wchar * getLocalisedUTF16(int id)
    {
		TRACE();
        static juce_wchar allocated[4][1024];
        static int count = 0;
        count = ((++count) % 4);
        char * u = getLocalisationEntry(id).string;
        String::fromUTF8((juce::uint8*)u, strlen(u)+1).copyToBuffer(allocated[count], 1024);
		TRACE();
        return allocated[count];
    }
};
