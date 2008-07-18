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

#include <cstdio>
#include <cstdarg>
#include <fstream>

#include "_common.h"

using namespace std;

#define INCLUDE_THREADID
#define INSTANT_FLASH

namespace n02 {

    Logger defaultLogger;

    Logger::Logger(char * file, bool append)
    {
        initialize(file, append);
    }

    Logger::Logger()
    {
        fileHandle = 0;
    }

    Logger::~Logger()
    {
        terminate();
    }

    void Logger::initialize(char * file, bool append)
    {
        ofstream * of = new ofstream(file, append? ofstream::app: ofstream::out);
        fileHandle = reinterpret_cast<void*>(of);
    }

    void Logger::terminate()
    {
        if (fileHandle != 0) {
            ofstream * of = reinterpret_cast<ofstream*>(fileHandle);
            of->close();
            fileHandle = 0;
        }
    }

    void Logger::logLine(char * line)
    {
        printf(line);
        if (fileHandle != 0) {
            ofstream * of = reinterpret_cast<ofstream*>(fileHandle);
            of->write(line, strlen(line));
            of->write("\r\n", 2);
#ifdef INSTANT_FLASH
            of->flush();
#endif
        }
    }

    //void Logger::logcprintf(char * format, void**args)
    //{
    //    char print_buffer[1025];
    //    StringUtils::cprintf(print_buffer, format, args);
    //    logLine(print_buffer);
    //}

    void Logger::logprintf(char * format, ...)
    {
#ifdef INCLUDE_THREADID
        char print_buffer[1025];
        sprintf(print_buffer, "%i: ", PosixThread::getCurrentThreadId());
        va_list args;
        va_start(args, format);
        vsprintf(print_buffer + strlen(print_buffer), format, args);
        va_end( args );
        logLine(print_buffer);
#else
        char print_buffer[1025];
        va_list args;
        va_start(args, format);
        vsprintf(print_buffer, format, args);
        va_end( args );
        logLine(print_buffer);
#endif
    }

    void Logger::logBuffer(void * bytes, int length, char * name)
    {
        logprintf("%s: %08x Length: %04x", name, bytes, length);

        if (length > 0) {
            logLine("--+   0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f");

            unsigned char * ptr = (unsigned char *) bytes;

            for (int x = 0; x < length; x+= 16) {
                char buffer[130];
                sprintf(buffer, "%03x  ", x >> 4);
                StringUtils::bytesToStr(buffer + strlen(buffer), ptr + x, common_min(length - x, 16), 1);
                int temp = strlen(buffer);

                for (; temp < 54; temp++) {
                    buffer[temp] = ' ';				
                }

                StringUtils::bytesToPrintable(buffer + 54, ptr + x, common_min(length - x, 16));
                logLine(buffer);
            }
        }
    }

};
