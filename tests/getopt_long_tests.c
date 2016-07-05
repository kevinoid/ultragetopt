/* Unit tests for the getopt_long function.
 *
 * This file is part of Ultragetopt.
 * It is licensed under the terms of the MIT License.
 * The complete text of the license is available in the project documentation.
 *
 * Copyright 2016 Kevin Locke <kevin@kevinlocke.name>
 */

#include <criterion/criterion.h>

#ifdef TEST_HOST_GETOPT
# include <getopt.h>
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

TestSuite(getopt_long, .init = setup);

Test(getopt_long, noname_noopts) {
    char * const argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, noname_opts) {
    char * const argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, name_noopts) {
    char * const argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, name_opts) {
    char * const argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, arg) {
    char * const argv[] = {
        CMDNAME,
        "arg"
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, arg_dash) {
    char * const argv[] = {
        CMDNAME,
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, arg_dashdashdash) {
    char * const argv[] = {
        CMDNAME,
        "---",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, arg_dashdashdashopt) {
    char * const argv[] = {
        CMDNAME,
        "---",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"-", no_argument, 0, '-'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '-');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, opt) {
    char * const argv[] = {
        CMDNAME,
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, optflag) {
    char * const argv[] = {
        CMDNAME,
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    int flag = 0;
    const struct option longopts[] = {
        {"noarg", no_argument, &flag, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 0);
    cr_expect_eq(flag, 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, shortopt) {
    char * const argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, separate_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        "optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, separate_reqoptarg_dash) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, combined_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqargarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, combined_reqoptarg_dash) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_empty_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_optlike_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg=-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "nr:";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, spaced_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, spaced_empty_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg ",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, spaced_optlike_reqoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg -n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, separate_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--optarg",
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"optarg", optional_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, combined_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--optargarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"optarg", optional_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--optarg=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"optarg", optional_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_empty_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--optarg=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"optarg", optional_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_optlike_optoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--optarg=-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"optarg", optional_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optarg, argv[1] + 9);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_nonoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--noarg=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, assigned_empty_nonoptarg) {
    char * const argv[] = {
        CMDNAME,
        "--noarg=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, dash) {
    char * const argv[] = {
        CMDNAME,
        "-",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, dashdash) {
    char * const argv[] = {
        CMDNAME,
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, dashdash_opt) {
    char * const argv[] = {
        CMDNAME,
        "--",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, missing_optarg) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'R');
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, missing_optarg_colon) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = ":r:";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), ':');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'R');
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, missing_optarg_flag) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    int flag = 0;
    const struct option longopts[] = {
        {"reqarg", required_argument, &flag, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(flag, 0);
    cr_expect_eq(optind, 2);
#ifdef ULTRAGETOPT_OPTOPT_0_IF_FLAG
    cr_expect_eq(optopt, 0);
#else
    cr_expect_eq(optopt, 'R');
#endif
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, missing_optarg_dashdash) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = ":r:";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, missing_optarg_dashdash_opt) {
    char * const argv[] = {
        CMDNAME,
        "--reqarg",
        "--",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = ":r:";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 4);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 4);
}

Test(getopt_long, arg_before_option) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[1], orig_argv[1]);
    cr_expect_eq(argv[2], orig_argv[2]);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[1]);
    cr_expect_eq(argv[3], orig_argv[3]);
}

Test(getopt_long, arg_before_dashdash_option) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[1]);
    cr_expect_eq(argv[3], orig_argv[3]);
}

Test(getopt_long, arg_before_option_dashdash) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[1], orig_argv[1]);
    cr_expect_eq(argv[2], orig_argv[2]);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
    cr_expect_eq(argv[0], orig_argv[0]);
    cr_expect_eq(argv[1], orig_argv[2]);
    cr_expect_eq(argv[2], orig_argv[3]);
    cr_expect_eq(argv[3], orig_argv[1]);
    cr_expect_eq(argv[4], orig_argv[4]);
}

