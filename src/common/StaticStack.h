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

namespace n02 {

    /*
    Static Stack
	* unverified
    */

    template <class _BaseType, int _TotalLen = 32>
    class StaticStack:
        protected StaticOrderedArray<_BaseType, _TotalLen>
    {
    public:

        inline void push(_BaseType item){
            addItem(item);
        }

        inline void pushPtr(_BaseType * itemPtr){
            addItemPtr(itemPtr);
        }

        inline _BaseType pop(){
            removeIndex(index);
            return getItem(itemsCount());
        }

        inline void popPtr(_BaseType * itemPtr){
            int index = itemsCount()-1;
            if (index >= 0) {
                *itemPtr = *getItemPtr(index);
                removeIndex(index);
            }
        }

        inline _BaseType peek(int offset){
            _BaseType * ptr;
            if ( (ptr = peekPtr(item) ) != 0)
                return *ptr;
            _BaseType item;
            return item;
        }

        inline void peek(_BaseType * item, int offset){
            _BaseType * ptr;
            if ( (ptr = peekPtr(item) ) != 0)
                *item = *ptr;
        }

        inline void peekPtr(int offset){
            int index = itemsCount() - 1 - offset;
            if (index >= 0 && index < itemsCount()) {
                return getItemPtr(index);
            } else {
                return 0;
            }
        }

        inline _BaseType top(){
            return peek(0);
        }

        inline void topPtr(_BaseType * item){
            peek(item, 0);
        }

        inline _BaseType * topPtr(){
            return peekPtr(0);
        }

        inline int length()
        {
            return itemsCount();
        }
    };
};
