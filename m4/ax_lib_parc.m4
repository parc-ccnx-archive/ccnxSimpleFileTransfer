# SYNOPSIS
#
#   AX_LIB_PARC([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   This macro searches for the LIBPARC library in the user specified location.
#   The user may specify the location either by defining the environment
#   variable LIBPARC or by using the --with-libparc option to configure. If the
#   environment variable is defined it has precedent over everything else. If
#   no location was specified then it searches in /usr/lib and /usr/local/lib
#   and /usr/local/parc/lib for the library and in /usr/include and
#   /usr/local/include and /usr/local/parc/include for the header files. Upon
#   sucessful completion the variables LIBPARC_LIB and LIBPARC_INCLUDE are set.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a LIBPARC library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will define
#   HAVE_LIBPARC. If ACTION-IF-NOT-FOUND is not specified then an error will be
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

AC_DEFUN([AX_LIB_PARC], [
	AC_MSG_CHECKING(for Libparc library)
	AC_REQUIRE([AC_PROG_CC])
	#
	# User hints...
	#
	AC_ARG_VAR([LIBPARC], [LIBPARC library location])
	AC_ARG_WITH([libparc],
		[AS_HELP_STRING([--with-libparc],
		[user defined path to LIBPARC library])],
		[
			if test -n "$LIBPARC" ; then
				AC_MSG_RESULT(yes)
				with_libparc=$LIBPARC
			elif test "$withval" != no ; then
				AC_MSG_RESULT(yes)
				with_libparc=$withval
			else
				AC_MSG_RESULT(no)
			fi
		],
		[
			if test -n "$LIBPARC" ; then
				with_libparc=$LIBPARC
				AC_MSG_RESULT(yes)
			else
				with_libparc=/usr
				if test ! -f "$with_libparc/include/parc/libparc_About.h" ; then
					with_libparc=/usr/local
					if test ! -f "$with_libparc/include/parc/libparc_About.h" ; then
					  with_libparc=/usr/local/parc
					  if test ! -f "$with_libparc/include/parc/libparc_About.h" ; then
						  with_libparc=""
						  AC_MSG_RESULT(failed)
					  else
						AC_MSG_RESULT($with_libparc)
					  fi
				    else
					  AC_MSG_RESULT($with_libparc)
					fi
				else
					AC_MSG_RESULT($with_libparc)
				fi
			fi
		])
	#
	# locate LIBPARC library
	#
		if test -n "$with_libparc" ; then
			old_CFLAGS=$CFLAGS
			old_LDFLAGS=$LDFLAGS
			CFLAGS="-I$with_libparc/include $CFLAGS"
			LDFLAGS="-L$with_libparc/lib $LDFLAGS"

			AC_LANG_SAVE
			AC_LANG_C

			AC_CHECK_LIB(parc, libparcAbout_Version,
				[parc_lib=yes], [parc_lib=no], 
                [$LONGBOW_LIB -llongbow -llongbow-textplain -lcrypto -lm])
			AC_CHECK_HEADER(parc/libparc_About.h, 
                [parc_h=yes], [parc_h=no], [/* check */])

			AC_LANG_RESTORE

			CFLAGS=$old_CFLAGS
			LDFLAGS=$old_LDFLAGS

			AC_MSG_CHECKING(if  Libparc in $with_libparc works)
			if test "$parc_lib" = "yes" -a "$parc_h" = "yes" ; then
				AC_SUBST(LIBPARC_INCLUDE, [-I$with_libparc/include])
				AC_SUBST(LIBPARC_LIB, [-L$with_libparc/lib])
				AC_SUBST(LIBPARC_LIB_PATH, [$with_libparc/lib])
				AC_MSG_RESULT(yes)
			else
				AC_MSG_RESULT(failed)
			fi
		fi
		#
		#
		#
		if test x = x"$LIBPARC_LIB" ; then
			ifelse([$2],,[AC_MSG_ERROR(Failed to find valid LIBPARC library)],[$2])
			:
		else
			ifelse([$1],,[AC_DEFINE(HAVE_LIBPARC,1,[Define if you have LIBPARC library])],[$1])
			:
		fi
	])dnl AX_LIB_LIBPARC

