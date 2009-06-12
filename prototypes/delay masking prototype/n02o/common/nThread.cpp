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
#define nPThreadB
#include "nThread.h"

#ifdef __GNUC__
#pragma GCC visibility push(internal)
#endif /* __GNUC__ */

#include "../errr.h"

slist<nPThread*, NTHREAD_MAX> nPThread::list;

void* __stdcall nPThreadProc (nPThread* thread) {
	nPThread::list.add(thread);
	#if !defined(linux)
#ifndef DONT_USE_SEH
	__try {
#endif
		thread->run();
#ifndef DONT_USE_SEH
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		nPThread::list.remove(thread);
		return 0;
	}
#endif
	#else
		thread->run();
	#endif
	nPThread::list.remove(thread);
	return 0;
}


int nPThread::create(){
	if (0==pthread_create(&t.handlep, NULL, (void* (*)(void*))nPThreadProc, this)) {
		return -1;
	}
	return 0;
}

int nPThread::capture(){
	list.add(this);
	t.handlep = pthread_self();
	return 0;
}

int nPThread::destroy(){
	list.remove(this);
	return (pthread_kill(t.handlep, 0)==0)?1:0;
}

int nPThread::prioritize(int priority){
	return 0;
}

int nPThread::status(){
	return 0;
}

void nPThread::yield(){
	pthread_yield();
}

void nPThread::sleep(int seconds){
	Sleep(seconds * 1000);
}
void nPThread::msleep(int ms){
	Sleep(ms);
}


#ifdef __GNUC__
#pragma GCC visibility pop
#endif /* __GNUC__ */
