/******************************************************************************
>         .d8888b.   .d8888b.                                                 <
>        d88P  Y88b d88P  Y88b                                                <
>        888    888        888                                                <
88888b.  888    888      .d88P                                                <
888 "88b 888    888  .od888P"                                                 <
888  888 888    888 d88P"                                                     <
888  888 Y88b  d88P 888"                                                      <
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
#include "modRecorder.h"
#include "module.h"
#include "common.h"
#include <time.h>
#include <fstream>
#include "zlib.h"
#include "juce02.h"

#define N02_KREC_COMPRESS_STREAM

#define INDIVIDUAL_BUFFER_SIZE 100 * 1024

namespace n02 {
    namespace recording {

#include "krec.h"

        static DynamicOrderedArray<StaticBuffer<INDIVIDUAL_BUFFER_SIZE>*> data;
        static DynamicOrderedArray<char*> metadata;
        StaticBuffer<INDIVIDUAL_BUFFER_SIZE> * buffer = 0;

        bool recording = false;
        krecFileHeader01 fileHeader;

        static void resetStorage()
        {
            memset(&fileHeader, 0, sizeof(fileHeader));

            recording = false;

            while (data.itemsCount() > 0) {
                delete data[0];
                data.removeIndex(0);
            }

            if (buffer != 0)
                delete buffer;

            buffer = 0;

            while (metadata.itemsCount() > 0)
            {
                delete metadata[0];
                metadata.removeIndex(0);
            }
        }

        static void bufferAllocateNewIfNeeded(int size)
        {
            if (buffer != 0) {
                if (buffer->getSpaceLeft() < size) {
					//printf("[n]");
                    data.addItem(buffer);
                    buffer = new StaticBuffer<INDIVIDUAL_BUFFER_SIZE>;
                }
            } else {
                buffer = new StaticBuffer<INDIVIDUAL_BUFFER_SIZE>;
            }
        }


        static int N02CCNV start (const char * gameName, const int playerNo, const int totalPlayers, const char * comments)
        {
            resetStorage();

            strncpy(fileHeader.game, gameName, sizeof(fileHeader.game)-1);
            fileHeader.playerNo = (unsigned short) playerNo;
            fileHeader.totalPlayers = (unsigned short) totalPlayers;
            strncpy(fileHeader.comment, comments, sizeof(fileHeader.comment)-1);
            fileHeader.time = (unsigned int)time(0);

            recording = true;

            LOGS(Starts recording);

            return 0;
        }

        static void N02CCNV stop ()
        {
            TRACE();
            if (recording) {
                TRACE();
                LOGS(Stops recording);

				LOGS(Preparing to compress data);

                // dynamic buffer
                DynamicBuffer<100000> final;

                // put metadata at the top
                {
                    while (metadata.itemsCount() > 0) {
                        krecDataChunkHeader01 head = {RDATA_EOF, 0};
                        head.size = (short)strlen(metadata[0]) + 1;
                        final.writeBytes(&head, sizeof(head));
                        final.writeString(metadata[0], head.size-1);
                        metadata.removeIndex(0);
                    }
                }

                // now put in the buffers
                {
                    while (data.itemsCount() > 0) {
                        StaticBuffer<INDIVIDUAL_BUFFER_SIZE> * buf = data[0];
                        data.removeIndex(0);
                        final.writeBytes(buf->getCurrentStringPtr() - buf->getFilledSize(), buf->getFilledSize());
                        delete buf;						
                    }

					final.writeBytes(buffer->getCurrentStringPtr() - buffer->getFilledSize(), buffer->getFilledSize());
                    delete buffer;
					buffer = 0;

					krecDataChunkHeader01 endMarker;
					endMarker.type = RDATA_EOF;
					endMarker.size = 0;
					final.writeBytes(&endMarker, 3);
                }

                TRACE();

#ifdef N02_KREC_COMPRESS_STREAM
                LOGS(Compressing data);

                uLongf compLength = (final.getFilledSize() * 3) / 2;
                unsigned char * compressedBufrer = commonAlloc<unsigned char>(compLength);

                compress2(compressedBufrer, &compLength, (unsigned char*)(final.getCurrentStringPtr() - final.getFilledSize()), final.getFilledSize(), 9);

                fileHeader.version = KREC_VERSION1;
                strcpy(fileHeader.app, client->app.name);
				if (strlen(client->app.version)!=0) {
					strcat(fileHeader.app, ";");
					strcat(fileHeader.app, client->app.version);
				}
                fileHeader.Compressed = 1;
                fileHeader.crc = crc32(crc32(0L, Z_NULL, 0), compressedBufrer, compLength);

                LOGS(Writing to file);

                char filename[1024];
                sprintf(filename, "records%s%08x_%02i_%02i.krec", PATH_SEPERATOR, fileHeader.time, fileHeader.playerNo, fileHeader.totalPlayers);

                {
                    juce::File fl("records");
                    fl.createDirectory();
                }

                std::ofstream of (filename, std::ios_base::out | std::ios_base::binary);

                if (of.is_open()) {
                    of.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
                    int t = final.getFilledSize();
                    of.write(reinterpret_cast<char*>(&t), 4);
                    of.write(reinterpret_cast<char*>(compressedBufrer), compLength);
                    of.close();
                    LOG(Successfully written to %s, filename);
                }
                LOGS(Cleaning up);
                commonFree<unsigned char>(compressedBufrer);
#else
                fileHeader.version = KREC_VERSION1;
                strcpy(fileHeader.app, client->app.name);
				if (strlen(client->app.version)!=0) {
					strcat(fileHeader.app, ";");
					strcat(fileHeader.app, client->app.version);
				}
                fileHeader.Compressed = 0;
                fileHeader.crc = crc32(crc32(0L, Z_NULL, 0), (unsigned char*)(final.getCurrentStringPtr() - final.getFilledSize()), final.getFilledSize());

                LOGS(Writing to file);
                TRACE();

                {
                    juce::File fl("records");
                    fl.createDirectory();
                }

                char filename[1024];
                sprintf(filename, "records%s%08x_%02i_%02i.krec", PATH_SEPERATOR, fileHeader.time, fileHeader.playerNo, fileHeader.totalPlayers);

				std::ofstream of (filename, std::ios_base::out | std::ios_base::binary);

                TRACE();

                if (of.is_open()) {
                    TRACE();of.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
                    TRACE();int t = final.getFilledSize();
                    TRACE();of.write(reinterpret_cast<char*>(&t), 4);
                    TRACE();of.write(reinterpret_cast<char*>(final.getCurrentStringPtr() - final.getFilledSize()), final.getFilledSize());
                    TRACE();of.close();
                    TRACE();LOG(Successfully written to %s, filename);
                } else {
                    LOGS(Could not open file);
                }
#endif

            }

            recording = false;
        }

        static int N02CCNV addMetadata (const char * dataName, const char * data)
        {
            if (recording) {
                char xxx[1034];
                sprintf(xxx, "%s:%s", dataName, data);
                metadata.addItem(strdup(xxx));
            }
            return 0;
        }

        static int N02CCNV addComment (const char * comment)
        {
            if (recording) {
                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_COMMENT;
                dchunk.size = (short)strlen(comment) + 1;
                bufferAllocateNewIfNeeded(3 + dchunk.size);
                buffer->writeBytes(&dchunk, 3);
                buffer->writeBytes(comment, dchunk.size);
            }
            return 0;
        }

        static int N02CCNV addGameSyncData (const void * value, const int len)
        {
            if (recording) {
                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_GSDATA;
                dchunk.size = (short)len;
                bufferAllocateNewIfNeeded(3 + dchunk.size);
                buffer->writeBytes(&dchunk, 3);
                buffer->writeBytes(value, dchunk.size);
            }
            return 0;
        }

        static int N02CCNV addSyncData (const void * value, const int len)
        {
            if (recording) {
                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_SDATA;
                dchunk.size = (short)len;
                bufferAllocateNewIfNeeded(3 + dchunk.size);
                buffer->writeBytes(&dchunk, 3);
                if (dchunk.size > 0)
                    buffer->writeBytes(value, dchunk.size);
            }
            return 0;
        }

        static int N02CCNV addAsyncData (const void * value, const int len)
        {
            if (recording) {
                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_ADATA;
                dchunk.size = (short)len;
                bufferAllocateNewIfNeeded(3 + dchunk.size);
                buffer->writeBytes(&dchunk, 3);
                if (dchunk.size > 0)
                    buffer->writeBytes(value, dchunk.size);
            }
            return 0;
        }

        static int N02CCNV addChat (const char * username, const char * chat)
        {
            if (recording) {
                int unlen = strlen(username) + 1;
                int charl = strlen(chat) + 1;

                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_ADATA;
                dchunk.size = (short)(unlen + charl);

                bufferAllocateNewIfNeeded(3 + dchunk.size);

                buffer->writeBytes(&dchunk, 3);

                buffer->writeBytes(username, unlen);
                buffer->writeBytes(chat, charl);
            }
            return 0;
        }

        static int N02CCNV addDrop (const char * username, int playerNo)
        {
            if (recording) {
                int unlen = strlen(username) + 1;

                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_ADATA;
                dchunk.size = (short)(unlen + 2);

                bufferAllocateNewIfNeeded(3 + dchunk.size);

                buffer->writeBytes(&dchunk, 3);

                buffer->writeBytes(username, unlen);
                buffer->writeSignedInt16((short)playerNo);
            }
            return 0;
        }

        static void N02CCNV setDefaultInput (void * value, int len)
        {
            if (recording) {
                krecDataChunkHeader01 dchunk;
                dchunk.type = RDATA_DEFDATA;
                dchunk.size = (short)len;
                bufferAllocateNewIfNeeded(3 + dchunk.size);
                buffer->writeBytes(&dchunk, 3);
                buffer->writeBytes(value, dchunk.size);
            }
        }

        static void N02CCNV reserved(int, int)
        {
        }

    };

    RecorderInterface02 riRecorder = {
        recording::start,
        recording::stop,
        recording::addMetadata,
        recording::addComment,
        recording::addGameSyncData,
        recording::addSyncData,
        recording::addAsyncData,
        recording::addChat,
        recording::addDrop,
        recording::setDefaultInput,
        recording::reserved,
        recording::reserved,
        recording::reserved
    };

    STDMODULE(modRecorder, "krc1.encoder", MTYPE02_RECORDER, riRecorder, 0, 0, MOD02_STATUS_WORKING, "KRC1 Recorder Module");

};
