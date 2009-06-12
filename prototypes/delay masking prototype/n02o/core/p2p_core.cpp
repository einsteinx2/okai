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
#include "p2p_core.h"

#include "p2p_message.h"
#include "../common/k_framecache.h"
#include "../errr.h"
void __cdecl kprintf(char * arg_0, ...);

#define p2p_PING_TIMES 4
#define MAX_TSYNC_READJUSTMENTS 4
#define P2P_DATA_TIMEOUT 10000

#define INCLUDE_LRDTCHECKER 1

// ------------------------------------------------------------------------

struct P2PC {
	
	bool initialized;

	// connection object
	p2p_message * connection;

	enum {NC=0, CN=1, SC=2, LD=3, LF=4, TS=5, PS=6, PL=7, DC=8} state;

	bool HOST; // host? false if connector
//	int status;//0 = NC; 1 = C; 2 = GL; 3 = NS; 4 = PL

	//misc variables for gameplay
	int throughput; // no of frames on the fly from sendFrame to recvFrame
	int frameno;	// current frame no
	int DATALEN;	//length of individual data

	//cache
	k_framecache USERDATA;
	k_framecache PEERDATA;
	

	//information
	char IP[128];
	int PORT;
	char APP[128];
	char GAME[128];

	//ready to load?
	bool USERREADY;
	bool PEERREADY;

	//names
	TCHAR USERNAME[32];
	TCHAR PEERNAME[32];

	//for pinging inside game
	int ping;
	int pingc;

	//game loaded status
	bool USERLOADED;
	bool PEERLOADED;

#ifdef INCLUDE_LRDTCHECKER
	//lower level reliable data transfer shit checker... 
	// if it desynced due to inconsistancy in lower level, we want to know
	DWORD ExpectedDataIndex;
	DWORD ToSendDataIndex;
#endif

} P2PCORE = {0};

///////////////////////////////////////////////////////////////////////////
// SSRV shit
void p2p_send_ssrv_packet(char * cmd, int len, char * host, int port){
	n02_TRACE();
	if (P2PCORE.initialized){
		P2PCORE.connection->send_ssrv(cmd, len, host, port);
	}
}
void p2p_send_ssrv_packet(char * cmd, int len, void * sadr){
	n02_TRACE();
	if (P2PCORE.initialized){
		P2PCORE.connection->send_ssrv(cmd, len, (sockaddr_in*)sadr);
	}
}

///////////////////////////////////////////////////////////////////////////
// Core initialization and stuff functions

bool p2p_core_initialize(bool host, int port, char * appname, char * gamename, char * username) {
	n02_TRACE();
	
	if (P2PCORE.initialized)
		p2p_core_cleanup();

	P2PCORE.initialized = false;

	P2PCORE.HOST = host;
	P2PCORE.PORT = port;
	strncpy(P2PCORE.APP, appname, 128);
	strncpy(P2PCORE.USERNAME, username, 32);
	strncpy(P2PCORE.GAME, gamename, 128);
	
	P2PCORE.connection = new p2p_message;

	if (!P2PCORE.connection){
		return false;
	}

	if (!P2PCORE.connection->initialize(P2PCORE.PORT, 1024*4)){
		delete P2PCORE.connection;
		P2PCORE.connection = 0;
		return false;
	}

	P2PCORE.PORT = P2PCORE.connection->get_port();
	P2PCORE.state = P2PC::NC;

	P2PCORE.USERREADY = false;
	P2PCORE.PEERREADY = false;

	P2PCORE.initialized = true;

	return true;
}


bool p2p_core_cleanup(){
	n02_TRACE();
	if (P2PCORE.initialized){
		if (P2PCORE.connection)
			delete P2PCORE.connection;
		P2PCORE.connection = 0;
		P2PCORE.initialized = false;
	}
	return true;
}

