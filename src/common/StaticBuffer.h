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

#include <string.h>

#ifndef min
#define min(a,b) ((a<b)? a:b)
#endif

#ifndef max
#define max(a,b) ((a>b)? a:b)
#endif

#pragma intrinsic(memcpy, strlen)

namespace n02 {

    // unverified

    template <int _MaxLength>
    class StaticBuffer
    {
    protected:

        unsigned char * ptr;
        unsigned char * end;
        unsigned char * begin;

    private:

        unsigned char body[_MaxLength];

    public:

        StaticBuffer(){
            reset();
        }

        StaticBuffer(const char * sourceBuffer, const int length){
            presetBufferPtr(sourceBuffer, length);
        }


    public:

        inline void writeBytes(const void * bytes, const int length)
        {
            int max_bytes_to_copy = min(length, end - ptr);
            memcpy(ptr, bytes, max_bytes_to_copy);
            ptr += max_bytes_to_copy;
        }

        inline void writeString(const char * stringValue, int length)
        {
            writeBytes(stringValue, length + 1);
        }

        inline void writeString(const char * stringValue)
        {
            writeString(stringValue, strlen(stringValue));
        }

        inline void writeSignedInt32(const int value)
        {
            *((int*)(ptr)) = value;
            ptr += sizeof(int);
        }

        inline void writeSignedInt16(const short value)
        {
            *((short*)(ptr)) = value;
            ptr += sizeof(short);
        }

        inline void writeSignedInt8(const char value)
        {
            *((char*)(ptr)) = value;
            ptr += sizeof(char);
        }

        inline void writeUnsignedInt32(const unsigned int value)
        {
            *((unsigned int*)(ptr)) = value;
            ptr += sizeof(unsigned int);
        }

        inline void writeUnsignedInt16(const unsigned short value)
        {
            *((unsigned short*)(ptr)) = value;
            ptr += sizeof(unsigned short);
        }

        inline void writeUnsignedInt8(const unsigned char value)
        {
            *((unsigned char*)(ptr)) = value;
            ptr += sizeof(unsigned char);
        }

    public:

        inline int readBytes(void * bytes, const int length)
        {
            int max_bytes_to_copy = min(end - ptr, length);
            if (le > 0) {
                memcpy(bytes, ptr, max_bytes_to_copy);
                pos += max_bytes_to_copy;
                return max_bytes_to_copy;
            }
            return 0;
        }

        inline int readSignedInt32()
        {
            int return_value = *((int*)(ptr));
            pos += sizeof(int);
            return return_value;
        }

        inline short readSignedInt16()
        {
            short return_value = *((short*)(ptr));
            pos += sizeof(short);
            return return_value;
        }

        inline char readSignedInt8()
        {
            short return_value = *((char*)(ptr));
            pos += sizeof(char);
            return return_value;
        }

        inline unsigned int readUnsignedInt32()
        {
            unsigned int return_value = *((unsigned int*)(ptr));
            pos += sizeof(unsigned int);
            return return_value;
        }

        inline unsigned short readUnsignedInt16()
        {
            short return_value = *((unsigned short*)(ptr));
            pos += sizeof(unsigned short);
            return return_value;
        }

        inline unsigned char readUnsignedInt8()
        {
            short return_value = *((unsigned char*)(ptr));
            pos += sizeof(unsigned char);
            return return_value;
        }


        inline int readString(char * stringBuffer, const int maxBufferLength)
        {
            int max_bytes_to_copy = strlen((char *)ptr) + 1;
            max_bytes_to_copy = min(max_bytes_to_copy, maxBufferLength);
            int return_value = readBytes(stringBuffer, max_bytes_to_copy);
            stringBuffer[return_value] = 0;
            return return_value;
        }

    public:

        inline char * getCurrentStringPtr()
        {
            return (char*)ptr;
        }

        inline void * getCurrentBinaryPtr()
        {
            return ptr;
        }

        inline int getFilledSize()
        {
            return ptr - begin;
        }

        inline int getTotalSize()
        {
            return end - begin;
        }

        inline int getSpaceLeft()
        {
            return end - ptr;
        }

    public:

        inline int copyBuffer(void * targetBuffer)
        {
            memcpy(targetBuffer, begin, ptr - begin);
            return ptr - begin;
        }

        inline void presetBuffer(const char * sourceBuffer, const int length)
        {
            int copied_length;
            memcpy(begin, sourceBuffer, copied_length = min(length, end - begin));
            ptr = begin;
            end = begin + copied_length;
        }

        inline void presetBufferPtr(const char * sourceBuffer, const int length)
        {
            ptr = begin = sourceBuffer;
            end = begin + length;
        }

        inline void reset()
        {
            begin = body;
            ptr = begin;
            end = begin + _MaxLength;
        }

    };

};


