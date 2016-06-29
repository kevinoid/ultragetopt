#!/bin/sh

set -x

# Remove the auto* files and get fresh copies
rm -rf autom4te.cache build-aux
aclocal -Wall
libtoolize --copy
automake -Wall --gnits --add-missing --copy
autoconf -Wall
