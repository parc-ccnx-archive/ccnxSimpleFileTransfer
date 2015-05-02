AC_INIT([CCNx-Tutorial-Demo], m4_esyscmd([tr -d '\n' < BASE_VERSION]), [bugs@ccnx.org])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

m4_include(m4/ax_lib_longbow.m4)
m4_include(m4/ax_lib_parc.m4)
m4_include(m4/ax_lib_ccnx.m4)

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

AC_MSG_CHECKING(for struct stat has mtimespec)
AC_CACHE_VAL(ac_cv_stat_mtimespec,
[AC_TRY_COMPILE([
#include <sys/types.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
#include <sys/stat.h>],[struct stat teststat; teststat.st_mtimespec;],
ac_cv_stat_mtimespec=st_mtimespec)
])

if test -z "$ac_cv_stat_mtimespec" ; then #{
AC_TRY_COMPILE([
#include <sys/types.h>
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#endif
#include <sys/stat.h>],[struct stat teststat; teststat.st_mtim;],
ac_cv_stat_mtimespec=st_mtim)
fi #}

if test -z "$ac_cv_stat_mtimespec"; then
ac_cv_stat_mtimespec=no
fi

AC_MSG_RESULT($ac_cv_stat_mtimespec)
if test ! "$ac_cv_stat_mtimespec" = no; then
    AC_DEFINE_UNQUOTED(ST_STAT_MTIMESPEC,$ac_cv_stat_mtimespec,"What do we have for mtimespec in stat struct")
fi

AC_MSG_CHECKING(for dcache line size)
uname_s=`uname -s`
# Let's figure out what platform we're in...
AS_IF([test x$uname_s = xDarwin], 
	  		[ac_cache_line_size=`sysctl -n hw.cachelinesize`],
      [test x$uname_s = xLinux], 
	        [ac_cache_line_size=`getconf LEVEL1_DCACHE_LINESIZE`],
      [ac_cache_line_size="unknown"])

if test "$ac_cache_line_size" = unknown; then
  ac_cache_line_size_result="unknown... setting to 64"
  ac_cache_line_size=64
else
  ac_cache_line_size_result=$ac_cache_line_size
fi

AC_DEFINE_UNQUOTED(LEVEL1_DCACHE_LINESIZE,$ac_cache_line_size,CPU Cache line size)
AC_MSG_RESULT($ac_cache_line_size_result)

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/test/Makefile
])

#AC_CONFIG_SUBDIRS([transport/transport_rta/libevent])

AH_TOP([#include <config_top.h>])


AX_LIB_LONGBOW
AX_LIB_PARC
AX_LIB_CCNX

#CFLAGS="$CFLAGS -I/usr/local/ccn/include"
#CXXFLAGS="$CXXFLAGS -I/usr/local/ccn/include"
#LDFLAGS="$LDFLAGS -L/usr/local/ccn/lib"

#AC_CHECK_LIB(longbow-textplain,longBowReportRuntime_Message,[],[
#										   echo "ERROR, longbow-textplain not found!"
#										   echo "Please install LongBow or use --with-longbow to tell configure where it is"
#										   exit -1
#										   ])

#AC_CHECK_LIB(longbow,longBowAbout_Version,
#			 [],
#			 [
#			   echo "ERROR, LongBow not found!"
#			   echo "Please install LongBow or use --with-longbow to tell configure where it is"
#			   exit -1
#			 ],
#			 -llongbow-textplain)

#AC_CHECK_LIB(parc,libparcAbout_Version,
#			 [],
#			 [
#			   echo "ERROR, LongBow not found!"
#			   echo "Please install LongBow or use --with-longbow to tell configure where it is"
#			   exit -1
#			 ],
#			 -llongbow-textplain)

AC_ARG_WITH([libevent],
    [AS_HELP_STRING([--with-libevent[=DIR]],
        [specify libevent directory @<:@default=check@:>@])],
        AC_SUBST(LIBEVENT_INCLUDE,-I${withval}/include)
        AC_SUBST(LIBEVENT_LIB,-L${withval}/lib)
    )

AC_ARG_ENABLE(debug,
   [AS_HELP_STRING([--enable-debug],
              [turns on debugging])],
   [AS_IF([test "x$enableval" == "xyes"],
         [AC_SUBST(DEBUG_FLAG,[-DDEBUG=1])
          DEBUG_CPPFLAGS="-g -O0 -Wall -pedantic"
          AC_SUBST(DEBUG_CPPFLAGS)
		],
         [DEBUG=false])],
         [DEBUG=false])


# Update version if possible
if test -f scripts/setversion.sh; then
	scripts/setversion.sh ${PACKAGE_VERSION}
fi

# Still no version file... create one
if test ! -f VERSION; then
    echo ${PACKAGE_VERSION} > VERSION
fi
release_version=`cat VERSION`

echo "Checking for RELEASE_VERSION... " $release_version
AC_DEFINE_UNQUOTED(RELEASE_VERSION,["$release_version"],"Release version")


AC_OUTPUT
