/******************************************************************************
***********   .d8888b.   .d8888b    *******************************************
**********   d88P  Y88b d88P  Y88b   ******************************************
*            888    888        888   ******************************************
*    8888b.  888    888      .d88P   ******************************************
*   888 "88b 888    888  .od888P"  ********************************************
*   888  888 888    888 d88P"    **********************************************
*   888  888 Y88b  d88P 888"        *******************************************
*   888  888  "Y8888P"  888888888            Open Kaillera: http://okai.sf.net
******************************************************************************/
#pragma once

#ifndef TRACE

	#ifndef MSTR
	#define MSTR(X) #X
	#endif

	#ifdef N_DEBUG
		#define TRACE(CLASS_, FCN_, OBJ_)\
			printf("(%x)%s::%s\t\t" __FILE__ ":%i\n", OBJ_, MSTR(CLASS_), MSTR(FCN_),__LINE__);
	#else
		#define TRACE(X, Y, Z)
	#endif

#endif

#include "slist.h"

///////////////////////////////////////////////////////////////////////////////
// Build Parameters
///////////////////////////////////////////////////////////////////////////////
// Maximum no of threads
#define NTHREAD_MAX 256


///////////////////////////////////////////////////////////////////////////////
// Includes and compatibility
///////////////////////////////////////////////////////////////////////////////

#ifdef nPThreadB
	#if !defined(linux)

		#define _WIN32_WINNT 0x0400
		#include <windows.h>

		#define NTHREAD_PRIORITY_CRITICAL	THREAD_PRIORITY_TIME_CRITICAL
		#define NTHREAD_PRIORITY_HIGH		THREAD_PRIORITY_HIGHEST
		#define NTHREAD_PRIORITY_NORMAL		THREAD_PRIORITY_NORMAL
		#define NTHREAD_PRIORITY_LOW		THREAD_PRIORITY_LOWEST
		#define NTHREAD_PRIORITY_IDLE		THREAD_PRIORITY_IDLE
		
		#define pthread_t HANDLE

		inline int pthread_create (pthread_t *thread, void *attr, void *(*start_routine) (void *), void *arg) {
			HANDLE handle;
			handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, 0);
			if (handle) {
				*thread = handle;
				return 0;
			} else {
				return GetLastError(); 
			}
		}

		inline int pthread_kill(pthread_t thread, int sig){
			DWORD exiT;
			GetExitCodeThread(thread, &exiT);
			return (TerminateThread(thread, exiT)!=0)? 0: GetLastError();
		}

		#define pthread_self() GetCurrentThread()
		#define pthread_yield() SwitchToThread();

	#else

		#include <signal.h>
		#include <pthread.h>

		#define NTHREAD_PRIORITY_CRITICAL	15
		#define NTHREAD_PRIORITY_HIGH		2
		#define NTHREAD_PRIORITY_NORMAL		0
		#define NTHREAD_PRIORITY_LOW		-2
		#define NTHREAD_PRIORITY_IDLE		-15
		
		#define Sleep(X) usleep(X * 1000);

	#endif
#endif


///////////////////////////////////////////////////////////////////////////////


class nPThread {
protected:
	union {
		void * handle;
	#ifdef nPThreadB
		pthread_t handlep;
	#endif
	} t;

public:
	static slist<nPThread*, NTHREAD_MAX> list;

public:
	virtual void run(void) {}
public:


	int create();
	int capture();
	int destroy();
	int prioritize(int priority);
	int status();
	void yield();
	void sleep(int seconds);
	void msleep(int ms);

};


#define nThread nPThread
