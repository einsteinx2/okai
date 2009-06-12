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

#include <string.h>
#pragma intrinsic(memcpy, strlen)

//#define USE_ASSERT
//#define DISABLE_CHECKS

#ifndef DISABLE_CHECKS
#define ENABLE_CHECKS
#endif

#ifdef USE_ASSERT

#ifdef NDEBUG

#ifndef DISABLE_CHECKS
#undef NODEBUG
#define NODEBUGREF

#endif

#endif

#include <assert.h>


#ifdef NODEBUGREF
#define NODEBUG
#endif

#endif

namespace n02 {

    void reportAssertionError(const char * condition, const char * file, const int line, const char * function);


#ifdef ENABLE_CHECKS

#ifdef USE_ASSERT

#define require(CONDITION) assert(CONDITION)

#else

#define require(CONDITION) \
    if (!(CONDITION)) \
    reportAssertionError(#CONDITION, __FILE__, __LINE__, __FUNCTION__)

#endif

#else

#define require(CONDITION)

#endif

    template <class baseType> inline baseType * commonAlloc(int length) {
        return new baseType[length];
    }

    template <class baseType> inline void commonFree(baseType * allocated) {
        require(allocated != 0);
        delete allocated;
    }

    template <class baseType> inline baseType * commonReAlloc(baseType * allocated, int currentLength, int length) {
        require(allocated != 0);
        if (currentLength < length) {
            baseType * newlyAllocated = commonAlloc<baseType>(length);
            memcpy(newlyAllocated, allocated, currentLength * sizeof(baseType));
            commonFree<baseType>(allocated);
            return newlyAllocated;
        } else {
            return allocated;
        }
    }

#define common_max(a, b) ((a >= b)? a : b)
#define common_min(a, b) ((a <= b)? a : b)

};

