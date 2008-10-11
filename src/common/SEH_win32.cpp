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
#include <windows.h>

namespace n02 {

    static char * ExceptionCodeToStr(int code) {
#define ExceptionCodeToStr_h(X) if (code==X) return #X;
        ExceptionCodeToStr_h(EXCEPTION_ACCESS_VIOLATION);
        ExceptionCodeToStr_h(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
        ExceptionCodeToStr_h(EXCEPTION_BREAKPOINT);
        ExceptionCodeToStr_h(EXCEPTION_DATATYPE_MISALIGNMENT);
        ExceptionCodeToStr_h(EXCEPTION_FLT_DENORMAL_OPERAND);
        ExceptionCodeToStr_h(EXCEPTION_FLT_DIVIDE_BY_ZERO);
        ExceptionCodeToStr_h(EXCEPTION_FLT_INEXACT_RESULT);
        ExceptionCodeToStr_h(EXCEPTION_FLT_INVALID_OPERATION);
        ExceptionCodeToStr_h(EXCEPTION_FLT_OVERFLOW);
        ExceptionCodeToStr_h(EXCEPTION_FLT_STACK_CHECK);
        ExceptionCodeToStr_h(EXCEPTION_FLT_UNDERFLOW);
        ExceptionCodeToStr_h(EXCEPTION_ILLEGAL_INSTRUCTION);
        ExceptionCodeToStr_h(EXCEPTION_IN_PAGE_ERROR);
        ExceptionCodeToStr_h(EXCEPTION_INT_DIVIDE_BY_ZERO);
        ExceptionCodeToStr_h(EXCEPTION_INT_OVERFLOW);
        ExceptionCodeToStr_h(EXCEPTION_INVALID_DISPOSITION);
        ExceptionCodeToStr_h(EXCEPTION_NONCONTINUABLE_EXCEPTION);
        ExceptionCodeToStr_h(EXCEPTION_PRIV_INSTRUCTION);
        ExceptionCodeToStr_h(EXCEPTION_SINGLE_STEP);
        ExceptionCodeToStr_h(EXCEPTION_STACK_OVERFLOW);
        static char ExceptionCodeToStr_t[16];
        wsprintfA(ExceptionCodeToStr_t, "%i", code);
        return ExceptionCodeToStr_t;
#undef ExceptionCodeToStr_h
    }

    //LRESULT CALLBACK ErrorReporterDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    //	switch (uMsg) {
    //		case WM_INITDIALOG:
    //			if (lParam != 0) {
    //				_EXCEPTION_POINTERS *EI = (_EXCEPTION_POINTERS*)lParam;
    //				char EBX[2000];
    //				wsprintf(EBX, "EXCEPTION_RECORD {\r\n"
    //								"\tExceptionCode=%s;\r\n"
    //								"\tExceptionFlags=%i;\r\n"
    //								"\tExceptionAddress=0x%08x;\r\n"
    //								"\tExceptionInformation=%i;\r\n"
    //								"};",
    //								ExceptionCodeToStr(EI->ExceptionRecord->ExceptionCode),
    //								EI->ExceptionRecord->ExceptionFlags,
    //								EI->ExceptionRecord->ExceptionAddress,
    //								(EI->ExceptionRecord->ExceptionInformation!=0 && EI->ExceptionRecord->NumberParameters>0)? EI->ExceptionRecord->ExceptionInformation[0]:-1
    //								);
    //
    //				HWND ct = GetDlgItem(hDlg, TXT_CHAT);
    //
    //				re_append(ct, EBX, 0x000000FF);
    //				kprintf(EBX);
    //#ifdef TRACE
    //				re_append(ct, "TRACE STACK: \r\n", 0x0000FF00);
    //				for (unsigned int x = 1; x <= n02_TRACE_stack_len && x <=n02_TRACE_stack_pos ; x++ ) {
    //					wsprintf(EBX, "%u: %s[%i]\r\n", (n02_TRACE_stack_pos-x), n02_TRACE_stack[(n02_TRACE_stack_pos-x)&n02_TRACE_stack_mask].file, n02_TRACE_stack[(n02_TRACE_stack_pos-x)&n02_TRACE_stack_mask].line);
    //					kprintf(EBX);
    //					re_append(ct, EBX, 0x00FF0000);
    //				}
    //#endif
    //				flash_window(hDlg);
    //
    //				SendMessage(ct, WM_VSCROLL, SB_TOP, 0);
    //
    //			}
    //			break;
    //		case WM_CLOSE:
    //			EndDialog(hDlg, 0);
    //			break;
    //		case WM_COMMAND:
    //			if (LOWORD(wParam)==BTN_REPORT){
    //
    //			} else if (LOWORD(wParam)==BTN_CLOSE) {
    //				SendMessage(hDlg, WM_CLOSE, 0, 0);
    //			} else if (LOWORD(wParam)==BTN_LEAVE){
    //				ExitProcess(1);
    //			}
    //			break;
    //	};
    //	return 0;
    //}

    LONG CALLBACK seh_exception_filter_function(_EXCEPTION_POINTERS * EI) {
		LOGBASIC("Exception %i %s", PosixThread::getCurrentThreadId(), ExceptionCodeToStr(EI->ExceptionRecord->ExceptionCode));
		LOGBASIC(" - flags = %i", EI->ExceptionRecord->ExceptionFlags);
		LOGBASIC(" - address = %08x", EI->ExceptionRecord->ExceptionAddress);
		LOGBASIC(" - info = %i", (EI->ExceptionRecord->ExceptionInformation!=0 && EI->ExceptionRecord->NumberParameters>0)? EI->ExceptionRecord->ExceptionInformation[0]:-1);
		LOGBASIC(" - trace stack:");
        trace_log();
		MessageBoxA(0, "See log file for more details", "Exception occured", 0);
		return EXCEPTION_CONTINUE_EXECUTION;
    }


    void sehInitialize(){
        SetUnhandledExceptionFilter(seh_exception_filter_function);

        OSVERSIONINFO osvi;
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        GetVersionEx (&osvi);

        char * WINDESCS[] = {
            "Win32s",
            "Win9x",
            "WinNT",
            "Other",
            "Other",
            "Other"
        };

		LOGBASIC("%s %i.%i %s", WINDESCS[osvi.dwPlatformId], osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.szCSDVersion);
    }

    void sehTerminate(){

    }

};
