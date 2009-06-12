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
                int min_len = (int)strlen(user) + 2;TRACE();
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
            void logFilled() {
                LOG(instruction type %i; user %s, type, user);
                LOGBUFFER("Body", begin, reinterpret_cast<unsigned int>(ptr) - reinterpret_cast<unsigned int>(begin));
            }
        };
    };
};
