
/* define ULTRAGETOPT_REPLACE_GETOPT for ultragetopt*() to replace getopt*() */
/* define ULTRAGETOPT_ONLY_DOS for ultragetopt*_dos() to replace ultragetopt*() */

#ifndef INCLUDED_GETOPT_H
#define INCLUDED_GETOPT_H 1

#ifdef __cplusplus
extern "C" {
#endif

/* Flag values to pass to getopt_tunable() */
#define UGO_CASEINSENSITIVE	0x1
#define UGO_SINGLELEADERLONG	0x2
#define UGO_OPTIONPERMUTE	0x4
#define UGO_NONOPTARG		0x8
#define UGO_NOPRINTERR		0x10
#define UGO_OPTIONALARG		0x20
#define UGO_MISSINGCOLON	0x40
#define UGO_SEPARATEDOPTIONAL	0x80
#define UGO_SHORTOPTASSIGN	0x100
#define UGO_NOEATDASHDASH	0x200
#define UGO_LONGOPTADJACENT	0x400
#define UGO_HYPHENARG		0x800
#define UGO_SINGLELEADERONLY	0x1000

#ifndef required_argument
# define no_argument 0
# define required_argument 1
# define optional_argument 2

struct option {
    const char *name;	/* Name of the option */
    int has_arg;	/* Does the option take an argument? */
    int *flag;		/* Location to store val when option encountered */
    int val;		/* Value to return (or store in flag) for option */
};
#endif /* required_argument */

extern char *ultraoptarg;
extern int ultraoptind, ultraopterr, ultraoptopt, ultraoptreset;

int ultragetopt(int argc, char *const argv[], const char *optstring);

int ultragetopt_long(int argc, char *const argv[], const char *shortopts,
		     const struct option *longopts, int *indexptr);

int ultragetopt_long_only(int argc, char *const argv[], const char *shortopts,
			  const struct option *longopts, int *indexptr);

int ultragetopt_dos(int argc, char * const argv[], const char *optstring);

int ultragetopt_long_dos(int argc, char *const argv[], const char *shortopts,
		    	 const struct option *longopts, int *indexptr);

/* Getopt with modifiable (tunable) behavior - also the backend for all other
 * getopt functions.
 * assigners - string of characters accepted to assign to an option
 *	       (e.g. --outfile=file.txt where '=' is the assigner)
 * optleaders - string of characters that indicate an option
 *	        (usually "-" on UNIX, "/" on DOS)
 * flags - see README for list of accepted flags
 */
int ultragetopt_tunable(int argc, char *const argv[], const char *shortopts,
			const struct option *longopts, int *indexptr,
			const char *assigners, const char *optleaders,
			int flags);

#ifdef ULTRAGETOPT_REPLACE_GETOPT
# define optarg ultraoptarg
# define optind ultraoptind
# define opterr ultraopterr
# define optopt ultraoptopt
# define optreset ultraoptreset
# define getopt(argc, argv, optstring) \
	 ultragetopt(argc, argv, optstring)
# define getopt_long(argc, argv, shortopts, longopts, indexptr) \
	 ultragetopt_long(argc, argv, shortopts, longopts, indexptr)
# define getopt_long_only(argc, argv, shortopts, longopts, indexptr) \
	 ultragetopt_long_only(argc, argv, shortopts, longopts, indexptr)
# define getopt_dos(argc, argv, optstring) \
	 ultragetopt_dos(argc, argv, optstring)
# define getopt_long_dos(argc, argv, shortopts, longopts, indexptr) \
	 ultragetopt_long_dos(argc, argv, shortopts, longopts, indexptr)
#endif /* GETOPT_NO_EXTENSIONS */

#ifdef ULTRAGETOPT_DOS_ONLY
# define ultragetopt(argc, argv, optstring) \
	 ultragetopt_dos(argc, argv, optstring)
# define ultragetopt_long(argc, argv, shortopts, longopts, indexptr) \
	 ultragetopt_long_dos(argc, argv, shortopts, longopts, indexptr)
# define ultragetopt_long_only(argc, argv, shortopts, longopts, indexptr) \
	 ultragetopt_long_dos(argc, argv, shortopts, longopts, indexptr)
#endif /* ULTRAGETOPT_DOS_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_GETOPT_H */
