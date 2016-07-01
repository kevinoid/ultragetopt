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

TestSuite(getopt_long_only, .init = setup);

Test(getopt_long_only, shortopt) {
    char * const argv[] = {
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
    char * const argv[] = {
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
    char * const argv[] = {
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
    char * const argv[] = {
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

Test(getopt_long_only, longopt_match) {
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
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigshort) {
    char * const argv[] = {
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
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'n');
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigshorts) {
    char * const argv[] = {
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

Test(getopt_long_only, ambigshortarg) {
    char * const argv[] = {
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
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambiglong) {
    char * const argv[] = {
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
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, ambigshortassign) {
    char * const argv[] = {
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
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), 'N');
    cr_expect_eq(optarg, argv[1] + 3);
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, longestonly) {
    char * const argv[] = {
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
    char * const argv[] = {
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
    char * const argv[] = {
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

Test(getopt_long_only, nomatch) {
    char * const argv[] = {
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
    cr_expect_eq(optind, 2);
    cr_expect_eq(getopt_long_only(argc, argv, shortopts, longopts, NULL), -1);
    cr_expect_eq(optind, 2);
}

Test(getopt_long_only, nomatch_short) {
    char * const argv[] = {
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
    char * const argv[] = {
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
