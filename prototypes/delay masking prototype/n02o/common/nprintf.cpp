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
#include "nprintf.h"

inline void c2hex_(char * strbuffer, unsigned char byte){
	static char h2ctable[16] = 
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	unsigned char lob = byte & 0xf;
	unsigned char hib = (byte & 0xf0)>>4;
	*strbuffer++ = h2ctable[hib];
	*strbuffer = h2ctable[lob];
}


//345678901234567890123456789012345678901234567890123456789012345678901234567890
// Convert an unsigned integer to alpha
void nutoa(char * a, const unsigned int i) {
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


//345678901234567890123456789012345678901234567890123456789012345678901234567890
// Convert a signed integer to alpha
void nitoa(char * a, const int i) {
	unsigned int u = (unsigned int) i;
	if (u & (1<<31)) { //signed
		u = ~(u-1);
		*a++ = '-';
	}
	nutoa(a, u);
}


//345678901234567890123456789012345678901234567890123456789012345678901234567890
// Convert an unsigned integer to hex alpha
void nutoah(char * a, const unsigned int i) {
	unsigned int _i = i;
	a += 8; *a = 0;
	a -= 2; c2hex_(a, _i & 0xFF); _i = _i >> 8;
	a -= 2; c2hex_(a, _i & 0xFF); _i = _i >> 8;
	a -= 2; c2hex_(a, _i & 0xFF); _i = _i >> 8;
	a -= 2; c2hex_(a, _i & 0xFF);
}



//345678901234567890123456789012345678901234567890123456789012345678901234567890
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
	bool hasPadding;
	bool hasPaddingZero;
	int fieldWidth;
	nprintftype type;
} nprintfToken;


inline void advance_get_token(const char ** txt, nprintfToken * token){
	
	const char * str = *txt;
	token->fieldWidth = 0;

	//check if its a variable
	if (*str == '%') {
		str++;
		switch (*str++) {
			case 'u':
				token->type = npft_uint;
				break;
			case 'i':
				token->type = npft_int;
				break;
			case 's':
				token->type = npft_str;
				break;
			case 'c':
				token->type = npft_char;
				break;
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

#define S_(X) *((char**)X)
#define I_(X) *((int*)X)
#define C_(X) *((char*)X)
#define U_(X) *((unsigned int*)X)


#define _ADVANCE_ARG(B) (void*)(((char *)B) + sizeof(char**))


void __cdecl nvsprintf( char *buffer, const char *format, void * argbase) {
	
	void * arg = _ADVANCE_ARG(argbase);

	const char * fmt = format;

	if (format !=0 && *format != 0) {
		nprintfToken token;
		bool loop = true;
		do {
			const char * base = fmt;
			advance_get_token(&fmt, &token);

			switch (token.type) {
				case npft_uint:
					{
						//current argument is a string
						unsigned int u = U_(arg);
						//move to next argument
						arg = _ADVANCE_ARG(arg);

						nutoa(buffer, u);
						while (*buffer)
							buffer++;
					}
					break;
				case npft_int:
					{
						//current argument is a string
						int i = U_(arg);
						//move to next argument
						arg = _ADVANCE_ARG(arg);

						nitoa(buffer, i);
						while (*buffer)
							buffer++;
					}
					break;
				case npft_hex:
					{
						//current argument is a string
						unsigned int u = U_(arg);
						//move to next argument
						arg = _ADVANCE_ARG(arg);

						nutoah(buffer, u);
						while (*buffer)
							buffer++;
					}
					break;
				case npft_char:
					{
						//current argument is a string
						char c = C_(arg);
						//move to next argument
						arg = _ADVANCE_ARG(arg);
						*buffer++ = c;
					}
					break;
				case npft_str:
					{
						//current argument is a string
						char *str = S_(arg);
						//move to next argument
						arg = _ADVANCE_ARG(arg);
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
					loop = false;
					break;
			};
		} while(loop);
	}
}




void __cdecl nsprintf( char *buffer, const char *format, ... ){
	nvsprintf(buffer, format, &format);
}

#include <windows.h>

static HANDLE OutpHandle = 0;

void __cdecl nprintf( const char *format, ... ){
	char buffer[2048];
	nvsprintf(buffer, format, &format);


	if (!OutpHandle) {
		OutpHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	DWORD xxx;

	WriteFile(OutpHandle, buffer, strlen(buffer), &xxx, 0);
}


void __cdecl nvprintf(const char *format, void * argbase){
	char buffer[2048];
	nvsprintf(buffer, format, argbase);


	if (!OutpHandle) {
		OutpHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	}
	DWORD xxx;

	WriteFile(OutpHandle, buffer, strlen(buffer), &xxx, 0);
}

