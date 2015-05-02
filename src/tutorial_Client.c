/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 * Copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), a Xerox company.  All Rights Reserved.
 * The content of this file, whole or in part, is subject to licensing terms.
 * If distributing this software, include this License Header Notice in each
 * file and provide the accompanying LICENSE file. 
 */
/**
 * @author Glenn Scott, Alan Walendowski, Computing Science Laboratory, PARC
 * @copyright 2014-2015 Palo Alto Research Center, Inc. (PARC), A Xerox Company. All Rights Reserved.
 */
#include <config.h>
#include <stdio.h>
#include <strings.h>

#include "tutorial_Common.h"
#include "tutorial_FileIO.h"
#include "tutorial_About.h"

#include <LongBow/runtime.h>

#include <ccnx/api/ccnx_Portal/ccnx_Portal.h>
#include <ccnx/api/ccnx_Portal/ccnx_PortalRTA.h>

#include <ccnx/common/ccnx_ContentObject.h>
#include <ccnx/common/ccnx_Interest.h>
#include <ccnx/common/ccnx_Name.h>

#include <parc/algol/parc_Memory.h>

#include <parc/security/parc_Security.h>
#include <parc/security/parc_IdentityFile.h>
#include <parc/security/parc_PublicKeySignerPkcs12Store.h>

/**
 * Create a new CCNxPortalFactory instance using a randomly generated identity saved to
 * the specified keystore.
 *
 * @return A new CCNxPortalFactory instance which must eventually be released by calling ccnxPortalFactory_Release().
 */
static CCNxPortalFactory *
_setupConsumerPortalFactory(void)
{
    const char *keystoreName = "tutorialClient_keystore";
    const char *keystorePassword = "keystore_password";
    const char *subjectName = "tutorialClient";

    return tutorialCommon_SetupPortalFactory(keystoreName, keystorePassword, subjectName);
}

/**
 * Given a sequential chunk of a 'list' response, append it to the in-memory buffer
 * that holds the listing. When the directory listing is complete, return it as a
 * string. The string must be freed by the caller.
 *
 * @param [in] payload A PARCBuffer containing the chunk of the directory listing to be appended.
 * @param [in] chunkNumber The number of the chunk that this payload belongs to.
 * @param [in] finalChunkNumber The number of the final chunk in this list response.
 *
 * @return A string containing the complete directory listing, or NULL if the complete directory
 *         listing hasn't yet been received.
 */
static char *
_assembleDirectoryListing(PARCBuffer *payload, uint64_t chunkNumber, uint64_t finalChunkNumber)
{
    char *result = NULL;
    static PARCElasticBuffer *directoryList = NULL;

    if (directoryList == NULL) {
        directoryList = parcElasticBuffer_Create();
    }

    if (chunkNumber == 0) {
        parcElasticBuffer_Clear(directoryList);
    }

    parcElasticBuffer_PutBuffer(directoryList, payload);

    if (chunkNumber == finalChunkNumber) {
        parcElasticBuffer_Flip(directoryList);

        // Since this was the last chunk, return the completed directory listing.
        result = parcElasticBuffer_ToString(directoryList);
        parcElasticBuffer_Release(&directoryList);
    }

    return result;
}

/**
 * Given a sequential chunk of a 'fetch' response, append it to the file that we are assembling.
 * Return true if the final chunk has been written, false otherwise.
 *
 * @param [in] fileName The full name of the file to write the payload in to.
 * @param [in] payload A PARCBuffer containing the chunk of the file to write.
 * @param [in] chunkNumber The number of the chunk to be written.
 * @param [in] finalChunkNumber The number of the final chunk in the file.
 *
 * @return true if the entire file has been written, false otherwise.
 */
