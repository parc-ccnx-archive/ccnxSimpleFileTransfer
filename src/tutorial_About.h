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

#ifndef tutorial_About_h
#define tutorial_About_h
/**
 * Embedded string containing information for the what(1) command.
 *
 */
extern const char *tutorial_What;

/**
 * Return the name as a C string.
 *
 * @return The name as a C string.
 */
const char *tutorialAbout_Name(void);

/**
 * Return the version as a C string.
 *
 * @return The version as a C string.
 */
const char *tutorialAbout_Version(void);

/**
 * Return the About text as a C string.
 *
 * @return The About text as a C string.
 */
const char *tutorialAbout_About(void);

/**
 * Return the minimum copyright notice as a C string.
 *
 * @return The minimum copyright notice as a C string.
 */
const char *tutorialAbout_MiniNotice(void);

/**
 * Return the short copyright notice as a C string.
 *
 * @return The short copyright notice as a C string.
 */
const char *tutorialAbout_ShortNotice(void);

/**
 * Return the long copyright notice as a C string.
 *
 * @return The long copyright notice as a C string.
 */
const char *tutorialAbout_LongNotice(void);

#endif // tutorial_about_h
