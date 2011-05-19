#!/bin/sh

set -x

# Remove the auto* files and get fresh copies
rm -f aclocal.m4 config.* depcomp install-sh missing ltmain.sh
rm -rf autom4te.cache
aclocal -Wall
libtoolize --copy
automake -Wall --gnits --add-missing --copy
autoconf -Wall
