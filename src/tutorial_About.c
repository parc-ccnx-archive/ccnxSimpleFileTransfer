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
#include "tutorial_About.h"

const char *tutorial_What = "@(#)" PACKAGE_NAME " " RELEASE_VERSION "\n"
							"@(#)\tCopyright 2014-2015 Palo Alto Research Center (PARC), a Xerox company.\n@(#)\tAll Rights Reserved. Use is subject to license terms.";

const char *
tutorialAbout_Name(void)
{
    return PACKAGE_NAME;
}

const char *
tutorialAbout_Version(void)
{
    return RELEASE_VERSION;
}

const char *
tutorialAbout_About(void)
{
    return PACKAGE_NAME " " RELEASE_VERSION "\nCopyright 2014-2015 Palo Alto Research Center (PARC), a Xerox company.\nAll Rights Reserved. Use is subject to license terms.\n";
}

const char *
tutorialAbout_MiniNotice(void)
{
    return "Copyright 2014-2015 Palo Alto Research Center (PARC), a Xerox company.\nAll Rights Reserved. Use is subject to license terms.\n";
}

const char *
tutorialAbout_ShortNotice(void)
{
    return "Copyright 2014-2015 Palo Alto Research Center (PARC), a Xerox company.\nAll Rights Reserved. Use is subject to license terms.\n\nPARC and the PARC Logo are trademarks or registered trademarks of\nPalo Alto Research Center Incorporated, in the U.S. and other countries.\n\nU.S. Government Restricted Rights: Use or reproduction of this Software and\naccompanying documentation are subject to restrictions set forth \nin the Federal Acquisition Regulation and its supplements.\n";
}

const char *
tutorialAbout_LongNotice(void)
{
    return "Copyright 2014-2015 Palo Alto Research Center (PARC), a Xerox company.\n3333 Coyote Hill Road, Palo Alto, CA 94304 USA\nAll Rights Reserved. Use is subject to license terms.\n\nPARC and the PARC Logo are trademarks or registered trademarks of\nPalo Alto Research Center Incorporated, in the U.S. and other countries.\n\nU.S. Government Restricted Rights: Use or reproduction of this Software and\naccompanying documentation are subject to restrictions set forth \nin the Federal Acquisition Regulation and its supplements.\n\nThis Software may be covered and controlled by U.S. Export Control laws and\nmay be subject to the export or import laws in other countries. \nExport or reexport to countries subject to U.S. embargo or to entities identified on\nU.S. export exclusion lists, including, but not limited to, the denied persons\nand specially designated nationals lists is strictly prohibited.\n";
}