static bool
_assembleFile(const char *fileName, const PARCBuffer *payload, uint64_t chunkNumber, uint64_t finalChunkNumber)
{
    if (chunkNumber == 0) {
        // If we're the first chunk (chunk #0), then make sure we're starting with an empty file.
        tutorialFileIO_DeleteFile(fileName);
    }

    // Note that the tutorialFileIO_AppendFileChunk() function should be replaced with something that keeps
    // an open file pointer instead of repeatedly re-opening it. This method simply opens (possibly creating)
    // the file and appends the specified payload). It is not an efficient implementation.
    tutorialFileIO_AppendFileChunk(fileName, payload);

    return (chunkNumber == finalChunkNumber); // true, if we just wrote the final chunk
}

/**
 * Receive a chunk of a directory listing and add it to the directory listing that we're
 * building. When it's complete, print it and return true. We assume the chunks arrive in the
 * correct order.
 *
 * @param [in] payload A PARCBuffer containing the chunk of the directory listing to write.
 * @param [in] chunkNumber The number of the chunk to be written.
 * @param [in] finalChunkNumber The number of the final chunk in the directory listing.
 *
 * @return true if the entire listing has been received, false otherwise.
 */
static bool
_receiveDirectoryListingChunk(PARCBuffer *payload, uint64_t chunkNumber, uint64_t finalChunkNumber)
{
    bool result = false;
    char *directoryList = _assembleDirectoryListing(payload, chunkNumber, finalChunkNumber);

    // When the directory listing is complete, dirListing will be non-NULL.
    if (directoryList != NULL) {
        printf("Directory Listing follows:\n");
        printf("%s", directoryList);
        parcMemory_Deallocate((void **) &directoryList);
        result = true;
    }
    return result;
}

/*
 * Receive a chunk of a file and append it to the local file of the specified name. When the file is
 * complete, print a message stating so and return true. Otherwise, print a message showing the
 * file transfer progress and return false. We assume the file chunks arrive in the correct order.
 *
 * @param [in] fileName The full path to the file to be received.
 * @param [in] payload A PARCBuffer containing the chunk of the file to write.
 * @param [in] chunkNumber The number of the chunk to be written.
 * @param [in] finalChunkNumber The number of the final chunk in the directory listing.
 *
 * @return true if the entire file has been written, false otherwise.
 */
static bool
_receiveFileChunk(const char *fileName, const PARCBuffer *payload, uint64_t chunkNumber, uint64_t finalChunkNumber)
{
    bool isComplete = _assembleFile(fileName, payload, chunkNumber, finalChunkNumber);

    if (isComplete) {
        printf("File '%s' has been fully transferred in %ld chunks.\n", fileName, (unsigned long) finalChunkNumber + 1L);
    } else {
        printf("File '%s' has been %04.2f%% transferred.\r", fileName,
               ((float) chunkNumber / (float) finalChunkNumber) * 100.0f);
        fflush(stdout);
    }

    return isComplete;
}

/**
 * Receive a ContentObject message that comes back from the tutorial_Server in response to an Interest we sent.
 * This message will be a chunk of the requested content, and should be received in ordered sequence.
 * Depending on the CCNxName in the content object, we hand it off to either _receiveFileChunk() or
 * _receiveDirectoryListingChunk() to process.
 *
 * @param [in] contentObject A CCNxContentObject containing a response to an CCNxInterest we sent.
 * @param [in] domainPrefix A CCNxName containing the domain prefix of the content we requested.
 *
 * @return The number of chunks of the content left to transfer.
 */
