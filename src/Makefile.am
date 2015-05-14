include ../config.mk

SUBDIRS = test

# Set the CPP flags, inherited from ../config.mk (see first line)
AM_CPPFLAGS+= $(CCN_TUTORIAL_DEMO_INC_CPPFLAGS)

bin_PROGRAMS =  tutorial_Client tutorial_Server


# These header files are listed so they can be distributed via autoconf
tutorial_HDRS = \
	tutorial_About.h \
	tutorial_Common.h \
	tutorial_FileIO.h

# These targets define the source files and the libraries to use when building
# the executables defined in bin_PROGRAMS
tutorial_Client_SOURCES = \
    ${tutorial_HDRS} \
    tutorial_Common.c \
    tutorial_FileIO.c \
	tutorial_Client.c \
    tutorial_About.c

tutorial_Client_LDADD = \
    ${CCN_TUTORIAL_DEMO_INC_LFLAGS}

tutorial_Server_SOURCES = \
    ${tutorial_HDRS} \
    tutorial_Common.c \
    tutorial_FileIO.c \
	tutorial_Server.c \
    tutorial_About.c

tutorial_Server_LDADD = \
    ${CCN_TUTORIAL_DEMO_INC_LFLAGS}

# This line generates a list of coverage output files that need to be cleaned up later
libccn_tutorial_demo_a_coverage_files = \
    $(libccn_tutorial_demo_a_SOURCES:.c=.gcno) \
    $(libccn_tutorial_demo_a_SOURCES:.c=.gcda)


# This line adds the list of coverage files to the list of files that 'make clean' should delete
CLEANFILES = ${libccn_tutorial_demo_a_coverage_files} 
