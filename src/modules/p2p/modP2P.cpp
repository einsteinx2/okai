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

#include "modP2P.h"
#include "module.h"
#include "common.h"
#include "p2p_Core.h"
#include "p2p_uiSelect.h"


// automation stuff here
namespace n02 {
	namespace p2p {
		static void N02CCNV activete()
		{
		}
	};
};


/* ===================== entry point stuff ======================= */
namespace n02 {
	namespace p2p {
		static void N02CCNV initialize()
		{
		}
		static void N02CCNV terminate()
		{
		}
		static int N02CCNV getGameplayType() {
			return GAME_TYPE_GAMEPLAY;
		}
		static void N02CCNV  reserved(int, int)
		{
		}
	};
	// Transport module definition
    STDTRANSPORTDEFNNMS(trModP2P, p2p);
    // abstraction stuff...
    STDMODULE(modP2P, "n02.p2p", MTYPE02_TRANSPORT, trModP2P, 0, 0, MOD02_STATUS_WORKING, "P2P module"); 
};
