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
#include "trace.h"
#include "Logger.h"
#include "PosixThread.h"
namespace n02 {

	typedef struct {
		char * file;
		char * function;
		int line;
		int threadID;
	} TraceStackElement;

#define TRACE_HISTORY_LEVEL 7
#define TRACE_STACK_LEN	(1<<TRACE_HISTORY_LEVEL)
#define TRACE_STACK_MASK (TRACE_STACK_LEN-1)

	static TraceStackElement traceStack[TRACE_STACK_LEN];
	static unsigned int traceStackPtr = 0;

	void trace_log(){
		for (unsigned int x = 1; x <= TRACE_STACK_LEN && x <= traceStackPtr ; x++ ) {
			int index = (traceStackPtr-x) & TRACE_STACK_MASK;
			LOGTRACE(%09u:%i-%s:%i:%s,
				(traceStackPtr-x),
				traceStack[index].threadID,
				traceStack[index].file,
				traceStack[index].line,
				traceStack[index].function);
		}
	}

	void _n02_trace(char * file, char * function, int line) {
		traceStack[traceStackPtr&TRACE_STACK_MASK].file = file;
		traceStack[traceStackPtr&TRACE_STACK_MASK].function = function;
		traceStack[traceStackPtr&TRACE_STACK_MASK].line = line;
		traceStack[traceStackPtr&TRACE_STACK_MASK].threadID = PosixThread::getCurrentThreadId();
		traceStackPtr++;
	}

};

