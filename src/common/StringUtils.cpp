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

#include "StringUtils.h"
#include "common.h"
#include <string.h>

namespace n02 {

    static char h2ctable[16] = 
    {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'a', 'b',
        'c', 'd', 'e', 'f'
    };

    static unsigned char c2htable[256];

    static char b2ptable[256];

    static char tempBuffer[128];

    static char b64encode[64];
    static char b64decode[256];


    static inline void char_to_hex_(char * strbuffer, unsigned char byte) {
        unsigned char lob = byte & 0xf;
        unsigned char hib = (byte & 0xf0)>>4;
        *strbuffer++ = h2ctable[hib];
        *strbuffer = h2ctable[lob];
    }


    void StringUtils::initialize() {

        // char to hex table
        memset(c2htable, 0, sizeof(c2htable));

        c2htable['0'] = 0;
        c2htable['1'] = 1;
        c2htable['2'] = 2;
        c2htable['3'] = 3;
        c2htable['4'] = 4;
        c2htable['5'] = 5;
        c2htable['6'] = 6;
        c2htable['7'] = 7;
        c2htable['8'] = 8;
        c2htable['9'] = 9;
        c2htable['a'] = c2htable['A'] = 10;
        c2htable['b'] = c2htable['B'] = 11;
        c2htable['c'] = c2htable['C'] = 12;
        c2htable['d'] = c2htable['D'] = 13;
        c2htable['e'] = c2htable['E'] = 14;
        c2htable['f'] = c2htable['F'] = 15;

        //bytes to printable table
        memset(b2ptable, 0, sizeof(b2ptable));

        for (int x = 0; x < 256; x++) {
            b2ptable[x] = '.';
        }

        for (char x = ' '; x <= '~'; x++) {
            b2ptable[x] = x;
        }

        { // base 64 encoding and decoding table
            int i = 0;

            memset(b64encode, 0, sizeof(b64encode));
            memset(b64decode, 0, sizeof(b64encode));

            for (char x = 'A'; x <= 'Z'; x++) {
                b64encode[i] = x;
                b64decode[x] = i & 0xff;
                i++;
            }
            for (char x = 'a'; x <= 'z'; x++) {
                b64encode[i] = x;
                b64decode[x] = i & 0xff;
                i++;
            }
            for (char x = '0'; x <= '9'; x++) {
                b64encode[i] = x;
                b64decode[x] = i & 0xff;
                i++;
            }
            b64encode[i] = '+';
            b64decode['+'] = i & 0xff;
            i++;
            b64encode[i] = '/';
            b64decode['/'] = i & 0xff;
            i++;
        }



        //for (int p = 1; p < 6; p++) {

        //	char buffer[257];
        //	for(int x = 0; x < 256; x++)
        //		buffer[x] = 0xff;

        //	LOGBUFFER("x", buffer, p);
        //	char buf[(16 * 8 / 3) + 2];
        //	StringUtils::base64encode(buf, buffer, p);
        //	memset(buffer, 0, sizeof(buffer));
        //	LOG(%s=%i, buf, strlen(buf));
        //	int l = StringUtils::base64decode(buffer, buf);
        //	LOGBUFFER("T(x)", buffer, l);

        //}



    }



    void StringUtils::intToAlpha(char * a, const int i) {

        unsigned int u = (unsigned int) i;
        if (u & (1<<31)) { //signed
            u = ~(u-1);
            *a++ = '-';
        }
        uintToAlpha(a, u);

    }

    char * StringUtils::intToAlpha(const int i) {
        intToAlpha(tempBuffer, i);
        return tempBuffer;
    }

    void StringUtils::uintToAlpha(char * a, const unsigned int i) {
        unsigned int _i = i;
        char buffer[12];
        int co = 0;
        while (_i > 0 || co == 0) {
            char c = (_i % 10);
            _i = _i / 10;
            buffer[co++] = c + '0';
        }
        while (co>0) {
            *a++ = buffer[--co];
        }
        *a = 0;	
    }