Test(getopt_long, arg_before_option_nopremute) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "+";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt_long, arg_before_option_order) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "-";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt_long, arg_before_option_posix, .init = set_posixly_correct,
        .fini = unset_posixly_correct) {
    char * const argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt_long, emptyopt) {
    char * const argv[] = {
        CMDNAME,
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"", no_argument, 0, 'E'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, emptyopt_assign) {
    char * const argv[] = {
        CMDNAME,
        "--=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"", required_argument, 0, 'E'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'E');
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, equalopt) {
    char * const argv[] = {
        CMDNAME,
        "--=",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"=", no_argument, 0, '='},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, '=');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, equalequalopt) {
    char * const argv[] = {
        CMDNAME,
        "--==",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"==", no_argument, 0, '='},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, '=');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, ambigshortopt) {
    char * const argv[] = {
        CMDNAME,
        "--n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n-";
    const struct option longopts[] = {
        {"n", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match) {
    char * const argv[] = {
        CMDNAME,
        "--no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_assign) {
    char * const argv[] = {
        CMDNAME,
        "--no=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, argv[1] + 5);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_assign_noarg) {
    char * const argv[] = {
        CMDNAME,
        "--no=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_exact_first) {
    char * const argv[] = {
        CMDNAME,
        "--no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"noarg", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_exact_last) {
    char * const argv[] = {
        CMDNAME,
        "--no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_ambig) {
    char * const argv[] = {
        CMDNAME,
        "--n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"noarg", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, match_ambig_assign) {
    char * const argv[] = {
        CMDNAME,
        "--n=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {"noarg", required_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, nomatch) {
    char * const argv[] = {
        CMDNAME,
        "--nob",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"noarg", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, nomatch_assign) {
    char * const argv[] = {
        CMDNAME,
        "--nob=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {"noarg", required_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_noarg) {
    char * const argv[] = {
        CMDNAME,
        "-W",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'W');
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_arg) {
    char * const argv[] = {
        CMDNAME,
        "-Warg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {{0, 0, 0, 0}};
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'W');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_separg) {
    char * const argv[] = {
        CMDNAME,
        "-W",
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {{0, 0, 0, 0}};
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'W');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
#else
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, wsemi_opt) {
    char * const argv[] = {
        CMDNAME,
        "-Wopt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_sepopt) {
    char * const argv[] = {
        CMDNAME,
        "-W",
        "opt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

/* Note:  Choice of 'V' is arbitrary.  Just checking how non-W is handled. */
Test(getopt_long, vsemi_sepopt) {
    char * const argv[] = {
        CMDNAME,
        "-V",
        "opt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "V;";
    const struct option longopts[] = {
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'V');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_assignopt) {
    char * const argv[] = {
        CMDNAME,
        "-W=opt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    /* Note:  The glibc behavior appears to match everything, so having
     * multiple long options results in "ambiguous match".  Having one long
     * option without argument causes "wrong argument" due to presence of "=".
     */
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optopt, 0);
#else
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optopt, 'O');
#endif
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_matchopt) {
    char * const argv[] = {
        CMDNAME,
        "-Wno",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_optarg) {
    char * const argv[] = {
        CMDNAME,
        "-Wreq",
        "optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, wsemi_optarg_assign) {
    char * const argv[] = {
        CMDNAME,
        "-Wreq=optarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[1] + 6);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_missingoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-Wreq",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    cr_expect_eq(optopt, 0);
#else
    cr_expect_eq(optopt, 'R');
#endif
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_dashdashoptarg) {
    char * const argv[] = {
        CMDNAME,
        "-Wreq",
        "--",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, optwsemi) {
    char * const argv[] = {
        CMDNAME,
        "-nWn",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "nW;";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, optwwsemi) {
    char * const argv[] = {
        CMDNAME,
        "-nWWn",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "nW;";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'W');
    cr_expect_eq(optarg, argv[1] + 3);
#else
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}
