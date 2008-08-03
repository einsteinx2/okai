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
#include <cstdio>

namespace n02 {

    /*
    Queue for data?
    */
    class DataQueue {

    protected:

        /* pointer to write cursor */
        unsigned char *	ptr;
        /* pointer to begining of the buffer */
        unsigned char *	begin;
        /* pointer to the end of the buffer */
        unsigned char *	end;

    public:

        /* constructor */
        DataQueue ()
        {
            begin = end = ptr = 0;
        }

        /* constructor */
        DataQueue(void * presetBuffer, int presetBufferLen)
        {
            require(presetBuffer != 0 && presetBufferLen > 0);
            begin = commonAlloc<unsigned char>(presetBufferLen);
            ptr = end = begin + presetBufferLen;
            memcpy(begin, presetBuffer, presetBufferLen);
        }

        /* constructor */
        DataQueue(int initialAllocation)
        {
            ptr = begin = commonAlloc<unsigned char>(initialAllocation);
            end = begin + initialAllocation;
        }

        /* distructor */
        ~DataQueue()
        {
            if (begin) {
                commonFree<unsigned char>(begin);
            }
            begin = end = ptr = 0;
        }

    public:

        /* return front */
        inline void * front()
        {
            return reinterpret_cast<void*>(begin);
        }

        /* enqueue some data */
        int push(const void * data, int dataLen)
        {
            require (dataLen > 0 && data != 0);
            ensureSized(dataLen);
            memcpy(ptr, data, dataLen);
			//printf("[%08x:%i]", *reinterpret_cast<int*>(ptr), ptr - begin);
            ptr += dataLen;
            return ptr - begin;
        }

        /* peek at the data */
        int peek (void * data, int dataLen)
        {
            require (dataLen > 0 && data != 0);
            int min_copy_length;
            if ((min_copy_length = common_min (dataLen, ptr - begin)) > 0) {
                memcpy(data, begin, min_copy_length);
                return min_copy_length;
            }
            return 0;
        }

        /* blank data removal */
        void pop (int lenToRemove)
        {
            lenToRemove = common_min (lenToRemove, ptr - begin);

            if (lenToRemove > 0) {
                memcpy(begin, begin+lenToRemove, lenToRemove);
                ptr -= lenToRemove;
            }			
        }

        /* data removal */
        int pop (void * dataBuffer, int dataBufferLen)
        {
            int min_copy_length;
            if ((min_copy_length = common_min (dataBufferLen, ptr - begin)) > 0) {
                memcpy(dataBuffer, begin, min_copy_length);
                memcpy(begin, begin+min_copy_length, (ptr - begin) - min_copy_length);
                ptr -= min_copy_length;
                return min_copy_length;
            }
            return 0;
        }

        /* reset pointer */
        void reset()
        {
            ptr = begin;
        }

        /* return queued length */
        int length() {
            return ptr - begin;
        }


    protected:

        void ensureSized(int extraDataLen)
        {
            if (begin) {
                if (ptr + extraDataLen > end) {
					//printf("redo");
                    int new_size = (end - begin) << 1;
                    while (begin + new_size < ptr + extraDataLen) {
                        new_size = new_size << 1;
                    }
                    ptr -= reinterpret_cast<int>(begin);
                    begin = commonReAlloc<unsigned char>(begin, reinterpret_cast<int>(ptr), new_size);
                    ptr += reinterpret_cast<int>(begin);
                    end = begin + new_size;
                }
            } else {
				//printf("inalloc");
                begin = ptr = commonAlloc<unsigned char>(extraDataLen * 8);
                end = begin + extraDataLen * 8;
            }
        }
    };
};

