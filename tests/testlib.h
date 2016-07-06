#ifndef INCLUDED_TESTLIB_H
#define INCLUDED_TESTLIB_H 1

#include <stddef.h> /* size_t */

#ifdef __cplusplus
extern "C" {
#endif

#define CMDNAME "testcmd"

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
