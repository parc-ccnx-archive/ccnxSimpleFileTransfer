# SYNOPSIS
#
#   AX_LIB_LONGBOW([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   This macro searches for the LONGBOW library in the user specified location.
#   The user may specify the location either by defining the environment
#   variable LONGBOW or by using the --with-longbow option to configure. If the
#   environment variable is defined it has precedent over everything else. If
#   no location was specified then it searches in /usr/lib and /usr/local/lib
#   and /usr/local/parc/lib for the library and in /usr/include and
#   /usr/local/include and /usr/local/parc/include for the header files. Upon
#   sucessful completion the variables LONGBOW_LIB and LONGBOW_INCLUDE are set.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a LONGBOW library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will define
#   HAVE_LONGBOW. If ACTION-IF-NOT-FOUND is not specified then an error will be
#   generated halting configure.
#
#   This file is based on an autoconf module by Ben Bergen.
#
# LICENSE
#
#   Copyright (c) 2015 Ignacio Solis <Ignacio.Solis@parc.com>
#   Copyright (c) 2008 Ben Bergen <ben@cs.fau.de>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice and
#   this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 1

AC_DEFUN([AX_LIB_LONGBOW], [
	AC_MSG_CHECKING(for LongBow library)
	AC_REQUIRE([AC_PROG_CC])
	#
	# User hints...
	#
	AC_ARG_VAR([LONGBOW], [LONGBOW library location])
	AC_ARG_WITH([longbow],
		[AS_HELP_STRING([--with-longbow[=DIR]],
		[user defined path to LONGBOW library])],
		[
			if test -n "$LONGBOW" ; then
				AC_MSG_RESULT(yes)
				with_longbow=$LONGBOW
			elif test "$withval" != no ; then
				AC_MSG_RESULT(yes)
				with_longbow=$withval
			else
				AC_MSG_RESULT(no)
			fi
		],
		[
			if test -n "$LONGBOW" ; then
				with_longbow=$LONGBOW
				AC_MSG_RESULT(yes)
			else
				with_longbow=/usr
				if test ! -f "$with_longbow/include/LongBow/longBow_About.h" ; then
					with_longbow=/usr/local
					if test ! -f "$with_longbow/include/LongBow/longBow_About.h" ; then
					  with_longbow=/usr/local/parc
					  if test ! -f "$with_longbow/include/LongBow/longBow_About.h" ; then
						  with_longbow=""
						  AC_MSG_RESULT(failed)
					  else
						AC_MSG_RESULT($with_longbow)
					  fi
				    else
					  AC_MSG_RESULT($with_longbow)
					fi
				else
					AC_MSG_RESULT($with_longbow)
				fi
			fi
		])
	#
	# locate LONGBOW library
	#
		if test -n "$with_longbow" ; then
			old_CFLAGS=$CFLAGS
			old_LDFLAGS=$LDFLAGS
			CFLAGS="-I$with_longbow/include $CFLAGS"
			LDFLAGS="-L$with_longbow/lib $LDFLAGS"

			AC_LANG_SAVE
			AC_LANG_C

			AC_CHECK_LIB(longbow, longBowAbout_Version,
				[longbow_lib=yes], [longbow_lib=no], [-llongbow-textplain])
			AC_CHECK_HEADER(LongBow/longBow_About.h, [longbow_h=yes],
				[longbow_h=no], [/* check */])

			AC_LANG_RESTORE

			CFLAGS=$old_CFLAGS
			LDFLAGS=$old_LDFLAGS

			AC_MSG_CHECKING(if  Longbow in $with_longbow works)
			if test "$longbow_lib" = "yes" -a "$longbow_h" = "yes" ; then
				AC_SUBST(LONGBOW_INCLUDE, [-I$with_longbow/include])
				AC_SUBST(LONGBOW_LIB, [-L$with_longbow/lib])
				AC_SUBST(LONGBOW_LIB_PATH, [$with_longbow/lib])
				AC_MSG_RESULT(yes)
			else
				AC_MSG_RESULT(failed)
			fi
		fi
		#
		#
		#
		if test x = x"$LONGBOW_LIB" ; then
			ifelse([$2],,[AC_MSG_ERROR(Failed to find valid LONGBOW library)],[$2])
			:
		else
			ifelse([$1],,[AC_DEFINE(HAVE_LONGBOW,1,[Define if you have LONGBOW library])],[$1])
			:
		fi
	])dnl AX_LIB_LONGBOW

