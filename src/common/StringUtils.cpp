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

#include "StringUtils.h"

namespace n02 {

	static char h2ctable[16] = 
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};

	static unsigned char c2htable[256] = {0};

	static char b2ptable[256] = {0};

	static char tempBuffer[32];


	static inline void char_to_hex_(char * strbuffer, unsigned char byte) {
		unsigned char lob = byte & 0xf;
		unsigned char hib = (byte & 0xf0)>>4;
		*strbuffer++ = h2ctable[hib];
		*strbuffer = h2ctable[lob];
	}


	void StringUtils::initialize() {
		c2htable['0'] = 0;
		c2htable['1'] = 1;
		c2htable['2'] = 2;
		c2htable['3'] = 3;
		c2htable['4'] = 4;
		c2htable['5'] = 5;
		c2htable['6'] = 6;
		c2htable['7'] = 7;
		c2htable['8'] = 8;
		c2htable['9'] = 9;
		c2htable['a'] = c2htable['A'] = 10;
		c2htable['b'] = c2htable['B'] = 11;
		c2htable['c'] = c2htable['C'] = 12;
		c2htable['d'] = c2htable['D'] = 13;
		c2htable['e'] = c2htable['E'] = 14;
		c2htable['f'] = c2htable['F'] = 15;

		for (int x = 0; x < 256; x++) {
			b2ptable[x] = '.';
		}
		for (char x = ' '; x <= '~'; x++) {
			b2ptable[x] = x;
		}

	}



	void StringUtils::intToAlpha(char * a, const int i) {

		unsigned int u = (unsigned int) i;
		if (u & (1<<31)) { //signed
			u = ~(u-1);
			*a++ = '-';
		}
		uintToAlpha(a, u);

	}
	char * StringUtils::intToAlpha(const int i) {
		intToAlpha(tempBuffer, i);
		return tempBuffer;
	}

	void StringUtils::uintToAlpha(char * a, const unsigned int i) {
		unsigned int _i = i;
		char buffer[12];
		int co = 0;
		while (_i > 0 || co == 0) {
			char c = (_i % 10);
			_i = _i / 10;
			buffer[co++] = c + '0';
		}
		while (co>0) {
			*a++ = buffer[--co];
		}
		*a = 0;	
	}

	char * StringUtils::uintToAlpha(const unsigned int i) {
		uintToAlpha(tempBuffer, i);
		return tempBuffer;	
	}

	void StringUtils::uintToHex(char * a, const unsigned int i) {
		unsigned int _i = i;
		a += 8; *a = 0;
		a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
		a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
		a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
		a -= 2; char_to_hex_(a, _i & 0xFF);
	}
	char * StringUtils::uintToHex(const unsigned int i) {
		uintToHex(tempBuffer, i);
		return tempBuffer;	
	}
	unsigned int StringUtils::alphaToUint(const char * a) {

		unsigned int _i = 0;

		while (isAlnum(*a)) {
			_i *= 10;
			_i += *a++ - '0';
		}
		return _i;

	}

	int StringUtils::alphaToInt(const char * a) {
		int sign = 1;
		if (*a=='-') {
			sign = -1;
			a++;
		}
		return alphaToUint(a) * sign;
	}

	// buffering
	void StringUtils::bytesToStr(char * strbuffer, const unsigned char * bytes_buffer, const int _len, const int spacing) {
		int len = _len;
		int counter = 0;
		while (len-->0) {
			char_to_hex_(strbuffer, *bytes_buffer++);
			strbuffer += 2;
			if (++counter % spacing == 0) {
				*strbuffer = ' ';
				strbuffer++;
			}
		}
		*strbuffer = 0;
	}

	void StringUtils::strToBytes(unsigned char * bytes_buffer, char * strbuffer, int len) {
		while (len-->0) {
			unsigned char hib = c2htable[*strbuffer++];
			unsigned char lob = c2htable[*strbuffer++];
			*bytes_buffer++ = (hib << 4) | lob;
		}
	}

	void StringUtils::bytesToPrintable(char * strbuffer, unsigned char * bytes_buffer, int len) {
		while (len-->0) {
			*strbuffer = b2ptable[*bytes_buffer];
			strbuffer++;
			bytes_buffer++;
		}
		*strbuffer = 0;
	}

	//misc
	bool StringUtils::isPrintable(char x) {
		if (isAlpha(x) ||
			isAlnum(x) ||
			x == '.' ||
			x == ' '
			) return true;
		return false;
	}
	bool StringUtils::isAlpha(char x) {
		return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'A');
	}
	bool StringUtils::isAlnum(char x) {
		return (x >= '0' && x <= '9');
	}
	void StringUtils::trim(char * buffer) {
		while (*buffer != 0) {
			buffer++;
		}
		buffer--;
		while (*buffer == ' ' || *buffer == '\t' || *buffer == '\r' || *buffer == '\n') {
			*buffer = 0;
			*buffer--;
		}
	}

};

#include <string.h>
#pragma intrinsic(memcpy, strcpy, strlen)

namespace n02 {

	typedef enum {
		npft_uint,
		npft_int,
		npft_hex,
		npft_char,
		npft_str,
		npft_static,
		npft_end
	} nprintftype;

	typedef struct {
		nprintftype type;
		int argno;
		int fieldWidth;
	} nprintfToken;

	static void advance_get_token(const char ** txt, nprintfToken * token){
		const char * str = *txt;

		token->fieldWidth = 0;

		if (*str == '%') {
			str++;

			token->argno = StringUtils::alphaToInt(str);

			while (StringUtils::isAlnum(*str))
				str++;

			str++;

			switch (*str++) {
			case 'u':
				token->type = npft_uint;
				break;
			case 'd':
			case 'i':
				token->type = npft_int;
				break;
			case 's':
				token->type = npft_str;
				break;
			case 'c':
				token->type = npft_char;
				break;
			case 'X':
			case 'x':
				token->type = npft_hex;
				break;
			};

			*txt = str;

		} else {
			//static text
			if (*str) { //parse till we receive end of line or a variable
				token->type = npft_static;
				while (*str && *str != '%') {
					token->fieldWidth++;
					str++;			
				}
				*txt = str;
			} else { // end of line
				token->type = npft_end;
			}
		}
	}





#define ARGNO(N, T) ((T)(arglist[N]))

	// string formating
	void StringUtils::jprintf ( char *buffer, const char *format, void**arglist ) {

		const char * fmt = format;

		if (format !=0 && *format != 0) {

			nprintfToken token;

			token.type = npft_end;

			do {
				const char * base = fmt;
				advance_get_token(&fmt, &token);

				switch (token.type) {
				case npft_uint:
					{
						StringUtils::uintToAlpha(buffer, ARGNO(token.argno, unsigned int));
						buffer += strlen(buffer);
					}
					break;
				case npft_int:
					{
						StringUtils::intToAlpha(buffer, ARGNO(token.argno, int));
						buffer += strlen(buffer);
					}
					break;
				case npft_hex:
					{
						StringUtils::uintToHex(buffer, ARGNO(token.argno, unsigned int));
						buffer += strlen(buffer);
					}
					break;
				case npft_char:
					{
						char c = ARGNO(token.argno, char);
						*buffer++ = c;
					}
					break;
				case npft_str:
					{
						char *str =ARGNO(token.argno, char*);
						while (*str) {
							*buffer++ = *str++;
						}
					}
					break;
				case npft_static:
					while (token.fieldWidth-->0){
						*buffer++ = *base++;
					}
					break;
				case npft_end:
					*buffer = 0;
					break;
				};

			} while(token.type != npft_end);

		}

	}

#undef ARGNO

};

