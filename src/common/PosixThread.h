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


#ifdef N02_WIN32
#define nPThreaadCCNV __stdcall
#else
#define nPThreaadCCNV
#endif

namespace n02 {

    /*
    Class for handling threads
    */

    class PosixThread {

    private:

        // handle of the thread
        volatile void * handle;
        // true while the thread is running
        volatile bool running;
        // priority of the thread
        int priority;
        // pointer to reference to waitable mechanisms
        volatile void * waitable;

    public:

        virtual void run(void) {}

        /* constructor */
        PosixThread(bool captureCurrent = false);
        /* distructor */
        ~PosixThread();


        /* start the thread, returns 0 on success */
        int start();
        /* stop the thread, returns 0 on success */
        int stop();

        /* returns if the thread is running */
        bool isRunning();
        /* returns if the thread is waiting to be notified */
        bool isWaiting();

        /* set priority for the thread */
        int prioritize(int priority);

        /* wait for netfy for specified no of ms. returns true on notify, false for everything else */
        bool wait(int timeout);

        /* notify */
        void notify();

        /* give up the thread's current timeslice */
        static void yield();
        /* sleep for a while */
        static void sleep(int ms);

        static int getCurrentThreadId();

    private:

        /* thread proc */
        friend void* nPThreaadCCNV posix_thread_proc (PosixThread* thread);

    };
};

//#ifndef N02_WIN32

#define PTHREAD_PRIORITY_CRITICAL	15
#define PTHREAD_PRIORITY_HIGH		2
#define PTHREAD_PRIORITY_NORMAL		0
#define PTHREAD_PRIORITY_LOW		(-2)
#define PTHREAD_PRIORITY_IDLE		(-15)

//#endif

