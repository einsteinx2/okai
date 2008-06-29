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

#include "DynamicOrderedArray.h"
#include "_common.h"

namespace n02 {

    /*
    Dynamic Stack
    */

    template <class _BaseType, int _BlockLen = 32>
    class DynamicStack:
        private DynamicOrderedArray<_BaseType, _BlockLen>
    {
    public:

        // push an item on the stack
        inline void push(_BaseType item)
        {
            addItem(item);
        }

        // push an item on the stack
        inline void pushPtr(_BaseType * itemPtr)
        {
            addItemPtr(itemPtr);
        }

        // pops an item from the list
        inline _BaseType pop()
        {
            register int index = itemsCount()-1;
            require(index >= 0);
            register _BaseType * returnValue = getItemPtr(index);
            removeIndex(index);
            return *returnValue;
        }

        // pops an item from the list
        inline void popPtr(_BaseType * itemPtr)
        {
            register int index = itemsCount()-1;
            require(index >= 0);
            *itemPtr = *getItemPtr(index);
            removeIndex(index);
        }

        // peeks an item on the stack at specified offset from the top
        inline _BaseType peek(int offset)
        {
            return *peekPtr(offset);
        }

        // peeks an item on the stack at specified offset from the top
        inline void peek(_BaseType * item, int offset)
        {
            *item = *peekPtr(offset);
        }

        // peeks pointer of an item on the stack at specified offset from the top
        inline _BaseType * peekPtr(int offset)
        {
            require(offset < length && length > 0);
            return getItemPtr(length - 1 - offset);
        }

        // returns the top element
        inline _BaseType top()
        {
            return peek(0);
        }

        // returns the top element
        inline void topPtr(_BaseType * item)
        {
            peek(item, 0);
        }

        // returns the top element
        inline _BaseType * topPtr()
        {
            return peekPtr(0);
        }

        // returns stack height
        inline int itemsCount()
        {
            return length;
        }

        // reset stack
        inline void clearItems()
        {
            length = 0;
        }
    };
};
