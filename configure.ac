# configure.ac - Used with autoconf to create configure script
# This script is part of ultragetopt, see COPYING for licensing details

# Initialize and configure
AC_INIT([ultragetopt], [0.6.0], [Kevin Locke <kwl7@cornell.edu>])
AC_CONFIG_SRCDIR([ultragetopt.c])
# AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_FILES([Makefile])
# AC_GNU_SOURCE
AM_INIT_AUTOMAKE

# Support Windows DLL later...
#AC_LIBTOOL_WIN32_DLL

# Prevent AC_PROG_LIBTOOL from checking for C++, Fortran, and Java
# this hack should be unnecessary soon
m4_defun([_LT_AC_LANG_CXX_CONFIG], [:])
m4_defun([_LT_AC_LANG_F77_CONFIG], [:])
m4_defun([_LT_AC_LANG_GCJ_CONFIG], [:])

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for header files.
#AC_HEADER_STDC
#AC_CHECK_HEADERS([stdarg.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for libraries.

# Checks for library functions.
# Note:  Could add replacements and get more advanced here but need
#	 ultragetopt.c to work in standard build systems with little fuss
#AC_REPLACE_FUNCS([strcasecmp strncasecmp])
AC_CHECK_FUNCS([strchr index], [break])
AC_CHECK_FUNCS([strrchr rindex], [break])
AC_CHECK_FUNCS([strcasecmp _stricmp], [break])
AC_CHECK_FUNCS([strncasecmp _strnicmp], [break])

AC_OUTPUT