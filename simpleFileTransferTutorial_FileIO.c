/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>

#include <LongBow/runtime.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_BufferComposer.h>

#include "simpleFileTransferTutorial_FileIO.h"
#include "simpleFileTransferTutorial_Common.h"

PARCBuffer *
simpleFileTransferTutorialFileIO_GetFileChunk(const char *fileName, size_t chunkSize, uint64_t chunkNum)
{
    FILE *file = fopen(fileName, "r");

    // NOTE: Opening and seeking in the file for each chunk is NOT a very efficient way to
    //       retrieve chunks. Consider keeping the file open, or caching in memory, or...

    assertNotNull(file, "Could not open file '%s' - stopping.", fileName);

    // Seek to the location of the desired chunk in the file.
    assertTrue(fseek(file, chunkSize * chunkNum, SEEK_SET) == 0, "Could not seek to desired chunk");

    // If we're here, we were able to seek to the start of the desired chunk

    PARCBuffer *result = parcBuffer_Allocate(chunkSize);

    size_t numberOfBytesNeeded = chunkSize;
    size_t numberOfBytesRead = 0;       // # bytes read in each read.
    size_t totalNumberOfBytesRead = 0;  // Overall # of bytes read

    // Read until we get the required number of bytes.
    while (numberOfBytesNeeded > 0
           && (numberOfBytesRead = fread(parcBuffer_Overlay(result, 0), 1, numberOfBytesNeeded, file)) > 0) {
        numberOfBytesNeeded -= numberOfBytesRead;
        parcBuffer_SetPosition(result, parcBuffer_Position(result) + numberOfBytesRead);

        totalNumberOfBytesRead += numberOfBytesRead;
    }

    parcBuffer_SetLimit(result, totalNumberOfBytesRead);
    parcBuffer_Flip(result);

    fclose(file);

    return result;
}

size_t
simpleFileTransferTutorialFileIO_AppendFileChunk(const char *fileName, const PARCBuffer *chunk)
{
    size_t numBytesWritten = 0;

    FILE *file = fopen(fileName, "a"); // Open for appending. Create if it doesn't exist.

    assertNotNull(file, "Could not open file '%s' - stopping.", fileName);

    const void *buffer = parcBuffer_Overlay((PARCBuffer *) chunk, 0); // We're un-const'ing for parcBuffer_Overlay, but we do not change the buffer state.

    numBytesWritten = fwrite(buffer, 1, parcBuffer_Remaining(chunk), file);

    assertTrue(numBytesWritten == parcBuffer_Remaining(chunk),
               "Couldn't write requested chunk to file: %s", fileName);

    fclose(file);

    return numBytesWritten;
}

bool
simpleFileTransferTutorialFileIO_IsFileAvailable(const char *filePath)
{
    return (access(filePath, F_OK | R_OK) == 0);
}

size_t
simpleFileTransferTutorialFileIO_GetFileSize(const char *filePath)
{
    size_t fileSize = 0;

    FILE *fp = fopen(filePath, "r");

    assertNotNull(fp, "Could not open file '%s' to get size.", filePath);

    if (NULL != fp) {
        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        fclose(fp);
    }
    return fileSize;
}

PARCBuffer *
simpleFileTransferTutorialFileIO_CreateDirectoryListing(const char *directoryName)
{
    DIR *directory = opendir(directoryName);

    assertNotNull(directory, "Couldn't open directory '%s' for reading.", directoryName);

    PARCBufferComposer *directoryListing = parcBufferComposer_Create();

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        switch (entry->d_type) {
            case DT_REG: {
                // a regular file

                // We need the full file path to check its size.
                PARCBufferComposer *fullFilePath = parcBufferComposer_Create();
                parcBufferComposer_Format(fullFilePath, "%s/%s", directoryName, entry->d_name);

                PARCBuffer *fileNameBuffer = parcBufferComposer_ProduceBuffer(fullFilePath);
                char *fullFilePathString = parcBuffer_ToString(fileNameBuffer);
                parcBuffer_Release(&fileNameBuffer);

                if (simpleFileTransferTutorialFileIO_IsFileAvailable(fullFilePathString)) {
                    parcBufferComposer_Format(directoryListing, "  %s  (%zu bytes)\n",
                                              entry->d_name, simpleFileTransferTutorialFileIO_GetFileSize(fullFilePathString));
                }

                parcBufferComposer_Release(&fullFilePath);
                parcMemory_Deallocate((void **) &fullFilePathString);

                break;
            }

            case DT_LNK:
            case DT_DIR:
            default:
                // ignore everything but regular files
                break;
        }
    }

    closedir(directory);

    PARCBuffer *result = parcBufferComposer_ProduceBuffer(directoryListing);
    parcBufferComposer_Release(&directoryListing);

    return result;
}

bool
simpleFileTransferTutorialFileIO_DeleteFile(const char *fileName)
{
    // Unlink the file. Return true if succesful, false if not.
    // False could mean the file didn't originally exist.
    return (unlink(fileName) == 0);
}
