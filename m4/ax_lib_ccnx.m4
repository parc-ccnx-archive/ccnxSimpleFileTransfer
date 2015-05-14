# SYNOPSIS
#
#   AX_LIB_CCNX([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   This macro searches for the LIBCCNX library in the user specified location.
#   The user may specify the location either by defining the environment
#   variable LIBCCNX or by using the --with-libccnx option to configure. If the
#   environment variable is defined it has precedent over everything else. If
#   no location was specified then it searches in /usr/lib and /usr/local/lib
#   and /usr/local/ccnx/lib for the library and in /usr/include and
#   /usr/local/include and /usr/local/ccnx/include for the header files. Upon
#   sucessful completion the variables LIBCCNX_LIB and LIBCCNX_INCLUDE are set.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a LIBCCNX library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will define
#   HAVE_LIBCCNX. If ACTION-IF-NOT-FOUND is not specified then an error will be
#   generated halting configure.
#
#   This file is based on an autoconf module by Ben Bergen.
#
# LICENSE
#
#   Copyright (c) 2015 Ignacio Solis <Ignacio.Solis@ccnx_common.com>
#   Copyright (c) 2008 Ben Bergen <ben@cs.fau.de>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice and
#   this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 1

AC_DEFUN([AX_LIB_CCNX], [
	AC_MSG_CHECKING(for Libccnx library)
	AC_REQUIRE([AC_PROG_CC])
	#
	# User hints...
	#
	AC_ARG_VAR([LIBCCNX], [LIBCCNX library location])
	AC_ARG_WITH([libccnx],
		[AS_HELP_STRING([--with-libccnx],
		[user defined path to LIBCCNX library])],
		[
			if test -n "$LIBCCNX" ; then
				AC_MSG_RESULT(yes)
				with_libccnx=$LIBCCNX
			elif test "$withval" != no ; then
				AC_MSG_RESULT(yes)
				with_libccnx=$withval
			else
				AC_MSG_RESULT(no)
			fi
		],
		[
			if test -n "$LIBCCNX" ; then
				with_libccnx=$LIBCCNX
				AC_MSG_RESULT(yes)
			else
				with_libccnx=/usr
				if test ! -f "$with_libccnx/include/ccnx/common/libccnxCommon_About.h" ; then
					with_libccnx=/usr/local
					if test ! -f "$with_libccnx/include/ccnx/common/libccnxCommon_About.h" ; then
					  with_libccnx=/usr/local/ccnx
					  if test ! -f "$with_libccnx/include/ccnx/common/libccnxCommon_About.h" ; then
						  with_libccnx=""
						  AC_MSG_RESULT(failed)
					  else
						AC_MSG_RESULT($with_libccnx)
					  fi
				    else
					  AC_MSG_RESULT($with_libccnx)
					fi
				else
					AC_MSG_RESULT($with_libccnx)
				fi
			fi
		])
	#
	# locate LIBCCNX library
	#
		if test -n "$with_libccnx" ; then
			old_CFLAGS=$CFLAGS
			old_LDFLAGS=$LDFLAGS
			CFLAGS="-I$with_libccnx/include $CFLAGS"
			LDFLAGS="-L$with_libccnx/lib $LDFLAGS"

			AC_LANG_SAVE
			AC_LANG_C

			AC_CHECK_LIB(ccnx_common, libccnxCommonAbout_Version,
				[libccnx_lib=yes], [libccnx_lib=no], 
                [$LIBPARC_LIB $LONGBOW_LIB -lparc -llongbow -llongbow-textplain -lcrypto -lm])
			AC_CHECK_HEADER(ccnx/common/libccnxCommon_About.h, [libccnx_h=yes],
				[libccnx_h=no], [/* check */])

			AC_LANG_RESTORE

			CFLAGS=$old_CFLAGS
			LDFLAGS=$old_LDFLAGS

			AC_MSG_CHECKING(if  Libccnx_common in $with_libccnx works)
			if test "$libccnx_lib" = "yes" -a "$libccnx_h" = "yes" ; then
				AC_SUBST(LIBCCNX_INCLUDE, [-I$with_libccnx/include])
				AC_SUBST(LIBCCNX_LIB, [-L$with_libccnx/lib])
				AC_SUBST(LIBCCNX_LIB_PATH, [$with_libccnx/lib])
				AC_MSG_RESULT(yes)
			else
				AC_MSG_RESULT(failed)
			fi
		fi
		#
		#
		#
		if test x = x"$LIBCCNX_LIB" ; then
			ifelse([$2],,[AC_MSG_ERROR(Failed to find valid LIBCCNX library)],[$2])
			:
		else
			ifelse([$1],,[AC_DEFINE(HAVE_LIBCCNX,1,[Define if you have LIBCCNX library])],[$1])
			:
		fi
	])dnl AX_LIB_LIBCCNX

