# Makefile.am - Script used in combination with automake to produce Makefile.in

# Note:  Tests are compiled with hardening, fortification, and -Werror to
#        ensure it behaves correctly under these conditions.

# Note:  Non-const argv makes const string literals very annoying for tests.
#        Disable write-strings warning to avoid this annoyance.

AM_CPPFLAGS = -D_FORTIFY_SOURCE=2
AM_CFLAGS = $(HARDENING_CFLAGS) $(WARN_CFLAGS) -Wno-write-strings -Werror
AM_LDFLAGS = $(WARN_LDFLAGS)

TESTS = check_ultragetopt

check_PROGRAMS = check_ultragetopt check_system_getopt

check_ultragetopt_SOURCES = getopt_tests.c ../ultragetopt.c
check_ultragetopt_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_srcdir)
check_ultragetopt_LDADD = -lcriterion

check_system_getopt_SOURCES = getopt_tests.c
check_system_getopt_CPPFLAGS = $(AM_CPPFLAGS) -DTEST_SYSTEM_GETOPT=1
check_system_getopt_LDADD = -lcriterion