    char * StringUtils::uintToAlpha(const unsigned int i) {
        uintToAlpha(tempBuffer, i);
        return tempBuffer;	
    }

    void StringUtils::uintToHex(char * a, const unsigned int i) {
        unsigned int _i = i;
        a += 8; *a = 0;
        a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
        a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
        a -= 2; char_to_hex_(a, _i & 0xFF); _i = _i >> 8;
        a -= 2; char_to_hex_(a, _i & 0xFF);
    }

    char * StringUtils::uintToHex(const unsigned int i) {
        uintToHex(tempBuffer, i);
        return tempBuffer;	
    }

    unsigned int StringUtils::alphaToUint(const char * a) {

        unsigned int _i = 0;

        while (isAlnum(*a)) {
            _i *= 10;
            _i += *a++ - '0';
        }
        return _i;

    }

    int StringUtils::alphaToInt(const char * a) {
        int sign = 1;
        if (*a=='-') {
            sign = -1;
            a++;
        }
        return alphaToUint(a) * sign;
    }


    // buffering
    void StringUtils::bytesToStr(char * strbuffer, const unsigned char * bytes_buffer, const int _len, const int spacing) {
        int len = _len;
        int counter = 0;
        while (len-->0) {
            char_to_hex_(strbuffer, *bytes_buffer++);
            strbuffer += 2;
            if (++counter % spacing == 0) {
                *strbuffer = ' ';
                strbuffer++;
            }
        }
        *strbuffer = 0;
    }


    void StringUtils::strToBytes(unsigned char * bytes_buffer, char * strbuffer, int len) {
        while (len-->0) {
            unsigned char hib = c2htable[*strbuffer++];
            unsigned char lob = c2htable[*strbuffer++];
            *bytes_buffer++ = (hib << 4) | lob;
        }
    }


    void StringUtils::bytesToPrintable(char * strbuffer, unsigned char * bytes_buffer, int len) {
        while (len-->0) {
            *strbuffer = b2ptable[*bytes_buffer];
            strbuffer++;
            bytes_buffer++;
        }
        *strbuffer = 0;
    }

    //misc
    bool StringUtils::isPrintable(char x) {
        if (isAlpha(x) ||
            isAlnum(x) ||
            x == '.' ||
            x == ' '
            ) return true;
        return false;
    }

    bool StringUtils::isAlpha(char x) {
        return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'A');
    }

    bool StringUtils::isAlnum(char x) {
        return (x >= '0' && x <= '9');
    }

    void StringUtils::trim(char * buffer) {
        while (*buffer != 0) {
            buffer++;
        }
        buffer--;
        while (*buffer == ' ' || *buffer == '\t' || *buffer == '\r' || *buffer == '\n') {
            *buffer = 0;
            *buffer--;
        }
    }

};

namespace n02 {
    int StringUtils::base64encode(char * destination, const void * sourceBuffer, const int len)
    {
        unsigned char encBuffer[1025];
        memset(encBuffer, 0, sizeof(encBuffer));
        memcpy(encBuffer, sourceBuffer, len);

        int totalConvertedLen = (len * 4 / 3) + ((len % 3 == 0) ? 0 : 1);
        unsigned char * c = encBuffer;
        unsigned char current = *c; c++;

        unsigned char byte = 0;
        for (int x = 0; x < len; x++) {
            switch (x % 3) {
                case 0:
                    byte = (current & 0xfc) >> 2;
                    *destination = b64encode[byte];
                    destination++;
                    LOG(%i %i %x, x, x%3, byte);
                    byte = (current & 3) << 4;
                    break;
                case 1:
                    byte |= ((current & 0xf0) >> 4);
                    *destination = b64encode[byte];
                    destination++;
                    LOG(%i %i %x, x, x%3, byte);
                    byte = (current & 0x0f) << 2;
                    break;
                case 2:
                    byte |= ((current & 0xc0) >> 6);
                    *destination = b64encode[byte];
                    destination++;
                    LOG(%i %i %x, x, x%3, byte);
                    byte = current & 0x3f;
                    *destination = b64encode[byte];
                    destination++;
                    LOG(%i %i %x, x, x%3, byte);
                    byte = 0;
                    break;
            };
            current = *c; c++;
        }
        if (byte != 0) {
            *destination = b64encode[byte];
            destination++;
        }
        *destination = 0;
        return totalConvertedLen;
    }

