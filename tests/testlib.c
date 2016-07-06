/* Utility functions for unit tests.
 *
 * This file is part of Ultragetopt.
 * It is licensed under the terms of the MIT License.
 * The complete text of the license is available in the project documentation.
 *
 * Copyright 2016 Kevin Locke <kevin@kevinlocke.name>
 */

#include "testlib.h"

#ifdef TEST_HOST_GETOPT
# include <getopt.h>
#else
# define ULTRAGETOPT_REPLACE_GETOPT 1
# include "ultragetopt.h"
#endif

#include <stdlib.h>


void reset_getopt(void) {
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

void set_posixly_correct(void) {
    setenv("POSIXLY_CORRECT", "1", 1);
}

void unset_posixly_correct(void) {
    unsetenv("POSIXLY_CORRECT");
}
