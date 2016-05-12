/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 * 
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @author Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2016 Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../ccnxSimpleFileTransfer_ChunkList.c"

#include <parc/algol/parc_SafeMemory.h>
#include <LongBow/unit-test.h>

LONGBOW_TEST_RUNNER(ccnxSimpleFileTransfer_ChunkList)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(ccnxSimpleFileTransfer_ChunkList)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(ccnxSimpleFileTransfer_ChunkList)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, createRelease);
    LONGBOW_RUN_TEST_CASE(Global, acquireRelease);
    LONGBOW_RUN_TEST_CASE(Global, equals);
    LONGBOW_RUN_TEST_CASE(Global, setChunk);
    LONGBOW_RUN_TEST_CASE(Global, getChunk);
    LONGBOW_RUN_TEST_CASE(Global, hashCode);
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

LONGBOW_TEST_CASE(Global, createRelease)
{
    char *fileName = "filename.txt";
    CCNxSimpleFileTransferChunkList *chunkList = ccnxSimpleFileTransferChunkList_Create(fileName, 100);

    ccnxSimpleFileTransferChunkList_Release(&chunkList);
}

LONGBOW_TEST_CASE(Global, acquireRelease)
{
    char *fileName = "filename.txt";
    CCNxSimpleFileTransferChunkList *chunkList = ccnxSimpleFileTransferChunkList_Create(fileName, 10);
    CCNxSimpleFileTransferChunkList *ref = ccnxSimpleFileTransferChunkList_Acquire(chunkList);
    ccnxSimpleFileTransferChunkList_Release(&chunkList);
    ccnxSimpleFileTransferChunkList_Release(&ref);
}

LONGBOW_TEST_CASE(Global, equals)
{
    char *fileName = "filename.txt";
    CCNxSimpleFileTransferChunkList *a = ccnxSimpleFileTransferChunkList_Create(fileName, 100);
    CCNxSimpleFileTransferChunkList *b = ccnxSimpleFileTransferChunkList_Create(fileName, 100);
    CCNxSimpleFileTransferChunkList *c = ccnxSimpleFileTransferChunkList_Create(fileName, 100);

    char *fileNameD = "differentName.txt";
    CCNxSimpleFileTransferChunkList *d = ccnxSimpleFileTransferChunkList_Create(fileNameD, 100);
    CCNxSimpleFileTransferChunkList *e = ccnxSimpleFileTransferChunkList_Create(fileName, 200);

    assertEqualsContract(ccnxSimpleFileTransferChunkList_Equals, a, b, c, d, e);

    ccnxSimpleFileTransferChunkList_Release(&a);
    ccnxSimpleFileTransferChunkList_Release(&b);
    ccnxSimpleFileTransferChunkList_Release(&c);
    ccnxSimpleFileTransferChunkList_Release(&d);
    ccnxSimpleFileTransferChunkList_Release(&e);
}

LONGBOW_TEST_CASE(Global, setChunk)
{
    char *fileName = "filename.txt";
    CCNxSimpleFileTransferChunkList *chunkList = ccnxSimpleFileTransferChunkList_Create(fileName, 100);

    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CCNxContentObject *co = ccnxContentObject_CreateWithNameAndPayload(name, NULL);

    for (int i = 0; i < 10; i++) {
        ccnxSimpleFileTransferChunkList_SetChunk(chunkList, i, co);
    }

    // And set one on a previously used slot, too, to test that that works.
    ccnxSimpleFileTransferChunkList_SetChunk(chunkList, 2, co);

    ccnxName_Release(&name);
    ccnxContentObject_Release(&co);
    ccnxSimpleFileTransferChunkList_Release(&chunkList);
}

LONGBOW_TEST_CASE(Global, getChunk)
{
    char *fileName = "filename.txt";
    CCNxSimpleFileTransferChunkList *chunkList = ccnxSimpleFileTransferChunkList_Create(fileName, 100);

    CCNxName *name = ccnxName_CreateFromCString("lci:/boose/roo/pie");
    CCNxContentObject *co = ccnxContentObject_CreateWithNameAndPayload(name, NULL);

    assertTrue(NULL == ccnxSimpleFileTransferChunkList_GetChunk(chunkList, 0), "Expected NULL content");

    for (int i = 0; i < 10; i++) {
        ccnxSimpleFileTransferChunkList_SetChunk(chunkList, i, co);
    }

    for (int i = 0; i < 10; i++) {
        assertTrue(co == ccnxSimpleFileTransferChunkList_GetChunk(chunkList, i), "Expected the same content back");
    }

    assertTrue(NULL == ccnxSimpleFileTransferChunkList_GetChunk(chunkList, 11), "Expected NULL content");

    ccnxName_Release(&name);
    ccnxContentObject_Release(&co);
    ccnxSimpleFileTransferChunkList_Release(&chunkList);
}

LONGBOW_TEST_CASE(Global, hashCode)
{
    CCNxSimpleFileTransferChunkList *chunkList1 = ccnxSimpleFileTransferChunkList_Create("filename1", 100);
    CCNxSimpleFileTransferChunkList *chunkList2 = ccnxSimpleFileTransferChunkList_Create("filename1", 100);
    CCNxSimpleFileTransferChunkList *chunkList3 = ccnxSimpleFileTransferChunkList_Create("differentName", 100);

    assertTrue(ccnxSimpleFileTransferChunkList_HashCode(chunkList1) != 0, "Expected non-zero hash code");
    assertTrue(ccnxSimpleFileTransferChunkList_HashCode(chunkList1) ==
               ccnxSimpleFileTransferChunkList_HashCode(chunkList2), "Expected the same hash code");

    chunkList2->chunkSize = 10; // change the chunk size in chunkList 2;
    assertFalse(ccnxSimpleFileTransferChunkList_HashCode(chunkList1) ==
                ccnxSimpleFileTransferChunkList_HashCode(chunkList2), "Expected different hash codes");

    // Should be different with different name, too.
    assertFalse(ccnxSimpleFileTransferChunkList_HashCode(chunkList1) ==
                ccnxSimpleFileTransferChunkList_HashCode(chunkList3), "Expected different hash codes");

    ccnxSimpleFileTransferChunkList_Release(&chunkList1);
    ccnxSimpleFileTransferChunkList_Release(&chunkList2);
    ccnxSimpleFileTransferChunkList_Release(&chunkList3);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(ccnxSimpleFileTransfer_ChunkList);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
