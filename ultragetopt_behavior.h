#ifndef INCLUDED_ULTRAGETOPT_BEHAVIOR_H
#define INCLUDED_ULTRAGETOPT_BEHAVIOR_H 1
/* Ultragetopt behavior definitions.
 *
 * This file is part of Ultragetopt.
 * It is licensed under the terms of the MIT License.
 * The complete text of the license is available in the project documentation.
 *
 * Copyright (C) 2007-2016, Kevin Locke <kevin@kevinlocke.name>
 */

/* Supported defines:
 * ULTRAGETOPT_LIKE_BSD		Behave like BSD getopt()
 * ULTRAGETOPT_LIKE_DARWIN	Behave like Darwin (Mac OS) getopt()
 * ULTRAGETOPT_LIKE_GNU		Behave like GNU getopt()
 * ULTRAGETOPT_LIKE_POSIX   	Behave like POSIX definition of getopt()
 *
 * ULTRAGETOPT_ERROR_PROGNAME	Program name to use in error messages
 *				(default is argv[0] passed to getopt).
 *
 * ULTRAGETOPT_ASSIGNSPACE	Parse "-o value" as "value" rather than " value"
 *				Note: Only applicable when argv[x] == "-o value"
 *				      Not for argv[x] == "-o" [x+1] == "value"
 * ULTRAGETOPT_DEFAULTOPTOPT	Set optopt to this value by default on each
 *				call to getopt()
 * ULTRAGETOPT_HYPHENARG	Accept -option -arg as -option with argument
 *				"-arg" rather than -option missing argument
 *				Note:  A single "-" is always accepted as an
 *				argument
 * ULTRAGETOPT_LONGOPTADJACENT	Accept adjacent arguments to long options
 *				(e.g. --optionarg) based on first longest-match
 * ULTRAGETOPT_OPTIONPERMUTE	Permute options when calling getopt(3), do not
 *				stop at first non-option.
 *				Behaves like GNU getopt where leading '+' or
 *				$POSIXLY_CORRECT both stop this @ runtime
 *				Note:  Options are still permuted when calling
 *				getopt_long(3) or getopt_long_only(3).
 * ULTRAGETOPT_SHORTOPTASSIGN	Support -o=file syntax for short options
 * ULTRAGETOPT_SEPARATEDOPTIONAL Accept separated optional arguments
 *				 Parse -o file as -o  with argument file
 * ULTRAGETOPT_DOS_DASH		Support - and -- options in ultragetopt*_dos()
 * ULTRAGETOPT_BSD_ERRORS	Print error messages matching BSD getopt
 * ULTRAGETOPT_DARWIN_ERRORS	Print error messages matching Darwin getopt
 * ULTRAGETOPT_GNU_ERRORS	Print error messages matching GNU getopt
 * ULTRAGETOPT_NO_EATDASHDASH	Do not increment optind when argv[optind] is --
 * ULTRAGETOPT_NO_OPTIONALARG	Do not support GNU "::" optional argument
 *				Always supported in *_long*()
 * ULTRAGETOPT_NO_OPTIONASSIGN	Do not support --option=value syntax
 * ULTRAGETOPT_2CHAR_ARG_SHORT	When calling getopt_long_only, always interpret
 *                              2-character arguments (e.g. "-r") for a valid
 *                              short option as short options (default would be
 *                              "r" long option).
 * ULTRAGETOPT_1PREFIX_SHORT	When calling getopt_long_only and an argument
 *				is both a 1-character proper prefix of a long
 *				option and a valid short option, always
 *				interpret it as a short option (default "-r=a"
 *				would match a long option).
 * ULTRAGETOPT_ALLOW_OPTIND0	Do not treat optind == 0 as special (default is
 *				to reset the getopt state for optind == 0)
 * ULTRAGETOPT_NOMATCH_W_AS_ARG	When handling -W specially due to "W;" in
 *				shortopts and the argument does not match any
 *				long options, return it as the argument to the
 *				'W' short option.
 * ULTRAGETOPT_OPTOPT_0_IF_FLAG	Set optopt to 0 when a long option with a flag
 *                              has an argument error (default sets to val).
 */

#ifdef ULTRAGETOPT_LIKE_POSIX
# define ULTRAGETOPT_NO_OPTIONALARG
# define ULTRAGETOPT_NO_OPTIONASSIGN
# undef ULTRAGETOPT_NO_EATDASHDASH
# undef ULTRAGETOPT_ASSIGNSPACE
# undef ULTRAGETOPT_BSD_ERRORS
# undef ULTRAGETOPT_DARWIN_ERRORS
# undef ULTRAGETOPT_GNU_ERRORS
# undef ULTRAGETOPT_OPTIONPERMUTE
# undef ULTRAGETOPT_SHORTOPTASSIGN
#elif defined(ULTRAGETOPT_LIKE_GNU)
# define ULTRAGETOPT_2CHAR_ARG_SHORT
# define ULTRAGETOPT_GNU_ERRORS
# define ULTRAGETOPT_HYPHENARG
# define ULTRAGETOPT_NOMATCH_W_AS_ARG
# define ULTRAGETOPT_OPTIONPERMUTE
# undef ULTRAGETOPT_ASSIGNSPACE
# undef ULTRAGETOPT_NO_OPTIONALARG
# undef ULTRAGETOPT_NO_OPTIONASSIGN
# undef ULTRAGETOPT_NO_EATDASHDASH
# undef ULTRAGETOPT_SHORTOPTASSIGN
# undef ULTRAGETOPT_SEPARATEOPTIONAL
# undef ULTRAGETOPT_LONGOPTADJACENT
#elif defined(ULTRAGETOPT_LIKE_BSD)
# define ULTRAGETOPT_1PREFIX_SHORT
# define ULTRAGETOPT_ALLOW_OPTIND0
# define ULTRAGETOPT_BSD_ERRORS
# define ULTRAGETOPT_DEFAULTOPTOPT '?'
# define ULTRAGETOPT_ERROR_PROGNAME getprogname()
# define ULTRAGETOPT_OPTOPT_0_IF_FLAG
# undef ULTRAGETOPT_ASSIGNSPACE
# undef ULTRAGETOPT_NO_OPTIONALARG
# undef ULTRAGETOPT_NO_OPTIONASSIGN
# undef ULTRAGETOPT_NO_EATDASHDASH
# undef ULTRAGETOPT_OPTIONPERMUTE
# undef ULTRAGETOPT_SHORTOPTASSIGN
# undef ULTRAGETOPT_SEPARATEOPTIONAL
# undef ULTRAGETOPT_LONGOPTADJACENT
#elif defined(ULTRAGETOPT_LIKE_DARWIN)
# define ULTRAGETOPT_DARWIN_ERRORS
# define ULTRAGETOPT_OPTIONPERMUTE
# undef ULTRAGETOPT_ASSIGNSPACE
# undef ULTRAGETOPT_NO_OPTIONALARG
# undef ULTRAGETOPT_NO_OPTIONASSIGN
# undef ULTRAGETOPT_SHORTOPTASSIGN
# undef ULTRAGETOPT_NO_EATDASHDASH
# undef ULTRAGETOPT_SEPARATEOPTIONAL
# undef ULTRAGETOPT_LONGOPTADJACENT
#endif

#endif /* INCLUDED_ULTRAGETOPT_BEHAVIOR_H */
