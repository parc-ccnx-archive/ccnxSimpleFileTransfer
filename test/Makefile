EXECUTABLES = test_tutorial_FileIO

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

all: test_tutorial_FileIO

test_tutorial_FileIO: test_tutorial_FileIO.c 
	${CC} $? ${CFLAGS} -o $@

check: test_tutorial_FileIO
	./test_tutorial_FileIO

clean:
	rm -rf ${EXECUTABLES}
