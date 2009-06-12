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

typedef struct {
	int length;
	char ** items;
}spcharlist;

#define spcharlist_new(name) spcharlist * name = new spcharlist; name->items = 0; name->length = 0;
void spcharlist_add(spcharlist*name, char*item);
void spcharlist_delete(spcharlist*name);


spcharlist * static_explode_char(char chr, char * stringg);
spcharlist * static_find_chart_tokens(char chr, char * stringg, int length);
spcharlist * static_find_chart_tokens(char chr, char * stringg, int length, int limit);
