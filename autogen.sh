#!/bin/sh

set -x

# Remove the auto* files and get fresh copies
rm -f aclocal.m4 config.* depcomp install-sh missing ltmain.sh
rm -rf autom4te.cache
#rm -rf intl
#gettextize --intl --no-changelog < /dev/null
#mv configure.ac~ configure.ac
#mv Makefile.am~ Makefile.am
aclocal #-I m4
#autoheader
libtoolize --copy
automake --add-missing --copy
autoconf
