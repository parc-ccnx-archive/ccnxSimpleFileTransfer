/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file. 
 */
/**
 * @author Alan Walendowski, Computing Science Laboratory, PARC
 * @copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company. All Rights Reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../tutorial_FileIO.c"
#include "../tutorial_Common.h"

#include <stdlib.h>

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(tutorial_FileIO)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(tutorial_FileIO)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(tutorial_FileIO)
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

FILE *
createTestFile(char *fileName, size_t chunkSize, int numChunks)
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
char *
createTempFileName(char *template)
{
    int numberOfChars = (int)strlen(template) + 1;
    char *fileName = parcMemory_AllocateAndClear(numberOfChars * sizeof(char));
    assertNotNull(fileName, "parcMemory_AllocateAndClear(%zu) returned NULL", numberOfChars * sizeof(char));
    strncpy(fileName, template, numberOfChars);
    mktemp(fileName);

    return fileName; // This must be parcMemory_Deallocate()'d by the caller.
}

LONGBOW_TEST_CASE(Global, getFileChunk)
{
    char *fileName = createTempFileName("/tmp/tutorial_testData.XXXXXXXX");
    size_t chunkSize = 3300;            // arbitrary
    int numberOfChunksInTestFile = 20; // arbitrary

    FILE *fp = createTestFile(fileName, chunkSize, numberOfChunksInTestFile);
    fclose(fp);

    // At this point, we have a file containing 'aaaa...' 'bbbbb...', etc, with each
    // letter repeating chunkSize times.

    PARCBuffer *bufA = tutorialFileIO_GetFileChunk(fileName, chunkSize, 4);
    PARCBuffer *bufB = tutorialFileIO_GetFileChunk(fileName, chunkSize, 5);

    assertTrue('e' == (char) parcBuffer_GetAtIndex(bufA, 0), "Expected 'e' at this location in the chunk buffer");
    assertTrue('e' == (char) parcBuffer_GetAtIndex(bufA, chunkSize - 1), "Expected 'e' at this location in the chunk buffer");
    assertTrue('f' == (char) parcBuffer_GetAtIndex(bufB, 0), "Expected 'f' at this location in the chunk buffer");
    assertTrue('f' == (char) parcBuffer_GetAtIndex(bufB, chunkSize - 1), "Expected 'f' at this location in the chunk buffer");

    parcBuffer_Release(&bufA);
    parcBuffer_Release(&bufB);

    unlink(fileName);
    parcMemory_Deallocate((void **)&fileName);
}

LONGBOW_TEST_CASE(Global, appendFileChunk)
{
    char *inFileName = createTempFileName("/tmp/tutorial_testData-src.XXXXXXXX");
    char *outFileName = createTempFileName("/tmp/tutorial_testData-dst.XXXXXXXX");

    size_t chunkSize = 2300;            // arbitrary
    int numberOfChunksInTestFile = 20;  // arbitrary

    size_t fileSize = 0;

    // Create our source data file
    FILE *fp = createTestFile(inFileName, chunkSize, numberOfChunksInTestFile);
    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    fclose(fp);

    assertTrue((chunkSize * numberOfChunksInTestFile) == fileSize, "Unexpected source data file size");

    FILE *fpDup = fopen(outFileName, "w"); // open, truncate if exists

    // This should copy inFileName to outFileName, chunk by sequential chunk.
    for (int c = 0; c < numberOfChunksInTestFile; c++) {
        PARCBuffer *buf = tutorialFileIO_GetFileChunk(inFileName, chunkSize, c);
        tutorialFileIO_AppendFileChunk(outFileName, buf);
        parcBuffer_Release(&buf);
    }

    assertTrue(tutorialFileIO_GetFileSize(inFileName) == tutorialFileIO_GetFileSize(outFileName),
               "Expected copied file to be the same size");

    // Now check a chunk of the contents of the files for sameness.
    PARCBuffer *bufA = tutorialFileIO_GetFileChunk(inFileName, chunkSize + 4, 9);
    PARCBuffer *bufB = tutorialFileIO_GetFileChunk(outFileName, chunkSize + 4, 9);

    assertTrue(parcBuffer_Equals(bufA, bufB), "Expected the file chunks to be the same");

    parcBuffer_Release(&bufA);
    parcBuffer_Release(&bufB);

    fclose(fpDup);

    unlink(inFileName);
    unlink(outFileName);
    parcMemory_Deallocate((void **)&inFileName);
    parcMemory_Deallocate((void **)&outFileName);
}

LONGBOW_TEST_CASE(Global, getFileSize)
{
    char *fileName = createTempFileName("/tmp/tutorial_testData-getFileSize.XXXXXXXX");
    size_t chunkSize = 50; // arbitrary
    int numChunks = 11;

    FILE *fp = createTestFile(fileName, chunkSize, numChunks);
    fclose(fp);

    size_t fileSize = tutorialFileIO_GetFileSize(fileName);

    assertTrue((chunkSize * numChunks) == fileSize, "File size didn't match expected size");

    unlink(fileName);
    parcMemory_Deallocate((void **)&fileName);
}

LONGBOW_TEST_CASE(Global, isFileAvailable)
{
    char *fileName = createTempFileName("/tmp/tutorial_testData-getFileSize.XXXXXXXX");

    // Make sure it's not there
    assertFalse(tutorialFileIO_IsFileAvailable(fileName), "Did not expect file to be available.");

    // Now create it
    FILE *fp = createTestFile(fileName, 10, 10);
    fclose(fp);
    assertTrue(tutorialFileIO_IsFileAvailable(fileName), "Expected file to be available.");

    // Now remove it 
    unlink(fileName);
    assertFalse(tutorialFileIO_IsFileAvailable(fileName), "Did not expect file to be available.");

    parcMemory_Deallocate((void **)&fileName);
}

LONGBOW_TEST_CASE(Global, createtDirectoryListing)
{
    PARCBuffer *listing = tutorialFileIO_CreateDirectoryListing(".");

    assertNotNull(listing, "Expected a non-null listing buffer");

    parcBuffer_Release(&listing);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(tutorial_FileIO);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
