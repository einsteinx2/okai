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

// vary basic functions

void nitoa(char * a, const int i);
void nutoa(char * a, const unsigned int i);
void nutoah(char * a, const unsigned int i);

void __cdecl nprintf( const char *format, ... );
void __cdecl nsprintf( char *buffer, const char *format, ... );
void __cdecl nvsprintf( char *buffer, const char *format, void * argbase);


