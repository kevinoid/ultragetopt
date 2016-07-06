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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/** Read offset for stderr capture. */
static off_t errreadoff = 0;
/** dup(2) of stderr from before capture. */
static int origerrfd = -1;
/** Position of stderr from before capture. */
static fpos_t origerrpos;

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

/** Opens a file descriptor which can be used to capture output.
 *
 * TODO:  Ideally this would use memfd_create(2) when available (Linux) or
 * shm_open(2) with SHM_ANON (BSD).
 */
static int open_capture_sink(void) {
#ifdef O_TMPFILE
    return open(P_tmpdir, O_TMPFILE | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
#else
    char path[] = P_tmpdir "/ultragetopt_capture_XXXXXX";
    int fd = mkstemp(path);
    if (fd != -1) {
        unlink(path);
    }
    return fd;
#endif
}

/** Starts capturing output written to a given file stream (or directly to its
 * underlying file descriptor).
 * @param stream Stream to capture.
 * @param origpos Receives the position of the stream before capturing.
 * @return File descriptor dup(2) of the stream's underlying file descriptor
 * before capturing, or -1 on error.
 */
static int start_capture_stream(FILE *stream, fpos_t *origpos) {
    int capturefd, origfd, streamfd;

    streamfd = fileno(stream);
    if (streamfd == -1) {
        return streamfd;
    }

    if (fflush(stream) == EOF) {
        return -1;
    }
    fgetpos(stream, origpos);

    capturefd = open_capture_sink();
    if (capturefd == -1) {
        return capturefd;
    }

    origfd = dup(streamfd);
    if (origfd == -1) {
        close(capturefd);
        return origfd;
    }

    clearerr(stream);
    if (dup2(capturefd, streamfd) == -1) {
        close(origfd);
        close(capturefd);
        return -1;
    }

    close(capturefd);
    return origfd;
}

/** Stops capturing output written to a given file stream and restore its
 * original file descriptor.
 * @param stream Stream being captured.
 * @param origfd Underlying file descriptor from before capture.
 * @param origpos Position of the stream before capturing.
 * @return 0 on success.  -1 on failure.
 */
static int stop_capture_stream(FILE *stream, int origfd,
        const fpos_t *origpos) {
    int ret;
    int streamfd = fileno(stream);
    if (streamfd == -1) {
        return streamfd;
    }

    fflush(stream);
    ret = dup2(origfd, streamfd);
    if (ret != -1) {
        ret = fsetpos(stream, origpos);
    }
    return ret;
}

/** Reads output from the file descriptor for a stream being captured as a
 * string.
 * @param stream Stream being captured.
 * @param readoff Offset from which to read.  Receives offset read up to.
 * @param bufsize Receives size of returned buffer (string length + 1)
 * @return Captured data since last read with added NUL terminator.
 */
static char *read_capture_str(FILE *stream, off_t *readoff, size_t *bufsize) {
    char *buf;
    size_t datasize;
    ssize_t readsize;
    int streamfd = fileno(stream);
    off_t writeoff;

    if (streamfd == -1) {
        return NULL;
    }

    if (fflush(stream) == EOF) {
        return NULL;
    }

    writeoff = lseek(streamfd, 0, SEEK_CUR);
    if (writeoff == -1 || writeoff < *readoff) {
        return NULL;
    }

    datasize = (size_t)(writeoff - *readoff);
    *bufsize = datasize + 1;
    buf = malloc(*bufsize);
    if (buf == NULL) {
        return NULL;
    }

    if (lseek(streamfd, *readoff, SEEK_SET) == -1) {
        free(buf);
        return NULL;
    }

    readsize = read(streamfd, buf, datasize);
    if (readsize == -1 || (size_t)readsize != datasize) {
        lseek(streamfd, writeoff, SEEK_SET);
        free(buf);
        return NULL;
    }

    buf[readsize] = '\0';
    *readoff = writeoff;
    return buf;
}

char *read_capture_str_stderr(size_t *bufsize) {
    if (origerrfd == -1) {
        return NULL;
    }

    return read_capture_str(stderr, &errreadoff, bufsize);
}

void init_capture(void) {
    opterr = 1;
    if (origerrfd == -1) {
        origerrfd = start_capture_stream(stderr, &origerrpos);
        errreadoff = 0;
    }
}

void fini_capture(void) {
    opterr = 0;
    if (origerrfd != -1) {
        stop_capture_stream(stderr, origerrfd, &origerrpos);
        origerrfd = -1;
    }
}
