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

// 10-233. Helper objects and misc related ids
// 300-499. Kaillera specific
// 500-699. P2P specific
// 700-899. SC specific
// 900-1099. zsnesnet specific
// 1100-1299. Player specific

#define LID_NAME 1
#define LID_DEL1 10
#define LID_DEL2 11
#define LID_YES1 12
#define LID_NO01 13
#define LID_ADD1 14
#define LID_EDT1 15
#define LID_SELG 16
#define LID_ERR1 17
#define LID_NGOL 18
#define LID_PLRS 19
#define LID_STS1 20
#define LID_FRMP 21
#define LID_ADD2 22
#define LID_CAN1 23
#define LID_EDT2 24
#define LID_OK01 25
#define LID_NM01 26
#define LID_IP01 27
#define LID_UCON 28
#define LID_UCO1 29

#define LID_KAILLERA_C 300
#define LID_KAILLERA_J 301
#define LID_KAILLERA_L 302
#define LID_KAILLERA_LI 303
#define LID_KAILLERA_K 304
#define LID_KAILLERA_GJ 305
#define LID_KAILLERA_GL 306
#define LID_KAILLERA_GS 307
#define LID_KAILLERA_GE 308
#define LID_KAILLERA_CT 309
#define LID_KAILLERA_EI 310
#define LID_KAILLERA_EP 311
#define LID_KAILLERA_PD 312
#define LID_KAILLERA_WN 313
#define LID_KAILLERA_WP 314
#define LID_KAILLERA_WC 315
#define LID_KAILLERA_WS 316
#define LID_KAILLERA_WG 317
#define LID_KAILLERA_WA 318
#define LID_KAILLERA_WO 319
#define LID_KAILLERA_WP2 321
#define LID_KAILLERA_C2 322

// make sure these are sorted by ID

#define DEFAULT_LOC_ENTRIES \
	_ENTRY(LID_NAME, "Open Kaillera") \
	_ENTRY(LID_DEL1, "Delete item") \
	_ENTRY(LID_DEL2, "Do you want to delete the seleted item") \
	_ENTRY(LID_YES1, "Yes") \
	_ENTRY(LID_NO01, "No") \
	_ENTRY(LID_ADD1, "Add Item") \
	_ENTRY(LID_EDT1, "Edit Item") \
	_ENTRY(LID_SELG, "Select game") \
	_ENTRY(LID_ERR1, "Error") \
	_ENTRY(LID_NGOL, "No games in list") \
	_ENTRY(LID_PLRS, "%i players") \
	_ENTRY(LID_STS1, ", states") \
	_ENTRY(LID_FRMP, "+%i frames") \
	_ENTRY(LID_ADD2, "Add") \
	_ENTRY(LID_CAN1, "Cancel") \
	_ENTRY(LID_EDT2, "Edit") \
	_ENTRY(LID_OK01, "ok") \
	_ENTRY(LID_NM01, "Name:") \
	_ENTRY(LID_IP01, "IP:") \
	_ENTRY(LID_UCON, "Placeholder") \
	_ENTRY(LID_UCO1, "This feature has not been implemented yet") \
	\
	\
	\
	_ENTRY(LID_KAILLERA_C, "Connecting...") \
	_ENTRY(LID_KAILLERA_J, "* %s joined the partyline") \
	_ENTRY(LID_KAILLERA_L, "* %s left the partyline") \
	_ENTRY(LID_KAILLERA_LI, "Logged in") \
	_ENTRY(LID_KAILLERA_K, "* You have been kicked out of the game") \
	_ENTRY(LID_KAILLERA_GJ, "* %s joined the game") \
	_ENTRY(LID_KAILLERA_GL, "* %s left the game") \
	_ENTRY(LID_KAILLERA_GS, "Game started") \
	_ENTRY(LID_KAILLERA_GE, "Game ended") \
	_ENTRY(LID_KAILLERA_CT, "Connecting to %s") \
	_ENTRY(LID_KAILLERA_EI, "Error initializing core") \
	_ENTRY(LID_KAILLERA_EP, "Unable to parse IP address") \
	_ENTRY(LID_KAILLERA_PD, "* %s (Player %i) dropped") \
	_ENTRY(LID_KAILLERA_WN, "Nick") \
	_ENTRY(LID_KAILLERA_WP, "Ping") \
	_ENTRY(LID_KAILLERA_WC, "C.S.") \
	_ENTRY(LID_KAILLERA_WS, "Status") \
	_ENTRY(LID_KAILLERA_WG, "Game") \
	_ENTRY(LID_KAILLERA_WA, "App") \
	_ENTRY(LID_KAILLERA_WO, "Owner") \
	_ENTRY(LID_KAILLERA_WP2, "Players") \
	_ENTRY(LID_KAILLERA_C2, "Connected to ")




