#ifndef INCLUDED_TESTLIB_H
#define INCLUDED_TESTLIB_H 1

#ifdef __cplusplus
extern "C" {
#endif

extern char *const CMDNAME;

void reset_getopt(void);

void set_posixly_correct(void);

void unset_posixly_correct(void);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDED_TESTLIB_H */