void p2p_re_initialize(){
	n02_TRACE();

	P2PCORE.state = P2PC::NC;
	P2PCORE.connection->state = p2p_message::NC;
	P2PCORE.USERREADY = false;
	P2PCORE.PEERREADY = false;
	
	if (P2PCORE.HOST) {
		
		p2p_core_debug("reinitializing host...");

		if (P2PCORE.connection)
			delete P2PCORE.connection;

		P2PCORE.connection = new p2p_message;
		
		if (!P2PCORE.connection->initialize(P2PCORE.PORT)){
			p2p_core_cleanup();
			p2p_core_debug("Error initializing socket at specified port");
			
		} else p2p_core_debug("Done");
		
	}
}

///////////////////////////////////////////////////////////////////////////
// Connection and disconnection functions

bool p2p_disconnect(){
	n02_TRACE();
	// connected and not disconnected
	if (P2PCORE.state >= P2PC::SC && P2PCORE.state != P2PC::DC) {
		// is it playing?
		if (P2PCORE.state == P2PC::PL) {
			p2p_core_debug("Cant quit while in game");
			return false;
		} else {
			//otherwise exit
			P2PCORE.connection->send_tinst(EXIT, 0);
			P2PCORE.state = P2PC::DC; //disconnected
			P2PCORE.connection->state = p2p_message::DC;
			P2PCORE.connection->close();
			return true;
		}
	} else {
		P2PCORE.state = P2PC::DC;
		return true;
	}
	n02_TRACE();
}


