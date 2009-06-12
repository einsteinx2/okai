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
#include "spcharlist.h"
#include <string.h>
#include <memory>

#include <windows.h>





void spcharlist_add(spcharlist * name, char * item) {
	if (name->items == 0) {
		name->items = (char**)malloc(32*4);
	} else {
		name->items = (char**)realloc(name->items, ((name->length / 32) + 1) * 32 * 4); 
	}
	name->items[name->length++] = item;
}

void spcharlist_delete(spcharlist * name) {
	if (name->items != 0) 
		free(name->items); 
	delete name;
}

spcharlist * static_explode_char(char chr, char * stringg) {
	spcharlist * reta = 0;
	__try { 
		spcharlist_new(rets);
		reta = rets;
		char * bptr = stringg;
		char * ptr = stringg;
		char * end = ptr + strlen(ptr);
		while (ptr < end) {
			while (ptr < end && *ptr != chr)
				ptr++;
			if (*ptr == chr) {
				*ptr = 0;
				spcharlist_add(rets, bptr);
				bptr = ++ptr;
			}
		}
		spcharlist_add(rets, bptr);
		return rets;
		
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		char ERRORRR[200];
		wsprintfA(ERRORRR, "Exception handler @ " __FILE__ ":%i activated", __LINE__);
		MessageBoxA(0, ERRORRR, "Exception handler", MB_ICONSTOP);

		if (reta != 0) {
			reta->length = 0;
		}
        return reta;

    }
}

spcharlist * static_find_chart_tokens(char chr, char * stringg, int length) {
	spcharlist_new(rets);
	char * bptr = stringg;
	char * ptr = stringg;
	char * end = ptr + length;
	while (ptr < end) {
		while (ptr < end && *ptr != chr)
			ptr++;
		if (*ptr == chr) {
			spcharlist_add(rets, bptr);
			bptr = ++ptr;
		}
	}
	spcharlist_add(rets, bptr);
	return rets;
}

spcharlist * static_find_chart_tokens(char chr, char * stringg, int length, int limit){
	spcharlist_new(rets);
	char * bptr = stringg;
	char * ptr = stringg;
	char * end = ptr + length;
	while (ptr < end && rets->length < limit) {
		while (ptr < end && *ptr != chr)
			ptr++;
		if (*ptr == chr) {
			spcharlist_add(rets, bptr);
			bptr = ++ptr;
		}
	}
	if (rets->length < limit)
		spcharlist_add(rets, bptr);
	return rets;
}
