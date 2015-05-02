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
#include <config.h>

#include "tutorial_Common.h"
#include "tutorial_About.h"

#include <LongBow/runtime.h>

#include <ccnx/common/ccnx_NameSegmentNumber.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>
#include <parc/security/parc_IdentityFile.h>

/**
 * The CCNx Name prefix we'll use for the tutorial.
 */
const char *tutorialCommon_DomainPrefix = "lci:/ccnx/tutorial";

/**
 * The size of a chunk. We break CCNx Content payloads up into pieces of this size.
 * 1200 was chosen as a size that should prevent IP fragmentation of CCNx ContentObject Messages.
 */
const uint32_t tutorialCommon_ChunkSize = 1200;

/**
 * The string we use for the 'fetch' command.
 */
const char *tutorialCommon_CommandFetch = "fetch";

/**
 * The string we use for the 'list' command.
 */
const char *tutorialCommon_CommandList = "list";

PARCIdentity *
tutorialCommon_CreateAndGetIdentity(const char *keystoreName, const char *keystorePassword, const char *subjectName)
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
tutorialCommon_SetupPortalFactory(const char *keystoreName, const char *keystorePassword, const char *subjectName)
{
    PARCIdentity *identity = tutorialCommon_CreateAndGetIdentity(keystoreName, keystorePassword, subjectName);
    CCNxPortalFactory *result = ccnxPortalFactory_Create(identity);
    parcIdentity_Release(&identity);

    return result;
}

uint64_t
tutorialCommon_GetChunkNumberFromName(const CCNxName *name)
{
    size_t numberOfSegmentsInName = ccnxName_GetSegmentCount(name);
    CCNxNameSegment *chunkNumberSegment = ccnxName_GetSegment(name, numberOfSegmentsInName - 1);

    assertTrue(ccnxNameSegment_GetType(chunkNumberSegment) == CCNxNameLabelType_CHUNK,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_CHUNK,
               ccnxNameSegment_GetType(chunkNumberSegment)) {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegmentNumber_Value(chunkNumberSegment);
}

char *
tutorialCommon_CreateFileNameFromName(const CCNxName *name)
{
    // For the Tutorial, the second to last NameSegment is the filename.
    CCNxNameSegment *fileNameSegment = ccnxName_GetSegment(name, ccnxName_GetSegmentCount(name) - 2); // '-2' because we want the second to last segment

    assertTrue(ccnxNameSegment_GetType(fileNameSegment) == CCNxNameLabelType_NAME,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_NAME,
               ccnxNameSegment_GetType(fileNameSegment)) {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegment_ToString(fileNameSegment); // This memory must be freed by the caller.
}

char *
tutorialCommon_CreateCommandStringFromName(const CCNxName *name, const CCNxName *domainPrefix)
{
    // For the Tutorial, the NameSegment immediately following the domain prefix contains the command.
    CCNxNameSegment *commandSegment = ccnxName_GetSegment(name, ccnxName_GetSegmentCount(domainPrefix));

    assertTrue(ccnxNameSegment_GetType(commandSegment) == CCNxNameLabelType_NAME,
               "Last segment is the wrong type, expected CCNxNameLabelType %02X got %02X",
               CCNxNameLabelType_NAME,
               ccnxNameSegment_GetType(commandSegment)) {
        ccnxName_Display(name, 0); // This executes only if the enclosing assertion fails
    }

    return ccnxNameSegment_ToString(commandSegment); // This memory must be freed by the caller.
}

int
tutorialCommon_processCommandLineArguments(int argc, char **argv,
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
                case 'v': {
                    printf("%s version: %s\n", argv[0], tutorialAbout_Version());
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
