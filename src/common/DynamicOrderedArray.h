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
    Class for managing dynamic ordered array
    _BlockLen: must be > 0
    */
    template <class _BaseType, int _BlockLen = 32>
    class DynamicOrderedArray
    {

    protected:

        /* holds the actual allocated block */
        _BaseType * items;

        /* items count */
        int length;

    private:

        /* Size of the allocated block */
        int _size;

    public:

        /* constructor */
        DynamicOrderedArray(void)
        {
            length = 0;
            items = 0;
            _size = 0;
        }

        /* distructor */
        ~DynamicOrderedArray(void)
        {
            if (items) {
                delete items;
                items = 0;
            }
            length = 0;
            _size = 0;
        }

        /* adds an item to the list */
        inline void addItem(_BaseType item) {
            checkAllocationSize();
            items[length++] = item;
        }

        /* adds an item to the list */
        inline void addItemPtr(_BaseType * itemPtr) {
            checkAllocationSize();
            items[length++] = *itemPtr;
        }

        /* inserts an item at specified index */
        inline void insertItem(_BaseType item, int index)
        {
            require(index >= 0);
            checkAllocationSize();
            if (index >= length) {
                addItem(item);
            } else {
                memcpy(&items[index+1], &items[index], (length - index) * sizeof(_BaseType));
                items[index] = item;
                length++;
            }
        }

        /* inserts an item at specified index */
        inline void insertItemPtr(_BaseType * itemPtr, int index)
        {
            require(index >= 0);
            checkAllocationSize();
            if (index >= length) {
                addItemPtr(itemPtr);
            } else {
                memcpy(&items[index+1], &items[index], (length - index) * sizeof(_BaseType));
                items[index] = *itemPtr;
                length++;
            }
        }

        /* removes item at index */
        inline void removeIndex(int index)
        {
            require(index >= 0 && index < length);
            if (length-1!=index) {
                memcpy(&items[index], &items[index+1], (length-1-index) * sizeof(_BaseType));
            }
            length = length-1;
        }

        /* remove an item by value */
        inline void removeItem(_BaseType item)
        {
            for (int i = 0; i < length; i++) {
                if (items[i] == item) {
                    removeIndex(i);
                    break;
                }
            }
        }

        /* set value of an item on index */
        inline void setItem(_BaseType item, int index)
        {
            require (index >=0 && index < length);
            items[index] = item;
        }

        /* set value of an item on index */
        inline void setItemPtr(_BaseType * item, int index)
        {
            require (index >=0 && index < length);
            items[index] = *item;
        }

        /* get item at an index */
        inline _BaseType getItem(int index) const
        {
            require (index >=0 && index < length);
            return items[index];
        }

        /* get item at an index */
        inline _BaseType * getItemPtr(int index) const
        {
            require (index >=0 && index < length);
            return &items[index];
        }

        /* array access operator overload */
        inline _BaseType& operator[] (const int index)
        {
            require (index >=0 && index < length);	
            return items[index];
        }

        /* reset item count to 0 */
        inline void clearItems()
        {
            length = 0;
            if (items != 0){
                items = commonReAlloc<_BaseType>(items, length, _size = _BlockLen);
            }
        }

        /* returns size */
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
