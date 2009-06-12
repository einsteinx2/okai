#define OKAI_BUILD
#include "../okaimodule.h"
#include "../okaiclient.h"
#include "../zlib/include/zlib.h"
#include <time.h>

#ifndef okai_recorder_common
#define okai_recorder_common

#include <windows.h>
#include <commctrl.h>

extern HINSTANCE hx;


class nProgressBar{
public:
	HWND handle;
	void SetRange(int min, int max){
		SendMessage(handle, PBM_SETRANGE32, min, max);
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



#pragma pack(push, 1)

typedef struct {
	int VERSION;// 4
	unsigned int TIMESTAMP;// 8
	unsigned short PlayerNo; // 10
	unsigned short TotalPlayers; // 12
	char Game[128]; // 12 + 128
	char App[128]; // 12 + 256
	char Comment[239]; // 12 + 256 + 243
	unsigned int crc; //checksum
	unsigned char Compressed; // 12 + 256 + 244 = 256 + 256 = 512
} record_file_header_V1;


typedef struct {
	unsigned char type;
	short size;
} recorder_chunk_head_V1;

#pragma pack(pop)

#define RDATA_VALUES 1
#define RDATA_DATA 2
#define RDATA_CHAT 3
#define RDATA_DROP 4
#define RDATA_EOF 5


#define KREC_VERSION0 *((int*)"KRC0")
#define KREC_VERSION1 *((int*)"KRC1")

// version
#define record_file_header record_file_header_V1
#define KREC_VERSION KREC_VERSION1
#define recorder_chunk_head recorder_chunk_head_V1

extern okaiModulesHandler * modHandler;

extern modDevPackInterface mod_dev;
extern gameListInterface gamelist;


//debugging
#include <cstdio>
inline void __cdecl kprintf(char * arg_0, ...) {
#if 1
	char V8[1024];
	char V88[2048];

	sprintf(V8, "%s\r\n",arg_0);
	va_list args;
	va_start (args, arg_0);
	vsprintf (V88, V8, args);
	va_end (args);
	printf(V88);
#if 0
	static HFILE hx = HFILE_ERROR;

	if (hx == HFILE_ERROR) {
		OFSTRUCT of;
		hx = OpenFile("keaa.txt", &of, OF_CREATE|OF_WRITE);
	}
	_lwrite(hx, V88, strlen(V88));
#endif
#endif
}




#endif

