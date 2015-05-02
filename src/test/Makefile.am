include ../../config.mk

AM_CPPFLAGS+= $(CCN_TUTORIAL_DEMO_INC_CPPFLAGS) $(CCN_TUTORIAL_DEMO_TEST_C_FLAGS)
AM_LDFLAGS+=$(CCN_TUTORIAL_DEMO_TEST_L_FLAGS)

LDADD = -L$(abs_top_srcdir)/src ${CCN_TUTORIAL_DEMO_INC_LFLAGS}

LONGBOW_TESTS = \
    test_tutorial_FileIO 

TESTS = ${LONGBOW_TESTS}

check_PROGRAMS = ${LONGBOW_TESTS}

test_coverage_files = \
    $(LONGBOW_TESTS:=.gcno) \
    $(LONGBOW_TESTS:=.gcda)

CLEANFILES = ${test_coverage_files}
