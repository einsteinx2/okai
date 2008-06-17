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


#ifndef linux
#define nPThreaadCCNV __stdcall
#else
#define nPThreaadCCNV
#endif

namespace n02 {

	class PosixThread {

	private:

		volatile void * handle;
		volatile bool running;
		int priority;
		volatile void * waitable;

	public:
		
		virtual void run(void) {}

		PosixThread(bool captureCurrent = false);
		~PosixThread();


		int start();
		int stop();
		bool isRunning();
		bool isWaiting();

		int prioritize(int priority);

		bool wait(int timeout);

		void notify();

		static void yield();
		static void sleep(int ms);

	private:

		friend void* nPThreaadCCNV posix_thread_proc (PosixThread* thread);

	};
};

#ifndef linux

#define PTHREAD_PRIORITY_CRITICAL	15
#define PTHREAD_PRIORITY_HIGH		2
#define PTHREAD_PRIORITY_NORMAL		0
#define PTHREAD_PRIORITY_LOW		(-2)
#define PTHREAD_PRIORITY_IDLE		(-15)

#endif

