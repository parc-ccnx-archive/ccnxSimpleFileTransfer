/*
 * Copyright (c) 2014, 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @copyright 2014, 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../simpleFileTransferTutorial_FileIO.c"

#include <stdlib.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(simpleFileTransferTutorial_FileIO)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(simpleFileTransferTutorial_FileIO)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(simpleFileTransferTutorial_FileIO)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, getFileSize);
    LONGBOW_RUN_TEST_CASE(Global, appendFileChunk);
    LONGBOW_RUN_TEST_CASE(Global, getFileChunk);
    LONGBOW_RUN_TEST_CASE(Global, isFileAvailable);
    LONGBOW_RUN_TEST_CASE(Global, createtDirectoryListing);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

static FILE *
_createTestFile(char *fileName, size_t chunkSize, int numChunks)
{
    // Create a file of known contents and size. We went for simplicity
    // over efficiency.

    FILE *fp = fopen(fileName, "w"); // open, and truncate to 0 if it exists
    for (int c = 0; c < numChunks; c++) {
        for (int i = 0; i < chunkSize; i++) {
            fputc((int) (c + 'a'), fp);
        }
    }
    return fp;
}

/**
 * Create a temporary filename from a template.
 * The resulting fileName must be freed by calling parcMemory_Deallocate()
 */
static char *

_createTempFileName(char *template)
{
    int numberOfChars = (int) strlen(template) + 1;
    char *fileName = parcMemory_AllocateAndClear(numberOfChars * sizeof(char));
    assertNotNull(fileName, "parcMemory_AllocateAndClear(%zu) returned NULL", numberOfChars * sizeof(char));
    strncpy(fileName, template, numberOfChars);
    mktemp(fileName);

    return fileName; // This must be parcMemory_Deallocate()'d by the caller.
}

LONGBOW_TEST_CASE(Global, getFileChunk)
{
    char *fileName = _createTempFileName("/tmp/simpleFileTransferTutorial_testData.XXXXXXXX");
    size_t chunkSize = 3300;        // arbitrary
    int numberOfChunksInTestFile = 20; // arbitrary

    FILE *fp = _createTestFile(fileName, chunkSize, numberOfChunksInTestFile);
    fclose(fp);

// At this point, we have a file containing 'aaaa...' 'bbbbb...', etc, with each
// letter repeating chunkSize times.

    PARCBuffer *bufA = simpleFileTransferTutorialFileIO_GetFileChunk(fileName, chunkSize, 4);
    PARCBuffer *bufB = simpleFileTransferTutorialFileIO_GetFileChunk(fileName, chunkSize, 5);

    assertTrue('e' == (char) parcBuffer_GetAtIndex(bufA, 0), "Expected 'e' at this location in the chunk buffer");
    assertTrue('e' == (char) parcBuffer_GetAtIndex(bufA, chunkSize - 1), "Expected 'e' at this location in the chunk buffer");
    assertTrue('f' == (char) parcBuffer_GetAtIndex(bufB, 0), "Expected 'f' at this location in the chunk buffer");
    assertTrue('f' == (char) parcBuffer_GetAtIndex(bufB, chunkSize - 1), "Expected 'f' at this location in the chunk buffer");

    parcBuffer_Release(&bufA);
    parcBuffer_Release(&bufB);

    unlink(fileName);
    parcMemory_Deallocate((void **) &fileName);
}

LONGBOW_TEST_CASE(Global, appendFileChunk)
{
    char *inFileName = _createTempFileName("/tmp/simpleFileTransferTutorial_testData-src.XXXXXXXX");
    char *outFileName = _createTempFileName("/tmp/simpleFileTransferTutorial_testData-dst.XXXXXXXX");

    size_t chunkSize = 2300;        // arbitrary
    int numberOfChunksInTestFile = 20; // arbitrary

    size_t fileSize = 0;

// Create our source data file
    FILE *fp = _createTestFile(inFileName, chunkSize, numberOfChunksInTestFile);
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    fclose(fp);

    assertTrue((chunkSize * numberOfChunksInTestFile) == fileSize, "Unexpected source data file size");

    FILE *fpDup = fopen(outFileName, "w"); // open, truncate if exists

// This should copy inFileName to outFileName, chunk by sequential chunk.
    for (int c = 0; c < numberOfChunksInTestFile; c++) {
        PARCBuffer *buf = simpleFileTransferTutorialFileIO_GetFileChunk(inFileName, chunkSize, c);
        simpleFileTransferTutorialFileIO_AppendFileChunk(outFileName, buf);
        parcBuffer_Release(&buf);
    }

    assertTrue(simpleFileTransferTutorialFileIO_GetFileSize(inFileName)
               == simpleFileTransferTutorialFileIO_GetFileSize(outFileName),
               "Expected copied file to be the same size");

// Now check a chunk of the contents of the files for sameness.
    PARCBuffer *bufA = simpleFileTransferTutorialFileIO_GetFileChunk(inFileName, chunkSize + 4, 9);
    PARCBuffer *bufB = simpleFileTransferTutorialFileIO_GetFileChunk(outFileName, chunkSize + 4, 9);

    assertTrue(parcBuffer_Equals(bufA, bufB), "Expected the file chunks to be the same");

    parcBuffer_Release(&bufA);
    parcBuffer_Release(&bufB);

    fclose(fpDup);

    unlink(inFileName);
    unlink(outFileName);
    parcMemory_Deallocate((void **) &inFileName);
    parcMemory_Deallocate((void **) &outFileName);
}

LONGBOW_TEST_CASE(Global, getFileSize)
{
    char *fileName = _createTempFileName("/tmp/simpleFileTransferTutorial_testData-getFileSize.XXXXXXXX");
    size_t chunkSize = 50; // arbitrary
    int numChunks = 11;

    FILE *fp = _createTestFile(fileName, chunkSize, numChunks);
    fclose(fp);

    size_t fileSize = simpleFileTransferTutorialFileIO_GetFileSize(fileName);

    assertTrue((chunkSize * numChunks) == fileSize, "File size didn't match expected size");

    unlink(fileName);
    parcMemory_Deallocate((void **) &fileName);
}

LONGBOW_TEST_CASE(Global, isFileAvailable)
{
    char *fileName = _createTempFileName("/tmp/simpleFileTransferTutorial_testData-getFileSize.XXXXXXXX");

// Make sure it's not there
    assertFalse(simpleFileTransferTutorialFileIO_IsFileAvailable(fileName), "Did not expect file to be available.");

// Now create it
    FILE *fp = _createTestFile(fileName, 10, 10);
    fclose(fp);
    assertTrue(simpleFileTransferTutorialFileIO_IsFileAvailable(fileName), "Expected file to be available.");

// Now remove it
    unlink(fileName);
    assertFalse(simpleFileTransferTutorialFileIO_IsFileAvailable(fileName), "Did not expect file to be available.");

    parcMemory_Deallocate((void **) &fileName);
}

LONGBOW_TEST_CASE(Global, createtDirectoryListing)
{
    PARCBuffer *listing = simpleFileTransferTutorialFileIO_CreateDirectoryListing(".");

    assertNotNull(listing, "Expected a non-null listing buffer");

    parcBuffer_Release(&listing);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(simpleFileTransferTutorial_FileIO);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
