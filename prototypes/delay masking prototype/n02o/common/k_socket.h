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
#ifndef K_SOCKET
#define K_SOCKET


//#define FD_SETSIZE 100

/*************************************************
 Dependencies ************************************
*************************************************/
#if defined(linux)

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define SOCKET int
#define INVALID_SOCKET -1
#define TIMEVAL timeval
#define closesocket ::close
#define SOCKET_ERROR -1

#else

#include "winsock.h"

#define ioctl ioctlsocket
#define socklen_t int

#endif

#include "nSTL.h"

/*************************************************
 Sockets Class ***********************************
*************************************************/

class k_socket {

protected:

	//socket
    SOCKET sock;
	//port no
    unsigned short port;	
	//does it have data waiting?
    bool has_data_waiting;
	//to address
    sockaddr_in addr;
	//list of k_sockets
    static slist<k_socket*,FD_SETSIZE> list;
	//ndfs
    static SOCKET ndfs;
	//list of sockets
    static fd_set sockets;

public:

    k_socket();
    ~k_socket();
	//close socket
    void close();
	//initialize socket
    virtual bool initialize(int param_port, int minbuffersize = 32 * 1024);
	//set address in terms of hostname and ip
    virtual bool set_address(const char * cp, const u_short hostshort);
	//set address addr ipv4 struct
    virtual bool set_addr(sockaddr_in * arg_addr);
	// set address port no
    virtual bool set_aport(int port);
	//send bytes of data
    virtual bool send(char * buf, int len);
	//recv bytes of data
    virtual bool check_recv (char* buf, int * len, sockaddr_in* addrp);
	//does the socket have data?
    virtual bool has_data();
	//get port no used by socket
    virtual int get_port();	



	//checks for data of sockets
    static bool check_sockets(int secs, int ms);
	//inititalize ... must be called before anything else
    static bool Initialize();
	//kills all sockets
	static void Zero();
	//cleanup...after everything else
    static void Cleanup();
};

#endif
