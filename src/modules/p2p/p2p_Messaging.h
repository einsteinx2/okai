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

#include "p2p_Instruction.h"

#ifndef MAX_CACHED_INSTRS
#define MAX_CACHED_INSTRS 32
#endif

namespace n02 {
    namespace p2p {

        // callbacks
        namespace core {
            void instructionArrivalCallback(Instruction & instr);
            void ssrvCallback(char *, int);
        };

        // state
        enum MessagingState {
            UNINITIALIZED, // uninitialized
            INITIALIZED, // initialized but not connected
            INSTSONLY // connected
        };

        //class
        class Messaging: private UdpSocket, private DynamicOrderedArray<PackedInstruction, MAX_CACHED_INSTRS> {
        private:
            MessagingState state;

            // last isntruction we included
            unsigned int lastIncludedInstruction;
            // last instruction we received
            unsigned int lastReceivedInstruction;
            // no of new instructions since last send
            int newInstructions;

        public:
            SocketAddress lastAddress;

        public:
            Messaging() : state(UNINITIALIZED), UdpSocket(){}

            bool initialize(int localPort)
            {
                require(state == UNINITIALIZED);
                resetState();
                if (UdpSocket::initialize(localPort, 32000)) {
                    state = INITIALIZED;
                } else {
                    state = UNINITIALIZED;
                    close();
                }
                return state == INITIALIZED;
            }

            void resetState()
            {
                lastIncludedInstruction = 0xFFFFFFFF;
                lastReceivedInstruction = 0xFFFFFFFF;
                newInstructions = 0;

                while (itemsCount()) {
                    delete getItemPtr(0)->body;
                    removeIndex(0);
                }

                state = INITIALIZED;
            }

            void terminate()
            {
                TRACE();
                require(state != UNINITIALIZED);

                while (itemsCount() > 0) {
                    delete getItemPtr(0)->body;
                    removeIndex(0);
                }

                close();

                state = UNINITIALIZED;
                TRACE();
            }

            void setConnected(bool c)
            {
                require(state != UNINITIALIZED);
                if (c)
                    state = INSTSONLY;
                else
                    state = INITIALIZED;			
            }

            void setAddress(const SocketAddress & addr)
            {
                defaultAddress = addr;
            }
            void setLastAddress()
            {
                defaultAddress = lastAddress;
            }

            void sendSSRV(const char * c, const int len, const SocketAddress & addr)
            {
                StaticBuffer<1024> message;
                void * top = message.getCurrentBinaryPtr();
                message.writeSignedInt8(0);
                message.writeBytes(c, len);
                BsdSocket::sendTo(top, message.getFilledSize(), addr);
            }

            void include(Instruction & instr) {
                TRACE();
                PackedInstruction packedFormat;
                instr.pack(packedFormat);
                packedFormat.serial = 0xffffffff & ++lastIncludedInstruction;
                addItem(packedFormat);
                newInstructions++;
                TRACE();
            }

            bool hasNewInstructions()
            {
                TRACE();
                return newInstructions > 0;
            }

            // sends out our message to server
            inline void sendMessage(const int noOfInstructions)
            {
                TRACE();
                require(state != UNINITIALIZED);

                register StaticBuffer<2048> message;
                register int totalCount = itemsCount();
                register int noToSend = common_min(common_min(totalCount, noOfInstructions), 64);
                void * top = message.getCurrentBinaryPtr();

                message.writeUnsignedInt8(noToSend ==0? 0xff : 0xff & noToSend);
                message.writeUnsignedInt32(lastIncludedInstruction);
                message.writeUnsignedInt32(lastReceivedInstruction);

                for (int x = totalCount - noToSend; x < totalCount; x++) {
                    PackedInstruction * packed = getItemPtr(x);
                    message.writeSignedInt8(packed->length);
                    message.writeBytes(packed->body, packed->length);
                }

                BsdSocket::sendTo(top, message.getFilledSize(), defaultAddress);

                //LOGBUFFER("Sending", top, message.getFilledSize());

                newInstructions = 0;
                TRACE();
            }

            // sends out our message to server
            inline void sendMessage()
            {
                TRACE();
                sendMessage(newInstructions + 2);
            }

            // checks for data
            inline void step(int ms) {
                TRACE();
                BsdSocket::step(0, ms);
            }

            // todo add sendSSRV

        private:

            // this is called when data is available
            void dataArrivalCallback()
            {
                TRACE();
                require(state != UNINITIALIZED);

                int bufLen = BIG_RECV_BUFFER_SIZE;

                if (BsdSocket::recvFrom(BsdSocket::bigRecvBuffer, bufLen, lastAddress)) {

                    //LOGBUFFER("Received", BsdSocket::bigRecvBuffer, bufLen);

                    if (bufLen >= 9) {
                        StaticBuffer<1> message(reinterpret_cast<unsigned char*>(BsdSocket::bigRecvBuffer), bufLen);
                        unsigned char count = message.readUnsignedInt8();
                        if (count != 0) {
                            unsigned int latestInstr = message.readUnsignedInt32();
                            unsigned int latestAck = message.readUnsignedInt32();

                            if (count == 0xff)
                                count = 0;

                            if (latestAck != 0xffffffff) {
                                //LOG(%i .. %u %u , itemsCount(), latestAck, getItemPtr(0)->serial);
                                while (itemsCount() > 0 && getItemPtr(0)->serial <= latestAck) {
                                    delete getItemPtr(0)->body;
                                    removeIndex(0);
                                }
                            }

                            latestInstr -= count;
                            for (int x = 0; x < count; x++) {
                                unsigned char len = message.readUnsignedInt8();
                                if ((latestInstr + x) == lastReceivedInstruction) {
                                    Instruction i(reinterpret_cast<unsigned char*>(message.getCurrentBinaryPtr()), len);
                                    core::instructionArrivalCallback(i);
                                    lastReceivedInstruction++;
                                }
                                message.seek(len);
                            }
                        } else {
                            count = message.readUnsignedInt8();
                            if (count != 0) {
                                if (state == INITIALIZED) {
                                    message.seek(-1);
                                    core::ssrvCallback(message.getCurrentStringPtr(), message.getSpaceLeft());
                                }
                            } else {
                                //TODO: other
                            }
                        }
                    } else {
                        LOG(Invalid len message %i, bufLen);
                    }
                } else {
                    LOG(Socket Error occured %i, errno);
                }
                TRACE();
            }
        };
    };
};

