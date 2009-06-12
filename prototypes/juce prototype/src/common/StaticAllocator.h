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

#include "StaticArray.h"
#include "_common.h"

namespace n02 {

    /*
    Class to manage a fixed no of allocations
    */

    template <class _BaseType, int _Count>
    class StaticAllocator
    {

    protected:


        /* the actual items */
        _BaseType items[_Count];

        /* list of unallocated items */
        StaticArray<_BaseType*, _Count> unallocated;


    public:

        /* constructor */
        StaticAllocator()
        {
            resetAllocation();
        }

        /* allocate 1 element */
        inline _BaseType * allocate()
        {
            require(unallocated.itemsCount() > 0);
            register _BaseType * _riele = unallocated[0];
            unallocated.removeIndex(0);
            return _riele;
        }

        // free allocated element
        inline void free(_BaseType * element)
        {
            require(element >= &items[0] && element <= &items[_Count-1]);
            unallocated.addItem(element);
        }

        /* returns no of allocated objects count */
        inline int allocatedCount() const
        {
            return _Count - unallocated.itemsCount();
        }

        /* returns no of free elements count */
        inline int freeCount() const
        {
            return unallocated.itemsCount();
        }

        /* resets allocation */
        void resetAllocation()
        {
            unallocated.clearItems();
            for(int x =0; x < _Count; x++){
                unallocated.addItem(&items[x]);
            }
        }
    };
};
