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
#include "clientgui.h"
#include "juceLicenseComponent.h"
#include "n02version.h"

//#define COLLECT_STATISTICS "127.0.0.1"
//#define COLLECT_STATISTICS "203.219.14.183"

namespace n02 {

    SIMPLEWINDOW(License, "License Window", Colours::whitesmoke, DocumentWindow::closeButton, juceLicenseComponent, 600, 320);

    static int licenseNotAccepted;

    void LicenseDeclined() {
        JUCEApplication::quit();
        licenseNotAccepted = 1;
    }
    void LicenseAccepted() {
        licenseNotAccepted = 0;
        JUCEApplication::quit();
    }

    void License::OnClose(){
        LicenseDeclined();
    }

    class OpenKailleraLicenseApp: public JUCEApplication {
    public:
        void initialise (const String& commandLineParameters) {
            License::createAndShow();
        }

        void shutdown() {
            delete License::window;
        }

        const String getApplicationName() {
            return T("okai license");
        }
    };


    CONFIG_START(licenseConfig)
        CONFIG_INTVAR("accepted", licenseNotAccepted, 1)
        CONFIG_END

        char *argv[] = {"", ""};

    int GuiShowLicenseDialog() {

        LOGS(License dialog);

        ConfigurationManager config(licenseConfig);
        config.load("license");

        if (licenseNotAccepted) {
            JUCEApplication::main(0, argv, new OpenKailleraLicenseApp);
			
#ifdef COLLECT_STATISTICS
			TRACE();
			{
				juce::String testStatistic;

				SystemStats system;
				testStatistic << licenseNotAccepted << ",";
				testStatistic << system.getNumCpus() << "," << system.getCpuSpeedInMegaherz() << "," << system.getCpuVendor() << ",";
				testStatistic << system.getOperatingSystemName() << "," << system.getOperatingSystemType() << ",";
				testStatistic << N02_DESCRIPTION << "," << __DATE__ << "," << __TIME__;

				char * xxx = strdup(testStatistic.toUTF8());

				SocketAddress sa(COLLECT_STATISTICS, 27810);
				UdpSocket ux(0, 8000, true);
				ux.sendTo(xxx, strlen(xxx) + 1, sa);

				LOG(%s, xxx);
			}
			TRACE();
#endif
		}
        config.save("license");

        return licenseNotAccepted;
    }

};

