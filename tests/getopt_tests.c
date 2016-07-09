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
#include "testlib.h"
#include "ultragetopt_behavior.h"

#ifdef ULTRAGETOPT_ERROR_PROGNAME
# define ERR_PROGNAME ULTRAGETOPT_ERROR_PROGNAME
#else
# define ERR_PROGNAME argv[0]
#endif

TestSuite(getopt, .init = reset_getopt);

Test(getopt, noname_noopts) {
    char *argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, noname_opts) {
    char *argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, name_noopts) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, name_opts) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
}

Test(getopt, arg) {
    char *argv[] = {
        CMDNAME,
        "arg"
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt, arg_dash) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        "-rarg",
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
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        "-r=arg",
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt, spaced_reqoptarg) {
    char *argv[] = {
        CMDNAME,
        "-r arg",
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

Test(getopt, spaced_empty_reqoptarg) {
    char *argv[] = {
        CMDNAME,
        "-r ",
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

Test(getopt, spaced_optlike_reqoptarg) {
    char *argv[] = {
        CMDNAME,
        "-r -n",
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
    char *argv[] = {
        CMDNAME,
        "-o",
        "arg",
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
    char *argv[] = {
        CMDNAME,
        "-oarg",
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
    char *argv[] = {
        CMDNAME,
        "-o=arg",
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt, default_optarg) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    optarg = "test";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 1);
}

Test(getopt, default_optopt) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    optopt = 1;
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
#ifdef ULTRAGETOPT_DEFAULTOPTOPT
    cr_expect_eq(optopt, ULTRAGETOPT_DEFAULTOPTOPT);
#else
    cr_expect_eq(optopt, 0);
#endif
}

Test(getopt, dash) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":r:";
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    /* As in missing_optarg:  POSIX specifies 3, everyone sets 2 */
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, missing_optarg_dashdash) {
    char *argv[] = {
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

Test(getopt, missing_optarg_dashdash_opt) {
    char *argv[] = {
        CMDNAME,
        "-r",
        "--",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":nr:";
    cr_expect_eq(getopt(argc, argv, optstring), 'r');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 4);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 4);
}

Test(getopt, arg_before_option) {
    char *argv[] = {
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
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#endif
}

Test(getopt, arg_before_dashdash_option) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt, arg_before_option_colon_nopremute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":+r:";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
#ifdef ULTRAGETOPT_OPTIONPERMUTE
    // + is not respected if it is not first
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[1]);
#else
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    optind = 2;
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 'r');
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#endif
}

Test(getopt, arg_before_option_nopermute_colon) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "+:r:";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    optind = 2;
    // : is still respected if not first
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, order) {
    char *argv[] = {
        CMDNAME,
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-";
    cr_expect_eq(getopt(argc, argv, optstring), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdash_order) {
    char *argv[] = {
        CMDNAME,
        "arg1",
        "--",
        "arg2",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-";
    cr_expect_eq(getopt(argc, argv, optstring), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, colon_order) {
    char *argv[] = {
        CMDNAME,
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":-";
    // - is not respected if not first
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt, order_colon) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-:r:";
    cr_expect_eq(getopt(argc, argv, optstring), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    // : is still respected if not first
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt, arg_before_option_order_nopremute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "-+n";
    char *orig_argv[ARRAY_SIZE(argv)];
    // Respect - and not +
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 3);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, arg_before_option_nopermute_order) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "+-n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    // Respect + and not -
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, arg_before_option_order) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        "-+",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    /* Note:  optstring[0] == '+' requests not permuting arguments for glibc */
    const char *optstring = "n+";
    cr_expect_eq(getopt(argc, argv, optstring), '+');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, dashdash_as_opt) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt, optind0_cmd) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    optind = 0;
#if defined(ULTRAGETOPT_ALLOW_OPTIND0) && defined(ULTRAGETOPT_OPTIONPERMUTE)
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    cr_expect_eq(argv[0], orig_argv[1]);
    cr_expect_eq(argv[1], orig_argv[0]);
    cr_expect_eq(argv[2], orig_argv[2]);
#elif defined(ULTRAGETOPT_ALLOW_OPTIND0)
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 0);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#else
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#endif
}

Test(getopt, optind0_dashcmd) {
    char *argv[] = {
        /* Note:  This occurs for login shells. */
        "-bash",
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    optind = 0;
#ifdef ULTRAGETOPT_ALLOW_OPTIND0
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 0);
    cr_expect_eq(optopt, 'b');
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 0);
    cr_expect_eq(optopt, 'a');
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 0);
    cr_expect_eq(optopt, 's');
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 1);
    cr_expect_eq(optopt, 'h');
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
#else
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
#endif
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, respect_argc_nocmd) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = 0;
    const char *optstring = "n:";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    /* glibc sets optind == 0, BSD sets optind == 1
     * This is enough of a corner-case that ultragetopt behavior is unspecified.
     */
    cr_expect(optind == 0 || optind == 1);
}

