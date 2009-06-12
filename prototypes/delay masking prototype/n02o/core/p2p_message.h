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
#include "../common/k_socket.h"
#include "../common/rTimer.h"
#include "../errr.h"

#include "p2p_instruction.h"

//keep this a power of 2
#define ICACHESIZE 64

#define MAX_IND_Q_LEN	 ICACHESIZE
#define MAX_SPARES_Q_LEN ICACHESIZE * 2

#define MAX_INSTS 15
#define MAX_IPM 12
#define MIN_IPM 2

#define PACKET_STACK

// Connection level HC data retransmission time
#define HCRETRT 30
// Connection timeout time
#define CONTIT 30000
#define PINGINT 500

#pragma pack(push, 1)

typedef struct {
	unsigned int serial;
    unsigned short length;
	char bytes[257];
} p2p_instruction_bin;

typedef struct {
	unsigned char count;
	unsigned int LatestInstrSerialNo;
	unsigned int LatestACKSerialNo;
	char body[2048];
	void encrypt() {
		LatestInstrSerialNo ^= -1;
		LatestACKSerialNo ^= -1;
	}
	void decrypt() {
		LatestInstrSerialNo ^= -1;
		LatestACKSerialNo ^= -1;
	}
} p2p_std_message;

typedef struct {
	unsigned char count;
	unsigned char count2;
	unsigned char messageType;
	unsigned int params[10];
} p2p_message_ctrl_message;

typedef struct {
	unsigned char count;
	char body[1024];	
} p2p_ssrv_message;

typedef struct {
	unsigned short length;
	char body[257];
} p2p_instr_packed_format;

#pragma pack(pop)

#define P2P_MLMG 0
#define P2P_MLMG_RETRALL    (P2P_MLMG + 1)
#define P2P_MLMG_ECHO       (P2P_MLMG + 2)
#define P2P_MLMG_ECHOR      (P2P_MLMG + 3)
#define P2P_MLMG_DISCONNECT (P2P_MLMG + 5)


class p2p_message : public k_socket, public rTimer {
public:

	enum {NC, SC, HC, DC} state : 2;

	unsigned int LastInCachedSerial;  // <-- latest one we have in cache
	unsigned int LastOutCacheSerial;  // <-- latest one we sent

	unsigned int LastPacketSentTime;  // Time the last packet was sent
	unsigned int LastPacketReceivedTime; // 

#ifdef PACKET_STACK
	static unsigned int PacketReceivedStack[16];
	static unsigned int PacketReceivedStackPtr;
#endif
	unsigned int CurrentTime;
	int ping;

	sockaddr_in iaddr;				// last incoming address

public:
	
	int                 default_ipm; // <-- default no of previous instructions to send per message
	int                 NewlyOutCachedInstructions; // <-- default no of previous instructions to send per message

	//Caches
	struct cache_ {
		// we'll keep them static..so we never have any memory leaks and such
		p2p_instruction_bin static_instructions[MAX_SPARES_Q_LEN];
		//for allocation putposes
		slist<p2p_instruction_bin*, MAX_SPARES_Q_LEN> spares;
		//in and out cache
		oslist<p2p_instruction_bin*, MAX_IND_Q_LEN> out;
		oslist<p2p_instruction_bin*, MAX_IND_Q_LEN> in;
		//disco cache...for 80s people
		//oslist<p2p_instruction_bin*, MAX_IND_Q_LEN> disc;

		p2p_instruction_bin * AllocateNew(){
			//get a spare instruction space
			p2p_instruction_bin * spare = spares[0];
			//remove from cache.spares queue
			spares.removei(0);
			return spare;
		}

		void DeAllocate(p2p_instruction_bin * x){
			spares.add(x);
		}

		void Reset(){
			//clear caches
			out.clear();
			in.clear();
			spares.clear();

			//make all static instruction cache.spares
			for(int x =0; x < ICACHESIZE*2; x++){
				spares.add(&static_instructions[x]);			
			}
		}

	} cache;

	//"connectionless user level peer to peer data transfer protocol messages" cache... we'll just call them "special service" -> "ssrv" messages
	odlist<char*, 1> ssrv_cache;


	// == Initializers ===============================
	// -- construcorerer -----------------------------
    p2p_message();

	// == Transmission stuff =========================

