#pragma once

#include "common.h"

namespace n02 {

	namespace kaillera {

		enum InstructionType {
			INVDNONE, USERLEAV, USERJOIN, USERLOGN, LONGSUCC, SERVPING, USERPONG, PARTCHAT,
			GAMECHAT, TMOUTRST, GAMEMAKE, GAMRLEAV, GAMRJOIN, GAMRSLST, GAMESTAT, GAMRKICK,
			GAMESHUT, GAMEBEGN, GAMEDATA, GAMCDATA, GAMRDROP, GAMRSRDY, LOGNSTAT, MOTDLINE
		};

		typedef struct {
			unsigned short serial;
			unsigned short length;
			unsigned char* body;
		} PackedInstruction;

		class Instruction: public DynamicBuffer<16>
		{
		public:
			// instruction type
			InstructionType type;
			// username field of the instruction
			char user[32];

			// constructors
			Instruction ()
			{
				type = INVDNONE;
				user[0] = 0;
			}

			Instruction (InstructionType p_type)
			{
				type = p_type;
				user[0] = 0;
			}

			Instruction (const unsigned char * buffer, const int len)
			{
				encapsulate(buffer, len);
			}

			// pack into instruction
			int pack (PackedInstruction & pack)
			{
				TRACE();
				// calculate the size required
				int min_len = (int)strlen(user) + 2;
				int body_len = getFilledSize();
				// allocate
				pack.body = new unsigned char[pack.length = static_cast<unsigned short>(min_len + body_len)];
				// encode
				*pack.body = static_cast<unsigned char>(type);
				strcpy(reinterpret_cast<char*>(pack.body + 1), user);
				copyBuffer(pack.body+min_len);
				return pack.length;
			}

			// unpack into instruction
			inline void unPack (const unsigned char * buffer, const int len)
			{
				TRACE();
				register unsigned char * packedBuffer = const_cast<unsigned char *>(buffer);
				register int packedBufferLen = len;
				// minimum length of a kaillera instruction
				if (packedBufferLen >= 2) {
					type = static_cast<InstructionType>(*packedBuffer);
					packedBuffer++;
					int username_len = strlen(reinterpret_cast<char*>(packedBuffer));
					strncpy(user, reinterpret_cast<char*>(packedBuffer), 32);
					packedBuffer += username_len;
					packedBufferLen -= username_len + 2;
					if (packedBufferLen > 0) {
						presetBuffer(packedBuffer, packedBufferLen);
					}
				}
			}
			inline void encapsulate(const unsigned char * buffer, const int len)
			{
				TRACE();
				register unsigned char * packedBuffer = const_cast<unsigned char *>(buffer);
				register int packedBufferLen = len;
				// min len
				if (packedBufferLen >= 2) {
					type = static_cast<InstructionType>(*packedBuffer);
					packedBuffer++;
					int username_len = strlen(reinterpret_cast<char*>(packedBuffer));
					strncpy(user, reinterpret_cast<char*>(packedBuffer), 32);
					packedBuffer += username_len + 1;
					packedBufferLen -= username_len + 2;
					if (packedBufferLen > 0) {
						presetBufferPtr(packedBuffer, packedBufferLen);
					}
				}
			}
			void log() {
				LOG(instruction type %i; user %s, type, user);
				LOGBUFFER("Body", ptr, reinterpret_cast<unsigned int>(end) - reinterpret_cast<unsigned int>(ptr));
			}
		};
	};
};