bool p2p_core_connect(char * ip, int port){
	
	n02_TRACE();
	
	strncpy(P2PCORE.IP, ip, 128);

	if (!P2PCORE.HOST && P2PCORE.connection->set_address(ip, port)){
		n02_TRACE();

		p2p_instruction contreq(LOGN, LOGN_REQ);
		contreq.store_sstring(P2PCORE.USERNAME);	
		contreq.store_string(P2PCORE.APP);
		P2PCORE.connection->send_instruction(&contreq);

		P2PCORE.state = P2PC::CN;

		return true;
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
// Status retrival functions
void p2p_print_core_status(){
	/*n02_TRACE();
	p2p_core_debug( "P2PCORE{ \r\n"
		"\tHOST: %s \r\n"
		"\tPORT: %i \r\n"
		"\tIP: %s \r\n"
		"\tAPP: %s \r\n"
		"\tGAME: %s \r\n"
		"\tUSERREADY: %s \r\n"
		"\tPEERREADY: %s \r\n"
		"\tUSERNAME: %s \r\n"
		"\tPEERNAME: %s \r\n"
		"\tping: %i \r\n"
		"\tpingc: %i \r\n"
		"\tCONNECTED: %s \r\n"
		"\tUSERLOADED: %s \r\n"
		"\tPEERLOADED: %s \r\n"
		"\tstatus: %i \r\n"
		"\tthroughput: %i \r\n"
		"\tframeno: %i \r\n"
		"\tDATALEN: %i \r\n"
		"\tINCACHE: %i \r\n"
		"\tOUTCACHE: %i \r\n"
		"\tCOMPILE: " __DATE__ " - " __TIME__ "\r\n}"
		, P2PCORE.HOST? "true":"false"
		, P2PCORE.PORT
		, P2PCORE.HOST? "irrelevent" : P2PCORE.IP
		, P2PCORE.APP
		, P2PCORE.GAME
		, P2PCORE.USERREADY? "true":"false"
		, P2PCORE.PEERREADY? "true":"false"
		, P2PCORE.USERNAME
		, P2PCORE.PEERNAME
		, P2PCORE.ping
		, P2PCORE.pingc
		, P2PCORE.CONNECTED? "true":"false"
		, P2PCORE.USERLOADED? "true":"false"
		, P2PCORE.PEERLOADED? "true":"false"
		, P2PCORE.status
		, P2PCORE.throughput
		, P2PCORE.frameno
		, P2PCORE.DATALEN
		, P2PCORE.USERDATA.pos
		, P2PCORE.PEERDATA.pos);*/
}

void p2p_get_core_stats(p2p_core_stats*stats) {
	if (P2PCORE.DATALEN) {
		stats->IncomingLocalQueueLen = P2PCORE.PEERDATA.pos / P2PCORE.DATALEN;
		stats->OutgoingLocalQueueLen = P2PCORE.USERDATA.pos / P2PCORE.DATALEN;
	}

	if (P2PCORE.connection) {

		stats->PIPM = P2PCORE.connection->default_ipm;

		stats->InstsSpare = P2PCORE.connection->cache.spares.size();
		stats->InstsInUse = P2PCORE.connection->cache.in.size();
		stats->InstsOutUse = P2PCORE.connection->cache.out.size();

		//stats->time = P2PCORE.connection->GetTime();
		stats->time = P2PCORE.connection->CurrentTime;

		stats->ping = P2PCORE.connection->ping;
	}
	stats->frameno = P2PCORE.frameno;
}

int p2p_core_get_port(){
	n02_TRACE();
	return P2PCORE.PORT;
}

bool p2p_is_connected(){
	return P2PCORE.state >= P2PC::SC && P2PCORE.state != P2PC::DC;
}
bool p2p_is_connecting(){
	return P2PCORE.state == P2PC::CN;
}
bool p2p_is_playing(){
	return P2PCORE.state > P2PC::SC && P2PCORE.state < P2PC::DC;
}

int p2p_get_frameno() {
	return P2PCORE.frameno;  
}

void p2p_retransmit(){
	n02_TRACE();
	//if (p2p_is_connected()){
	if (P2PCORE.connection){
		P2PCORE.connection->send_message(MAX_IPM);
	}
}


void p2p_drop_game_pasv(){
	n02_TRACE();
	if (p2p_is_connected()){
		P2PCORE.state = P2PC::SC;
		P2PCORE.connection->state = p2p_message::SC;
		P2PCORE.USERREADY = false;
		P2PCORE.PEERREADY = false;
		p2p_client_dropped_callback(P2PCORE.PEERNAME, P2PCORE.HOST? 2: 1);
		p2p_client_dropped_callback(P2PCORE.USERNAME, P2PCORE.HOST? 1: 2);
		p2p_end_game_callback();
	}
}


void p2p_set_ready(bool bx){
	n02_TRACE();
	if (P2PCORE.USERREADY != bx) {
		
		P2PCORE.USERREADY = bx;
		if (p2p_is_connected()){
			P2PCORE.connection->send_tinst(PREADY, bx? PREADY_READY:PREADY_NREADY);
		}
		
		p2p_core_debug("You are marked as %s.", (bx? "ready":"not ready"));
		
	}
}


void p2p_ping(){
	n02_TRACE();
	/*
	n02_TRACE();
	if (!P2PCORE.status) return;
	if (P2PCORE.status == 1) {
		/*
		n02_TRACE();
		p2p_instruction kx;
		kx.inst.type = PING;
		kx.inst.flags = PING_PING;
		kx.store_sstring((char*)&P2PCORE);
		if (P2PCORE.connection->send_instruction(&kx)) {
			p2p_core_debug("Peer connectivity timed out");
			p2p_peer_left_callback();
			p2p_re_initialize();
		}
		p2p_PING_TIME = p2p_GetTime();
		
	} else {
		p2p_retransmit();
	}
	*/
}



///////////////////////////////////////////////////////////////////////////
// CP2PP stuff

void p2p_send_chat(char * xxx){
	n02_TRACE();

	if (p2p_is_connected()){

		p2p_instruction kx(CP2PP, CP2PP_CHAT);
		kx.store_vstring(xxx);

		if (p2p_is_playing())
			P2PCORE.connection->send_include(&kx);
		else
			P2PCORE.connection->send_instruction(&kx);

		p2p_chat_callback(P2PCORE.USERNAME, xxx);

	}
}
void p2p_change_game(char* game){
	n02_TRACE();

	if (P2PCORE.HOST && !p2p_is_playing()) {
		p2p_instruction kx(CP2PP, CP2PP_CHNGG);
		kx.store_vstring(game);
		if (P2PCORE.connection->send_instruction(&kx)) {
			p2p_core_debug("Peer connectivity timed out");
			p2p_peer_left_callback();
			p2p_re_initialize();
		}
		strcpy(P2PCORE.GAME, game);
		p2p_core_debug("game changed to %s", P2PCORE.GAME);
	} else {
		p2p_core_debug("Error changing game");
	}
}

inline void p2p_handle_cp2pp(p2p_instruction * ki){	
	n02_TRACE();

	if (ki->inst.flags == CP2PP_CHAT) {
		char MSG[255];
		ki->load_vstring(MSG);
		p2p_chat_callback(P2PCORE.PEERNAME, MSG);
	} else if (ki->inst.flags == CP2PP_CHNGG){
		ki->load_vstring(P2PCORE.GAME);
		p2p_change_game_callback(P2PCORE.GAME);
	} else if (ki->inst.flags == CP2PP_RINFO){
		
	}

}

///////////////////////////////////////////////////////////////////////////
// Main instruction processor function
void p2p_process_instruction(p2p_instruction * ki){
	switch (P2PCORE.state) {
	case P2PC::NC: // not connected
		if (P2PCORE.HOST && ki->inst.type == LOGN) {
			if (ki->inst.flags == LOGN_REQ) {
				//set address
				P2PCORE.connection->set_addr(&P2PCORE.connection->iaddr);

				//load peername
				ki->load_sstring(P2PCORE.PEERNAME);

				//appname
				char peerapp[128];
				ki->load_string(peerapp);
				p2p_core_debug("Connection Request from %s (%s)...", P2PCORE.PEERNAME, peerapp);

				if (strcmp(peerapp, P2PCORE.APP)!=0){
					int x = 6;
					while (x-->0)
						p2p_send_chat("Emulator/version difference alert! Game will desync!");

					if (!p2p_allow_incompatible()) {
						p2p_core_debug("Invalid application/version. Rejecting peer.");
						// neg reply
						p2p_instruction kx;
						kx.inst.type = LOGN;
						kx.inst.flags = LOGN_RNEG;
						char strrrr[256];
						wsprintf(strrrr, "Application version mismatch. Host using: %s", P2PCORE.APP);
						kx.store_vstring(strrrr);
						P2PCORE.connection->send_instruction(&kx);
						p2p_re_initialize();
						return;
					}
				}

				p2p_core_debug("Waiting for reconfirmation...");

				///* // pos reply
				{
					p2p_instruction kx;
					kx.inst.type = LOGN;
					kx.inst.flags = LOGN_RPOS;
					kx.store_sstring(P2PCORE.USERNAME);
					kx.store_string(P2PCORE.GAME);
					P2PCORE.connection->send_instruction(&kx);
				}
				//*/

				/*
				
				//*/

				P2PCORE.state = P2PC::CN;
			}
		}
		break;
	case P2PC::CN: // connecting
		if (ki->inst.type == LOGN){
			if (P2PCORE.HOST) {
				if (ki->inst.flags == LOGN_RPOS) {

					p2p_core_debug("Peer reconfirmed connection");

					P2PCORE.connection->state = p2p_message::SC;
					P2PCORE.state = P2PC::SC;

					p2p_send_chat("Using version: " P2P_VERSION " - Things may behave in an unexpected manner if different versions are used");

					if (P2PCORE.USERREADY) {
						P2PCORE.connection->send_tinst(PREADY, PREADY_READY);
					}

					p2p_peer_joined_callback(P2PCORE.PEERNAME, P2PCORE.APP);

				} else {

					p2p_core_debug("Peer dropped connection (probbly doensot have the game)");
					p2p_re_initialize();

				}
			} else {
				if (ki->inst.flags == LOGN_RPOS) {

					ki->load_sstring(P2PCORE.PEERNAME);
					ki->load_string(P2PCORE.GAME);
					p2p_core_debug("Peer replied: %s (%s)", P2PCORE.PEERNAME, P2PCORE.GAME);

					P2PCORE.connection->state = p2p_message::SC;
					P2PCORE.state = P2PC::SC;

					P2PCORE.connection->send_tinst(LOGN, LOGN_RPOS);

					p2p_hosted_game_callback(P2PCORE.GAME);

					p2p_send_chat("Using version: " P2P_VERSION);

					p2p_peer_joined_callback(P2PCORE.PEERNAME, P2PCORE.APP);

				} else {

					char xxx[257];
					ki->load_vstring(xxx);

					p2p_core_debug("Peer rejected connection (%s)", xxx);

					P2PCORE.state = P2PC::DC;

				}
			}
		}
		//if (P2PCORE.connection && P2PCORE.connection->getLSElapsedTime() > 500) {
		//	p2p_retransmit();
		//}
		break;
	case P2PC::SC: // simple connection made
		{
			switch(ki->inst.type){
			case CP2PP:
				p2p_handle_cp2pp(ki);
				break;
			case PREADY:
				P2PCORE.PEERREADY = (ki->inst.flags == PREADY_READY);
				p2p_core_debug("%s is %s", P2PCORE.PEERNAME,P2PCORE.PEERREADY? "ready":"not ready");
				if (P2PCORE.USERREADY && P2PCORE.PEERREADY) {
					p2p_send_chat("both players are ready, starting game");

					P2PCORE.connection->send_tinst(LOAD, LOAD);

					Sleep(min(P2P_GAMECB_WAIT,P2PCORE.connection->ping));
					P2PCORE.state = P2PC::LD;
					p2p_game_callback(P2PCORE.GAME, P2PCORE.HOST? 1:2, 2);
				}
				break;
			case LOAD:
				P2PCORE.state = P2PC::LD;
				p2p_game_callback(P2PCORE.GAME, P2PCORE.HOST? 1:2, 2);
				break;
			case EXIT:
				p2p_peer_left_callback();
				p2p_re_initialize();
				if (!P2PCORE.HOST) {
					P2PCORE.state = P2PC::DC;
				}
				break;
			};
			//if (P2PCORE.connection && P2PCORE.connection->getLSElapsedTime() > 500) {
			//	p2p_retransmit();
			//}
		}
		break;
	case P2PC::LD: // were loading the game
		switch(ki->inst.type){
		case LOAD:
			P2PCORE.PEERLOADED = true;
			P2PCORE.connection->send_tinst(LOAD, LOAD_LOADED);
			P2PCORE.connection->state = p2p_message::HC;
			p2p_core_debug("Peer Loaded");
			P2PCORE.state = P2PC::LF;
			break;
		case CP2PP:
			p2p_handle_cp2pp(ki);
			break;
		case DROP:
			p2p_core_debug("peer dropped");
			p2p_drop_game_pasv();
			break;
		};
		break;
	case P2PC::LF: // Load Finished... well calculate ping
		switch(ki->inst.type){
		case PING:
			if (ki->inst.flags==PING_PING){
				if (P2PCORE.HOST && ++P2PCORE.pingc>=p2p_PING_TIMES) {
					P2PCORE.state = P2PC::TS;
					break;
				}
				P2PCORE.connection->send_instruction(ki);
			} else {
				P2PCORE.state = P2PC::TS;
				P2PCORE.ping = ki->load_int();
			}
			break;
		case CP2PP:
			p2p_handle_cp2pp(ki);
			break;
		case DROP:
			p2p_core_debug("peer dropped");
			p2p_drop_game_pasv();
			break;
		};
		break;
	case P2PC::TS: // stuff for readjusting ping and time synchronization
		switch(ki->inst.type){
		case TTIME:
			P2PCORE.throughput = ki->load_int();
			P2PCORE.state = P2PC::PS;
			break;
		case CP2PP:
			p2p_handle_cp2pp(ki);
			break;
		case DROP:
			p2p_core_debug("peer dropped");
			p2p_drop_game_pasv();
			break;
		};
		break;
	case P2PC::PS:
		switch(ki->inst.type){
		case START:
			P2PCORE.state = P2PC::PL;
			Sleep(ki->load_int());
			break;
		case CP2PP:
			p2p_handle_cp2pp(ki);
			break;
		case DROP:
			p2p_core_debug("peer dropped");
			p2p_drop_game_pasv();
			break;
		};
		break;
	case P2PC::PL:
		break;
	case P2PC::DC:
		break;
	};
}
inline bool ProcessGameStuff(){
	n02_TRACE();
	while (P2PCORE.connection->has_data()){
		p2p_instruction ki;
		if (P2PCORE.connection->receive_instruction(&ki)){
			if (ki.inst.type == DATA) {
				if ((ki.inst.flags | DATA_HAS_SERIAL) == DATA_HAS_SERIAL) {
					DWORD sl;
					if ( (sl = ki.load_uint()) != P2PCORE.ExpectedDataIndex) {
						p2p_core_debug("Packet misorder descrambling algorithm error. expected frame %u and received %u. DIPM=%i. Please contact the devs.", P2PCORE.ExpectedDataIndex, sl, P2PCORE.connection->default_ipm);

						char xxx[200];
						*xxx = 0;
						
						
						for(int x = 1; x <= 16; x++) {
							wsprintf(xxx + strlen(xxx), "%i, ", p2p_message::PacketReceivedStack[(p2p_message::PacketReceivedStackPtr-x)&15]);
						}

						p2p_core_debug("Packet stack: %s", xxx);


						if (sl < P2PCORE.ExpectedDataIndex) { //ok its still recoverable
							p2p_core_debug("attempting to recover");
							return false;
						}
					} else {
						//p2p_core_debug("Expected %u received %u", ExpectedDataIndex, sl);
						P2PCORE.ExpectedDataIndex++;						
					}
				}
				char data[32];
				ki.load_bytes(data, P2PCORE.DATALEN);
				P2PCORE.PEERDATA.put_data(data, P2PCORE.DATALEN);
			} else if (ki.inst.type == CP2PP) {
				p2p_handle_cp2pp(&ki);
			} else if (ki.inst.type == DROP) {
				p2p_core_debug("peer dropped");
				p2p_drop_game_pasv();
				return true;
			}
		}
		//data chat and drop
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////
// Main step function
void p2p_step(){
	//kprintf(__FILE__":%i", __LINE__);
	n02_TRACE();
	while (P2PCORE.initialized && P2PCORE.connection && P2PCORE.connection->step(100)){
		n02_TRACE();
		p2p_instruction ki;
		n02_TRACE();
		if (P2PCORE.connection->receive_instruction(&ki)){
			p2p_process_instruction(&ki);			
		}
		n02_TRACE();
	}
	n02_TRACE();
	if (P2PCORE.initialized && !p2p_is_connected() && P2PCORE.connection && P2PCORE.connection->has_ssrv()){
		n02_TRACE();
		char xxx[2000];
		sockaddr_in saddr;
		p2p_ssrv_packet_recv_callback(xxx, P2PCORE.connection->receive_ssrv(xxx, &saddr), &saddr);
	}
}


////////////


#define KDBFC(X, ...)\
	kprintf("DEBUG: %s:%i#%s (" #X ")",__FILE__, __LINE__, __FUNCTION__ , __VA_ARGS__);

static bool _WaitForPeerToLoadOrDie(){
	n02_TRACE();

	p2p_core_debug("Game loaded, waiting for peer.");
	P2PCORE.USERLOADED = true;
	P2PCORE.PEERLOADED = false;
	P2PCORE.connection->send_tinst(LOAD, LOAD_LOADED);

	while (P2PCORE.state == P2PC::LD){
		//if (P2PCORE.connection->has_data() || (k_socket::check_sockets(1,0) && P2PCORE.connection->has_data())){
		if (P2PCORE.connection->has_data() || P2PCORE.connection->step(1000)){
			p2p_instruction ki;
			if (P2PCORE.connection->receive_instruction(&ki)) {
				p2p_process_instruction(&ki);
			}
		} else {
			p2p_core_debug(".");
		}
	}

	return P2PCORE.state == P2PC::LF;
}


static bool _CalculatePingOrDie(){
	n02_TRACE();
	
	if (P2PCORE.HOST) 
		Sleep(100);

	P2PCORE.pingc = 0;

	if (P2PCORE.HOST) {
		p2p_instruction kix(PING, PING_PING);
		int x = 4;
		while (x-->0)
			kix.store_uint(P2PCORE.connection->GetTime());
		
		P2PCORE.connection->send_tinst(PING, PING_PING);

		P2PCORE.ping = P2PCORE.connection->GetTime();
	}

	while (P2PCORE.state == P2PC::LF){
		if (P2PCORE.connection->has_data() || P2PCORE.connection->step(250)){
			p2p_instruction ki;
			if (P2PCORE.connection->receive_instruction(&ki)) {
				p2p_process_instruction(&ki);
			}
		} else {
			p2p_core_debug(".");
		}
	}

	if (P2PCORE.HOST && P2PCORE.state == P2PC::TS) {
		P2PCORE.ping = (P2PCORE.connection->GetTime() - P2PCORE.ping) / p2p_PING_TIMES;
		p2p_instruction kx(PING, PING_ECHO);
		kx.store_int(P2PCORE.ping);
		P2PCORE.connection->send_instruction(&kx);
	}

	return P2PCORE.state == P2PC::TS;
}


static bool _ReadjustPingAndCalculateDelayOrDie(){
	
	n02_TRACE();

	if (P2PCORE.HOST) {

		//p2p_core_debug("ping is %i", P2PCORE.ping);

		Sleep(120);

		int PAD = p2p_getSelectedDelay();

		int ttime = (P2PCORE.ping / 2) + ((PAD==1)? 4:0);

		P2PCORE.throughput = 1 + (ttime * 60/1000) + ((PAD>=2)? (PAD-1):0);

		p2p_instruction kxxx(TTIME, 0);
		kxxx.store_int(P2PCORE.throughput);
		P2PCORE.connection->send_instruction(&kxxx);

		P2PCORE.state = P2PC::PS;
		
		p2p_core_debug("latency: %ims", ttime);

	}

	while (P2PCORE.state == P2PC::TS){
		if (P2PCORE.connection->has_data() || P2PCORE.connection->step(1000)){
			p2p_instruction ki;
			if (P2PCORE.connection->receive_instruction(&ki)) {
				p2p_process_instruction(&ki);
			}
		} else {
			p2p_core_debug(".");
		}
	}

	return P2PCORE.state == P2PC::PS;
}






int  OKAI_CCNV p2p_synchronizeGame(){
	n02_TRACE();
	
	//initialize cache
	P2PCORE.USERDATA.reset();
	P2PCORE.PEERDATA.reset();

	n02_TRACE();
	if (_WaitForPeerToLoadOrDie()){
		p2p_core_debug("== Everyone Loaded");

		if (_CalculatePingOrDie()){
			p2p_core_debug("== Calculated Ping = %ims", P2PCORE.ping);

			if (_ReadjustPingAndCalculateDelayOrDie()) {
				p2p_core_debug("== Calculated delay %i frame(s)", P2PCORE.throughput);

				p2p_core_debug("== Gamesync in less than %i second(s)", (P2P_GAMESYNC_WAIT/1000)+1);

				P2PCORE.frameno = 0;

				if (P2PCORE.HOST) {

					p2p_instruction ki(START, 0);

					ki.store_int(P2PCORE.throughput * 1000 / 60);

					P2PCORE.connection->send_instruction(&ki);

					P2PCORE.connection->step(16);

					P2PCORE.state = P2PC::PL;

				} else {

					while (P2PCORE.state == P2PC::PS){
						if (P2PCORE.connection->has_data() || P2PCORE.connection->step(1000)){
							p2p_instruction ki;
							if (P2PCORE.connection->receive_instruction(&ki)) {
								p2p_process_instruction(&ki);
							}
						} else {
							p2p_core_debug(".");
						}
					}
				}

				if (P2PCORE.state == P2PC::PL) {
					P2PCORE.connection->default_ipm = max(MIN_IPM, min(P2PCORE.throughput+1, MAX_IPM));
					P2PCORE.ExpectedDataIndex = 0;
					P2PCORE.ToSendDataIndex = 0;
					n02_TRACE();
					return 0;
				}
			}
		}
	}
	return 1;
}



void OKAI_CCNV p2p_sendPlayValue(const void * value, const int len){
	n02_TRACE();
	if (P2PCORE.state == P2PC::PL) {
		P2PCORE.DATALEN = len;
		P2PCORE.USERDATA.put_data(value, P2PCORE.DATALEN);
		p2p_instruction kix(DATA, DATA_HAS_SERIAL);
		kix.store_uint(P2PCORE.ToSendDataIndex++);
		kix.store_bytes(value, P2PCORE.DATALEN);
		P2PCORE.connection->send_instruction(&kix);
	}
	//p2p_core_debug("send out");

}

int  OKAI_CCNV p2p_playDataCheck(int ms){
	//n02_TRACE();
	if (P2PCORE.connection->has_data() || P2PCORE.connection->step(ms)){
		//n02_TRACE();
		if (ProcessGameStuff())
			return 1;
	} else {
		//n02_TRACE();
		if (P2PCORE.connection->getLRElapsedTime() > P2P_DATA_TIMEOUT)  {
			p2p_core_debug("Data timeout. Dropping game");
			p2p_endGame();
			return 1;
		}
		//n02_TRACE();
	}
	return 0;
}

int  OKAI_CCNV p2p_recvPlayValues(void * values, const int len){ //recv aligned
	n02_TRACE();
	if (P2PCORE.state == P2PC::PL) {
		if (P2PCORE.frameno++ >= P2PCORE.throughput) {
			//n02_TRACE();
			DWORD TIT = 0;
			DWORD TITY = 0;
			while (P2PCORE.PEERDATA.pos < P2PCORE.DATALEN && P2PCORE.state == P2PC::PL){
				if (p2p_playDataCheck(1)) {
					//n02_TRACE();
					return -1;
				}
			}

			if (P2PCORE.PEERDATA.pos >= P2PCORE.DATALEN) {

				n02_TRACE();

				if (P2PCORE.HOST) {
					P2PCORE.USERDATA.get_data((char*)values, P2PCORE.DATALEN);
					P2PCORE.PEERDATA.get_data((((char*)values)+P2PCORE.DATALEN), P2PCORE.DATALEN);
				} else {
					P2PCORE.PEERDATA.get_data((char*)values, P2PCORE.DATALEN);
					P2PCORE.USERDATA.get_data((((char*)values)+P2PCORE.DATALEN), P2PCORE.DATALEN);
				}

				n02_TRACE();

				return P2PCORE.DATALEN * 2;

			}	

			n02_TRACE();
			return -1;
		} else {
			return 0;
		}
	} else return -1;
}
int  OKAI_CCNV p2p_getLocalQueueLen(){
	n02_TRACE();
	//p2p_core_debug("p2p_getLocalQueueLen P2PCORE.frameno = %i P2PCORE.throughput=%i", P2PCORE.frameno, P2PCORE.throughput);
	if (P2PCORE.state == P2PC::PL) {
		if (P2PCORE.frameno < P2PCORE.throughput)
			return P2PCORE.throughput;
		else
			return P2PCORE.PEERDATA.pos / P2PCORE.DATALEN;
	} else {
		return -1;
	}
}


void OKAI_CCNV p2p_endGame(){
	n02_TRACE();
	p2p_core_debug("dropping game");
	if (P2PCORE.connection) {
		P2PCORE.connection->send_tinst(DROP, 0);
		p2p_drop_game_pasv();
	}
}
void OKAI_CCNV p2p_asyncData(const void * value, const int len, const int position){
	n02_TRACE();
}


int  OKAI_CCNV p2p_exchangePlayValues(void * value, const int len){
	n02_TRACE();
	p2p_sendPlayValue(value, len);
	return p2p_recvPlayValues(value, len);
}
int  OKAI_CCNV p2p_getBaseDelay		(){
	n02_TRACE();
	return P2PCORE.throughput;	
}


int p2p_modify_play_values(void *values, int size){
	n02_TRACE();
	if (P2PCORE.state == P2PC::PL) {
		return p2p_exchangePlayValues(values, size);
	} else {
		if (p2p_synchronizeGame()) {
			return -1;
		}
		return p2p_modify_play_values(values, size);
	}
}


