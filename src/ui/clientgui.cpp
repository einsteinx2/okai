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
#ifdef N02_WIN32
#include <windows.h>
#endif

#include "common.h"
#include "transport.h"

#include "clientgui.h"

namespace n02 {

    char primary[128];
    char secondary[128];

    CONFIG_START(juceFontConfig)
        CONFIG_STRVAR("primary", primary, 128, "")
        CONFIG_STRVAR("secondary", secondary, 128, "")
        CONFIG_END;


    void GuiInitialize() {
        LOGS(Initializing gui);

        ConfigurationManager config(juceFontConfig);
        config.load("font");

		LOG(Juce version %s, SystemStats::getJUCEVersion().toUTF8());

        String stat;
        stat.printf(T(", %i cpus %imhz"), SystemStats::getNumCpus(), SystemStats::getCpuSpeedInMegaherz());
        LOG(%s %s, SystemStats::getCpuVendor().toUTF8(), stat.toUTF8());

        initializeLocalisation();

    }


    /* Thread juice */

    static char * argv[] = {"n02.dll",""};


    void N02CCNV  interfGuiThreadEndCallback();


	class CallbackHandler
        : public MessageListener
    {
		void  handleMessage (const Message & message)
		{
			if (message.pointerParameter != 0) {
				clientGuiCallbackPointerType * cb = reinterpret_cast<clientGuiCallbackPointerType*>(message.pointerParameter);
				cb();
				WaitableEvent * waitable = reinterpret_cast<WaitableEvent*>(message.intParameter2);
				if (waitable != 0)
					waitable->signal();
			}
		}
    public:
    } * caller = 0;



	void GuiJUCEThreadCallbackLock(clientGuiCallbackPointerType * ptr)
	{
		TRACE(); WaitableEvent * waitable = new WaitableEvent;
		
		if (caller != 0) {
			caller->postMessage(new Message(0, reinterpret_cast<int>(waitable), 0, ptr));
			TRACE(); while (!waitable->wait(10000));
		}
		TRACE(); delete waitable;
	}


	/* JUCE THREAD */
	static int juceThreadID;

	bool GuiIsJuceThread()
	{
		return juceThreadID == PosixThread::getCurrentThreadId();
	}

    class ThreadJuice : public PosixThread {

        class OpenKailleraJUCEApp: public JUCEApplication {
        public:

            void initialise (const String& commandLineParameters);

            void shutdown();

            const String getApplicationName()
			{
				return T("Open Kaillera client: n02");
			}
        };
    public:

        volatile int juceInitialized;

        void run()
		{
            LOG(Juce Thread Entry %i, juceThreadID = PosixThread::getCurrentThreadId());
            JUCEApplication::main(0, argv, new OpenKailleraJUCEApp);
            LOGS(Juce Thread Exit);
        }

    } juceThread;

    void ThreadJuice::OpenKailleraJUCEApp::initialise (const String& commandLineParameters)
	{
        juceThread.juceInitialized = 1;

        if (strlen(primary) > 2) {
            Font::setDefaultSansSerifFontName(primary);
        }
        if (strlen(secondary) > 2) {
            Font::setDefaultSansSerifFontName(secondary);
        }

		caller = new CallbackHandler;
    }

    void ThreadJuice::OpenKailleraJUCEApp::shutdown()
	{
        juceThread.juceInitialized = 0;
    }


    class GuiThread: public PosixThread {
    public:
        void run()
		{
            LOG(Gui thread entry %i, PosixThread::getCurrentThreadId());

            juceThread.juceInitialized = 0;
            juceThread.start();
            int counter = 500;
            while (juceThread.juceInitialized == 0 && counter-->0) {
                sleep(10); yield();
            }

            guiInitialized = 1;

            do {
                TRACE(); transportResetActivation();
                TRACE(); transport.initialize();
                TRACE(); transport.activeteGui();
                TRACE(); transport.terminate();
            } while (transportWasReActivated());

            guiInitialized = 0;
            JUCEApplication::quit(false);
            TRACE(); interfGuiThreadEndCallback();
            LOGS(Gui thread exit);

        }

		volatile int guiInitialized;

    } guiThread;


    int GuiStartSync() {

        TRACE(); guiThread.guiInitialized = 0;

        TRACE(); guiThread.start();

        TRACE(); 

        int counter = 500;
        while (guiThread.guiInitialized==0 && counter-->0) {
            PosixThread::sleep(10); PosixThread::yield();
        }

        LOG(GuiSync countter=%i, counter);
        return counter;
    }


    int GuiEndSync() {
        if (guiThread.guiInitialized) {
            JUCEApplication::quit(true);
        }
        guiThread.stop();
        guiThread.guiInitialized = 0;
        interfGuiThreadEndCallback();

        terminateLocalisation();

        return 0;
    }

    void GuiCleanup() {
        LOGS(Cleaning up gui);
    }

};


#ifdef N02_WIN32

#include <windows.h>

void n02GuiSetHinstance(HINSTANCE hx){
    juce::PlatformUtilities::setCurrentModuleInstanceHandle(hx);
}

#ifdef N02_EXPORTS
BOOL WINAPI DllMain(HINSTANCE hinstDLL,
                    DWORD fdwReason,
                    LPVOID)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        n02GuiSetHinstance(hinstDLL);
    return TRUE;
}

#endif

#endif /* N02_WIN32 */
