#pragma once

#include "kaillera_Instruction.h"

#ifndef MAX_CACHED_INSTRS
#define MAX_CACHED_INSTRS 32
#endif

namespace n02 {
	namespace kaillera {

		namespace core {
			void dataArrivalCallback(char * data, int len);
			void instructionArrivalCallback(Instruction & instr);
		};

		enum ClientMessagingState {
			UNINITIALIZED, // uninitialized
			DATAMSGS, // expecting ordinaty messages
			INSTRMSGS // expecting messaging containing instructions
		};

		class ClientMessaging:
			private UdpSocket,
			private DynamicOrderedArray<PackedInstruction, MAX_CACHED_INSTRS * 2>		
		{
		private:
			ClientMessagingState state;
			unsigned short lastIncludedInstruction;
			unsigned short lastReceivedInstruction;
			int newInstructions;
			int oldInstructionsPerMessage;

		public:
			SocketAddress lastAddress;

		public:
			ClientMessaging() : state(UNINITIALIZED), UdpSocket(){}

			bool initialize(int localPort)
			{
				TRACE();
				require(state == UNINITIALIZED);
				
				lastIncludedInstruction = 0xFFFF;
				lastReceivedInstruction = 0xFFFF;
				oldInstructionsPerMessage = 2;
				newInstructions = 0;

				if (UdpSocket::initialize(localPort, 32000)) {
					state = DATAMSGS;
				} else {
					close();
				}

				TRACE();
				return state == DATAMSGS;
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

			void setState(ClientMessagingState newState) {
				TRACE();
				require(newState != UNINITIALIZED);

				state = newState;
				TRACE();
			}

			void setAddress(const SocketAddress & addr) {
				TRACE();
				defaultAddress = addr;
				TRACE();
			}

			void sendData(const void * data, const int len) {
				TRACE();
				send(data, len);
				TRACE();
			}

			void includeInstruction(Instruction & instr) {
				TRACE();
				PackedInstruction packedFormat;
				instr.pack(packedFormat);
				packedFormat.serial = ++lastIncludedInstruction;
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
			void sendMessage(const int noOfInstructions)
			{
				TRACE();
				char sendBuffer[2042];

				int noToSend = min(min(itemsCount(), noOfInstructions), MAX_CACHED_INSTRS);

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
			void sendMessage()
			{
				TRACE();
				sendMessage(newInstructions + oldInstructionsPerMessage);
			}

			// checks for data
			void step(int ms) {
				TRACE();
				BsdSocket::step(0, ms);
				TRACE();
			}

		private:

			inline bool searchProcessInstruction(unsigned short forSerial, unsigned char * buffer, int bufferLen, int instrsCount) {
				TRACE();
				//LOG(fors = %i buflen = %i finstrount = %i, forSerial, bufferLen, instrsCount);
				while (instrsCount > 0) {
					unsigned short serial = *((unsigned short*)buffer); buffer+= 2;
					unsigned short length = *((unsigned short*)buffer); buffer+= 2;
					if (serial == forSerial) {
						TRACE();
						Instruction instr(buffer, length);
						core::instructionArrivalCallback(instr);
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
				require(state != UNINITIALIZED);

				int bufLen = BIG_RECV_BUFFER_SIZE;

				if (BsdSocket::recvFrom(BsdSocket::bigRecvBuffer, &bufLen, &lastAddress)) {
					if (state == INSTRMSGS) {
						TRACE();
						//LOGBUFFER("Received", BsdSocket::bigRecvBuffer, bufLen);
						if (bufLen > 1 + 4 + 2) {
							char instructionsCount = *bigRecvBuffer;
							while (searchProcessInstruction(lastReceivedInstruction+1, reinterpret_cast<unsigned char*>(bigRecvBuffer+1), bufLen, instructionsCount))
								lastReceivedInstruction++;
						} else {
							require(bufLen > 1 + 4 + 2);
						}
					} else {
						TRACE();
						core::dataArrivalCallback(BsdSocket::bigRecvBuffer, bufLen);
					}
				} else {
					if (errno == 0) {

					} else {
						LOG(Socket Error occured %i, errno);
					}
				}
				TRACE();
			}
		};
	};
};

