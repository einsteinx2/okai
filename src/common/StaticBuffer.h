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

#include "_common.h"

namespace n02 {

    /*
    static buffer management class
    */
    template <int _MaxLength>
    class StaticBuffer
    {
    protected:

        // write/read pointer
        unsigned char * ptr;
        // end of buffer
        unsigned char * end;
        // begining of buffer
        unsigned char * begin;

    private:

        // teh static body
        unsigned char body[_MaxLength];

    public:

        /* constructor */
        StaticBuffer(){
            reset();
        }

        /* constructor */
        StaticBuffer(const char * sourceBuffer, const int length){
            presetBufferPtr(sourceBuffer, length);
        }


    public:

        /* write specified no of bytes to the buffer */
        inline void writeBytes(const void * bytes, const int length)
        {
            require(end - ptr <= length && bytes != 0 && length != 0);
            memcpy(ptr, bytes, length);
            ptr += length;
        }

        /* write string */
        inline void writeString(const char * stringValue, int length)
        {
            writeBytes(stringValue, length + 1);
        }

        /* write string */
        inline void writeString(const char * stringValue)
        {
            writeString(stringValue, strlen(stringValue));
        }

#define _primitive_write(_type) \
    require(end - ptr < sizeof(_type)); \
    *reinterpret_cast<_type*>(ptr) = value; \
    ptr += sizeof(_type);

        /* write 32 bit signed integer */
        inline void writeSignedInt32(const int value)
        {
            _primitive_write(int);
        }

        /* write 16 bit signed integer */
        inline void writeSignedInt16(const short value)
        {
            _primitive_write(short);
        }

        /* write 8 bit signed integer */
        inline void writeSignedInt8(const char value)
        {
            _primitive_write(char);
        }

        /* write 32 bit unsigned integer */
        inline void writeUnsignedInt32(const unsigned int value)
        {
            _primitive_write(unsigned int);
        }

        /* write 16 bit unsigned integer */
        inline void writeUnsignedInt16(const unsigned short value)
        {
            _primitive_write(unsigned short);
        }

        /* write 8 bit unsigned integer */
        inline void writeUnsignedInt8(const unsigned char value)
        {
            _primitive_write(unsigned char);
        }

#undef _primitive_write

    public:

        /* read specified number of bytes */
        inline int readBytes(void * bytes, const int length)
        {
            require (end - ptr >= length && length > 0);
            memcpy(bytes, ptr, length);
            ptr += length;
            return length;
        }

        /* read string */
        inline int readString(char * stringBuffer, const int maxBufferLength)
        {
            register int bytes_to_remove = common_min(static_cast<int>(strlen(reinterpret_cast<char*>(ptr)) + 1), end - ptr);
            register int max_bytes_to_copy = common_min(bytes_to_remove, maxBufferLength);
            if (max_bytes_to_copy > 0) {
                memcpy(stringBuffer, ptr, max_bytes_to_copy);
                stringBuffer[max_bytes_to_copy] = 0;
                ptr += bytes_to_remove;
            }
            return bytes_to_remove;
        }

#define _primitive_read_return(_type) \
    require(end - ptr < sizeof(_type)); \
    register _type return_value = *reinterpret_cast<_type*>(ptr);\
    ptr += sizeof(_type);\
    return return_value;

        /* read signed 32 bit integer */
        inline int readSignedInt32()
        {
            _primitive_read_return(int);
        }

        /* read signed 16 bit integer */
        inline short readSignedInt16()
        {
            _primitive_read_return(short);
        }

        /* read signed 8 bit integer */
        inline char readSignedInt8()
        {
            _primitive_read_return(char);
        }

        /* read unsigned 32 bit integer */
        inline unsigned int readUnsignedInt32()
        {
            _primitive_read_return(unsigned int);
        }

        /* read unsigned 16 bit integer */
        inline unsigned short readUnsignedInt16()
        {
            _primitive_read_return(unsigned short);
        }

        /* read unsigned 8 bit integer */
        inline unsigned char readUnsignedInt8()
        {
            _primitive_read_return(unsigned char);
        }

#undef _primitive_read_return

    public:

        /* return current pointer as a string */
        inline char * getCurrentStringPtr()
        {
            return reinterpret_cast<char*>(ptr);
        }

        /* return current pointer as void* */
        inline void * getCurrentBinaryPtr()
        {
            return reinterpret_cast<void*>(ptr);
        }

        /* get the amount of space traversed/ filled */
        inline int getFilledSize()
        {
            return ptr - begin;
        }

        /* get the total size of the buffer */
        inline int getTotalSize()
        {
            return end - begin;
        }

        /* get the amount of space left/ can be filled */
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
            require(length <= _MaxLength);
            int copied_length;
            memcpy(begin, sourceBuffer, copied_length = common_min(length, end - begin));
            ptr = begin;
            end = begin + copied_length;
        }

        inline void presetBufferPtr(const unsigned char * sourceBuffer, const int length)
        {
            ptr = begin = const_cast<unsigned char*>(sourceBuffer);
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


