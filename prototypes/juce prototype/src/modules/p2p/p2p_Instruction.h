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

#include "common.h"

namespace n02 {

	// instruction types 
	enum InstructionType {
		HELLO, // initiation
		BYE,  // termination
		ECHO, // respose
		CHAT, // chat message
		CHANGE, // game change
		ASYNC, // non sync data
		SYNC, // sync data
		READY, // ready
		LOAD, // game load
		GSYNC, // synchronization
		DROP
	};

	// flags for instructions
#define P2P_INSTRF(X)	(1<<X)
#define P2P_INSTRSF(X)	(X)

	//== First Command
	// initiation
#define P2P_INSTF_HELLO_HELLO		P2P_INSTRF(0)
	// aknowledgement
#define P2P_INSTF_HELLO_ACK			P2P_INSTRF(1)
	// reject
#define P2P_INSTF_HELLO_REJECT		P2P_INSTRF(2)

	//== Last Command
	// user closed window
#define P2P_INSTF_BYE_USER			P2P_INSTRSF(0)
	// connection timed out
#define P2P_INSTF_BYE_TIMEOUT		P2P_INSTRSF(1)

	//== echo
	// reply
#define P2P_INSTF_ECHO_ACK			P2P_INSTRF(0)

	//== change game
	// get lock. Don't want both changing games at the same time
#define P2P_INSTF_CHANGE_LOCK		P2P_INSTRSF(0)
	// release lock
#define P2P_INSTF_CHANGE_RELEASE	P2P_INSTRSF(1)
	// new game
#define P2P_INSTF_CHANGE_CHANGE		P2P_INSTRSF(2)
	// lock failed
#define P2P_INSTF_CHANGE_LOCKFAIL	P2P_INSTRSF(3)
	// lock successful
#define P2P_INSTF_CHANGE_LOCKED		P2P_INSTRSF(4)

	//== Asynchronous data... partitioning flags
#define P2P_INSTR_ASYNC				5
	// begining of the data chunk
#define P2P_INSTF_ASYNC_BEGIN		P2P_INSTRF(0)
	// end of the chunk
#define P2P_INSTF_ASYNC_END 		P2P_INSTRF(1)
	// both begining and end
#define P2P_INSTF_ASYNC_WHOLE		(P2P_INSTF_ASYNC_BEGIN | P2P_INSTF_ASYNC_END)

	//== Synchronous data...
	// data has a 4 byte unsigned int serial field
#define P2P_INSTF_SYNC_SERIAL		P2P_INSTRF(0)

	//== readyness
#define P2P_INSTF_READY_READY		P2P_INSTRSF(1)
#define P2P_INSTF_READY_NOTREADY	P2P_INSTRSF(2)


	//== game sync
	// game synchronization data
#define P2P_INSTF_GSYNC_DATA		P2P_INSTRSF(0)
	// the delay calculated from one end
#define P2P_INSTF_GSYNC_MYDELAY		P2P_INSTRSF(3)


	// Game synchronized. Begin transmitting input
#define P2P_INSTR_DROP_TIMEOUT		P2P_INSTRF(0)
#define P2P_INSTR_DROP_USER			P2P_INSTRF(1)


	typedef struct {
		unsigned int serial;
		unsigned char length;
		unsigned char* body;
	} PackedInstruction;


	class Instruction: public StaticBuffer<254>
	{
	
	public:
		InstructionType type;
		int flags;

	public:
		// constructors
		Instruction(InstructionType _type, int _flags=0)
		{
			type = _type;
			flags = _flags;
		}
		// encapsulation
		Instruction (const unsigned char * buffer, const int len)
		{
			encapsulate(buffer, len);
		}

		// see if a flag or something is set
		bool has(int flag) const
		{
			return flag == flags || (flags & flag) == flag;
		}

		// pack into packed format
		int pack (PackedInstruction & pack)
		{
			pack.body = new unsigned char[pack.length = (unsigned char)common_min(1 + getFilledSize(), 255)];

			//LOG(xxx %i, pack.length );

			pack.body[0] = (type & 0x0f) | (flags & 0x0f) << 4;
			copyBuffer(pack.body + 1);
			return pack.length;
		}
		// unpack into instruction
		void unPack (const unsigned char * buffer, const int len)
		{
			//TODO: not needed... will code later
		}
		// read from a buffer
		void encapsulate(const unsigned char * buffer, const int len)
		{
			require(len >= 1);
			unsigned char ffield = *buffer;
			type = static_cast<InstructionType>(ffield & 0x0f);
			flags =  (ffield & 0xf0) >> 4;
			if (len > 1)
				presetBufferPtr(buffer + 1, len -1);
			else {
				begin = const_cast<unsigned char*>(buffer);
				ptr = begin;
				end = begin;
			}
		}
		// log
		void log() const {
			LOG(instruction to parse type %i; flags %x, type, flags);
			LOGBUFFER("Body", ptr, reinterpret_cast<unsigned int>(end) - reinterpret_cast<unsigned int>(ptr));
		}
		void logFilled() const {
			LOG(instruction filled type %i; flags %x, type, flags);
			LOGBUFFER("Body", begin, reinterpret_cast<unsigned int>(ptr) - reinterpret_cast<unsigned int>(begin));
		}

	};
};
