/*
	Parts of the code in this file including all the specs and PI definitions
	was stolen from Pcsx source 1.6 beta from http://www.pcsx.net/files.shtml
*/

#pragma once

#include <windows.h>

// header version
#define _PPDK_HEADER_VERSION		3

#define PLUGIN_VERSION				1

// plugin type returned by PSEgetLibType (types can be merged if plugin is multi type!)
#define PSE_LT_CDR					1
#define PSE_LT_GPU					2
#define PSE_LT_SPU					4
#define PSE_LT_PAD					8
#define PSE_LT_NET					16


// every function in DLL if completed sucessfully should return this value
#define PSE_ERR_SUCCESS				0

// undefined error but fatal one, that kills all functionality
#define PSE_ERR_FATAL				-1


// XXX_Init return values
// Those return values apply to all libraries
// currently obsolete - preserved for compatibilty


// initialization went OK
#define PSE_INIT_ERR_SUCCESS		0

// this driver is not configured
#define PSE_INIT_ERR_NOTCONFIGURED	-2

// this driver can not operate properly on this hardware or hardware is not detected
#define PSE_INIT_ERR_NOHARDWARE		-3



/*         NET PlugIn v2       */
/* Added by linuzappz@pcsx.net */

	typedef struct {
		char EmuName[32];
		char CdromID[9];	// ie. 'SCPH12345', no \0 trailing character
		char CdromLabel[11];
		void *psxMem;
		long (__stdcall* GPUshowScreenPic)(unsigned char *);
		long (__stdcall* GPUdisplayText)(unsigned char *);
		void (__stdcall* PADsetSensitive)(int);
		char GPUpath[256];	// paths must be absolute
		char SPUpath[256];
		char CDRpath[256];
		char MCD1path[256];
		char MCD2path[256];
		char BIOSpath[256];	// 'HLE' for internal bios
		char Unused[1024];
	} netInfo;

/* Modes bits for NETsendData/NETrecvData */
#define PSE_NET_BLOCKING	0x00000000
#define PSE_NET_NONBLOCKING	0x00000001


	struct tNetPadPSX {
		unsigned char type;
		unsigned short psxPAD;
		signed char AnalogX,AnalogY;
		signed char AnalogXr,AnalogYr;
		unsigned char MouseButton;
		signed char MouseX,MouseY;
	};

	// This block is common to each of the plugins
	long CALLBACK NETinit(void);
	long CALLBACK NETshutdown(void);
	long CALLBACK NETconfigure(void);
	long CALLBACK NETtest(void);
	void CALLBACK NETabout(void);
	long CALLBACK NETopen(void* hWnd);
	long CALLBACK NETclose();
	void CALLBACK NETpause();
	void CALLBACK NETresume();
	long CALLBACK NETqueryPlayer();

	// This is present and is part of PSENetv2
	long CALLBACK NETsendPadData(void *pData, int Size);
	long CALLBACK NETrecvPadData(void *pData, int Pad);
	long CALLBACK NETsendData(void *pData, int Size, int Mode);
	long CALLBACK NETrecvData(void *pData, int Size, int Mode);
	void CALLBACK NETsetInfo(netInfo *info);
	void CALLBACK NETkeypressed(int key);

	// This is part of ePSXe
	//long CALLBACK NETcompareData(void * blockPointer, int length);
	long CALLBACK NETpadState(tNetPadPSX* p1, tNetPadPSX * p2);
	long CALLBACK NETtransferData(char * name, void * blockPointer, int length);



