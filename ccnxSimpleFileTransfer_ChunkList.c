/*
 * Copyright (c) 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @author Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2016, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <stdio.h>
#include <unistd.h>

#include <LongBow/runtime.h>
#include <parc/algol/parc_Object.h>

#include "ccnxSimpleFileTransfer_Common.h"
#include "ccnxSimpleFileTransfer_ChunkList.h"

struct ccnxSimpleFileTransfer_ChunkList {
    uint64_t numChunks;
    PARCBuffer *fileName;
    size_t chunkSize;
    CCNxContentObject **chunkPointers;
};

static void
_chunkList_Finalize(CCNxSimpleFileTransferChunkList **chunkListPtr)
{
    CCNxSimpleFileTransferChunkList *chunkList = *chunkListPtr;
    CCNxContentObject **chunkPointers = chunkList->chunkPointers;

    for (uint64_t i = 0; i < chunkList->numChunks; i++) {
        if (chunkPointers[i] != NULL) {
            ccnxContentObject_Release(&chunkPointers[i]);
        }
    }

    parcMemory_Deallocate(&chunkList->chunkPointers);

    if (chunkList->fileName != NULL) {
        parcBuffer_Release(&chunkList->fileName);
    }
}

PARCHashCode
ccnxSimpleFileTransferChunkList_HashCode(const CCNxSimpleFileTransferChunkList *chunkList)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    parcBufferComposer_PutUint64(composer, chunkList->chunkSize);
    parcBufferComposer_PutBuffer(composer, chunkList->fileName);

    PARCBuffer *buf = parcBufferComposer_ProduceBuffer(composer);

    PARCHashCode result = parcBuffer_HashCode(buf);

    parcBuffer_Release(&buf);
    parcBufferComposer_Release(&composer);

    return result;
}


bool
ccnxSimpleFileTransferChunkList_Equals(const CCNxSimpleFileTransferChunkList *a,
                                       const CCNxSimpleFileTransferChunkList *b)
{
    bool result = false;
    if (a == b) {
        result = true;
    } else {
        result = (a != NULL && b != NULL
                  && a->chunkSize == b->chunkSize
                  && a->numChunks == b->numChunks
                  && parcBuffer_Equals(a->fileName, b->fileName));
    }

    return result;
}

parcObject_ExtendPARCObject(CCNxSimpleFileTransferChunkList,
                            _chunkList_Finalize,
                            NULL, NULL,
                            ccnxSimpleFileTransferChunkList_Equals,
                            NULL,
                            ccnxSimpleFileTransferChunkList_HashCode,
                            NULL);

CCNxSimpleFileTransferChunkList *
ccnxSimpleFileTransferChunkList_Create(const char *fileName, size_t numChunks)
{

    CCNxSimpleFileTransferChunkList *result = parcObject_CreateAndClearInstance(CCNxSimpleFileTransferChunkList);

    if (fileName != NULL) {
        result->fileName = parcBuffer_WrapCString((char *) fileName);
    }

    size_t sizeNeeded = (numChunks * sizeof(CCNxContentObject *));
    result->chunkPointers = parcMemory_AllocateAndClear(sizeNeeded);

    result->numChunks = numChunks;

    return result;
}

void
ccnxSimpleFileTransferChunkList_SetChunk(CCNxSimpleFileTransferChunkList *chunkList,
                                         int slot, const CCNxContentObject *content)
{
    CCNxContentObject **chunkPointers = chunkList->chunkPointers;

    if (chunkPointers[slot] != NULL) {
        ccnxContentObject_Release(&chunkPointers[slot]);
    }
    chunkPointers[slot] = ccnxContentObject_Acquire(content);
}

CCNxContentObject *
ccnxSimpleFileTransferChunkList_GetChunk(CCNxSimpleFileTransferChunkList *chunkList, int slot)
{
    CCNxContentObject **chunkPointers = chunkList->chunkPointers;
    return chunkPointers[slot];
}

uint64_t
ccnxSimpleFileTransferChunkList_GetNumChunks(CCNxSimpleFileTransferChunkList *chunkList)
{
    return chunkList->numChunks;
}


parcObject_ImplementAcquire(ccnxSimpleFileTransferChunkList, CCNxSimpleFileTransferChunkList);

parcObject_ImplementRelease(ccnxSimpleFileTransferChunkList, CCNxSimpleFileTransferChunkList);




