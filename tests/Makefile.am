# Makefile.am - Script used in combination with automake to produce Makefile.in

# Note:  Tests are compiled with hardening, fortification, and -Werror to
#        ensure it behaves correctly under these conditions.

# Note:  Non-const argv makes const string literals very annoying for tests.
#        Disable write-strings warning to avoid this annoyance.

AM_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
AM_CFLAGS = $(HARDENING_CFLAGS) \
	    $(WARN_CFLAGS) -Wno-write-strings -Werror \
	    $(CODE_COVERAGE_CFLAGS) \
	    $(DEBUG_CFLAGS)
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
TEST_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)
TEST_HOST_CPPFLAGS = $(TEST_CPPFLAGS) \
		     -DTEST_HOST_GETOPT=1 \
		     -DULTRAGETOPT_LIKE_$(HOST_GETOPT_BEHAVIOR)=1

check_ultragetopt_SOURCES = $(TEST_SOURCES) ../ultragetopt.c
check_ultragetopt_CPPFLAGS = $(TEST_CPPFLAGS) -D_FORTIFY_SOURCE=2
check_ultragetopt_CFLAGS = $(AM_CFLAGS)
check_ultragetopt_LDADD = -lcriterion

if TEST_HOST_GETOPT
TESTS += check_host_getopt
check_PROGRAMS += check_host_getopt
check_host_getopt_SOURCES = $(TEST_SOURCES)
check_host_getopt_CPPFLAGS = $(TEST_HOST_CPPFLAGS) -D_FORTIFY_SOURCE=2
check_host_getopt_CFLAGS = $(AM_CFLAGS)
check_host_getopt_LDADD = -lcriterion
endif

# FIXME:  Declarations for sanitizer versions seem redundant and error-prone
# Is there a way to macro-loop or wildcard-match to build different versions?
if SANITIZE_ADDRESS
TESTS += check_ultragetopt_address
check_PROGRAMS += check_ultragetopt_address
check_ultragetopt_address_SOURCES = $(check_ultragetopt_SOURCES)
# Disable _FORTIFY_SOURCE for https://github.com/google/sanitizers/issues/247
check_ultragetopt_address_CPPFLAGS = $(TEST_CPPFLAGS) -D_FORTIFY_SOURCE=0
check_ultragetopt_address_CFLAGS = $(check_ultragetopt_CFLAGS) \
				   -fsanitize=address
check_ultragetopt_address_LDADD = $(check_host_getopt_LDADD)

if TEST_HOST_GETOPT
TESTS += check_host_getopt_address
check_PROGRAMS += check_host_getopt_address
check_host_getopt_address_SOURCES = $(check_host_getopt_SOURCES)
# Disable _FORTIFY_SOURCE for https://github.com/google/sanitizers/issues/247
check_host_getopt_address_CPPFLAGS = $(TEST_HOST_CPPFLAGS) -D_FORTIFY_SOURCE=0
check_host_getopt_address_CFLAGS = $(check_host_getopt_CFLAGS) \
				   -fsanitize=address
check_host_getopt_address_LDADD = $(check_host_getopt_LDADD)
endif
endif

if SANITIZE_UNDEFINED
TESTS += check_ultragetopt_undefined
check_PROGRAMS += check_ultragetopt_undefined
check_ultragetopt_undefined_SOURCES = $(check_ultragetopt_SOURCES)
# Disable _FORTIFY_SOURCE for https://github.com/google/sanitizers/issues/247
check_ultragetopt_undefined_CPPFLAGS = $(TEST_CPPFLAGS) -D_FORTIFY_SOURCE=0
check_ultragetopt_undefined_CFLAGS = $(check_ultragetopt_CFLAGS) \
				     -fsanitize=undefined
check_ultragetopt_undefined_LDADD = $(check_host_getopt_LDADD)

if TEST_HOST_GETOPT
TESTS += check_host_getopt_undefined
check_PROGRAMS += check_host_getopt_undefined
check_host_getopt_undefined_SOURCES = $(check_host_getopt_SOURCES)
# Disable _FORTIFY_SOURCE for https://github.com/google/sanitizers/issues/247
check_host_getopt_undefined_CPPFLAGS = $(TEST_HOST_CPPFLAGS) -D_FORTIFY_SOURCE=0
check_host_getopt_undefined_CFLAGS = $(check_host_getopt_CFLAGS) \
				     -fsanitize=undefined
check_host_getopt_undefined_LDADD = $(check_host_getopt_LDADD)
endif
endif

@CODE_COVERAGE_RULES@
