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

#pragma once

#include <winsock.h>

#define socklen_t int

#define LOCAL_SOCKET_ADDRESS "0.0.0.0"

namespace n02 {

	class SocketAddress
	{

	protected:

		socklen_t length;
		sockaddr_in addr;

	public:

		SocketAddress();
		SocketAddress(const char* host);
		SocketAddress(const char* host, unsigned short port);

		bool set (const char * host, const u_short port);
		void setPort (const u_short port);
		bool parse(const char * hostp);

		inline bool isEqual(SocketAddress * dst)
		{
			return (dst->addr.sin_port ==addr.sin_port && dst->addr.sin_addr.s_addr == addr.sin_addr.s_addr);
		}

		sockaddr* getAddrPtr()
		{
			return (sockaddr*)&addr;
		}

		socklen_t getSize()
		{
			return sizeof(addr);
		}

		socklen_t * getSizePtr()
		{
			length = getSize();
			return &length;
		}

		int getPort()
		{
			return ntohs(addr.sin_port);
		}

		char * toString();

		static bool isLocalIP(char * ip);

	};
};
