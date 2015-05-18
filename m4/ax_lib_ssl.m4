# SYNOPSIS
#
#   AX_LIB_SSL([ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
#
# DESCRIPTION
#
#   This macro searches for the LIBSSL library in the user specified
#   location.  The user may specify the location either by defining the
#   environment variable LIBSSL or by using the --with-libssl option to
#   configure. If the environment variable is defined it has precedent over
#   everything else. If no location was specified then it searches in /usr/lib
#   and /usr/local/lib for the library and in /usr/include and
#   /usr/local/include for the header files. Upon sucessful completion the
#   variables LIBSSL_LIB and LIBSSL_INCLUDE are set.
#
#   ACTION-IF-FOUND is a list of shell commands to run if a LIBSSL library is
#   found, and ACTION-IF-NOT-FOUND is a list of commands to run it if it is not
#   found. If ACTION-IF-FOUND is not specified, the default action will define
#   HAVE_LIBSSL. If ACTION-IF-NOT-FOUND is not specified then an error will
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

AC_DEFUN([AX_LIB_SSL], [
	AC_MSG_CHECKING(for Libssl library)
	AC_REQUIRE([AC_PROG_CC])
	#
	# User hints...
	#
	AC_ARG_VAR([LIBSSL], [LIBSSL library location])
	AC_ARG_WITH([libssl],
		[AS_HELP_STRING([--with-libssl],
		[user defined path to LIBSSL library])],
		[
			if test -n "$LIBSSL" ; then
				AC_MSG_RESULT(yes)
				with_libssl=$LIBSSL
			elif test "$withval" != no ; then
				AC_MSG_RESULT(yes)
				with_libssl=$withval
			else
				AC_MSG_RESULT(no)
			fi
		],
		[
			if test -n "$LIBSSL" ; then
				with_libssl=$LIBSSL
				AC_MSG_RESULT(yes)
			else
				with_libssl=/usr
				if test ! -f "$with_libssl/include/openssl/ssl.h" ; then
					with_libssl=/usr/local
					if test ! -f "$with_libssl/include/openssl/ssl.h" ; then
					  with_libssl=""
					  AC_MSG_RESULT(failed)
				    else
					  AC_MSG_RESULT($with_libssl)
					fi
				else
					AC_MSG_RESULT($with_libssl)
				fi
			fi
		])
	#
	# locate LIBSSL library
	#
		if test -n "$with_libssl" ; then
			old_CFLAGS=$CFLAGS
			old_LDFLAGS=$LDFLAGS
			CFLAGS="-I$with_libssl/include $CFLAGS"
			LDFLAGS="-L$with_libssl/lib $LDFLAGS"

			AC_LANG_SAVE
			AC_LANG_C

			AC_CHECK_LIB(ssl, SSL_get_version,
				[libssl_lib=yes], [libssl_lib=no], 
                [-lcrypto])
			AC_CHECK_HEADER(openssl/ssl.h, [libssl_h=yes],
				[libssl_h=no], [/* check */])

			AC_LANG_RESTORE

			CFLAGS=$old_CFLAGS
			LDFLAGS=$old_LDFLAGS

			AC_MSG_CHECKING(if  Libssl in $with_libssl works)
			if test "$libssl_lib" = "yes" -a "$libssl_h" = "yes" ; then
				AC_SUBST(LIBSSL_INCLUDE, [-I$with_libssl/include])
				AC_SUBST(LIBSSL_LIB, [-L$with_libssl/lib])
				AC_SUBST(LIBSSL_LIB_PATH, [$with_libssl/lib])
				AC_MSG_RESULT(yes)
			else
				AC_MSG_RESULT(failed)
			fi
		fi
		#
		#
		#
		if test x = x"$LIBSSL_LIB" ; then
			ifelse([$2],,[AC_MSG_ERROR(Failed to find valid LIBSSL library)],[$2])
			:
		else
			ifelse([$1],,[AC_DEFINE(HAVE_LIBSSL,1,[Define if you have LIBSSL library])],[$1])
			:
		fi
	])dnl AX_LIB_LIBSSL

