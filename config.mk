####################################################
#
# config.mk
#
# This file contains general definitions and variables for the Makefile.am's
# Note that automake will include this file in the Makefile.in's, it will NOT
# be pulled at make or configure time. It will be done at automake time (or
# autoreconf).  So, changes to this file will not affect the build process
# unless you run automake.
#
##############

# 'abs_top_srcdir' is automatically set by the auto* tools. It is the root
# directory of the project. We include ROOT to be able to include config.h
ROOT_INCLUDE = $(abs_top_srcdir)
ROOT_INCLUDE_FLAG = -I$(abs_top_srcdir)


LIBCCNX_LIB_FLAG  = @LIBCCNX_LIB@ -lccnx_api_portal -lccnx_api_notify -lrta -lccnx_api_control -lccnx_common
LIBPARC_LIB_FLAG  = @LIBPARC_LIB@ -lparc -lcrypto -lm
LONGBOW_LIB_FLAG  = @LONGBOW_LIB@ -llongbow -llongbow-ansiterm
LIBEVENT_LIB_FLAG = @LIBEVENT_LIB@ -levent


# -pipe for performance
# -c99 to ensure compliance to C99 spec
AM_CPPFLAGS = -pipe --std=c99 @DEBUG_FLAG@ @DEBUG_CPPFLAGS@ 
AM_LDFLAGS  =


# Define project-wide CPP flags to be passed to child Makefiles
CCN_TUTORIAL_DEMO_INC_CPPFLAGS = 
CCN_TUTORIAL_DEMO_INC_CPPFLAGS+= $(ROOT_INCLUDE_FLAG) 
CCN_TUTORIAL_DEMO_INC_CPPFLAGS+= @LIBCCNX_INCLUDE@
CCN_TUTORIAL_DEMO_INC_CPPFLAGS+= @LIBPARC_INCLUDE@
CCN_TUTORIAL_DEMO_INC_CPPFLAGS+= @LONGBOW_INCLUDE@
CCN_TUTORIAL_DEMO_INC_CPPFLAGS+= @LIBEVENT_INCLUDE@

# Define project-wide linker flags to be passed to child Makefiles
# Link order matters
CCN_TUTORIAL_DEMO_INC_LFLAGS = 
CCN_TUTORIAL_DEMO_INC_LFLAGS+= $(LIBCCNX_LIB_FLAG)
CCN_TUTORIAL_DEMO_INC_LFLAGS+= $(LIBPARC_LIB_FLAG)
CCN_TUTORIAL_DEMO_INC_LFLAGS+= $(LONGBOW_LIB_FLAG)
CCN_TUTORIAL_DEMO_INC_LFLAGS+= $(LIBEVENT_LIB_FLAG)
CCN_TUTORIAL_DEMO_INC_LFLAGS+= -lcrypto
CCN_TUTORIAL_DEMO_INC_LFLAGS+= -lm
CCN_TUTORIAL_DEMO_INC_LFLAGS+= -lpthread

# These only apply to unit tests
CCN_TUTORIAL_DEMO_TEST_C_FLAGS=--coverage
CCN_TUTORIAL_DEMO_TEST_L_FLAGS=--coverage -rdynamic 


