# SYNOPSIS
#
#   AX_LIB_EVENT([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   This macro searches for the LIBEVENT library in the user specified
#   location.  The user may specify the location either by defining the
#   environment variable LIBEVENT or by using the --with-libevent option to
#   configure. If the environment variable is defined it has precedent over
#   everything else. If no location was specified then it searches in /usr/lib
#   and /usr/local/lib for the library and in /usr/include and
#   /usr/local/include for the header files. Upon sucessful completion the
#   variables LIBEVENT_LIB and LIBEVENT_INCLUDE are set.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a LIBEVENT library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will define
#   HAVE_LIBEVENT. If ACTION-IF-NOT-FOUND is not specified then an error will
#   be generated halting configure.
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

AC_DEFUN([AX_LIB_EVENT], [
	AC_MSG_CHECKING(for Libevent library)
	AC_REQUIRE([AC_PROG_CC])
	#
	# User hints...
	#
	AC_ARG_VAR([LIBEVENT], [LIBEVENT library location])
	AC_ARG_WITH([libevent],
		[AS_HELP_STRING([--with-libevent],
		[user defined path to LIBEVENT library])],
		[
			if test -n "$LIBEVENT" ; then
				AC_MSG_RESULT(yes)
				with_libevent=$LIBEVENT
			elif test "$withval" != no ; then
				AC_MSG_RESULT(yes)
				with_libevent=$withval
			else
				AC_MSG_RESULT(no)
			fi
		],
		[
			if test -n "$LIBEVENT" ; then
				with_libevent=$LIBEVENT
				AC_MSG_RESULT(yes)
			else
				with_libevent=/usr
				if test ! -f "$with_libevent/include/event2/event.h" ; then
					with_libevent=/usr/local
					if test ! -f "$with_libevent/include/event2/event.h" ; then
					  with_libevent=""
					  AC_MSG_RESULT(failed)
				    else
					  AC_MSG_RESULT($with_libevent)
					fi
				else
					AC_MSG_RESULT($with_libevent)
				fi
			fi
		])
	#
	# locate LIBEVENT library
	#
		if test -n "$with_libevent" ; then
			old_CFLAGS=$CFLAGS
			old_LDFLAGS=$LDFLAGS
			CFLAGS="-I$with_libevent/include $CFLAGS"
			LDFLAGS="-L$with_libevent/lib $LDFLAGS"

			AC_LANG_SAVE
			AC_LANG_C

			AC_CHECK_LIB(event, event_get_version_number,
				[libevent_lib=yes], [libevent_lib=no], 
                [])
			AC_CHECK_HEADER(event2/event.h, [libevent_h=yes],
				[libevent_h=no], [/* check */])

			AC_LANG_RESTORE

			CFLAGS=$old_CFLAGS
			LDFLAGS=$old_LDFLAGS

			AC_MSG_CHECKING(if  Libevent in $with_libevent works)
			if test "$libevent_lib" = "yes" -a "$libevent_h" = "yes" ; then
				AC_SUBST(LIBEVENT_INCLUDE, [-I$with_libevent/include])
				AC_SUBST(LIBEVENT_LIB, [-L$with_libevent/lib])
				AC_SUBST(LIBEVENT_LIB_PATH, [$with_libevent/lib])
				AC_MSG_RESULT(yes)
			else
				AC_MSG_RESULT(failed)
			fi
		fi
		#
		#
		#
		if test x = x"$LIBEVENT_LIB" ; then
			ifelse([$2],,[AC_MSG_ERROR(Failed to find valid LIBEVENT library)],[$2])
			:
		else
			ifelse([$1],,[AC_DEFINE(HAVE_LIBEVENT,1,[Define if you have LIBEVENT library])],[$1])
			:
		fi
	])dnl AX_LIB_LIBEVENT

