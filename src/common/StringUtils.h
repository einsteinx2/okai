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

#pragma once

#include <tchar.h>
#include <string.h>
// TODO : Review
// TODO : Test
namespace n02 {

	class StringUtils {

	public:

		// playing with integers
		static void intToAlpha(char * a, const int i);
		static void intToAlphaT(TCHAR * a, const int i);

		static char * intToAlpha(const int i);
		static TCHAR * intToAlphaT(const int i);

		static void uintToAlpha(char * a, const unsigned int i);
		static void uintToAlphaT(TCHAR * a, const unsigned int i);

		static char * uintToAlpha(const unsigned int i);
		static TCHAR * uintToAlphaT(const unsigned int i);

		static void uintToHex(char * a, const unsigned int i);
		static void uintToHexT(TCHAR * a, const unsigned int i);

		static char * uintToHex(const unsigned int i);
		static TCHAR * uintToHexT(const unsigned int i);

		static unsigned int alphaToUint(const char * a);
		static unsigned int alphaToUintT(const TCHAR * a);

		static int alphaToInt(const char * a);		
		static int alphaToIntT(const TCHAR * a);

		/*
		unordered string formating
		format: %paramNo$type

		types:
		u = unsigned int
		i, d = int
		s = string
		c = char
		x = hex unsigned int

		e.g. 

			cprintf(buffer, "second int = %2$i; first int = %1$i", firstInt, secondInt);

		does not support padding or other fancy stuff */

		static void cprintf(char *buffer, const char *format, void**arglist);
		static void tprintf(TCHAR *buffer, const TCHAR *format, void**arglist);
		


		// buffering
		static void bytesToStr(char * strbuffer, const unsigned char * bytes_buffer, const int len, const int spacing = 123456789);
		static void bytesToStrT(TCHAR * strbuffer, const unsigned char * bytes_buffer, const int len, const int spacing = 123456789);

		static void strToBytes(unsigned char * bytes_buffer, char * strbuffer, int len);
		static void strToBytesT(unsigned char * bytes_buffer, TCHAR * strbuffer, int len);

		static void bytesToPrintable(char * strbuffer, unsigned char * bytes_buffer, int len);

		//misc
		static bool isPrintable(char x);
		static bool isAlpha(char x);
		static bool isAlnum(char x);
		static bool isAlnumT(TCHAR x);
		static void trim(char * buffer);

		// conversion
		static int TCHARToUTF8 (unsigned char * buffer, const TCHAR * text);
		static int UTF8ToTCHAR (TCHAR * dest, const unsigned char * buffer, int bufferSizeBytes);

		//
		static void initialize();

	};

};

