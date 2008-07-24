/******************************************************************************
O                                                           8d8b. Yb  dP Yb dP 
O                                                           8P Y8  YbdP   `8.  
Open Kaillera: Kaillera Server                              8   8   dP   dP Yb 
*******************************************************************dP**********
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

#include "nyx.h"

class IncomingConnection :
	protected UdpSocket
{

public:

	IncomingConnection(int port) :
	  UdpSocket(port, 50, false)
	{}

	  bool isInitialized()
	  {
		  return UdpSocket::isInitialized();
	  }

protected:

	virtual void incomingDataCallback(char * data, int length, SocketAddress & source) = 0;

private:

	SocketAddress lastAddress;

	void dataArrivalCallback()
	{
		char buffer[15];

		int bufLen = 15;

		if (BsdSocket::recvFrom(buffer, bufLen, lastAddress)) {
			incomingDataCallback(buffer, bufLen, lastAddress);
		} else {
			LOG(Socket Error occured %i %s, errno, lastAddress.toString());
		}
	}

};