	// -- inclde instruction for send ------------------
	inline int send_include(p2p_instruction * arg_0){
		n02_TRACE();
		if ((cache.out.length & (MAX_IND_Q_LEN-1))==cache.out.length) {
			//get a spare instruction space
			p2p_instruction_bin * spare = cache.spares[0];
			//remove from cache.spares queue
			cache.spares.removei(0);

			//serialize
			spare->serial = LastOutCacheSerial++;
			//update no of new instructions to send
			NewlyOutCachedInstructions++;
			//update spare parameters
			spare->length = arg_0->write_to_message(spare->bytes);
			//add it to out outgoing queue
			cache.out.add(spare);
			return 0;
		} else return 1;
	}

	// -- actual send message proc --------------
	inline bool send_message(int NoToSend){
		// KDBFC(NoToSend=%i cache- out:%i in %i,NoToSend, cache.out.size(), cache.in.size());
		n02_TRACE();

		p2p_std_message message;
		NoToSend = min(NoToSend, cache.out.size());

		int TotalLength = 9; // initial header len
		char * buffer = message.body;

		if (NoToSend > 0) {

			for (int i = 0; i < NoToSend; i++) {

				//find source
				p2p_instruction_bin * src = cache.out[cache.out.size() - i -1];

				//destinationa addr in buffer
				p2p_instr_packed_format * inst = (p2p_instr_packed_format *)buffer;

				// we need 2 bytes for its length and its body
				memcpy(inst->body, src->bytes, inst->length = src->length);

				TotalLength += inst->length + 2;
				buffer += inst->length + 2;

				if (i==0)
					message.LatestInstrSerialNo = src->serial;

			}

		} else {

			message.LatestInstrSerialNo = LastOutCacheSerial -1;

		}

		message.LatestACKSerialNo = LastInCachedSerial;
		message.count = NoToSend;
		message.encrypt();

		LastPacketSentTime = CurrentTime = GetTime();

		return k_socket::send((char*)&message, TotalLength);
	}

	//---- send an instruction -----------------------
    int send_instruction(p2p_instruction * arg_0){
		n02_TRACE();

		if (send_include(arg_0)==0) {
			send_message(default_ipm + NewlyOutCachedInstructions);
			NewlyOutCachedInstructions = 0;
			return 0;
		}
		return 1;
    }

	// -- send tiny instruction ---- helper function
	inline int send_tinst(int type, int flags){
		p2p_instruction kx(type, flags);
		return send_instruction(&kx);
    }


	// -- send a low level instruction (NO RDT) -
	inline void send_ll_message(int type, int params, ...){
		p2p_message_ctrl_message ctlmessage;

		ctlmessage.count = ctlmessage.count2 = 0;
		ctlmessage.messageType = type;

		int * pars = &params;
		pars++;
		int x = 0;

		while (params-->0)
			ctlmessage.params[x++] = *pars++;

		if (P2P_MLMG_ECHOR != type)
			LastPacketSentTime = CurrentTime;

		k_socket::send((char*)&ctlmessage, ((char*)&ctlmessage.params[x]) - ((char*)&ctlmessage));

	}

	// -- resend last few instructions ----------
    inline void resend_message(int limit){
		n02_TRACE();
        send_message(limit);
    }

	// == message handlers ===========================
	inline void handle_hl_message(p2p_std_message * message){

		message->decrypt();

		//first we'll check for duel acks => retransmissions
		if (cache.out.size() > 0 && (cache.out[0]->serial - 1)==message->LatestACKSerialNo) {
			// KDBFC(duplicate ACK);

		} else {
			//KDBFC(normal ACK);
			//then we'll get rid of acked instructions
			for (int x = 0; x < cache.out.size(); x++) {
				if (cache.out[x]->serial <= message->LatestACKSerialNo) {
					cache.spares.add(cache.out[x]);
					cache.out.removei(x);
					x--;
				} else
					break;
			}
		}

		// then we'll check if this message has enough data
		int NewInstructions = message->LatestInstrSerialNo - LastInCachedSerial;

		//KDBFC(NewInstructions=%i,NewInstructions);

		if (NewInstructions > 0 && NewInstructions <= message->count) {

#ifdef PACKET_STACK
			PacketReceivedStack[(PacketReceivedStackPtr++)&15] = message->LatestInstrSerialNo;
#endif

			char * buffer = message->body;

			oslist<p2p_instruction_bin*, MAX_INSTS> disc_cache;

			for (int u=0; u<NewInstructions; u++) {

				p2p_instr_packed_format * body = (p2p_instr_packed_format *)buffer;

				p2p_instruction_bin * spare = cache.AllocateNew();

				//get serial and body ad length
				spare->serial = message->LatestInstrSerialNo - u;
				spare->length = body->length;
				memcpy(spare->bytes, body->body, body->length);

				//add it to our disco list
				disc_cache.add(spare);

				buffer += body->length + 2;

			}

			//put disco stuff into in cache
			for (int u=0; u<NewInstructions; u++) {
				cache.in.add(disc_cache[NewInstructions-u-1]);
			}

			disc_cache.clear();

			//update last in cached serial
			LastInCachedSerial = message->LatestInstrSerialNo;

		} else { //problem
			//	KDBFC(problem);
			send_ll_message(P2P_MLMG_RETRALL, 0);
		}
	}

