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
 * ULTRAGETOPT_OPTIONPERMUTE	Permute options, do not stop at first non-option
 *				Behaves like GNU getopt where leading '+' or
 *				$POSIXLY_CORRECT both stop this @ runtime
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
# define ULTRAGETOPT_GNU_ERRORS
# define ULTRAGETOPT_HYPHENARG
# define ULTRAGETOPT_OPTIONPERMUTE
# undef ULTRAGETOPT_ASSIGNSPACE
# undef ULTRAGETOPT_NO_OPTIONALARG
# undef ULTRAGETOPT_NO_OPTIONASSIGN
# undef ULTRAGETOPT_NO_EATDASHDASH
# undef ULTRAGETOPT_SHORTOPTASSIGN
# undef ULTRAGETOPT_SEPARATEOPTIONAL
# undef ULTRAGETOPT_LONGOPTADJACENT
#elif defined(ULTRAGETOPT_LIKE_BSD)
# define ULTRAGETOPT_BSD_ERRORS
# define ULTRAGETOPT_DEFAULTOPTOPT '?'
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