static uint64_t
_receiveContentObject(CCNxContentObject *contentObject, const CCNxName *domainPrefix)
{
    CCNxName *contentName = ccnxContentObject_GetName(contentObject);

    uint64_t chunkNumber = tutorialCommon_GetChunkNumberFromName(contentName);

    // Get the number of the final chunk, as specified by the sender.
    uint64_t finalChunkNumberSpecifiedByServer = ccnxContentObject_GetFinalChunkNumber(contentObject);

    // Get the type of the incoming message. Was it a response to a fetch' or a 'list' command?
    char *command = tutorialCommon_CreateCommandStringFromName(contentName, domainPrefix);

    // Process the payload.
    PARCBuffer *payload = ccnxContentObject_GetPayload(contentObject);

    if (strncasecmp(command, tutorialCommon_CommandList, strlen(command)) == 0) {
        // This is a chunk of the directory listing.
        _receiveDirectoryListingChunk(payload, chunkNumber, finalChunkNumberSpecifiedByServer);
    } else if (strncasecmp(command, tutorialCommon_CommandFetch, strlen(command)) == 0) {
        // This is a chunk of a file.
        char *fileName = tutorialCommon_CreateFileNameFromName(contentName);
        _receiveFileChunk(fileName, payload, chunkNumber, finalChunkNumberSpecifiedByServer);
        parcMemory_Deallocate((void **) &fileName);
    } else {
        printf("tutorial_Client: Unknown command: %s\n", command);
    }

    parcMemory_Deallocate((void **) &command);

    return (finalChunkNumberSpecifiedByServer - chunkNumber); // number of chunks left to transfer
}

/**
 * Create and return a CCNxInterest whose Name contains our commend (e.g. "fetch" or "list"),
 * and, optionally, the name of a target object (e.g. "file.txt"). The newly created CCNxInterest
 * must eventually be released by calling ccnxInterest_Release().
 *
 * @param command The command to embed in the created CCNxInterest.
 * @param targetName The name of the content, if any, that the command applies to.
 *
 * @return A newly created CCNxInterest for the specified command and targetName.
 */
static CCNxInterest *
_createInterest(const char *command, const char *targetName)
{
    CCNxName *interestName = ccnxName_CreateFromURI(tutorialCommon_DomainPrefix); // Start with the prefix. We append to this.

    // Create a NameSegment for our command, which we will append after the prefix we just created.
    PARCBuffer *commandBuffer = parcBuffer_WrapCString((char *) command);
    CCNxNameSegment *commandSegment = ccnxNameSegment_CreateTypeValue(CCNxNameLabelType_NAME, commandBuffer);
    parcBuffer_Release(&commandBuffer);

    // Append the new command segment to the prefix
    ccnxName_Append(interestName, commandSegment);
    ccnxNameSegment_Release(&commandSegment);

    // If we have a target, then create another NameSegment for it and append that.
    if (targetName != NULL) {
        // Create a NameSegment for our target object
        PARCBuffer *targetBuf = parcBuffer_WrapCString((char *) targetName);
        CCNxNameSegment *targetSegment = ccnxNameSegment_CreateTypeValue(CCNxNameLabelType_NAME, targetBuf);
        parcBuffer_Release(&targetBuf);
        

        // Append it to the ccnxName.
        ccnxName_Append(interestName, targetSegment);
        ccnxNameSegment_Release(&targetSegment);
    }

    CCNxInterest *result = ccnxInterest_CreateSimple(interestName);
    ccnxName_Release(&interestName);

    return result;
}

/**
 * Wait for a response to a previously issued Interest. This function reads from the specified Portal
 * until the requested content is fully received. It's not very clever, as it ignores all incoming
 * portal message types except those that are CCNxContentObjects.
 *
 * @param portal An instance of CCNxPortal to read from.
 *
 * @return true If the requested content has been fully received, false otherwise.
 */
static bool
_receiveResponseToIssuedInterest(CCNxPortal *portal, const CCNxName *domainPrefix)
{
    bool isTransferComplete = false;

    while (isTransferComplete == false && ccnxPortal_IsError(portal) == false) {
        CCNxMetaMessage *response = ccnxPortal_Receive(portal);

        if (response != NULL) {
            if (ccnxMetaMessage_IsContentObject(response)) {
                CCNxContentObject *contentObject = ccnxMetaMessage_GetContentObject(response);

                // Receive the content message. This returns the number of blocks remaining
                // in the transfer. If it returns 0, it was the final block of the content
                // and we're done.

                if (_receiveContentObject(contentObject, domainPrefix) == 0) {
                    isTransferComplete = true;
                }
            }
            ccnxMetaMessage_Release(&response);
        }
    }

    return isTransferComplete;
}

