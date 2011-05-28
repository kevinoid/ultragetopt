#!/bin/sh
# Test arguments for ultragetopt and vendor-provided getopts

# Please increment this when you add/changes tests!
VERSION="24"

if [ -n "$1" ] ; then
	GETOPT=$1
else
	GETOPT=./getopt
fi

if ! test -x "$GETOPT" ; then
	exec 1>&2
	echo "Please compile the test script (getopt.c) into an executable"
	echo "called getopt using the vendor-provided getopt functions before"
	echo "running this script."
	exit 1
fi

testopts()
{
	echo ""
	echo "--- Running Command $@ ---"
	"$GETOPT" "$@"
}

echo "=== Running Getopt Test Script Version $VERSION ==="
# Test general functionality (from SUS)
testopts "nr:o::" "" "progname" "-no" "arg" "path"
testopts "nr:o::" "" "progname" "-n" "-o" "arg1" "-r" "arg2" "path"
testopts "nr:o::" "" "progname" "-o" "arg" "-n" "path" "path"
testopts "nr:o::" "" "progname" "-n" "-o" "arg" "--" "path" "path"
testopts "nr:o::" "" "progname" "-n" "-o" "--" "path" "path"
testopts "nr:o::" "" "progname" "-n" "-orarg" "path" "path"

# Test options after terminator
testopts "nr:o::" "" "progname" "-n" "-o" "--" "-n" "-o" "path" "path"

# Test adjacent short arguments and with adjacent arguments
testopts "nr:o::" "" "progname" "-n-n"
testopts "nr:o::" "" "progname" "-o-o"
testopts "nr:o::" "" "progname" "-r-r"
testopts "nr:o::" "" "progname" "-nooarg" "path"
testopts "nr:o::" "" "progname" "-noarg" "path" "path"

# Arguments?
testopts "nr:o::" "" "progname" "-r=" "arg"
testopts "nr:o::" "" "progname" "-r=-n"

# Test missing optionals
testopts "nr:o::" "" "progname" "-o=" "arg"
testopts "nr:o::" "optarg::" "progname" "--optarg"
testopts "nr:o::" "optarg::" "progname" "--optarg" ""
testopts "nr:o::" "optarg::" "progname" "--optarg=" "arg"
testopts "nr:o::" "optarg::" "progname" "--optargfile"

# Adjacent required argument
testopts "nr:o::" "reqarg:" "progname" "--reqargfile"
testopts "nr:o::" "reqarg:" "progname" "--reqarg-n"
testopts "nr:o::" "reqarg:" "progname" "--reqarg=-n"
testopts "nr:o::" "reqarg:" "progname" "--reqarg" "-n"

# Test permuting arguments
testopts "nr:o::" "optarg::" "progname" "word" "--optarg" "-n"
testopts "nr:o::" "optarg::" "progname" "word" "--optarg" "--n"
testopts "nr:o::" "optarg:: n" "progname" "word" "--optarg" "--n"
testopts "nr:o::" "optarg::" "progname" "word" "--optarg"
testopts "nr:o::" "reqarg: optarg::" "progname" "word1" "word2" "--reqarg"

# Arguments to non-arg options
testopts "nr:o::" "noarg" "progname" "--noarg=arg"
testopts "nr:o::" "" "progname" "-n=arg"

# Punctuation arguments
testopts "nr:o:::" "" "progname" "-:" "path"
testopts "nr:o::=" "" "progname" "-=" "path"
testopts "nr:o::+" "" "progname" "-+" "path"

# Odd numbers of -s
testopts "nr:o::" "reqarg: optarg::" "progname" "- -n -"
testopts "nr:o::" "reqarg: optarg::" "progname" - -n -
testopts "nr:o::" "reqarg: optarg::" "progname" - ---reqarg -
testopts "nr:o::" "reqarg: optarg::" "progname" - -r -
testopts "nr:o::" "reqarg: optarg::" "progname" --- -r ---

# -- as option arguments
testopts "nr:o:::" "" "progname" "-r--"
testopts "nr:o:::" "" "progname" "-o--"
testopts "nr:o:::" "" "progname" "-r" "--" "foo"
testopts "nr:o:::" "" "progname" "-o" "--" "foo"