    // returns len of new buffer null terminated = (strlen(source) * 6 / 8) + 1
    int StringUtils::base64decode(void * destination, const char * source)
    {
        unsigned char * dst = reinterpret_cast<unsigned char*>(destination);
        unsigned char * c = reinterpret_cast<unsigned char*>(const_cast<char*>(source));
        int len = strlen(source);
        require (len > 1);
        len = (len * 3 / 4);
        unsigned char current = b64decode[*c]; c++;
        unsigned char next = b64decode[*c];

        for (int x = 0; x < len; x++) {
            switch (x % 3) {
                case 0:
                    *dst = (current << 2) | ((next & 0x30) >> 4);
                    break;
                case 1:
                    *dst = ((current&0x0f) << 4) | ((next & 0x3c) >> 2);
                    break;
                case 2:
                    *dst = ((current&0x03) << 6) | next;
                    current = next; 
                    next = b64decode[*c]; c++;
                    break;
            };
            current = next; 
            next = b64decode[*c]; c++;
            dst++;
        }
        return len;
    }
};


namespace n02 {

    /*
    The following code was taken from Julian Storer's JUCE so (c) him
    */

    int StringUtils::WIDEToUTF8 (unsigned char * buffer, const wchar_t * text) 
    {
        int num = 0, index = 0;

        for (;;)
        {
            const unsigned int c = (unsigned int) text [index++];

            if (c >= 0x80)
            {
                int numExtraBytes = 1;

                if (c >= 0x800)
                {
                    ++numExtraBytes;

                    if (c >= 0x10000)
                    {
                        ++numExtraBytes;

                        if (c >= 0x200000)
                        {
                            ++numExtraBytes;

                            if (c >= 0x4000000)
                                ++numExtraBytes;
                        }
                    }
                }

                buffer [num++] = (unsigned char) ((0xff << (7 - numExtraBytes)) | (c >> (numExtraBytes * 6)));
                while (--numExtraBytes >= 0)
                    buffer [num++] = (unsigned char) (0x80 | (0x3f & (c >> (numExtraBytes * 6))));
            }
            else
            {
                if (buffer != 0)
                    buffer [num] = (unsigned char) c;

                ++num;
            }

            if (c == 0)
                break;
        }

        return num;

    }

    int StringUtils::UTF8ToWIDE (wchar_t * dest, const unsigned char * buffer, int bufferSizeBytes)
    {
        wchar_t * pd = dest;
        int numBytes;
        for (numBytes = 0; numBytes < bufferSizeBytes; ++numBytes)
            if (buffer [numBytes] == 0)
                break;

        int i = 0;
        while (i < numBytes)
        {
            const unsigned char  c = buffer [i++];

            if ((c & 0x80) != 0)
            {
                int mask = 0x7f;
                int bit = 0x40;
                int numExtraValues = 0;

                while (bit != 0 && (c & bit) != 0)
                {
                    bit >>= 1;
                    mask >>= 1;
                    ++numExtraValues;
                }

                int n = (c & mask);

                while (--numExtraValues >= 0 && i < bufferSizeBytes)
                {
                    const unsigned char c = buffer[i];

                    if ((c & 0xc0) != 0x80)
                        break;

                    n <<= 6;
                    n |= (c & 0x3f);
                    ++i;
                }

                *dest++ = (wchar_t) n;
            }
            else
            {
                *dest++ = (wchar_t) c;
            }
        }

        *dest = 0;

        return dest - pd - 1;
    }
};

