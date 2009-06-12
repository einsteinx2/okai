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
#pragma once

class nSettings {
public:
	static void Initialize(char * submo = "p2p", bool forceGlobal = false);
	static void Terminate();
	static int get_int(char * key, int def_ = -1);
	static char* get_str(char * key, char * buf, char * def_=0);
	static void set_int(char * key, int val);
	static void set_str(char * key, char * val);
	static void set_struct(char * key, void * structt, int len);
	static void get_struct(char * key, void * structt, int len);
};

void convert_bytes_to_str(char * strbuffer, unsigned char * bytes_buffer, int len);
void convert_str_to_bytes(unsigned char * bytes_buffer, char * strbuffer, int len);
