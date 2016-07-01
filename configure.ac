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

# Determine the host system (used for reference in tests)
AC_CANONICAL_HOST

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
AC_CHECK_LIB([criterion], [criterion_initialize])

# Checks for system getopt behavior
HOST_GETOPT_BEHAVIOR=""
AS_CASE([$host_os],
	[darwin*|macos*], [HOST_GETOPT_BEHAVIOR="DARWIN"],
	[gnu*|linux*], [HOST_GETOPT_BEHAVIOR="GNU"],
	[*bsd*], [HOST_GETOPT_BEHAVIOR="BSD"],
	AC_MSG_WARN([Unrecognized host system.  Not checking system getopt.]))
AC_SUBST([HOST_GETOPT_BEHAVIOR])
AM_CONDITIONAL([TEST_HOST_GETOPT], [test "x$HOST_GETOPT_BEHAVIOR" != "x"])

# Support for code coverage checking
AX_CODE_COVERAGE

AC_OUTPUT
