/* Unit tests for getopt function.
 *
 * This file is part of Ultragetopt.
 * It is licensed under the terms of the MIT License.
 * The complete text of the license is available in the project documentation.
 *
 * Copyright 2016 Kevin Locke <kevin@kevinlocke.name>
 */

#include <criterion/criterion.h>

#ifdef TEST_HOST_GETOPT
# include <unistd.h>
#else
# define ULTRAGETOPT_REPLACE_GETOPT 1
# include "ultragetopt.h"
#endif

#include "array_size.h"
#include "ultragetopt_behavior.h"

static char *const CMDNAME = "cmd";

static void setup(void) {
    opterr = 0;
    /* Glibc uses optind = 0 to reinitialize.
     * Recent OpenBSD libc supports it (older require optreset = 1).
     * FreeBSD optind = 0 interprets argv[0] as first argument.
     */
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    optind = 0;
#else
    optind = 1;
    optreset = 1;
#endif
}

static void set_posixly_correct(void) {
    setenv("POSIXLY_CORRECT", "1", 1);
}

static void unset_posixly_correct(void) {
    unsetenv("POSIXLY_CORRECT");
}

TestSuite(getopt, .init = setup);

Test(getopt, noname_noopts) {
    char * const argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, noname_opts) {
    char * const argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, name_noopts) {
    char * const argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, name_opts) {
    char * const argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, arg) {
    char * const argv[] = {
        CMDNAME,
        "arg"
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt, arg_dash) {
    char * const argv[] = {
        CMDNAME,
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt, arg_dashdashdash) {
    char * const argv[] = {
        CMDNAME,
        "---",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, '-');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, '-');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, opt) {
    char * const argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, separate_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-r",
        "optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, separate_reqoptarg_dash) {
    char * const argv[] = {
        CMDNAME,
        "-r",
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, combined_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-roptarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, combined_reqoptarg_dash) {
    char * const argv[] = {
        CMDNAME,
        "-r-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-r=optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_empty_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-r=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_optlike_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-r=-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "nr:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, separate_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-o",
        "optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "o::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, combined_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-ooptarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "o::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-o=optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "o::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_empty_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-o=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "o::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_optlike_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-o=-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "no::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_nonoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":n";
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, '=');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, 'o');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, assigned_empty_nonoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-n=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":n";
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, '=');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dash) {
    char * const argv[] = {
        CMDNAME,
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    /* POSIX specifies optind is not changed */
    cr_expect_eq(optind, 1);
}

Test(getopt, dashdash) {
    char * const argv[] = {
        CMDNAME,
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    /* POSIX specifies "shall return -1 after incrementing optind" */
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdash_opt) {
    char * const argv[] = {
        CMDNAME,
        "--",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    /* POSIX specifies "shall return -1 after incrementing optind" */
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, missing_optarg) {
    char * const argv[] = {
        CMDNAME,
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    /* POSIX seems to be clear that optind should be 3 in this case:
     *
     * If the option was the last character in the string pointed to by an
     * element of argv, then optarg shall contain the next element of argv, and
     * optind shall be incremented by 2.  If the resulting value of optind is
     * greater than argc, this indicates a missing option-argument, and
     * getopt() shall return an error indication.
     *
     * However, glibc returns 2.
     */
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, missing_optarg_colon) {
    char * const argv[] = {
        CMDNAME,
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":r:";
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    /* As in missing_optarg:  POSIX specifies 3, glibc sets 2 */
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, missing_optarg_dashdash) {
    char * const argv[] = {
        CMDNAME,
        "-r",
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, arg_before_option) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
#ifdef ULTRAGETOPT_OPTIONPERMUTE
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[1], orig_argv[1]);
    cr_expect_eq(argv[2], orig_argv[2]);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[1]);
    cr_expect_eq(argv[3], orig_argv[3]);
#else
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
#endif
}

Test(getopt, arg_before_dashdash_option) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
#ifdef ULTRAGETOPT_OPTIONPERMUTE
    cr_expect_eq(optind, 2);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[1]);
    cr_expect_eq(argv[3], orig_argv[3]);
#else
    cr_expect_eq(optind, 1);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[1]);
    cr_expect_eq(argv[2], orig_argv[2]);
    cr_expect_eq(argv[3], orig_argv[3]);
#endif
}

