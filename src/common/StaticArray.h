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
    Class for managing an unordered static array
    */
    template <class _BaseType, int _TotalLen = 32>
    class StaticArray
    {

    protected:

        /* the actual array */
        _BaseType items[_TotalLen];
        /* number of items */
        int length;

    public:

        /* constructor */
        StaticArray(void)
        {
            length = 0;
        }

        /* Adds an item to the list */
        inline void addItem(const _BaseType item)
        {
            require(length < _TotalLen);
            items[length++] = item;
        }

        /* Adds an item to the list via pointer */
        inline void addItemPtr(const _BaseType * itemPtr)
        {
            require(length < _TotalLen);
            items[length++] = *itemPtr;
        }

        /* Removes an item from the list  pointed by index */
        inline void removeIndex(const int index)
        {
            require(index >= 0 && index < length);
            if (length-1!=index) {
                items[index] = items[length-1];
            }
            length = length-1;
        }

        /* Removes an item from the list by value */
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

        /* set the value of an item pointed by index */
        inline void setItem(_BaseType item, int index)
        {
            require(index >=0 && index < length);
            if (index >=0 && index < length) {
                items[index] = item;
            }
        }

        /* set the value of an item pointed by index */
        inline void setItemPtr(_BaseType * item, int index)
        {
            require(index >=0 && index < length);
            if (index >=0 && index < length) {
                items[index] = *item;
            }
        }

        /* Get an item in the array */
        inline _BaseType getItem(int index) const
        {
            require(index >=0 && index < length);
            return items[index];
        }

        /* Get pointer to an item in the array */
        inline _BaseType * getItemPtr(int index) const
        {
            require(index >=0 && index < length);
            return &items[index];
        }

        /* array access operator */
        inline _BaseType& operator[] (const int index)
        {
            require(index >=0 && index < length);
            return items[index];
        }

        /* reset items count to 0 */
        inline void clearItems()
        {
            length = 0;
        }

        /* returns the no of items in the array */
        inline int itemsCount() const
        {
            return length;
        }

    };

};
