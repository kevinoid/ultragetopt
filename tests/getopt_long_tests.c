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
#include "testlib.h"
#include "ultragetopt_behavior.h"

#ifdef ULTRAGETOPT_ERROR_PROGNAME
# define ERR_PROGNAME ULTRAGETOPT_ERROR_PROGNAME
#else
# define ERR_PROGNAME argv[0]
#endif

TestSuite(getopt_long, .init = reset_getopt);

Test(getopt_long, noname_noopts) {
    char *argv[] = {
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, noname_opts) {
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
}

Test(getopt_long, name_opts) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt_long, noindex) {
    char *argv[] = {
        CMDNAME,
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    int index = 10;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), -1);
    cr_expect_eq(index, 10);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, index) {
    char *argv[] = {
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
    int index = 10;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), 'N');
    cr_expect_eq(index, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, shortopt) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt_long, default_optarg) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    optarg = "test";
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, default_optarg_withopt) {
    char *argv[] = {
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
    optarg = "test";
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, NULL);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, default_optopt) {
    char *argv[] = {
        CMDNAME,
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    int testoptopt = 1;
    optopt = testoptopt;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
#ifdef ULTRAGETOPT_DEFAULT_OPTOPT
    cr_expect_eq(optopt, ULTRAGETOPT_DEFAULT_OPTOPT);
#else
    cr_expect_eq(optopt, testoptopt);
#endif
}

Test(getopt_long, dash) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt_long, allargs) {
    char *argv[] = {
        CMDNAME,
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "-";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, dashdash_allargs) {
    char *argv[] = {
        CMDNAME,
        "arg1",
        "--",
        "arg2",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "-";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, colon_allargs) {
    char *argv[] = {
        CMDNAME,
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = ":-";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    // - is not respected if not first
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, allargs_colon) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "--reqarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "-:";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    // : is still respected if not first
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), ':');
    cr_expect_eq(optind, 3);
    cr_expect_eq(optopt, 'R');
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long, arg_before_option_allargs) {
    char *argv[] = {
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

Test(getopt_long, arg_before_option_allargs_nopermute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "-+";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    // Respect - and not +
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 1);
    cr_expect_eq(optarg, argv[1]);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt_long, arg_before_option_nopermute_allargs) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "+-";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *orig_argv[ARRAY_SIZE(argv)];
    memcpy(orig_argv, argv, sizeof argv);
    // Respect + and not -
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
    cr_expect_arr_eq(argv, orig_argv, sizeof argv);
}

Test(getopt_long, arg_before_option_posix, .init = set_posixly_correct,
        .fini = unset_posixly_correct) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt_long, respect_argc_nocmd) {
    char *argv[] = {
        CMDNAME,
        "--noarg",
        NULL
    };
    int argc = 0;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    /* glibc sets optind == 0, BSD sets optind == 1
     * This is enough of a corner-case that ultragetopt behavior is unspecified.
     */
    cr_expect(optind == 0 || optind == 1);
}

Test(getopt_long, respect_argc_noargs) {
    char *argv[] = {
        CMDNAME,
        "--noarg",
        NULL
    };
    int argc = 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 1);
}

Test(getopt_long, respect_argc_opt) {
    char *argv[] = {
        CMDNAME,
        "--noarg",
        "--reqarg",
        "optarg",
        NULL
    };
    int argc = 2;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, respect_argc_optarg) {
    char *argv[] = {
        CMDNAME,
        "--reqarg",
        "optarg",
        NULL
    };
    int argc = 2;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'R');
#else
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'R');
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
#endif
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    cr_expect_eq(optind, 2);
#else
    cr_expect_eq(optind, 3);
#endif
}

Test(getopt_long, respect_argc_nopermute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        NULL
    };
    int argc = 2;
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

Test(getopt_long, respect_argc_permute) {
    char *argv[] = {
        CMDNAME,
        "arg",
        "--noarg",
        "--noarg",
        NULL
    };
    int argc = 3;
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

Test(getopt_long, wsemi_noarg) {
    char *argv[] = {
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
    char *argv[] = {
        CMDNAME,
        "-Warg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {{0, 0, 0, 0}};
    int index = 10;
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), 'W');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), '?');
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(index, 10);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_separg) {
    char *argv[] = {
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
    char *argv[] = {
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
    int index = 10;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), 'O');
    cr_expect_eq(index, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_sepopt) {
    char *argv[] = {
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
    int index = 10;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), 'O');
    cr_expect_eq(index, 0);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

/* Note:  Choice of 'V' is arbitrary.  Just checking how non-W is handled. */
Test(getopt_long, vsemi_sepopt) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    int index = 10;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, &index), 'N');
    cr_expect_eq(index, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, wsemi_optarg) {
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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
    char *argv[] = {
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

Test(getopt_long, msg_invalid_shortopt,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'n');
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: invalid option -- 'n'\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: unknown option -- -n\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: illegal option -- -n\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_invalid_longopt,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "--noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: unrecognized option '--noarg'\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: unknown option -- --noarg\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: illegal option -- --noarg\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_invalid_wsemi,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-Wnoarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), 'W');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
#else
    char *buf;
    size_t bufsize;

    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: unrecognized option '-W noarg'\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: unknown option -- -W noarg\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: illegal option -- -W noarg\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
#endif

    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_ambig,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "--no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noa", no_argument, 0, 'A'},
        {"nob", no_argument, 0, 'B'},
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '--no' is ambiguous; possibilities:"
            " '--noa' '--nob'\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `--no' is ambiguous\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: ambiguous option -- --no\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_ambig_wsemi,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-Wno",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"noa", no_argument, 0, 'A'},
        {"nob", no_argument, 0, 'B'},
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
#if defined(__GNU_LIBRARY__) || defined(__GLIBC__)
    optind = 2; /* https://sourceware.org/bugzilla/show_bug.cgi?id=20342 */
#else
    cr_expect_eq(optind, 2);
#endif
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '-W no' is ambiguous\n", ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `-W no' is ambiguous\n", ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: ambiguous option -- -W no\n", ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_noarg,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "--noarg=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'N');
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '--noarg' doesn't allow an argument\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `--noarg' doesn't allow an argument\n",
            ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: option does not take an argument -- --noarg\n",
            ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_noarg_wsemi,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-Wnoarg=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '-W noarg' doesn't allow an argument\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `-W noarg' doesn't allow an argument\n",
            ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: option does not take an argument -- -W noarg\n",
            ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_reqarg,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
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
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 'R');
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '--reqarg' requires an argument\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `--reqarg' requires an argument\n",
            ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: option requires an argument -- --reqarg\n";
            ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long, msg_reqarg_wsemi,
        .init = init_capture, .fini = fini_capture) {
    char *argv[] = {
        CMDNAME,
        "-Wreqarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"reqarg", required_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    char *buf;
    size_t bufsize;
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optind, 2);
    cr_expect_eq(optopt, 0);
    buf = read_capture_str_stderr(&bufsize);
#ifdef ULTRAGETOPT_GNU_ERRORS
    cr_expect_str_eqf(buf, "%s: option '-W reqarg' requires an argument\n",
            ERR_PROGNAME);
#elif defined(ULTRAGETOPT_BSD_ERRORS)
    cr_expect_str_eqf(buf, "%s: option `-W reqarg' requires an argument\n",
            ERR_PROGNAME);
#else
    cr_expect_str_eqf(buf, "%s: option requires an argument -- -W reqarg\n";
            ERR_PROGNAME);
#endif
    if (buf) {
        free(buf);
    }
    cr_expect_eq(getopt_long(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}
