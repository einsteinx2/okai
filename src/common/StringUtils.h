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
// Remnoving all TCHAR stuff...its not a c++ standard
//#include <tchar.h>
#include <string.h>
#include <cstdio>
#pragma intrinsic(memcpy, strcpy, strlen)

#if !defined(N02_WIN32) || defined(N02_GNU)

#define sprintf_s(Y, L, X, ...) \
{\
    char buffer[1024];\
    sprintf(buffer, X, __VA_ARGS__);\
    strncpy(Y, buffer, L);\
}
#endif
// TODO : Review
// TODO : Test
namespace n02 {

    class StringUtils {

    public:

        // playing with integers
        static void intToAlpha(char * a, const int i);

        static char * intToAlpha(const int i);

        static void uintToAlpha(char * a, const unsigned int i);

        static char * uintToAlpha(const unsigned int i);

        static void uintToHex(char * a, const unsigned int i);

        static char * uintToHex(const unsigned int i);

        static unsigned int alphaToUint(const char * a);

        static int alphaToInt(const char * a);		

        // buffering
        static void bytesToStr(char * strbuffer, const unsigned char * bytes_buffer, const int len, const int spacing = 123456789);

        static void strToBytes(unsigned char * bytes_buffer, char * strbuffer, int len);

        static void bytesToPrintable(char * strbuffer, unsigned char * bytes_buffer, int len);

        //misc
        static bool isPrintable(char x);
        static bool isAlpha(char x);
        static bool isAlnum(char x);

        static void trim(char * buffer);

        // conversion
        static int WIDEToUTF8 (unsigned char * buffer, const wchar_t * text);
        static int UTF8ToWIDE (wchar_t * dest, const unsigned char * buffer, int bufferSizeBytes);

		// base 64 encoding
		// ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890+/
		// well...base64 is 8 bit -> 6 bit encoding so string buffer length should be
		// max buffer len for encoding = 1024
		// returns len of new buffer null terminated = (len * 8 / 6) + 2
		static int base64encode(char * destination, const void * sourceBuffer, const int len);
		// returns len of new buffer null terminated = (strlen(source) * 6 / 8) + 1
		static int base64decode(void * destination, const char * source);

        //
        static void initialize();

    };

};

