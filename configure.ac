AC_INIT([CCNx-Tutorial-Demo], m4_esyscmd([tr -d '\n' < BASE_VERSION]), [bugs@ccnx.org])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

m4_include(m4/ax_lib_event.m4)
m4_include(m4/ax_lib_ssl.m4)
m4_include(m4/ax_lib_longbow.m4)
m4_include(m4/ax_lib_parc.m4)
m4_include(m4/ax_lib_ccnx.m4)
m4_include(m4/ax_release_version.m4)
m4_include(m4/ax_debug_option.m4)

: ${CFLAGS=""}
AC_PROG_CC([clang gcc cc])
AC_GNU_SOURCE
AM_PROG_CC_C_O
AM_PROG_AR
AM_SILENT_RULES([yes])
AC_PROG_RANLIB
AC_PROG_INSTALL
AC_CONFIG_HEADERS([config.h])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h errno.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MMAP
AC_CHECK_FUNCS([getcwd gettimeofday memset select socket strdup strncasecmp strndup])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/test/Makefile
])

#AC_CONFIG_SUBDIRS([transport/transport_rta/libevent])

AH_TOP([#include <config_top.h>])


AX_LIB_EVENT
AX_LIB_SSL
AX_LIB_LONGBOW
AX_LIB_PARC
AX_LIB_CCNX
AX_DEBUG_OPTION("-O3")
AX_RELEASE_VERSION($PACKAGE_VERSION)


AC_OUTPUT
