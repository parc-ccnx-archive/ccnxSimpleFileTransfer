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

#include "ccnxSimpleFileTransfer_Common.h"

#include <LongBow/runtime.h>

#include <ccnx/common/ccnx_NameSegmentNumber.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>
#include <parc/security/parc_IdentityFile.h>

/**
 * The name of this tutorial. This is what shows when you run the client or server with '-h'
 */
const char *ccnxSimpleFileTransferCommon_TutorialName = "CCNx 1.0 Simple File Transfer Tutorial";

/**
 * The CCNx Name prefix we'll use for the tutorial.
 */
const char *ccnxSimpleFileTransferCommon_DomainPrefix = "lci:/ccnx/tutorial";

/**
 * The size of a chunk. We break CCNx Content payloads up into pieces of this size.
 * 1200 was chosen as a size that should prevent IP fragmentation of CCNx ContentObject Messages.
 */
const uint32_t ccnxSimpleFileTransferCommon_ChunkSize = 1200;

/**
 * The string we use for the 'fetch' command.
 */
const char *ccnxSimpleFileTransferCommon_CommandFetch = "fetch";

/**
 * The string we use for the 'list' command.
 */
const char *ccnxSimpleFileTransferCommon_CommandList = "list";

PARCIdentity *
ccnxSimpleFileTransferCommon_CreateAndGetIdentity(const char *keystoreName,
                                                  const char *keystorePassword,
                                                  const char *subjectName)
{
    parcSecurity_Init();

    unsigned int keyLength = 1024;
    unsigned int validityDays = 30;

    bool success = parcPublicKeySignerPkcs12Store_CreateFile(keystoreName, keystorePassword, subjectName, keyLength, validityDays);
    assertTrue(success,
               "parcPublicKeySignerPkcs12Store_CreateFile('%s', '%s', '%s', %d, %d) failed.",
               keystoreName, keystorePassword, subjectName, keyLength, validityDays);

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentity *result = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    parcIdentityFile_Release(&identityFile);

    parcSecurity_Fini();

    return result;
}

CCNxPortalFactory *
ccnxSimpleFileTransferCommon_SetupPortalFactory(const char *keystoreName,
                                                const char *keystorePassword,
                                                const char *subjectName)
{
    PARCIdentity *identity = ccnxSimpleFileTransferCommon_CreateAndGetIdentity(keystoreName,
                                                                               keystorePassword,
                                                                               subjectName);
    CCNxPortalFactory *result = ccnxPortalFactory_Create(identity);
    parcIdentity_Release(&identity);

    return result;
}

uint64_t
ccnxSimpleFileTransferCommon_GetChunkNumberFromName(const CCNxName *name)
{
    size_t numberOfSegmentsInName = ccnxName_GetSegmentCount(name);
    CCNxNameSegment *chunkNumberSegment = ccnxName_GetSegment(name, numberOfSegmentsInName - 1);

    assertTrue(ccnxNameSegment_GetType(chunkNumberSegment) == CCNxNameLabelType_CHUNK,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_CHUNK,
               ccnxNameSegment_GetType(chunkNumberSegment))
    {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegmentNumber_Value(chunkNumberSegment);
}

char *
ccnxSimpleFileTransferCommon_CreateFileNameFromName(const CCNxName *name)
{
    // For the Tutorial, the second to last NameSegment is the filename.
    CCNxNameSegment *fileNameSegment = ccnxName_GetSegment(name,
                                                           ccnxName_GetSegmentCount(name) - 2); // '-2' because we want the second to last segment

    assertTrue(ccnxNameSegment_GetType(fileNameSegment) == CCNxNameLabelType_NAME,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_NAME,
               ccnxNameSegment_GetType(fileNameSegment))
    {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegment_ToString(fileNameSegment); // This memory must be freed by the caller.
}

char *
ccnxSimpleFileTransferCommon_CreateCommandStringFromName(const CCNxName *name, const CCNxName *domainPrefix)
{
    // For the Tutorial, the NameSegment immediately following the domain prefix contains the command.
    CCNxNameSegment *commandSegment = ccnxName_GetSegment(name, ccnxName_GetSegmentCount(domainPrefix));

    assertTrue(ccnxNameSegment_GetType(commandSegment) == CCNxNameLabelType_NAME,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_NAME,
               ccnxNameSegment_GetType(commandSegment))
    {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegment_ToString(commandSegment); // This memory must be freed by the caller.
}

int
ccnxSimpleFileTransferCommon_ProcessCommandLineArguments(int argc, char **argv,
                                                         int *commandArgCount, char **commandArgs,
                                                         bool *needToShowUsage, bool *shouldExit)
{
    int status = EXIT_SUCCESS;
    *commandArgCount = 0;
    *needToShowUsage = false;

    for (size_t i = 1; i < argc; i++) {
        char *arg = argv[i];
        if (arg[0] == '-') {
            switch (arg[1]) {
                case 'h': {
                    *needToShowUsage = true;
                    *shouldExit = true;
                    break;
                }
                default: { // Unexpected '-' option.
                    *needToShowUsage = true;
                    *shouldExit = true;
                    status = EXIT_FAILURE;
                    break;
                }
            }
        } else {
            // Not a '-' option, so save it as a command argument.
            commandArgs[(*commandArgCount)++] = arg;
        }
    }
    return status;
}