/**
 * Given a command (e.g "fetch") and an optional target name (e.g. "file.txt"), create an appropriate CCNxInterest
 * and write it to the Portal.
 *
 * @param command The command to be handled.
 * @param targetName The name of the target content, if any, that the command applies to.
 *
 * @return true If a CCNxInterest for the specified command and optional target was successfully issued and answered.
 */
static bool
_executeUserCommand(const char *command, const char *targetName)
{
    bool result = false;
    CCNxPortalFactory *factory = _setupConsumerPortalFactory();

    CCNxPortal *portal =
        ccnxPortalFactory_CreatePortal(factory, ccnxPortalRTA_Chunked, &ccnxPortalAttributes_Blocking);

    assertNotNull(portal, "Expected a non-null CCNxPortal pointer.");

    // Given the user's command and optional target, create an Interest.
    CCNxInterest *interest = _createInterest(command, targetName);

    // Send the Interest through the Portal, and wait for a response.
    CCNxMetaMessage *message = ccnxMetaMessage_CreateFromInterest(interest);
    if (ccnxPortal_Send(portal, message)) {
        CCNxName *domainPrefix = ccnxName_CreateFromURI(tutorialCommon_DomainPrefix);  // e.g. 'lci:/ccnx/tutorial'

        result = _receiveResponseToIssuedInterest(portal, domainPrefix);

        ccnxName_Release(&domainPrefix);
    }

    ccnxMetaMessage_Release(&message);
    ccnxInterest_Release(&interest);
    ccnxPortal_Release(&portal);
    ccnxPortalFactory_Release(&factory);

    return result;
}

/**
 * Display an explanation of arguments accepted by this program.
 *
 * @param [in] programName The name of this program.
 */
static void
_displayUsage(char *programName)
{
    printf("\n%s\n%s, %s\n\n", tutorialAbout_Version(), tutorialAbout_Name(), programName);

    printf(" This example application can retrieve a specified file or the list of available files from\n");
    printf(" the tutorialServer application, which should be running when this application is used. A CCNx\n");
    printf(" forwarder (e.g. Metis) must also be running.\n\n");

    printf("Usage: %s  [-h] [-v] [ list | fetch <filename> ]\n", programName);
    printf("  '%s list' will list the files in the directory served by tutorial_Server\n", programName);
    printf("  '%s fetch <filename>' will fetch the specified filename\n", programName);
    printf("  '%s -v' will show the tutorial demo code version\n", programName);
    printf("  '%s -h' will show this help\n\n", programName);
}

int
main(int argc, char *argv[argc])
{
    int status = EXIT_FAILURE;

    char *commandArgs[argc];
    int commandArgCount = 0;
    bool needToShowUsage = false;
    bool shouldExit = false;

    status = tutorialCommon_processCommandLineArguments(argc, argv, &commandArgCount, commandArgs, &needToShowUsage, &shouldExit);

    if (needToShowUsage) {
        _displayUsage(argv[0]);
    }

    if (shouldExit) {
        exit(status);
    }

    if (commandArgCount == 2
        && (strncmp(tutorialCommon_CommandFetch, commandArgs[0], strlen(commandArgs[0])) == 0)) {        // "fetch <filename>"
        status = _executeUserCommand(commandArgs[0], commandArgs[1]) ? EXIT_SUCCESS : EXIT_FAILURE;
    } else if (commandArgCount == 1
               && (strncmp(tutorialCommon_CommandList, commandArgs[0], strlen(commandArgs[0])) == 0)) {  // "list"
        status = _executeUserCommand(commandArgs[0], NULL) ? EXIT_SUCCESS : EXIT_FAILURE;
    } else {
        status = EXIT_FAILURE;
        _displayUsage(argv[0]);
    }

    exit(status);
}
