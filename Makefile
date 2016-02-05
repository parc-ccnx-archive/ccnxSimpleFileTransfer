EXECUTABLES = tutorial_Client tutorial_Server

all: ${EXECUTABLES}

# Set this to where you installed your CCNx build result
CCNX_HOME ?= /usr/local/ccnx

# Set this to where -libevent was installed
LIBEVENT_HOME ?= /usr

INCLUDE_DIR_FLAGS=-I. -I${CCNX_HOME}/include
LINK_DIR_FLAGS=-L${CCNX_HOME}/lib
CCNX_LIB_FLAGS=-lccnx_api_portal \
               -lccnx_api_notify \
               -lccnx_transport_rta \
               -lccnx_api_control \
               -lccnx_common

PARC_LIB_FLAGS=-lparc \
               -llongbow \
               -llongbow-ansiterm

DEP_LIB_FLAGS=-lcrypto -lm -L${LIBEVENT_HOME}/lib -levent

CFLAGS=-D_GNU_SOURCE \
     ${INCLUDE_DIR_FLAGS} \
     ${LINK_DIR_FLAGS} \
     ${CCNX_LIB_FLAGS} \
     ${PARC_LIB_FLAGS} \
     ${DEP_LIB_FLAGS} 

CC=gcc -O2 -std=c99

tutorial_Client: tutorial_Client.c tutorial_Common.c tutorial_About.c tutorial_FileIO.c
	${CC} $? ${CFLAGS} -o $@

tutorial_Server: tutorial_Server.c tutorial_Common.c tutorial_FileIO.c tutorial_About.c
	${CC} $? ${CFLAGS} -o $@

check:
	@${MAKE} -C test check

clean:
	rm -rf ${EXECUTABLES}
	@${MAKE} -C test clean
