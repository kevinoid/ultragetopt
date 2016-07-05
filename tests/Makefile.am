# Makefile.am - Script used in combination with automake to produce Makefile.in

# Note:  Tests are compiled with hardening, fortification, and -Werror to
#        ensure it behaves correctly under these conditions.

# Note:  Non-const argv makes const string literals very annoying for tests.
#        Disable write-strings warning to avoid this annoyance.

AM_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS) -D_FORTIFY_SOURCE=2
AM_CFLAGS = $(HARDENING_CFLAGS) \
	    $(WARN_CFLAGS) -Wno-write-strings -Werror \
	    $(CODE_COVERAGE_CFLAGS)
AM_LDFLAGS = $(WARN_LDFLAGS) $(CODE_COVERAGE_LDFLAGS)

LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) \
    $(top_srcdir)/build-aux/tap-driver.sh --comments --merge
TESTS_ENVIRONMENT = CRITERION_OUTPUTS='tap:-'
TESTS = check_ultragetopt

check_PROGRAMS = check_ultragetopt

TEST_SOURCES = array_size.h \
	       getopt_tests.c \
	       getopt_long_tests.c \
	       getopt_long_only_tests.c \
	       testlib.c \
	       testlib.h

check_ultragetopt_SOURCES = $(TEST_SOURCES) ../ultragetopt.c
check_ultragetopt_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)
check_ultragetopt_LDADD = -lcriterion

if TEST_HOST_GETOPT
TESTS += check_host_getopt
check_PROGRAMS += check_host_getopt
check_host_getopt_SOURCES = $(TEST_SOURCES)
check_host_getopt_CPPFLAGS = $(AM_CPPFLAGS) \
			     -I$(top_srcdir) \
			     -DTEST_HOST_GETOPT=1 \
			     -DULTRAGETOPT_LIKE_$(HOST_GETOPT_BEHAVIOR)=1
check_host_getopt_LDADD = -lcriterion
endif

@CODE_COVERAGE_RULES@
