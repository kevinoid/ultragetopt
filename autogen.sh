#!/bin/sh

set -x

# Remove the auto* files and get fresh copies
rm -rf autom4te.cache build-aux
mkdir -p m4
aclocal -Wall --install -I m4
libtoolize --copy
automake -Wall --gnits --add-missing --copy
autoconf -Wall
