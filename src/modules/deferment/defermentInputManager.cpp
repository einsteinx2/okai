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

#include "defermentInputManager.h"
#include "common.h"

namespace n02 {

	namespace deferment {

		extern int inputLength;

		struct frameInput {
			unsigned char bytes[256];
		};
		typedef frameInput frameInput;

		static DynamicAllocator<frameInput, 32> allocator;
		static DynamicOrderedArray<frameInput*> inputs;
		static frameInput reserve;

		void inputInitialize()
		{
			allocator.resetAllocation();
			inputs.clearItems();
		}

		void * getInput(int offset, int slot)
		{
			while (offset >= inputs.itemsCount()) {
				inputs.addItem(allocator.allocate());
			}
			return inputs[offset]->bytes + (slot * inputLength);
		}

		void * getInput(int offset)
		{
			while (offset >= inputs.itemsCount()) {
				inputs.addItem(allocator.allocate());
			}
			return inputs[offset]->bytes;
		}

		void * getReserveInput() {
			return reserve.bytes;
		}

		void * getReserveInput(int slot)
		{
			return reserve.bytes + (slot * inputLength);
		}

		void purgeInputBase(int no)
		{
			while (no-->0 && inputs.itemsCount() > 0) {
				allocator.free(inputs[0]);
				inputs.removeIndex(0);
			}
		}

		void inputTerminate() {
			inputInitialize();
		}

	};
};
