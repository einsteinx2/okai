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

namespace n02 {

    class Logger {
    private:

        void * fileHandle;

    public:

        Logger(char * file, bool append);
        Logger();
        virtual ~Logger();

        virtual void initialize(char * file, bool append);
        virtual void terminate();
        virtual void logLine(char * line);

        /*void logcprintf(char * format, void**args);*/
        void logprintf(char * format, ...);
        void logBuffer(void * bytes, int length, char * name);

    };

    extern Logger defaultLogger;

#ifdef LOG
#undef LOG
#endif

#ifdef LOGTRACE
#undef LOGTRACE
#endif

#ifdef LOGBUFFER
#undef LOGBUFFER
#endif

#ifdef DONT_LOG
#define LOG(X, ...)
#define LOGS(X)
#define LOGTRACE(X, ...)
#define LOGBUFFER(X, Y)
#define LOGBASICS(X)
#undef DONT_LOG
#else

#ifdef _MSC_VER

#define LOG(X, ...) \
    defaultLogger.logprintf(__FUNCTION__ ## "(" #X ")" , __VA_ARGS__)

#define LOGS(X) \
    defaultLogger.logLine(#X)

#define LOGBASIC(X, ...) \
    defaultLogger.logprintf(X, __VA_ARGS__)

#define LOGBASICS(X) \
	defaultLogger.logLine(X)

#define LOGTRACE(X, ...) \
    defaultLogger.logprintf(#X, __VA_ARGS__)

#define LOGBUFFER(N, X, Y) defaultLogger.logBuffer((void*)X, (int)Y, N)

#else


#define LOG(X, ...) \
    defaultLogger.logprintf("(" #X ")" , __VA_ARGS__)

#define LOGS(X) \
    defaultLogger.logLine(#X)

#define LOGBASIC(X, ...) \
    defaultLogger.logprintf(X, __VA_ARGS__)

#define LOGBASICS(X) \
    defaultLogger.logLine(X)

#define LOGTRACE(X, ...) \
    defaultLogger.logprintf(#X, __VA_ARGS__)

#define LOGBUFFER(N, X, Y) defaultLogger.logBuffer((void*)X, (int)Y, N)

#endif

#endif

};
