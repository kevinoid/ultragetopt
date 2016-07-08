/* Unit tests for the getopt_long_only function.
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

TestSuite(getopt_long_only, .init = reset_getopt);

Test(getopt_long_only, shortopt) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {{0, 0, 0, 0}};
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, longopt) {
    char *argv[] = {
        CMDNAME,
        "--no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, onlyopt) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, onlyopt_match) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, onlyopt_match1) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, longopt_match) {
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
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, same_short_long) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"n", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_2CHAR_ARG_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, same_shortarg_long) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"n", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_2CHAR_ARG_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'n');
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, same_short_longarg) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"n", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_2CHAR_ARG_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_matches_long) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_ambig_match_long) {
    char *argv[] = {
        CMDNAME,
        "-n",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"na", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, shorts_exact_long) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "no";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, shortarg_exact_long) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambig_long) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noa", no_argument, 0, 'A'},
        {"noarg", no_argument, 0, 'R'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, same_short_long_assign) {
    char *argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"n", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_matches_long_assign) {
    char *argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_1PREFIX_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, argv[1] + 3);
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_ambig_match_long_assign) {
    char *argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {"na", required_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_1PREFIX_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_matches_long_assign_badshort) {
    char *argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_1PREFIX_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
#endif
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_matches_long_assign_badlong) {
    char *argv[] = {
        CMDNAME,
        "-n=o",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_1PREFIX_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 'N');
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, longestonly) {
    char *argv[] = {
        CMDNAME,
        "-noarg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"noarg", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'A');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, bestfirst) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {"noarg", no_argument, 0, 'A'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, bestlast) {
    char *argv[] = {
        CMDNAME,
        "-no",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, shortlong) {
    char *argv[] = {
        CMDNAME,
        "-nno",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n";
    const struct option longopts[] = {
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, nomatch) {
    char *argv[] = {
        CMDNAME,
        "-nob",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", no_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, shorts_nomatch_long) {
    char *argv[] = {
        CMDNAME,
        "-nob",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "bno";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'o');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'b');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, shorts_some_nomatch_long) {
    char *argv[] = {
        CMDNAME,
        "-nob",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "no";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'o');
    cr_expect_eq(optind, 1);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, short_arg_nomatch_long) {
    char *argv[] = {
        CMDNAME,
        "-nob",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, nomatch_short_assign) {
    char *argv[] = {
        CMDNAME,
        "-nob=arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "n:";
    const struct option longopts[] = {
        {"noarg", no_argument, 0, 'A'},
        {"no", required_argument, 0, 'N'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optarg, argv[1] + 2);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigwsemi_noarg) {
    char *argv[] = {
        CMDNAME,
        "-W",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"W", no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_2CHAR_ARG_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'V');
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigwsemi_arg) {
    char *argv[] = {
        CMDNAME,
        "-Warg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"W", required_argument, 0, 'V'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_NOMATCH_W_AS_ARG
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'W');
    cr_expect_eq(optarg, argv[1] + 2);
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), '?');
    cr_expect_eq(optopt, 0);
#endif
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigwsemi_separg) {
    char *argv[] = {
        CMDNAME,
        "-W",
        "arg",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"W", required_argument, 0, 'V'},
        {0, 0, 0, 0}
    };
#ifdef ULTRAGETOPT_2CHAR_ARG_SHORT
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'W');
#else
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'V');
#endif
    cr_expect_eq(optarg, argv[2]);
    cr_expect_eq(optind, 3);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 3);
}

Test(getopt_long_only, ambigwsemi_opt) {
    char *argv[] = {
        CMDNAME,
        "-Wopt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"W", required_argument, 0, 'V'},
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'O');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigwsemi_assignopt) {
    char *argv[] = {
        CMDNAME,
        "-W=opt",
        NULL
    };
    int argc = ARRAY_SIZE(argv) - 1;
    const char *shortopts = "W;";
    const struct option longopts[] = {
        {"W", required_argument, 0, 'V'},
        {"opt", no_argument, 0, 'O'},
        {0, 0, 0, 0}
    };
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'V');
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}
