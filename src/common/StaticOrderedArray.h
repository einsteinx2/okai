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
    Class for managing ordered static array
    */
    template <class _BaseType, int _TotalLen = 32>
    class StaticOrderedArray
    {

    protected:

        /* items */
        _BaseType items[_TotalLen];
        /* items count */
        int length;

    public:

        /* constructor */
        StaticOrderedArray(void)
        {
            length = 0;
        }

        /* adds an item to the list */
        inline void addItem(_BaseType item)
        {
            require(length < _TotalLen);
            items[length++] = item;
        }

        /* adds an item to the list */
        inline void addItemPtr(_BaseType * itemPtr)
        {
            require(length < _TotalLen);
            items[length++] = *itemPtr;
        }

        /* inserts an item */
        inline void insertItem(_BaseType item, int index)
        {
            require(index >= 0);
            if (index >= length) {
                addItem(item);
            } else {
                memcpy(&items[index+1], &items[index], (length - index) * sizeof(_BaseType));
                items[index] = item;
                length++;
            }
        }

        /* inserts an item */
        inline void insertItemPtr(_BaseType * itemPtr, int index)
        {
            require(index >= 0);
            if (index >= length) {
                addItemPtr(itemPtr);
            } else {
                memcpy(&items[index+1], &items[index], (length - index) * sizeof(_BaseType));
                items[index] = *itemPtr;
                length++;
            }
        }

        /* inserts an item */
        inline void removeIndex(int index)
        {
            require (index >= 0 && index < length);
            if (length-1!=index) {
                memcpy(&items[index], &items[index+1], (length-1-index) * sizeof(_BaseType));
            }
            length = length-1;
        }

        /* inserts an item */
        inline void removeItem(_BaseType item)
        {
            for (int i = 0; i < length; i++)
            {
                if (items[i] == item) {
                    removeIndex(i);
                    break;
                }
            }
        }

        /* set an item value at an index */
        inline void setItem(_BaseType item, int index)
        {
            require(index >=0 && index < length);
            items[index] = item;
        }

        /* set an item value at an index */
        inline void setItemPtr(_BaseType * item, int index)
        {
            require(index >=0 && index < length);
            items[index] = *item;
        }

        /* get an item */
        inline _BaseType getItem(int index)
        {
            require(index >=0 && index < length);
            return items[index];
        }

        /* returns item pointer */
        inline _BaseType * getItemPtr(int index)
        {
            require(index >=0 && index < length);
            return &items[index];
        }

        /* array access operator overload */
        inline _BaseType& operator[] (const int index)
        {
            require(index >=0 && index < length);
            return items[index];
        }

        // resets item count to 0
        inline void clearItems()
        {
            length = 0;
        }

        // return size
        inline int itemsCount()
        {
            return length;
        }

    };
};
