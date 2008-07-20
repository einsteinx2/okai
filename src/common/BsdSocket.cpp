/******************************************************************************
          .d8888b.   .d8888b.  
         d88P  Y88b d88P  Y88b 
         888    888        888 
88888b.  888    888      .d88P 
888 "88b 888    888  .od888P"  
888  888 888    888 d88P"      
888  888 Y88b  d88P 888"       
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

#include "common.h"
#include "_common.h"

// TODO : Test
namespace n02 {

    StaticArray<BsdSocket*, FD_SETSIZE> BsdSocket::socketsList;
    SOCKET BsdSocket::ndfs;
    fd_set BsdSocket::fdList;
    fd_set BsdSocket::tempFdList;
    char BsdSocket::bigRecvBuffer[BIG_RECV_BUFFER_SIZE];



    inline void bsdOsdInitialize()
    {
#ifdef N02_WIN32
        WSAData ws;

        if (WSAStartup(0x0202, &ws)!=0) {
            LOGS(Winsock 2.2 initialization failed);
        }
#endif
    }

    inline void bsdOsdTerminate()
    {
#ifdef N02_WIN32
        WSACleanup();
#endif
    }


    BsdSocket::BsdSocket()
    {
        sock = (SOCKET)SOCKET_ERROR;
        if (socketsList.itemsCount() > 0){
            for (int x = 0; x < socketsList.itemsCount(); x++) {
                require(socketsList[x] != this);
            }
        }
        socketsList.addItem(this);
    }

    BsdSocket::BsdSocket(int family, int type, int protocol, int paramPort=0, bool blocking=false, int minBufferSize=8000) {

        sock = (SOCKET)SOCKET_ERROR;
        if (socketsList.itemsCount() > 0){
            for (int x = 0; x < socketsList.itemsCount(); x++) {
                require(socketsList[x] != this);
            }
        }
        socketsList.addItem(this);

		if (!socket(family, type, protocol, paramPort, blocking, minBufferSize)) {
			close();
		}

    }

    BsdSocket::~BsdSocket()
    {
        close();
        socketsList.removeItem(this);
    }

    bool BsdSocket::isInitialized() const
    {
        return sock != SOCKET_ERROR;
    }


    void BsdSocket::step(int secs, int ms)
    {
        TRACE();
        timeval tv;
        tv.tv_sec = secs;
        tv.tv_usec = ms * 1000;

#ifndef linux
        memcpy(&tempFdList, &fdList, 2 * sizeof(u_int) + sizeof(SOCKET) * fdList.fd_count);
#else
        memcpy(&tempFdList, &fdList, sizeof(fdList));
#endif

        if (select((int)(ndfs + 1), &tempFdList, 0, 0, &tv) != 0) {
            if (socketsList.itemsCount() > 0) {
                for (int i = 0; i < socketsList.itemsCount(); i++){
                    TRACE();
                    BsdSocket * sck = socketsList[i];
                    if (FD_ISSET(sck->sock, &tempFdList)!=0){
                        TRACE();
                        sck->dataArrivalCallback();
                    } else {

                    }
                }
            }
        }
        TRACE();
    }


    bool BsdSocket::socket(int family, int type, int protocol, int paramPort, bool blocking, int minBufferSize)
    {
        SocketAddress temp_addr;
        temp_addr.set(LOCAL_SOCKET_ADDRESS, paramPort & 0xFFFF);

        sock = ::socket(family, type, protocol);

        if (sock != SOCKET_ERROR) {
            if (bind(sock, temp_addr.getAddrPtr(), temp_addr.getSize())==0) {
                setBlockingMode(blocking);
                FD_SET(sock, &fdList);
                setMinimumBufferSize(minBufferSize);
                calcNdfs();
                return true;
            } else {
                LOG(Socket bind failed port = %i; protocol = %i; type = %i, paramPort, protocol, type);
                return false;
            }
        } else {
            LOG(Socket returned -1 protocol = %i; type = %i, protocol, type);
            return false;
        }

    }


    void BsdSocket::setMinimumBufferSize(int minBufferSize) const
    {
        if (sock != SOCKET_ERROR && minBufferSize > 0) {
            socklen_t lenn = sizeof(int);
            int val;			
            getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&val, &lenn);
            if (val < minBufferSize) {
                socklen_t lenn = sizeof(int);
                int val = minBufferSize;
                setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&val, lenn);
            }
        }
    }

    void BsdSocket::setBlockingMode(bool blocking) const
    {
        if (sock != SOCKET_ERROR) {
            unsigned long temp = blocking?0:1;
            ioctl (sock, FIONBIO, &temp);
        }
    }

    void BsdSocket::getLocalAddress(SocketAddress & saPtr) const
    {
        if (sock != SOCKET_ERROR) {
            getsockname(sock, saPtr.getAddrPtr(), saPtr.getSizePtr());
        }
    }

    void BsdSocket::close()
    {
        if (sock != SOCKET_ERROR) {
            shutdown(sock, 2);
            closesocket(sock);
            FD_CLR(sock, &fdList);
            sock = (SOCKET)SOCKET_ERROR;
        }
    }


    void BsdSocket::initialize()
    {
        memset(&fdList, 0, sizeof(fdList));
        FD_ZERO(&fdList);
        memset(&tempFdList, 0, sizeof(tempFdList));
        FD_ZERO(&tempFdList);
        ndfs = 0;
        bsdOsdInitialize();
    }

    void BsdSocket::terminate()
    {
        bsdOsdTerminate();
    }


    void BsdSocket::calcNdfs()
    {
        ndfs = 0;
        for (int i = 0; i < socketsList.itemsCount(); i++) {
            ndfs = common_max(socketsList[i]->sock, ndfs);
        }
    }

};
