# configure.ac - Used with autoconf to create configure script
# This script is part of ultragetopt, see COPYING for licensing details

# Initialize and configure
AC_INIT([ultragetopt], [0.6.2], [Kevin Locke <kevin@kevinlocke.name>])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([ultragetopt.c])
# AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_FILES([Makefile tests/Makefile])
AM_INIT_AUTOMAKE([std-options subdir-objects])
AC_REQUIRE_AUX_FILE([tap-driver.sh])

# Determine the host system (used for reference in tests)
AC_CANONICAL_HOST

# Support for code coverage checking
# Disable default optimizations added by AC_PROG_CC when CFLAGS is unset
# Note:  Must occur before AC_PROG_CC so that we can set CFLAGS
AX_CODE_COVERAGE
AS_IF([test x$enable_code_coverage = xyes], [: ${CFLAGS=""}])

# Checks for programs.
AC_PROG_CC
AM_PROG_AR

# Initialize libtool (must be after AM_PROG_AR)
LT_INIT([win32-dll])

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([stdarg.h stdlib.h string.h])
AC_CHECK_HEADERS([strings.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Check compiler hardening flag support
# Note:  This is only used for test binaries to ensure functionality.
# Enabling these flags for library builds is left to the caller (via CFLAGS).
AX_CHECK_COMPILE_FLAG([-fstack-protector-strong],
		      [AX_APPEND_FLAG([-fstack-protector-strong],
				      [HARDENING_CFLAGS])],
		      [AX_CHECK_COMPILE_FLAG([-fstack-protector],
					     [AX_APPEND_FLAG([-fstack-protector],
							     [HARDENING_CFLAGS])])])

# Enable compiler warnings
AX_COMPILER_FLAGS([WARN_CFLAGS],
		  [WARN_LDFLAGS],
		  [yes],
		  [],
		  [-Wconversion \
		   -Wstack-protector \
		   -Wunsafe-loop-optimizations \
		   -pedantic])

# Check for sanitizer support for tests
# Note:  FreeBSD 10.3 ships clang 3.4.1 without sanitizers
#        -fsanitize=* works for compile but causes link errors if used.
#        AX_CHECK_COMPILE_FLAG passes, AX_CHECK_LINK_FLAG can fail.
m4_foreach_w([sanitizer_type], [address undefined],
	     [AX_CHECK_LINK_FLAG([-fsanitize=]sanitizer_type,
				 [SANITIZE_]sanitizer_type[=yes],
				 [SANITIZE_]sanitizer_type[=no],
				 [],
				 # Use clock() to prevent optimizing out
				 # possibly-undefined arithmetic for UBSan
				 [AC_LANG_PROGRAM([#include <time.h>],
					  [if (1 / (clock() % 3)) return 1;])])
	     AM_CONDITIONAL([SANITIZE_]translit(sanitizer_type, 'a-z', 'A-Z'),[test "x$SANITIZE_]sanitizer_type[" = xyes])])

# Check support for additional debugging flags
# -fno-omit-frame-pointer for better backtraces in ASan and debugger
# -fno-common so ASan can instrument global variables (and to check for redefs)
m4_foreach_w([debug_flag], [-fno-omit-frame-pointer -fno-common],
	     [AX_CHECK_COMPILE_FLAG(debug_flag,
	      [AX_APPEND_FLAG(debug_flag, [DEBUG_CFLAGS])])])
AC_SUBST([DEBUG_CFLAGS])

# Checks for libraries.

# Checks for library functions.
# Note:  Could add replacements and get more advanced here but need
#	 ultragetopt.c to work in standard build systems with little fuss
#AC_REPLACE_FUNCS([strcasecmp strncasecmp])
AC_CHECK_FUNCS([strchr index], [break])
AC_CHECK_FUNCS([strcasecmp _stricmp], [break])
AC_CHECK_FUNCS([strncasecmp _strnicmp], [break])

# pkg-config support is currently added but unreleased
# PKG_CHECK_MODULES([CRITERION], [criterion])
AC_CHECK_LIB([criterion],
	     [criterion_initialize],
	     [AX_APPEND_FLAG([-lcriterion], [TEST_LIBS])],
	     [AC_MSG_WARN([libcriterion not found.  Testing not supported.])])
AC_SUBST([TEST_LIBS])

# Checks for system getopt behavior
HOST_GETOPT_BEHAVIOR=""
AS_CASE([$host_os],
	[darwin*|macos*], [HOST_GETOPT_BEHAVIOR="DARWIN"],
	[gnu*|linux*], [HOST_GETOPT_BEHAVIOR="GNU"],
	[*bsd*], [HOST_GETOPT_BEHAVIOR="BSD"],
	AC_MSG_WARN([Unrecognized host system.  Not checking system getopt.]))
AC_SUBST([HOST_GETOPT_BEHAVIOR])
AM_CONDITIONAL([TEST_HOST_GETOPT], [test "x$HOST_GETOPT_BEHAVIOR" != "x"])

AC_OUTPUT
