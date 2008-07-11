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

#include "SocketAddress.h"
#include "StringUtils.h"

#define in_addr_t int
#pragma comment(lib, "ws2_32.lib")

namespace n02 {

	SocketAddress::SocketAddress() {
		set(LOCAL_SOCKET_ADDRESS, 0);
	}

	SocketAddress::SocketAddress(const char* host) {
		parse(host);
	}

	SocketAddress::SocketAddress(const char* host, unsigned short port) {
		set(host, port);
	}

	bool SocketAddress::set (const char * host, const u_short port)
	{
		memset(&addr, 0, sizeof(addr));
		length = sizeof(addr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(host);
		if (addr.sin_addr.s_addr == (in_addr_t)-1) {
			hostent * h = gethostbyname(host);
			if (h!=0) {
				addr.sin_addr = *reinterpret_cast<struct in_addr*>(h->h_addr_list[0]);
				return true;
			}
			return false;
		}
		return true;
	}

	void SocketAddress::setPort (const u_short port)
	{
		addr.sin_port = htons(port);
	}

	bool SocketAddress::parse(const char * hostp)
	{
		int l = strlen(hostp);
		if (l < 2048 && l > 2) {
			char host[2048];
			strcpy(host, hostp);
			unsigned int port = 0;
			for (int x = 0; x < l; x++) {
				if (host[x]==':') {
					host[x] = 0;
					port = StringUtils::alphaToUint(&host[x]+1);
					break;
				}
			}
			return set(host, 0xFFFF & port);
		}
		return false;
	}


	char * SocketAddress::toString()
	{
		static char print_space[55];
		*print_space = 0;
		strcpy(print_space, inet_ntoa(addr.sin_addr));
		strcat(print_space, ":");
		StringUtils::intToAlpha(print_space + strlen(print_space), getPort());
		return print_space;
	}

	bool SocketAddress::isLocalIP(char * host) {
		if (strncmp("127.", host, 4)==0||strncmp("10.", host, 3)==0||strncmp("192.168.", host, 8)==0)
			return true;

		if (strncmp("172.", host, 4)==0){
			int secoct = atoi(host+4);
			if (secoct >= 16 && secoct <=31)
				return true;
		}

		if (inet_addr(host) == -1)
			return true;
		else
			return false;
	}

};

