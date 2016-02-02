# SYNOPSIS
#
#   AX_RELEASE_VERSION([RELEASE_VERSION_BASE])
#
# DESCRIPTION
#
# Add a RELEASE_VERSION variable. It's based on the package version, the date
# and the git repository hash (if under git).
#
# LICENSE
#
#   Copyright (c) 2016 Ignacio Solis <Ignacio.Solis@parc.com>
#   Copyright (c) 2008 Ben Bergen <ben@cs.fau.de>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice and
#   this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 1

AC_DEFUN([AX_RELEASE_VERSION], [
	AC_MSG_CHECKING(for release version)
	ifelse([$1],,[RELEASE_VERSION_BASE="0.0"],[RELEASE_VERSION_BASE=$1])

    GIT=`which git`
    
    if test -x $GIT -a -f $srcdir/.git/config; then 
		GIT_VERSION=.`cd $srcdir && git rev-parse HEAD | awk '{print substr(\$"1",1,8)}' 2> /dev/null`
    fi

    DATE_VERSION=`date "+%Y%m%d"`

    RELEASE_VERSION=$RELEASE_VERSION_BASE.$DATE_VERSION$GIT_VERSION

	AC_MSG_RESULT($RELEASE_VERSION)
	AC_SUBST(RELEASE_VERSION)


	])dnl

