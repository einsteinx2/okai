/******************************************************************************
>         .d8888b.   .d8888b.                                                 <
>        d88P  Y88b d88P  Y88b                                                <
>        888    888        888                                                <
88888b.  888    888      .d88P                                                <
888 "88b 888    888  .od888P"                                                 <
888  888 888    888 d88P"                                                     <
888  888 Y88b  d88P 888"                                                      <
888  888  "Y8888P"  888888888              Open Kaillera Arcade Netplay Library
*******************************************************************************
Copyright (c) Open Kaillera Team 2003-2008

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice must be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#pragma once


/////////////////////////////////////////////////
// KREC v1 definitions

#pragma pack(push, 1)

// header file for krec v1 files
// will have to remember to upgrade the time field to 64bit int when c++ standardizes its 64bit integer type
typedef struct {
	int version;// 4
	unsigned int time;// 8
	unsigned short playerNo; // 10
	unsigned short totalPlayers; // 12
	char game[128]; // 12 + 128
	char app[128]; // 12 + 256 "app;ver"
	char comment[239]; // 12 + 256 + 243
	unsigned int crc; //checksum
	unsigned char Compressed; // 12 + 256 + 244 = 256 + 256 = 512
} krecFileHeader01;


// head of a data chunk
typedef struct {
	unsigned char type;
	short size;
} krecDataChunkHeader01;

#pragma pack(pop)

// data types
#define RDATA_SDATA 1
#define RDATA_ADATA 2
#define RDATA_CHAT 3
#define RDATA_DROP 4
#define RDATA_EOF 5
#define RDATA_METADATA 6
#define RDATA_COMMENT 7
#define RDATA_GSDATA 8
#define RDATA_DEFDATA 9


// Version field values

#define KREC_VERSION0 *((int*)"KRC0")
#define KREC_VERSION1 *((int*)"KRC1")