	inline void handle_ll_message(p2p_message_ctrl_message * message){
		switch (message->messageType){
			case P2P_MLMG_RETRALL:
				send_message(cache.out.size());
				break;
			case P2P_MLMG_ECHO:
				send_ll_message(P2P_MLMG_ECHOR, 4, message->params[0], message->params[1], message->params[2], message->params[3]);
				break;
			case P2P_MLMG_ECHOR:
				ping = CurrentTime - message->params[0];
				//KDBFC(ping received %i ms, ping);
				break;
		};
	}

	bool step(int ms){
		n02_TRACE();
		k_socket::check_sockets(0, ms);
		
		CurrentTime = GetTime();

		if (has_data_waiting)
			process_incoming();

		else if (state==HC || state==SC) {
			//retrtransmission
			if (state==HC) {
				if ((CurrentTime - LastPacketSentTime) > HCRETRT)
					send_message(MAX_INSTS);
			} else {
				if ((CurrentTime - LastPacketSentTime) > PINGINT) {
					//KDBFC(Sending ping);
					send_ll_message(P2P_MLMG_ECHO, 4, CurrentTime, 0, 0, 0);
				}
			}
			//connection timeout
			if ((CurrentTime - LastPacketSentTime) > CONTIT) {
				send_ll_message(P2P_MLMG_DISCONNECT, 0);
				state = DC;
				return false;
			}
			return cache.in.size() > 0;
		} else if (state==DC) {
				return false;
		}
		return cache.in.size() > 0;
	}

	// == reception stuff ============================

	// -- process packets on system's incoming queue
	inline void process_incoming() {
		// must have data waiting;
		LastPacketReceivedTime = CurrentTime;

		union {
			p2p_std_message message;
			p2p_message_ctrl_message ctlmessage;
			p2p_ssrv_message ssrvmessage;
		};

		int  bufflen = sizeof(message);

		if (k_socket::check_recv((char*)&message, &bufflen, &iaddr) && bufflen > 2) {

			// data received

			if (message.count > 0 && message.count <= MAX_INSTS) { // set an upper limir

				//its a normal message
				handle_hl_message(&message);

			} else {

				if (message.count == 0) {
					// LL message
					if (ctlmessage.count2 == 0) {
						handle_ll_message(&ctlmessage);
					} else {
					// SSRV Message
						if (state == NC)
							handle_ssrv_message(&ssrvmessage, bufflen);
					}
				}
			}
		}

	}
	
	// --- checks for intoming stuff and returns one instruction -----
	bool check_recv (char* buf, int * len){
		n02_TRACE();
		if (has_data_waiting)
			process_incoming();

		if (cache.in.size() > 0) {
			*len = cache.in[0]->length;
			memcpy(buf, cache.in[0]->bytes, *len);

			cache.spares.add(cache.in[0]);
			cache.in.removei(0);

			return true;
		}

		return false;
	}

	// -- recvs an instruction -----------------------
    bool receive_instruction(p2p_instruction * arg_0) {
		n02_TRACE();
        char var_8000[1024];
        int var_8004 = 1024;
        if (check_recv(var_8000, &var_8004)) {
			////kprintf("Received:");
            arg_0->read_from_message(var_8000, var_8004);
			//arg_0->to_string();
            return true;
        }
		return false;
    }

    inline bool has_data(){
		n02_TRACE();
        if (cache.in.length == 0)
            return has_data_waiting;
        else
            return true;
    }

	// ===============================================
	// == stuff with timing and stuff ================
	// get time elapsed since we last received a packet
	inline unsigned int getLRElapsedTime(){
		return CurrentTime - LastPacketReceivedTime;
	}
	inline unsigned int getLSElapsedTime(){
		return CurrentTime - LastPacketSentTime;
	}
	// == ssrv stuff =================================
	void send_ssrv(char * bf, int bflen, sockaddr_in * arg_addr);
	void send_ssrv(char * bf, int bflen, char * host, int port);
	bool has_ssrv();
	int receive_ssrv(char * buf, sockaddr_in * addrr);
	void handle_ssrv_message(p2p_ssrv_message * message, int size);

};
