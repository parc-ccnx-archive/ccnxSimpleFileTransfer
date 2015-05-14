# SYNOPSIS
#
#   AX_DEBUG_OPTION([OPTIMIZED_CPP_FLAGS])
#
# DESCRIPTION
#
# Add a --enable-debug option to compile.  If debug is enable, set debug flags
# to -g -O0, else, set debug flags to "-O3" or optional argument.
# The variable DEBUG_CPPFLAGS will be set and substituted
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

AC_DEFUN([AX_DEBUG_OPTION], [
	AC_MSG_CHECKING(for debug option)
	ifelse([$1],,[DEBUG_CPPFLAGS="-O3"],[DEBUG_CPPFLAGS=$1])

    DEBUG_O_FLAG="-O0"

	AC_REQUIRE([AC_PROG_CC])

	AC_ARG_ENABLE(debug,
   		[AS_HELP_STRING([--enable-debug],
              [turns on debugging])],
   		[AS_IF([test "x$enableval" == "xyes"],
		 	# We have debug enabled, set the right flags...
         	[AC_SUBST(DEBUG_FLAG,[-DDEBUG=1])
          	DEBUG_CPPFLAGS="-g $DEBUG_O_FLAG" ],
		 	# We have a debug flag, disabled...
         	[DEBUG=false])],
		 	# We don't have a debug flag...
         	[DEBUG=false])

	AC_MSG_RESULT($DEBUG_CPPFLAGS)
	AC_SUBST(DEBUG_CPPFLAGS)

	])dnl AX_LIB_LIBPARC

