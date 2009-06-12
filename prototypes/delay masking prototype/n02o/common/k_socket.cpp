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
#include "k_socket.h"

//#define PRINT_DATA

#ifdef STATS
#include "../stats.h"
#endif
#include "../errr.h"

slist<k_socket*, FD_SETSIZE> k_socket::list;
SOCKET k_socket::ndfs = 0;
fd_set k_socket::sockets;

#ifndef linux
#pragma comment(lib, "ws2_32.lib")
#endif
#include "../okaimodules.h"
void convert_bytes_to_stra(char * strbuffer, unsigned char * bytes_buffer, int len);
k_socket::k_socket(){
	sock = SOCKET_ERROR;
	list.add(this);
	has_data_waiting = false;
	if(ndfs == 0) {
		FD_ZERO(&sockets);
	}
	port = 0;
}

k_socket::~k_socket(){
	close();
}

void k_socket::close(){
	if(sock != SOCKET_ERROR) {
		shutdown(sock, 2);
		closesocket(sock);
		FD_CLR(sock, &sockets);
		if(sock == ndfs) {
			ndfs = 0;
			SOCKET temp = 0;
			k_socket * ks;
			for (int i = 0; i < list.size(); i++) {
				if ((ks = list[i]) != this && ks->sock > ndfs){
					ndfs = ks->sock;
				}
			}
		}
		sock = SOCKET_ERROR;
	}
	list.remove(this);
}

bool k_socket::initialize(int param_port, int minbuffersize){
	port = param_port;
	sockaddr_in tempaddr;
	memset(&tempaddr, 0, sizeof(tempaddr));
	tempaddr.sin_family = AF_INET;
	tempaddr.sin_port = htons(param_port);
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (sock != SOCKET_ERROR) {
		tempaddr.sin_addr.s_addr = htonl(0);
		if (bind(sock, (sockaddr*)&tempaddr, sizeof(tempaddr))==0) {
			unsigned long ul = 1;
			ioctl (sock, FIONBIO, &ul);
			FD_SET(sock, &sockets);
			if (sock > ndfs)
				ndfs = sock;
			if (port == 0) {
				socklen_t ssi = sizeof(tempaddr);
				getsockname(sock, (sockaddr*)&tempaddr, &ssi);
				port = ntohs(tempaddr.sin_port);
			}
			if (minbuffersize > 0) {
				socklen_t lenn = sizeof(int);
				int val;			
				getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&val, &lenn);
				if (val < minbuffersize) {
					socklen_t lenn = sizeof(int);
					int val = minbuffersize;
					setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&val, lenn);
				}
			}
			return true;
		} else {
			return false;
		}
	} else {
		return 1;
	}
}

bool k_socket::set_address(const char * cp, const u_short hostshort){
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(hostshort);
	if (addr.sin_port != 0) {
		addr.sin_addr.s_addr = inet_addr(cp);
		if (addr.sin_addr.s_addr == -1) {
			hostent * h = gethostbyname(cp);
			if (h!=0) {
				addr.sin_addr = *(struct in_addr*)(h->h_addr_list[0]);
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

bool k_socket::set_addr(sockaddr_in * arg_addr) {
	memcpy(&addr, arg_addr, sizeof(addr));
	return true;
}
bool k_socket::set_aport(int port){
	return ((addr.sin_port = htons(port)) != 0);
}
bool k_socket::send(char * buf, int len){
#ifdef STATS
	SOCK_SEND_PACKETS++;
	SOCK_SEND_BYTES += len;
#endif

#ifdef PRINT_DATA
			{
				//time to dump packets
				char bufrer[2048];
				convert_bytes_to_stra(bufrer, (unsigned char*)buf, len);
				KDBFC(%s, bufrer);
			}
#endif

	//n02_TRACE();
	return (sendto(sock, buf, len, 0, (sockaddr*)&addr, 16) != SOCKET_ERROR);
}
bool k_socket::check_recv (char* buf, int * len, sockaddr_in* addrp)  {
	struct sockaddr saa;
	socklen_t V4 = sizeof(saa);
	has_data_waiting = 0;
	socklen_t lnx = *len;
	int  lenn = 0;
	if ((lenn = recvfrom(sock, buf, lnx, 0, &saa, & V4)) > 0) {
		*len = lenn;
		if(lenn != 0) {
			memcpy(addrp, &saa, sizeof(saa));
#ifdef STATS
			SOCK_RECV_PACKETS++;
			SOCK_RECV_BYTES+= lenn;
#endif
#ifdef PRINT_DATA
			{
				//time to dump packets
				char bufrer[2048];
				convert_bytes_to_stra(bufrer, (unsigned char*)buf, lenn);
				KDBFC(%s, bufrer);
			}
#endif
		}
		return true;
	}
	return false;
}
bool k_socket::has_data(){
	return has_data_waiting;
}
int k_socket::get_port(){
	return port;
}

bool k_socket::check_sockets(int secs, int ms){

	timeval tv;
	tv.tv_sec = secs;
	tv.tv_usec = ms * 1000;

	fd_set temp;
#ifndef linux
	memcpy(&temp, &sockets, 2 * sizeof(u_int) + sizeof(SOCKET) * sockets.fd_count);
#else
	memcpy(&temp, &sockets, sizeof(sockets));
#endif

	if(select((int)(ndfs + 1), &temp, 0, 0, &tv) != 0) {
		if(list.size() > 0) {
			for (int i = 0; i < list.size(); i++){
				k_socket * k = list[i];
				if (FD_ISSET(k->sock, &temp)!=0){
					k->has_data_waiting = true;
				}
			}
		}
		return true;
	}
	return false;
}

bool k_socket::Initialize(){
#ifndef linux
	WSAData ws;
	return (WSAStartup(0x0202, &ws)==0);
#else
	return true;
#endif
	
}
void k_socket::Cleanup(){
#ifndef linux
	WSACleanup();
#endif
}

void k_socket::Zero() {
	for (int i = 0; i < list.size(); i++) {
		delete list[i];
		list.removei(i);
		i--;
	}
	ndfs = 0;
}
