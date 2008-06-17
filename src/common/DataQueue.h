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

#include <memory>

#ifndef min
#define min(a,b) ((a<b)? a:b)
#endif

namespace n02 {

    /*
    Queue for data?
    * unverified
    */

    class DataQueue {

    protected:

        unsigned char *	ptr;
        unsigned char *	begin;
        unsigned char *	end;

    public:

        DataQueue ()
        {
            begin = end = ptr = 0;
        }

        DataQueue(void * presetBuffer, int presetBufferLen)
        {
            begin = (unsigned char*)malloc(presetBufferLen);
            ptr = end = begin + presetBufferLen;
            memcpy(begin, presetBuffer, presetBufferLen);
        }

        DataQueue(int initialAllocation)
        {
            ptr = begin = (unsigned char*)malloc(initialAllocation);
            end = begin + presetBufferLen;
        }

        ~DataQueue()
        {
            if (begin) {
                free(begin);
            }
            begin = end = ptr = 0;
        }

    public:

        void * front() {
            return (void*)begin;
        }

        int push(const void * data, int dataLen)
        {

            if (dataLen > 0) {			
                ensureSized(dataLen);
                memcpy(ptr, data, dataLen);
                ptr += dataLen;
            }
            return ptr - begin;
        }

        int peek (void * dataBuffer, int dataBufferLen)
        {
            int min_copy_length;
            if ((min_copy_length = min (dataBufferLen, ptr - begin)) > 0) {
                memcpy(dataBuffer, begin, min_copy_length);
                return min_copy_length;
            }
            return 0;
        }

        void pop (int lenToRemove)
        {
            lenToRemove = min(lenToRemove, ptr - begin);

            if (lenToRemove > 0) {
                memcpy(begin, begin+lenToRemove, lenToRemove);
                ptr -= lenToRemove;
            }			
        }

        int pop (void * dataBuffer, int dataBufferLen)
        {
            int min_copy_length;
            if ((min_copy_length = min (dataBufferLen, ptr - begin)) > 0) {
                memcpy(dataBuffer, begin, min_copy_length);
                memcpy(begin, begin+min_copy_length, min_copy_length);
                ptr -= min_copy_length;
                return min_copy_length;
            }
            return 0;
        }

        void reset()
        {
            ptr = begin;
        }

        int length() {
            return ptr - begin;
        }


    protected:

        void ensureSized(int extraDataLen)
        {
            if (begin) {
                if (ptr + extraDataLen > end) {
                    int new_size = (end - begin) << 1;
                    while (begin + new_size < ptr + extraDataLen) {
                        new_size = new_size << 1;
                    }
                    ptr -= (int) begin;
                    begin = (unsigned char*)realloc(begin, new_size);
                    ptr += (int) begin;
                    end = begin + new_size;
                }
            } else {
                begin = ptr = (unsigned char*)malloc(extraDataLen * 8);
                end = begin + extraDataLen * 8;
            }
        }
    };
};

