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

#include "kaillera_Instruction.h"

using namespace n02;
using namespace n02::kaillera;

#ifndef MAX_CACHED_INSTRS
#define MAX_CACHED_INSTRS 32
#endif

class UserMessaging:
	private UdpSocket,
	private DynamicOrderedArray<PackedInstruction, MAX_CACHED_INSTRS * 2>
{
private:
	
	static char sendingBuffer[32 * 1024];

	unsigned short lastIncludedInstruction;
	unsigned short lastReceivedInstruction;
	int newInstructions;
	int oldInstructionsPerMessage;

public:

	void log() {
		LOG(%s, defaultAddress.toString());
	}

	UserMessaging()
		: UdpSocket(0)
	{
		TRACE();
		lastIncludedInstruction = 0xFFFF;
		lastReceivedInstruction = 0xFFFF;
		oldInstructionsPerMessage = 2;
		newInstructions = 0;
	}

	~UserMessaging()
	{
		TRACE();
		while (itemsCount() > 0) {
			delete getItemPtr(0)->body;
			removeIndex(0);
		}
	}

	void includeInstruction(Instruction & instr)
	{
		TRACE();
		PackedInstruction packedFormat;
		instr.pack(packedFormat);
		packedFormat.serial = ++lastIncludedInstruction;
		addItem(packedFormat);
		newInstructions++;
		TRACE();
	}

	bool hasNewInstructions() const
	{
		TRACE();
		return newInstructions > 0;
	}

	void sendIfHasNewInstructions()
	{
		if (newInstructions > 0) {
			sendMessage();
		}
	}

	// sends out our message to server
	inline void sendMessage(const int noOfInstructions)
	{
		TRACE();
		char * sendBuffer = sendingBuffer;

		int noToSend = common_min(common_min(itemsCount(), noOfInstructions), MAX_CACHED_INSTRS);

		*sendBuffer = (char)noToSend;

		char * encodePtr = sendBuffer + 1;

		for (int x = 0; x < noToSend; x++) {
			PackedInstruction * packed = getItemPtr(itemsCount() - x - 1);
			*((unsigned short*)encodePtr) = packed->serial; encodePtr += 2;
			*((unsigned short*)encodePtr) = packed->length; encodePtr += 2;
			memcpy(encodePtr, packed->body, packed->length); encodePtr += packed->length;
		}

		BsdSocket::send(sendBuffer, encodePtr - sendBuffer);

		while (itemsCount() > MAX_CACHED_INSTRS) {
			delete getItemPtr(0)->body;
			removeIndex(0);
		}

		newInstructions = 0;

		TRACE();
	}

	// sends out our message to server
	inline void sendMessage()
	{
		TRACE();
		sendMessage(newInstructions + oldInstructionsPerMessage);
	}

	virtual void instructionArrivalCallback(Instruction & instr) {}

	int getPort() const
	{
		SocketAddress sadr;
		getLocalAddress(sadr);
		return sadr.getPort();
	}

private:

	// TODO: add checks here
	inline bool searchProcessInstruction(unsigned short forSerial, unsigned char * buffer, int bufferLen, int instrsCount)
	{
		TRACE();
		//LOG(fors = %i buflen = %i finstrount = %i, forSerial, bufferLen, instrsCount);
		while (instrsCount > 0) {
			unsigned short serial = *((unsigned short*)buffer); buffer+= 2;
			unsigned short length = *((unsigned short*)buffer); buffer+= 2;
			if (serial == forSerial) {
				TRACE();
				Instruction instr(buffer, length);
				instructionArrivalCallback(instr);
				TRACE();
				return true;
			} else {
				buffer += length;
				bufferLen -= (4 + length);
				instrsCount--;
			}
		}
		TRACE();
		return false;
	}

	// this is called when data is available
	void dataArrivalCallback()
	{
		TRACE();
		int bufLen = BIG_RECV_BUFFER_SIZE;
		if (BsdSocket::recvFrom(BsdSocket::bigRecvBuffer, bufLen, defaultAddress)) {
			TRACE();
			if (bufLen > 1 + 4 + 2) {
				char instructionsCount = *bigRecvBuffer;
				while (searchProcessInstruction(lastReceivedInstruction+1, reinterpret_cast<unsigned char*>(bigRecvBuffer+1), bufLen, instructionsCount))
					lastReceivedInstruction++;
			} else {
				require(bufLen > 1 + 4 + 2);
			}
		} else {
			int er = errno;
			
			if (er == 0) {
				Instruction leave(USERLEAV);
				leave.writeString("Connection closed");
				leave.seek(-1 * leave.getFilledSize());
				instructionArrivalCallback(leave);
			} else {
				LOG(Socket Error occured %i, er);
			}
		}
		TRACE();
	}
};

