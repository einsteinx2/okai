
#include "common.h"
#include <windows.h>

namespace n02 {

	char * ExceptionCodeToStr(int code) {
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
	//
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

	LONG CALLBACK seh_exception_filter_function(_EXCEPTION_POINTERS * /*ExceptionInfo*/) {
		LOG(Exception %i, PosixThread::getCurrentThreadId());
		trace_log();
		//DialogBoxParam(hx, (LPCTSTR)N02_ERRORDLG, 0, (DLGPROC)ErrorReporterDialogProc, (LPARAM)ExceptionInfo);
		return EXCEPTION_EXECUTE_HANDLER;
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

		LOG(%s %i.%i %s, WINDESCS[osvi.dwPlatformId], osvi.dwMajorVersion, osvi.dwMinorVersion, osvi.szCSDVersion);
	}

	void sehTerminate(){

	}

};
