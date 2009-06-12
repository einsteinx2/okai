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

#include "../okaiclient.h"


#define P2P_CHAT_BUFFER_LEN 32
#define P2P_GAMESYNC_WAIT 900
#define P2P_GAMECB_WAIT 200
#define P2P_VERSION "v0r7 (" __DATE__ ")"

void p2p_chat_callback(char * nick, char * msg);
void p2p_game_callback(char * game, int playerno, int maxplayers);
void p2p_end_game_callback();
void p2p_client_dropped_callback(char * nick, int playerno);
void __cdecl p2p_core_debug(char * arg_0, ...);
void p2p_change_game_callback(char* game);
bool p2p_allow_incompatible();

bool p2p_core_cleanup();
bool p2p_core_initialize(bool host, int port, char * appname, char * gamename, char * username);
bool p2p_core_connect(char * ip, int port);
void p2p_print_core_status();
void p2p_retransmit();
void p2p_drop_game();
void p2p_set_ready(bool bx);
void p2p_ping();
void p2p_send_chat(char * xxx);
bool p2p_disconnect();
void p2p_step();
bool p2p_is_connected();
bool p2p_is_connecting();
void p2p_fodipp();
void p2p_fodipp_callback(char * host);
void p2p_change_game(char* game);

int p2p_modify_play_values(void *values, int size);
int p2p_core_get_port();
void p2p_hosted_game_callback(char * game);
void p2p_ping_callback(int PING);

void p2p_send_ssrv_packet(char * cmd, int len, char * host, int port);
void p2p_ssrv_packet_recv_callback(char *cmd, int len, void*sadr);

void p2p_send_ssrv_packet(char * cmd, int len, void * sadr);


void p2p_peer_left_callback();
void p2p_peer_joined_callback(char * p33rname, char * app);
int p2p_get_frameno();


int  OKAI_CCNV p2p_synchronizeGame();
void OKAI_CCNV p2p_sendPlayValue(const void * value, const int len);
int  OKAI_CCNV p2p_playDataCheck(int ms);
int  OKAI_CCNV p2p_recvPlayValues(void * values, const int len);
int  OKAI_CCNV p2p_getLocalQueueLen();
void OKAI_CCNV p2p_endGame();
void OKAI_CCNV p2p_asyncData(const void * value, const int len, const int position);
int  OKAI_CCNV p2p_exchangePlayValues(void * value, const int len);
int  OKAI_CCNV p2p_getSelectedDelay();
int  OKAI_CCNV p2p_getBaseDelay	();

typedef struct {
	int IncomingLocalQueueLen;
	int OutgoingLocalQueueLen;

	int InstsSpare;
	int InstsInUse;
	int InstsOutUse;

	int PIPM;

	int time;

	int frameno;
	int ping;
} p2p_core_stats;

void p2p_get_core_stats(p2p_core_stats*stats);
