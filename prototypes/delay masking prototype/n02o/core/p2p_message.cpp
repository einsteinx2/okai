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
#include "p2p_message.h"

unsigned int p2p_message::PacketReceivedStack[16];
unsigned int p2p_message::PacketReceivedStackPtr;






p2p_message::p2p_message() {

	n02_TRACE();

	// lower level
	k_socket();

	//state is non conneted
	state = NC;

	LastOutCacheSerial = 0;
	LastInCachedSerial = -1;

	LastPacketSentTime = LastPacketReceivedTime = 0;

	NewlyOutCachedInstructions = 0;

	default_ipm = 2;

	cache.Reset();

	ResetTime();

	PacketReceivedStackPtr = 0;

	n02_TRACE();
}




















// == ssrv stuff =================================
void p2p_message::send_ssrv(char * bf, int bflen, sockaddr_in * arg_addr){
	n02_TRACE();
	sockaddr_in sad = addr;
	set_addr(arg_addr);
	char bufferr[1000];
	bufferr[0] = 0;
	memcpy(&bufferr[1], bf, bflen);
	k_socket::send(bufferr, bflen+1);
	set_addr(&sad);
}

void p2p_message::send_ssrv(char * bf, int bflen, char * host, int port){
	n02_TRACE();
	sockaddr_in sad = addr;
	set_address(host, port);
	char bufferr[1000];
	bufferr[0] = 0;
	memcpy(&bufferr[1], bf, bflen);
	k_socket::send(bufferr, bflen+1);
	set_addr(&sad);
}

bool p2p_message::has_ssrv(){
	n02_TRACE();
	return ssrv_cache.length > 0;
}

int p2p_message::receive_ssrv(char * buf, sockaddr_in * addrr){
	n02_TRACE();
	if (ssrv_cache.length > 0) {
		char * bbb = ssrv_cache[0];
		int len = *((short*)bbb);
		memcpy(buf, bbb+2+sizeof(sockaddr_in), len);
		memcpy(addrr, bbb+2, sizeof(sockaddr_in));
		free(bbb);
		ssrv_cache.removei(0);
		return len;
	}
	return 0;
}

void p2p_message::handle_ssrv_message(p2p_ssrv_message * message, int size){
	char * inb = (char*)malloc(size+1+sizeof(sockaddr_in));
	*((short*)inb) = size;
	*((struct sockaddr_in*)(inb+2)) = iaddr;
	memcpy(inb+2+sizeof(sockaddr_in), message->body, size-1);
	ssrv_cache.add(inb);
}




void print_p2pm_packet_stack(void (__cdecl * outpf) (char*,...)) {

	char xxx[200];
	*xxx = 0;

	for(int x = 1; x <= 16; x++) {
		wsprintf(xxx + strlen(xxx), "%i, ", p2p_message::PacketReceivedStack[(p2p_message::PacketReceivedStackPtr-x)&15]);
	}

	outpf("packet stack : %s", xxx);
}


