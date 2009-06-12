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
#include "nSettings.h"
#include <windows.h>
char file[5000];
char * subm = 0;


void convert_bytes_to_str(char * strbuffer, unsigned char * bytes_buffer, int len){
	static char h2ctable[16] = 
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'A', 'B',
		'C', 'D', 'E', 'F'
	};
	while (len-->0) {
		unsigned char lob = *bytes_buffer & 0xf;
		unsigned char hib = (*bytes_buffer++ & 0xf0)>>4;
		*strbuffer++ = h2ctable[hib];
		*strbuffer++ = h2ctable[lob];
	}
	*strbuffer = 0;
}

bool printchar(char x) {
	if (isalpha(x) ||
		isalnum(x) ||
		x == '.' ||
		x == ' '
		) return true;
	return false;
}

void convert_bytes_to_stra(char * strbuffer, unsigned char * bytes_buffer, int len){
	static char h2ctable[16] = 
	{
		'0', '1', '2', '3',
		'4', '5', '6', '7',
		'8', '9', 'a', 'b',
		'c', 'd', 'e', 'f'
	};
	while (len-->0) {
		unsigned char lob = *bytes_buffer & 0xf;
		unsigned char hib = (*bytes_buffer++ & 0xf0)>>4;
		*strbuffer++ = h2ctable[hib];
		*strbuffer++ = h2ctable[lob];
	}
	*strbuffer = 0;
}

void convert_str_to_bytes(unsigned char * bytes_buffer, char * strbuffer, int len){
	static bool initialized = false;
	static unsigned char c2htable[256];
	if (!initialized) {
		memset(c2htable, 0, sizeof(c2htable));
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
		initialized = true;
	}

	while (len-->0) {
		unsigned char hib = c2htable[*strbuffer++];
		unsigned char lob = c2htable[*strbuffer++];

		*bytes_buffer++ = (hib << 4) | lob;
	}
}


void __cdecl kprintf(char * arg_0, ...);


void nSettings::Initialize(char * submo, bool forceGlobal){
	if (!forceGlobal) {
		file[0] = 0;
		GetCurrentDirectoryA(5000, file);
		strcat(file, "\\");
		strcat(file, "n02.ini");
	}
	OFSTRUCT of;
	if (forceGlobal || OpenFile(file, &of, OF_EXIST) == HFILE_ERROR) {
		strcpy(file, "n02.ini");
	}
	subm = submo;
}
void nSettings::Terminate(){

}
int nSettings::get_int(char * key, int def_){
	return GetPrivateProfileIntA(subm, key, def_, file);
}
char* nSettings::get_str(char * key, char * buf, char * def_){
	GetPrivateProfileStringA(subm, key, def_, buf, 128, file);
	return buf;
}
void nSettings::set_int(char * key, int val){
	char bft [128];
	wsprintfA(bft, "%i", val);
	WritePrivateProfileStringA(subm, key, bft, file);
}
void nSettings::set_str(char * key, char * val){
	WritePrivateProfileStringA(subm, key, val, file);
}


void nSettings::set_struct(char * key, void * structt, int len){
	if (len < 256) {
		char buffer[520];
		buffer[0] = 0;
		convert_bytes_to_str(buffer, (unsigned char *)structt, len);
		set_str(key, buffer);
	}
}

void nSettings::get_struct(char * key, void * structt, int len){
	if (len < 256) {
		char buffer[520];
		buffer[0] = 0;
		get_str(key, buffer, "");
		int slen = strlen(buffer) >> 1;
		if (slen>=len){
			len = min(len, slen);
			convert_str_to_bytes((unsigned char *)structt, buffer, len);
		}
	}
}
