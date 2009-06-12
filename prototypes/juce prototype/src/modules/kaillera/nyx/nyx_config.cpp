/******************************************************************************
O                                                           8d8b. Yb  dP Yb dP 
O                                                           8P Y8  YbdP   `8.  
Open Kaillera: Kaillera Server                              8   8   dP   dP Yb 
*******************************************************************dP**********
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

using namespace n02;


namespace config {

	char name[64];
	char location[64];
	char url[64];
	

	int maxUsers;
	int macConnecting;
	int maxGames;
	int maxConset;
	int minConset;
	int maxPing;

	int port;

	int floodMsgNo;
	int floodMsgTime;
	
	n02::DynamicOrderedArray<char*> motd;

};

using namespace config;


CONFIG_START(serverConfig)
CONFIG_STRVAR("name", name, 64, "Ape's server")
CONFIG_STRVAR("location", location, 64, "Ape's Home")
CONFIG_STRVAR("url", url, 64, "http://ape.movsq.net")
CONFIG_INTVAR("maxUsers", maxUsers, 8)
CONFIG_INTVAR("macConnecting", macConnecting, 2)
CONFIG_INTVAR("maxGames", maxGames, 4)
CONFIG_INTVAR("maxConset", maxConset, 1)
CONFIG_INTVAR("minConset", minConset, 6)
CONFIG_INTVAR("maxPing", maxPing, 100)
CONFIG_INTVAR("port", port, 27888)
CONFIG_INTVAR("floodMsgNo", floodMsgNo, 5)
CONFIG_INTVAR("floodMsgTime", floodMsgTime, 3000)
CONFIG_STRLIST("motd", motd, 128)
CONFIG_END


void loadConfig() {
	ConfigurationManager config(serverConfig);
	config.load("main");
}

void saveCoinfig() {
	ConfigurationManager config(serverConfig);
	config.save("main");
}


