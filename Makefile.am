# Makefile.am - Script used in combination with automake to produce Makefile.in

include_HEADERS = ultragetopt.h
lib_LTLIBRARIES = libultragetopt.la
libultragetopt_la_SOURCES = ultragetopt.h ultragetopt.c
libultragetopt_la_LDFLAGS = -version-info 0:1:0

MAINTAINERCLEANFILES =	Makefile.in aclocal.m4 configure $(AUX_DIST)

EXTRA_DIST = autogen.sh \
	     BUGS \
	     ultragetopt.rc \
	     ultragetopt.sln \
	     ultragetopt.vcproj \
	     tests/getopt.c \
	     tests/testargs.sh
