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
#define LID_SWIT 30
#define LID_DEL3 31
#define LID_MODE 32
#define LID_DELA 33

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
#define LID_KAILLERA_NIK 323
#define LID_KAILLERA_CSS 324
#define LID_KAILLERA_LAN 325
#define LID_KAILLERA_EXC 326
#define LID_KAILLERA_GOO 327
#define LID_KAILLERA_AVG 328
#define LID_KAILLERA_BAD 329
#define LID_KAILLERA_LOW 330
#define LID_KAILLERA_IPP 331
#define LID_KAILLERA_FAV 332
#define LID_KAILLERA_CON 333
#define LID_KAILLERA_PIN 334
#define LID_KAILLERA_MSL 335
#define LID_KAILLERA_WST 336
#define LID_KAILLERA_SRV 337
#define LID_KAILLERA_IPA 338
#define LID_KAILLERA_NWG 339
#define LID_KAILLERA_STA 340
#define LID_KAILLERA_LEA 341
#define LID_KAILLERA_KIC 342
#define LID_KAILLERA_REC 343
#define LID_KAILLERA_DEL 344


#define LID_PLAYER_AVMM 1100
#define LID_PLAYER_LG 1101
#define LID_PLAYER_FTS 1102
#define LID_PLAYER_ANM 1103
#define LID_PLAYER_FVNS 1104
#define LID_PLAYER_PBE 1105
#define LID_PLAYER_CNRH 1106
#define LID_PLAYER_GL 1107
#define LID_PLAYER_PLG 1108
#define LID_PLAYER_SR 1109
#define LID_PLAYER_SEL 1110
#define LID_PLAYER_BR 1111
#define LID_PLAYER_PL 1112
#define LID_PLAYER_SV 1113
#define LID_PLAYER_RE 1114
#define LID_PLAYER_DE 1115
#define LID_PLAYER_GA 1116
#define LID_PLAYER_AP 1117
#define LID_PLAYER_TI 1118
#define LID_PLAYER_SI 1119
#define LID_PLAYER_CO 1120
#define LID_PLAYER_FI 1121


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
	_ENTRY(LID_OK01, "Ok") \
	_ENTRY(LID_NM01, "Name:") \
	_ENTRY(LID_IP01, "IP:") \
	_ENTRY(LID_UCON, "Placeholder") \
	_ENTRY(LID_UCO1, "This feature has not been implemented yet") \
	_ENTRY(LID_SWIT, "Switch") \
	_ENTRY(LID_DEL3, "Delete") \
	_ENTRY(LID_MODE, "Mode:") \
	_ENTRY(LID_DELA, "Delay:") \
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
	_ENTRY(LID_KAILLERA_C2, "Connected to ") \
	_ENTRY(LID_KAILLERA_NIK, "Nick:") \
	_ENTRY(LID_KAILLERA_CSS, "C.S.:") \
	_ENTRY(LID_KAILLERA_LAN, "LAN (60 PPS)") \
	_ENTRY(LID_KAILLERA_EXC, "Excelent (30 PPS)") \
	_ENTRY(LID_KAILLERA_GOO, "Good (20 PPS)") \
	_ENTRY(LID_KAILLERA_AVG, "Average (15 PPS)") \
	_ENTRY(LID_KAILLERA_BAD, "Bad (12 PPS)") \
	_ENTRY(LID_KAILLERA_LOW, "Low (10 PPS)") \
	_ENTRY(LID_KAILLERA_IPP, "IP:") \
	_ENTRY(LID_KAILLERA_FAV, "Fovorites:") \
	_ENTRY(LID_KAILLERA_CON, "connect") \
	_ENTRY(LID_KAILLERA_PIN, "ping") \
	_ENTRY(LID_KAILLERA_MSL, "master list") \
	_ENTRY(LID_KAILLERA_WST, "website") \
	_ENTRY(LID_KAILLERA_SRV, "Server") \
	_ENTRY(LID_KAILLERA_IPA, "IP Address") \
	_ENTRY(LID_KAILLERA_NWG, "new game") \
	_ENTRY(LID_KAILLERA_STA, "start") \
	_ENTRY(LID_KAILLERA_LEA, "leave") \
	_ENTRY(LID_KAILLERA_KIC, "kick") \
	_ENTRY(LID_KAILLERA_REC, "record game") \
	_ENTRY(LID_KAILLERA_DEL, "Delay") \
	\
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	 \
	_ENTRY(LID_PLAYER_AVMM, "Application version mismatch. Do you want to continue?") \
	_ENTRY(LID_PLAYER_LG, "Loading game") \
	_ENTRY(LID_PLAYER_FTS, "Replay file is too small") \
	_ENTRY(LID_PLAYER_ANM, "Record application name mismatch") \
	_ENTRY(LID_PLAYER_FVNS, "File version is not supported") \
	_ENTRY(LID_PLAYER_PBE, "Playback error") \
	_ENTRY(LID_PLAYER_CNRH, "Could not read header from file") \
	_ENTRY(LID_PLAYER_GL, "Game loaded") \
	_ENTRY(LID_PLAYER_PLG, "Playing game") \
	_ENTRY(LID_PLAYER_SR, "Save recorded file") \
	_ENTRY(LID_PLAYER_SEL, "Select a file to play") \
	_ENTRY(LID_PLAYER_BR, "browse") \
	_ENTRY(LID_PLAYER_PL, "play") \
	_ENTRY(LID_PLAYER_SV, "save") \
	_ENTRY(LID_PLAYER_RE, "refresh") \
	_ENTRY(LID_PLAYER_DE, "delete") \
	_ENTRY(LID_PLAYER_GA, "Game") \
	_ENTRY(LID_PLAYER_AP, "App/ver") \
	_ENTRY(LID_PLAYER_TI, "Time") \
	_ENTRY(LID_PLAYER_SI, "Size") \
	_ENTRY(LID_PLAYER_CO, "Comments") \
	_ENTRY(LID_PLAYER_FI, "File")


