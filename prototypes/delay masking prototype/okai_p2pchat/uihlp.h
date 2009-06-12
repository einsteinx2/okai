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

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>

#include "richedit.h"
#include "commctrl.h"



class nTab{
public:
	HWND handle;

	inline void AddTab(char * name, int index){
		TCITEM tie;
		tie.mask = TCIF_TEXT | TCIF_PARAM;
		tie.pszText = name;
		tie.lParam = index;
		TabCtrl_InsertItem(handle, index, &tie);
	}

	inline void DeleteAllTabs(){
		TabCtrl_DeleteAllItems(handle);
	}

	inline void SelectTab(int index){
		TabCtrl_SetCurSel(handle,index);
	}
	inline int SelectedTab(){
	//	TCITEM tie;
	//	tie.mask = TCIF_PARAM;
	//	TabCtrl_GetItem(handle, TabCtrl_GetCurSel(handle), &tie);
	//	return tie.lParam;
		return TabCtrl_GetCurSel(handle);
	}
};

class nProgressBar{
public:
	HWND handle;
	void SetRange(int min, int max){
		SendMessage(handle, PBM_SETRANGE, 0, MAKELPARAM(min, max));
	}
	void SetValue(int value){
		SendMessage(handle, PBM_SETPOS, value, 0);
	}
};

class nLVw {
protected:
	int cols;
	int format;
	int mask;
	int recz;

public:
    HWND handle;

	void initialize(){
		cols=0;
		recz = 0;
	}

	int AddRow (const char * content){
		LVITEM lvI;
		lvI.mask        = LVIF_TEXT | LVIF_PARAM;
		lvI.iItem       = recz++;
		lvI.iSubItem    = 0;
		lvI.pszText		= (char*)content;
		lvI.lParam		= lvI.iItem;

		return ListView_InsertItem(handle, &lvI);
	}


	int AddRow(char*content, int rowno){
		LVITEM lvI;
		lvI.mask        = LVIF_TEXT | LVIF_PARAM;
		lvI.iItem       = rowno;
		lvI.iSubItem    = 0;
		lvI.pszText		= content;
		lvI.lParam		= rowno;

		recz++;

		ListView_InsertItem(handle, &lvI);
		return rowno;
	}

	int Find(int rowno){
		LVFINDINFO fi;
		fi.flags = LVFI_PARAM;
		fi.lParam = rowno;
		return ListView_FindItem(handle, -1, &fi);
	}

	int Find(char * rowstr){
		LVFINDINFO fi;
		fi.flags = LVFI_STRING;
		fi.psz = rowstr;
		return ListView_FindItem(handle, -1, &fi);
	}

	int RowNo(int index){
		LVITEMA lvI;
		lvI.mask        = LVIF_PARAM;
		lvI.iItem       = index;
		lvI.iSubItem    = 0;
		SendMessage(handle, LVM_GETITEM, 0, (LPARAM)(&lvI));
		return lvI.lParam;
	}

	int RowsCount(){
		return ListView_GetItemCount(handle);
	}


	void DeleteRow(int rowno){
		if (rowno>0) {
			recz--;
			ListView_DeleteItem(handle, rowno);
		}
	}

	inline void FillRow (char * content, int coli, int rowi){
		/*
		LVITEMA lvI;
		lvI.mask        = LVIF_TEXT;
		lvI.iItem       = rowi;
		lvI.iSubItem    = coli;
		lvI.pszText		= content;
		SendMessage(handle, LVM_SETITEM, 0, (LPARAM)(&lvI));*/
		ListView_SetItemText(handle, rowi, coli, content);
	}

	void FillRows (int rowi, int colss, ...){
		va_list args;
		va_start (args, colss);
		for (int x = 1; x <= colss; x++) {
			FillRow(va_arg(args, char*), x, rowi);
		}
		va_end (args);
	}

	void CheckRow (char * content, int llen, int coli, int rowi){
		LVITEMA lvI;
		lvI.mask        = LVIF_TEXT;
		lvI.iItem       = rowi;
		lvI.iSubItem    = coli;
		lvI.pszText		= content;
		lvI.cchTextMax	= llen;
		SendMessage(handle, LVM_GETITEM, 0, (LPARAM)(&lvI));
	}

	void AddColumn(char * name, int colw){
		LVCOLUMNA lvc;
		lvc.mask		= LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		lvc.iSubItem	= cols++;
		lvc.pszText		= name;
		lvc.cx			= colw;
		lvc.fmt			= LVCFMT_LEFT;

		ListView_InsertColumn(handle, (cols-1), &lvc);
	}

	void DeleteAllRows(){
		ListView_DeleteAllItems(handle);
		recz=0;
	}

	int SelectedRow(){
		return ListView_GetSelectionMark(handle);
	}
	void SelectRow(int i){
		ListView_EnsureVisible(handle, i, FALSE);
	}

	void FullRowSelect(void){
		LRESULT l = SendMessage(handle, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
        SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, l|LVS_EX_FULLROWSELECT);
	}

	void FullRowUnSelect(void){
		LRESULT l = SendMessage(handle, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
		l = (l & ~LVS_EX_FULLROWSELECT);
        SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, l);
	}

};

#define COLOR_DEBUG 0x00FF0000
#define COLOR_ERROR 0x000000FF
#define COLOR_NORMAL 0x00000000
#define COLOR_SYSMSG 0x00999999


inline void re_append(HWND hwnd, char * line, COLORREF color = 0){
	//kprintf(line);
	int i = strlen(line);
	CHARRANGE cr;
	GETTEXTLENGTHEX gtx;
	gtx.codepage = CP_ACP;
	gtx.flags = GTL_PRECISE;
	cr.cpMin = GetWindowTextLength(hwnd);//SendMessage(p2p_ui_con_richedit, EM_GETTEXTLENGTHEX, (WPARAM)&gtx, 0);
	cr.cpMax = cr.cpMin +i;
	SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	CHARFORMATA crf;
	crf.dwMask = CFM_COLOR;
	crf.cbSize = sizeof(crf);
	crf.crTextColor = color;
	SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&crf);
	SendMessage(hwnd, EM_REPLACESEL, FALSE, (LPARAM)line);
	//SendMessage(hwnd, EM_EXSETSEL, 0, (LPARAM)&cr);
	//SendMessage(hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&crf);
	
	SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);
}

#include "winuser.h"
#ifndef FLASHW_ALL

typedef struct {  
	UINT cbSize;
	HWND hwnd;
	DWORD dwFlags;
	UINT uCount;
	DWORD dwTimeout;
} FLASHWINFO, *PFLASHWINFO;

WINUSERAPI BOOL WINAPI FlashWindowEx(
  PFLASHWINFO pfwi
);

#define FLASHW_ALL          3
#define FLASHW_TIMERNOFG    12
#define FLASHW_TIMER        0x00000004

#endif


void flash_window(HWND handle);

