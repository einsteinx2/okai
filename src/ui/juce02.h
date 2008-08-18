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

#include "common.h"


#ifdef linux

#ifndef LINUX
#define LINUX
#endif /* LINUX */

#ifdef JUCE_DLL
#undef JUCE_DLL
#endif /* JUCE_DLL */

#else

//#ifdef JUCE_DLL
//#pragma comment(lib, "..\\..\\juce\\bin\\JUCE.lib")
//#else
//#ifndef JUCE_DEBUG
//#pragma comment(lib, "..\\..\\juce\\bin\\jucelib_static_Win32.lib")
//#else
//#pragma comment(lib, "..\\..\\juce\\bin\\jucelib_static_Win32_debug.lib")
//#endif
//#endif

#endif /* linux */

#include "juce_amalgamated.h"

// support header for using juce inside this program

#define SIMPLEWINDOW(Name, Title, Background, Buttons, _Component, W, H)	\
	class Name : public DocumentWindow {									\
	public:																	\
		Name()																\
			: DocumentWindow(T(Title), Background, Buttons)					\
		{																	\
			cmponnt = new _Component();										\
			setContentComponent(cmponnt, true, true);						\
		}																	\
		~Name() {															\
			setContentComponent (0, true);									\
			cmponnt = 0;													\
		}																	\
		void closeButtonPressed() {											\
			OnClose();														\
			running = false;												\
			if (syncThread)													\
				syncThread->notify();										\
		}																	\
		void waitNotifyAndCloseNotify() {									\
			closeButtonPressed();											\
		}																	\
		static Name * window;												\
		static _Component * cmponnt;										\
		static void createAndShow() {										\
			window = new Name();											\
			window->centreWithSize (W, H + window->getTitleBarHeight());	\
			window->setVisible (true);										\
		}																	\
		static void createAndShowChild(Component * c) {						\
			window = new Name();											\
			c->addChildComponent(window, -1);								\
			window->centreAroundComponent (c, W, H + window->getTitleBarHeight());	\
			window->setVisible (true);										\
		}																	\
		static void createAndShowModal() {									\
			window = new Name();											\
			window->centreWithSize (W, H + window->getTitleBarHeight());	\
			window->setVisible (true);										\
			window->runModalLoop();											\
		}																	\
		static void waitForClose() {										\
			running = true;													\
			syncThread = new PosixThread(true);								\
			while (running)													\
				syncThread->wait(100000);									\
			delete syncThread;												\
			syncThread = 0;													\
		}																	\
        static void deleteAndZeroWindow() {									\
            if (window != 0) {												\
				window->setVisible(false);									\
				window->removeFromDesktop();								\
				delete window;												\
                window = 0;													\
            }																\
        }																	\
		static void OnClose();												\
		static volatile bool running;										\
		static PosixThread * syncThread;									\
	};																		\
	Name * Name::window;													\
	_Component * Name::cmponnt;												\
	volatile bool Name::running;											\
	PosixThread * Name::syncThread = 0



#define COMMAND(X, Y) ((X << 24) | Y)
#define COMMAND_X(X) ((0xFF000000&X)>>24)
#define COMMAND_Y(X) (0xFFFFFF&X)


#define FROMUTF8(UTF8) String::fromUTF8((juce::uint8*)UTF8, strlen(UTF8)+1)