Test(getopt, arg_before_option_dashdash) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "-n",
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
#ifdef ULTRAGETOPT_OPTIONPERMUTE
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[1], orig_argv[1]);
    cr_expect_eq(argv[2], orig_argv[2]);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[3]);
    cr_expect_eq(argv[3], orig_argv[1]);
    cr_expect_eq(argv[4], orig_argv[4]);
#else
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
#endif
}

Test(getopt, arg_before_option_nopremute) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "+n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, arg_before_option_order) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
#ifdef ULTRAGETOPT_OPTIONPERMUTE
    cr_expect_eq(getopt(argc, argv, optstring), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#else
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
#endif
}

Test(getopt, arg_before_option_posix, .init = set_posixly_correct,
        .fini = unset_posixly_correct) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, adjacent_nooptarg_with_dash) {
    char * const argv[] = {
        CMDNAME,
        "-n-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":n";
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, '-');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, adjacent_optoptarg_with_dash) {
    char * const argv[] = {
        CMDNAME,
        "-o-o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":o::";
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, adjacent_reqoptarg_with_dash) {
    char * const argv[] = {
        CMDNAME,
        "-r-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":r:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, triplecolon) {
    char * const argv[] = {
        CMDNAME,
        "-:",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "o:::";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, ':');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, colon_optstring) {
    char * const argv[] = {
        CMDNAME,
        "-:",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, ':');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, doublecolon_optstring) {
    char * const argv[] = {
        CMDNAME,
        "-:",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "::";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optopt, ':');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, plus_opt) {
    char * const argv[] = {
        CMDNAME,
        "-+",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    /* Note:  optstring[0] == '+' requests permuting arguments for glibc */
    const char *optstring = "n+";
    cr_expect_eq(getopt(argc, argv, optstring), '+');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdash_as_opt) {
    char * const argv[] = {
        CMDNAME,
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdashopt) {
    char * const argv[] = {
        CMDNAME,
        "--n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n-";
    cr_expect_eq(getopt(argc, argv, optstring), '-');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdashoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n-:";
    cr_expect_eq(getopt(argc, argv, optstring), '-');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, equal_opt) {
    char * const argv[] = {
        CMDNAME,
        "-=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "=";
    cr_expect_eq(getopt(argc, argv, optstring), '=');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, equal_opt_assigned_empty_optarg) {
    char * const argv[] = {
        CMDNAME,
        "-==",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "=:";
    cr_expect_eq(getopt(argc, argv, optstring), '=');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, equal_opt_assigned_optarg) {
    char * const argv[] = {
        CMDNAME,
        "-===",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "=:";
    cr_expect_eq(getopt(argc, argv, optstring), '=');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

/* Test parsing of examples from SUSv3 (POSIX) */

Test(getopt, posix_example1_1) {
    char * const argv[] = {
        CMDNAME,
        "-ao",
        "arg",
        "path",
        "path",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":abf:o:";
    cr_expect_eq(getopt(argc, argv, optstring), 'a');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, posix_example1_2) {
    char * const argv[] = {
        CMDNAME,
        "-a",
        "-o",
        "arg",
        "path",
        "path",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":abf:o:";
    cr_expect_eq(getopt(argc, argv, optstring), 'a');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 4);
    cr_expect_eq(optarg, argv[3]);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 4);
}

/* Note:  Third sub-example has different option order.  Skip it. */

Test(getopt, posix_example1_4) {
    char * const argv[] = {
        CMDNAME,
        "-a",
        "-o",
        "arg",
        "--",
        "path",
        "path",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":abf:o:";
    cr_expect_eq(getopt(argc, argv, optstring), 'a');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 4);
    cr_expect_eq(optarg, argv[3]);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 5);
}

Test(getopt, posix_example1_5) {
    char * const argv[] = {
        CMDNAME,
        "-a",
        "-oarg",
        "path",
        "path",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":abf:o:";
    cr_expect_eq(getopt(argc, argv, optstring), 'a');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optarg, argv[2] + 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, posix_example1_6) {
    char * const argv[] = {
        CMDNAME,
        "-aoarg",
        "path",
        "path",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":abf:o:";
    cr_expect_eq(getopt(argc, argv, optstring), 'a');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt(argc, argv, optstring), 'o');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}
