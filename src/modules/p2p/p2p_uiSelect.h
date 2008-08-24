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
#include "juce02.h"

namespace n02 {

	namespace p2p {

		void N02CCNV activeteGui();
		int  N02CCNV getSelectedAutorunIndex();
		int  N02CCNV getSelectedAutorunDelay();
		int  N02CCNV isRecordingEnabled();
		void N02CCNV endGui();


		void uiModChangeCallback(int index);

		extern char nick[32];
		extern char ip[128];
		extern int port;

		class StoredListListboxModel
			: public TableListBoxModel {
		public:
			StoredListListboxModel(){}
			int  getNumRows ();
			void  paintRowBackground (Graphics &g, int rowNumber, int width, int height, bool rowIsSelected);
			void  paintCell (Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected);
			void  cellClicked (int rowNumber, int columnId, const MouseEvent &e);
			void  cellDoubleClicked (int rowNumber, int columnId, const MouseEvent &e);
			void  deleteKeyPressed (int lastRowSelected);
			void  returnKeyPressed (int lastRowSelected);
		};

		void uiAddServer();
		void uiEditServer(int);
		void uiDeleteServer(int);

		void uiConnect();
		void uiHost();
	};
};

