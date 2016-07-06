#ifndef INCLUDED_TESTLIB_H
#define INCLUDED_TESTLIB_H 1

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

#define CMDNAME "testcmd"

/* Asserts that a string is equal to the result of string formatting on the
 * remaining arguments. */
#define cr_expect_str_eqf(actual, ...)                                        \
    do {                                                                      \
        char *cr_fmt_expected = NULL;                                         \
        cr_asprintf(&cr_fmt_expected, __VA_ARGS__);                           \
        cr_expect_str_eq(actual, cr_fmt_expected);                            \
        if (cr_fmt_expected) free(cr_fmt_expected);                           \
    } while (0)

void reset_getopt(void);

void set_posixly_correct(void);

void unset_posixly_correct(void);

void init_capture(void);

void fini_capture(void);

char *read_capture_str_stderr(size_t *bufsize);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_TESTLIB_H */
