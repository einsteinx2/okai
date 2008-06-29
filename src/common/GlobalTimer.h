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

#ifndef linux
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else
#include <time.h>
#endif

#define GLOBAL_TIMER_PERIOD 1

#ifdef linux

inline unsigned int timeGetTime() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec * 1000 ) + (ts.tv_nsec / 1000000);
}

inline void timeBeginPeriod(int) {}
inline void timeEndPeriod(int) {}

#endif

namespace n02 {

    class GlobalTimer {

    protected:

        unsigned int fixedTime;

    public:

        inline unsigned int fixGetTime()
        {
            return fixedTime = getTime();
        }

        inline unsigned int getFixedTime()
        {
            return fixedTime;
        }

        static unsigned int getTime()
        {
            return timeGetTime();
        }

        static void initialize()
        {
            timeBeginPeriod(GLOBAL_TIMER_PERIOD);
        }

        static void terminate()
        {
            timeEndPeriod(GLOBAL_TIMER_PERIOD);
        }

    };

};

