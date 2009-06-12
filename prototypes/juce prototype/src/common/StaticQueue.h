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

#include "StaticOrderedArray.h"
#include "_common.h"

namespace n02 {

    /*
    Represents a Queue object implemented over ordered static array
    */
    template <class _BaseType, int _TotalLength = 32>
    class StaticQueue        
    {
    protected:
        /* The queue items */
        _BaseType items[_TotalLength];

        /* length of the queue */
        int length;

        /* base of the queue */
        unsigned int base;
        /*
        base = 2, len = 3:
        [-][-][1][2][3][-][-][-]		
        base = 5, len = 5:
        [4][5][-][-][-][1][2][3]
        */
    public:

        /* constructor */
        StaticQueue() {
            length = base = 0;
        }

        /* enqueue an item */
        inline void push(_BaseType item)
        {
            require(length < _TotalLength);
            items[(base + length++) % _TotalLength] = item;
        }
        /* enqueue an item */
        inline void pushPtr(_BaseType * itemPtr)
        {
            require(length < _TotalLength);
            items[(base + length++) % _TotalLength] = *itemPtr;
        }

        /* put in the front of the queue */
        inline void pushFront(_BaseType item)
        {
            require(length < _TotalLength);
            length++;
            items[(--base) % _TotalLength] = item;
        }

        /* put in the front of the queue */
        inline void pushFront(_BaseType * itemPtr)
        {
            require(length < _TotalLength);
            length++;
            items[(--base) % _TotalLength] = *itemPtr;
        }

        /* un-queue an item */
        inline _BaseType pop()
        {
            require(length > 0);
            length--;
            return items[(base++) % _TotalLength];
        }

        /* un-queue an item */
        inline void popPtr(_BaseType * itemPtr)
        {
            require(length > 0);
            length--;
            *itemPtr = items[(base++) % _TotalLength];
        }

        /* send an item to hell */
        inline void popBlank()
        {
            require(length > 0);
            length--;
            base++;
        }

        /* abduct the last person in the queue */
        inline _BaseType popBack()
        {
            require(length > 0);
            length--;
            return items[(base+length) % _TotalLength];
        }

        /* abduct the last person in the queue */
        inline void popBackPtr(_BaseType * itemPtr)
        {
            require(length > 0);
            length--;
            *itemPtr = items[(base+length) % _TotalLength];
        }


        /* send the last person in the queue to hell */
        inline _BaseType popBackBlank()
        {
            require(length > 0);
            length--;
        }

        /* peek ahead in the queue */
        inline _BaseType peek(int offset) const
        {
            require(length > 0 && offset < length);
            return items[(base+offset) % _TotalLength];
        }

        /* peek ahead in the queue */
        inline void peek(_BaseType * item, int offset) const
        {
            require(length > 0 && offset < length);
            *item = items[(base+offset) % _TotalLength];
        }

        /* peek ahead in the queue */
        inline _BaseType* peekPtr(int offset) const
        {
            require(length > 0 && offset < length);
            return &items[(base+offset) % _TotalLength];
        }

        /* peek from the back */
        inline _BaseType peekBack(int offset) const
        {
            return peek(size()-offset-1);
        }

        /* peek from the back */
        inline void peekBack(_BaseType * item, int offset) const
        {
            peek(item, size()-offset-1);
        }

        /* peek from the back */
        inline _BaseType* peekBackPtr(int offset) const
        {
            return peekPtr(size()-offset-1);
        }

        /* returns the front of thq queue */
        inline _BaseType front() const
        {
            require(length > 0);
            return peek(0);
        }

        /* returns the front of thq queue */
        inline void frontPtr(_BaseType * item) const
        {
            require(length > 0);
            peek(item, 0);
        }

        /* returns the back of thq queue */
        inline _BaseType back() const
        {
            return peek(size()-1);
        }

        /* returns the back of thq queue */
        inline _BaseType * backPtr() const
        {
            return peekPtr(size()-1);
        }

        /* returns the back of thq queue */
        inline int size() const
        {
            return length;
        }
        inline int itemsCount() const
        {
            return length;
        }

        /* reset queue */
        inline void clear()
        {
            length = 0;
            base = 0;
        }

    };
};
