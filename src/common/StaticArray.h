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

namespace n02 {

	/*
		Unordered static array
	*/
    template <class _BaseType, int _TotalLen = 32>
    class StaticArray
    {

    protected:

        _BaseType items[_TotalLen];
        int length;

    public:

        StaticArray(void)
        {
            length = 0;
        }

        inline void addItem(_BaseType item)
        {
            items[length++] = item;
        }


        inline void addItemPtr(_BaseType * itemPtr)
        {
            items[length++] = *itemPtr;
        }

        inline void removeIndex(int index)
        {
            if (index >= 0 && index < length) {
                if (length-1!=index) {
                    items[index] = items[length-1];
                }
                length = length-1;
            }
        }

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

        inline void setItem(_BaseType item, int index)
        {
            if (index >=0 && index < length) {
                items[index] = item;
            }
        }

        inline void setItemPtr(_BaseType * item, int index)
        {
            if (index >=0 && index < length) {
                items[index] = *item;
            }
        }

        inline _BaseType getItem(int index)
        {
            return items[index];
        }

        inline _BaseType * getItemPtr(int index)
        {
            return &items[index];
        }

        inline _BaseType& operator[] (const int index)
        {
            return items[index];
        }

        inline void clearItems()
        {
            length = 0;
        }

        inline int itemsCount()
        {
            return length;
        }

    };

};
