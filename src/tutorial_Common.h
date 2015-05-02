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

#ifndef tutorial_Common_h
#define tutorial_Common_h

#include <stdint.h>

#include <parc/security/parc_Identity.h>

#include <ccnx/common/ccnx_Name.h>

#include <ccnx/api/ccnx_Portal/ccnx_Portal.h>

/**
 * See tutorial_Common.h for the initilization of these constants.
 */

/**
 * The CCNx Name prefix we'll use for the tutorial.
 */
extern const char *tutorialCommon_DomainPrefix;

/**
 * The size of a chunk. We break CCNx Content payloads up into pieces of this size.
 * 1200 was chosen as a size that should prevent IP fragmentation of CCNx ContentObject Messages.
 */
extern const uint32_t tutorialCommon_ChunkSize;

/**
 * The string we use for the 'fetch' command.
 */
extern const char *tutorialCommon_CommandFetch;

/**
 * The string we use for the 'list' command.
 */
extern const char *tutorialCommon_CommandList;


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
PARCIdentity *tutorialCommon_CreateAndGetIdentity(const char *keystoreName, const char *keystorePassword, const char *subjectName);

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
CCNxPortalFactory *tutorialCommon_SetupPortalFactory(const char *keystoreName, const char *keystorePassword, const char *subjectName);

/**
 * Given a CCNxName instance, return the numeric value of the chunk specified by the Name.
 * The chunk number is contained in the final NameSegment of the Name.
 *
 * @param [in] name A CCNxName instance from which to extract the chunk number.
 * @return The chunk number encoded in the supplied CCNxName instance.
 */
uint64_t tutorialCommon_GetChunkNumberFromName(const CCNxName *name);

/**
 * Given a CCNxName instance, structured for this tutorial, return a string representation
 * of the file name in the CCNxName. For the tutorial, this is located in the second to
 * last CCnxNameSegment in the CCNxName. The string returned here must eventually be freed
 * by calling parcMemory_Deallocate().
 *
 * @param [in] name A CCNxName instance from which to extract the filename.
 * @return A C string representation of the filename encoded in the supplied CCNxName instance.
 */
char *tutorialCommon_CreateFileNameFromName(const CCNxName *name);

/**
 * Given a CCNxName instance, structured for this tutorial, return a string representation
 * of the command string (e.g. "fetch" or "list"). For the tutorial, this is located in the CCnxNameSegment
 * immediately following the domain prefix in the CCNxName. The string returned here must eventually be freed
 * by calling parcMemory_Deallocate().
 *
 * @param [in] name A CCNxName instance from which to extract the filename.
 * @return A C string representation of the filename encoded in the supplied CCNxName instance.
 */
char *tutorialCommon_CreateCommandStringFromName(const CCNxName *name, const CCNxName *domainPrefix);

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
int tutorialCommon_processCommandLineArguments(int argc, char **argv,
                                               int *commandArgCount, char **commandArgs,
                                               bool *needToShowUsage, bool *shouldExit);
#endif // tutorial_Common.h
