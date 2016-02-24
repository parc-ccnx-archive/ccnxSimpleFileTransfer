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

#ifndef simpleFileTransferTutorialCommon_h
#define simpleFileTransferTutorialCommon_h

#include <stdint.h>

#include <parc/security/parc_Identity.h>

#include <ccnx/common/ccnx_Name.h>

#include <ccnx/api/ccnx_Portal/ccnx_Portal.h>

/**
 * See simpleFileTransferTutorialCommon.h for the initilization of these constants.
 */

extern const char *simpleFileTransferTutorialCommon_TutorialName;

/**
 * The CCNx Name prefix we'll use for the tutorial.
 */
extern const char *simpleFileTransferTutorialCommon_DomainPrefix;

/**
 * The size of a chunk. We break CCNx Content payloads up into pieces of this size.
 * 1200 was chosen as a size that should prevent IP fragmentation of CCNx ContentObject Messages.
 */
extern const uint32_t simpleFileTransferTutorialCommon_ChunkSize;

/**
 * The string we use for the 'fetch' command.
 */
extern const char *simpleFileTransferTutorialCommon_CommandFetch;

/**
 * The string we use for the 'list' command.
 */
extern const char *simpleFileTransferTutorialCommon_CommandList;


/**
 * Creates and returns a new randomly generated Identity, which is required for signing.
 * In a real application, you would actually use a real Identity. The returned instance
 * must eventually be released by calling parcIdentity_Release().
 *
 * @param [in] keystoreName The name of the file to save the new identity.
 * @param [in] keystorePassword The password of the file holding the identity.
 * @param [in] subjectName The name of the owner of the identity.
 *
 *
 * @return A new, randomly generated, PARCIdentity instance.
 */
PARCIdentity *simpleFileTransferTutorialCommon_CreateAndGetIdentity(const char *keystoreName,
                                                                    const char *keystorePassword,
                                                                    const char *subjectName);

/**
 * Initialize and return a new instance of CCNxPortalFactory. A randomly generated identity is
 * used to initialize the factory. The returned instance must eventually be released by calling
 * ccnxPortalFactory_Release().
 *
 * @param [in] keystoreName The name of the file to save the new identity.
 * @param [in] keystorePassword The password of the file holding the identity.
 * @param [in] subjectName The name of the owner of the identity.
 *
 * @return A new instance of a CCNxPortalFactory initialized with a randomly created identity.
 */
CCNxPortalFactory *simpleFileTransferTutorialCommon_SetupPortalFactory(const char *keystoreName,
                                                                       const char *keystorePassword,
                                                                       const char *subjectName);

/**
 * Given a CCNxName instance, return the numeric value of the chunk specified by the Name.
 * The chunk number is contained in the final NameSegment of the Name.
 *
 * @param [in] name A CCNxName instance from which to extract the chunk number.
 * @return The chunk number encoded in the supplied CCNxName instance.
 */
uint64_t simpleFileTransferTutorialCommon_GetChunkNumberFromName(const CCNxName *name);

/**
 * Given a CCNxName instance, structured for this tutorial, return a string representation
 * of the file name in the CCNxName. For the tutorial, this is located in the second to
 * last CCnxNameSegment in the CCNxName. The string returned here must eventually be freed
 * by calling parcMemory_Deallocate().
 *
 * @param [in] name A CCNxName instance from which to extract the filename.
 * @return A C string representation of the filename encoded in the supplied CCNxName instance.
 */
char *simpleFileTransferTutorialCommon_CreateFileNameFromName(const CCNxName *name);

/**
 * Given a CCNxName instance, structured for this tutorial, return a string representation
 * of the command string (e.g. "fetch" or "list"). For the tutorial, this is located in the CCnxNameSegment
 * immediately following the domain prefix in the CCNxName. The string returned here must eventually be freed
 * by calling parcMemory_Deallocate().
 *
 * @param [in] name A CCNxName instance from which to extract the filename.
 * @return A C string representation of the filename encoded in the supplied CCNxName instance.
 */
char *simpleFileTransferTutorialCommon_CreateCommandStringFromName(const CCNxName *name, const CCNxName *domainPrefix);

/**
 * Process our command line arguments. If we're given '-h' or '-v', we handle them by displaying
 * the usage help or version, respectively. Unexpected will cause a return value of EXIT_FAILURE.
 * While processing the argument array, we also populate a list of pointers to non '-' arguments
 * and return those in the `commandArgs` parameter.
 *
 * @param [in] argc The count of command line arguments in `argv`.
 * @param [in] argv A pointer to the list of command line argument strings.
 * @param [out] commandArgCount A pointer to a int which will contain the number of non '-' arguments in `argv`.
 * @param [out] commandArgs A pointer to an array of pointers. The pointers will be set to the non '-' arguments
 *                          that were passed in in `argv`.
 * @param [out] needToShowUsage A pointer to a boolean that will be set to true if the caller should display the
 *                          usage of this application.
 * @param [out] shouldExit A pointer to a boolean that will be set to true if the caller should exit instead of
 *                         processing the commandArgs.
 *
 * @return EXIT_FAILURE if an unexpected '-' option was encountered. EXIT_SUCCESS otherwise.
 */
int simpleFileTransferTutorialCommon_ProcessCommandLineArguments(int argc, char **argv,
                                                                 int *commandArgCount, char **commandArgs,
                                                                 bool *needToShowUsage, bool *shouldExit);
#endif // simpleFileTransferTutorialCommon.h