Test(getopt, respect_argc_noargs) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = 1;
    const char *optstring = "n:";
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt, respect_argc_opt) {
    char *argv[] = {
        CMDNAME,
        "-n",
        "-r",
        "optarg",
        NULL
    };
    int argc = 2;
    const char *optstring = "nr:";
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, respect_argc_optarg) {
    char *argv[] = {
        CMDNAME,
        "-r",
        "optarg",
        NULL
    };
    int argc = 2;
    const char *optstring = "r:";
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, respect_argc_nopermute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-n",
        NULL
    };
    int argc = 2;
    const char *optstring = "n";
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt, respect_argc_permute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "-n",
        "-n",
        NULL
    };
    int argc = 3;
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
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
#endif
}

Test(getopt, wsemi_noarg) {
    char *argv[] = {
        CMDNAME,
        "-W",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "W;";
    cr_expect_eq(getopt(argc, argv, optstring), 'W');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, wsemi_arg) {
    char *argv[] = {
        CMDNAME,
        "-W",
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "W;";
    cr_expect_eq(getopt(argc, argv, optstring), 'W');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

/* Note:  Choice of 'V' is arbitrary.  Just checking how non-W is handled. */
Test(getopt, vsemi_noarg) {
    char *argv[] = {
        CMDNAME,
        "-V",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "V;";
    cr_expect_eq(getopt(argc, argv, optstring), 'V');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, vsemi_arg) {
    char *argv[] = {
        CMDNAME,
        "-V",
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "V;";
    cr_expect_eq(getopt(argc, argv, optstring), 'V');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, msg_invalid_opt, .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-a",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 2);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: invalid option -- 'a'\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: illegal option -- a\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: unknown option -- a\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, msg_missing_arg, .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "r:";
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option requires an argument -- 'r'\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option requires an argument -- r\n",
            ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: option requires an argument -- r\n",
            ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, msg_none_opterr, .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-a",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "";
    char *buf;
    size_t bufsize;
    opterr = 0;
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 2);
    buf = read_capture_str_stderr(&bufsize);
    cr_expect_str_eq(buf, "");
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, msg_none_colon, .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-r",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = ":r:";
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt(argc, argv, optstring), ':');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'r');
    buf = read_capture_str_stderr(&bufsize);
    cr_expect_str_eq(buf, "");
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt, msg_longopt, .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "--n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *optstring = "n";
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt(argc, argv, optstring), '?');
    cr_expect_eq(optind, 1);
    cr_expect_eq(optopt, '-');
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: invalid option -- '-'\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: illegal option -- -\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: unknown option -- -\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt(argc, argv, optstring), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt(argc, argv, optstring), -1);
    cr_expect_eq(optind, 2);
}

/* Test parsing of examples from SUSv3 (POSIX) */

Test(getopt, posix_example1_1) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
