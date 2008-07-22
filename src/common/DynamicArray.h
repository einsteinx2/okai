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
    Class for manazing and sizing dynamic arrays
    _BlockLen: must be > 0
    */
    template <class _BaseType, int _BlockLen = 32>
    class DynamicArray
    {

    protected:

        /* pointer to our allocated memory block */
        _BaseType * items;

        /* no of items */
        int length;


    private:

        /* size of the allocated block */
        int _size;

    public:

        /* constructor */
        DynamicArray(void)
        {
            length = 0;
            items = 0;
            _size = 0;
        }

        /* distructor */
        ~DynamicArray(void)
        {
            if (items) {
                commonFree<_BaseType>(items);
                items = 0;
            }
            length = 0;
            _size = 0;
        }

        /* Adds an item to the list */
        inline void addItem(_BaseType item)
        {
            checkAllocationSize();
            items[length++] = item;
        }

        /* Adds an item to the list via pointer */
        inline void addItemPtr(_BaseType * itemPtr)
        {
            checkAllocationSize();
            items[length++] = *itemPtr;
        }

        /* removes an item from the list via index */
        inline void removeIndex(int index)
        {
            require(index >= 0 && index < length);
            if (length-1!=index) {
                items[index] = items[length-1];
            }
            length = length-1;
        }

        /* removes an item from the list via value */
        inline void removeItem(_BaseType item)
        {
            for (int i = 0; i < length; i++) {
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
            items[index] = item;
        }

        /* set the value of an item pointed by index */
        inline void setItemPtr(_BaseType * item, int index)
        {
            require(index >=0 && index < length);
            items[index] = *item;
        }

        /* get the value of an item pointed by index */
        inline _BaseType getItem(int index) const
        {
            require(index >=0 && index < length);
            return items[index];
        }

        /* set the pointere of an item pointed by index */
        inline _BaseType * getItemPtr(int index) const
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

        /* clear items count */
        inline void clearItems()
        {
            length = 0;
            if (items != 0) {
                items = commonReAlloc<_BaseType>(items, length, _size = _BlockLen);
            }
        }

        /* get size */
        inline int itemsCount() const
        {
            return length;
        }

    private:

        // makes sure there is extra space for one or more element
        inline void checkAllocationSize(int extra = 1)
        {
            if (items == 0) {
                items = commonAlloc<_BaseType>(_size = common_max(_BlockLen, extra));
            } else {
                if (length + extra >= _size) {
                    items = commonReAlloc<_BaseType>(items, length, _size = (extra + (length/_BlockLen)) * _BlockLen);
                }
            }
        }

    };

};
