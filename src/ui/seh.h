#pragma once

#include <windows.h>

namespace n02 {

	LONG CALLBACK seh_exception_filter_function(_EXCEPTION_POINTERS *ExceptionInfo);
	void seh_initialize();
	void seh_terminate();

};


