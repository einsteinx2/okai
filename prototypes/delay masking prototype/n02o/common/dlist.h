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
//dlists maintain a fixed length but can grow
#pragma once
#include <memory>

template <class _Type, int _MinLen = 32>//, _Type _DefValue = 0>
class dlist {
public:
	_Type * items;	
	int length;

public:

	dlist(){
		length = 0;
		items = 0;
	}

	~dlist(){
		if (items != 0) {
			free(items);
			items = 0;
			length = 0;
		}
	}

	//add 	adds an element to the list
	void add(_Type element){
		if (items == 0) {
			items = (_Type*)malloc(_MinLen * sizeof(_Type));
		} else {
			if (length >= _MinLen)
				items = (_Type*)realloc(items, (length + 1) * sizeof(_Type));
		}
		items[length++] = element;
	}

	//remove 	removes an element from the list
	void remove(int i){
		if (i >= 0 && i < length) {
			int l = length-1;
			if (l!=i) {
				items[i] = items[l];
			}
			length = l;
			if (l >= _MinLen) {
				items = (_Type*)realloc(items, l * sizeof(_Type));
			}
		}
	}

	void remove(_Type t){
		int m = length;
		for (int i = 0; i < m; i++) {
			if (items[i] == t) {
				remove(i);
				break;
			}
		}
	}

	//set 	sets an element value in the list
	void set(_Type v, int i){
		if (i >=0 & i < length) {
			items[i] = v;
		}
	}

	//get 	gets an element value in the list
	_Type get(int i){
		return (i >= 0 && i < length)? items[i] : 0;//_DefValue;
	}

	_Type operator[] (const int i) {
		return items[i];
	}

	//clear 	removes all elements from the list	
	void clear(){
		length = 0;
		if (items != 0){
			items = (_Type*)realloc(items, _MinLen * sizeof(_Type));
		}
	}

	//size 	returns the number of items in the list
	int size(){
		return length;
	}
};

