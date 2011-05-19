#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#ifdef ULTRAGETOPT_REPLACE_GETOPT
#include "ultragetopt.h"
#endif

int main(int argc, char **argv)
{
    struct option *longopts;
    char *shortopts, *name, *cur;
    int c, i, count;
    int longind = -1;

    if (argc < 4) {
	printf("Usage:  %s <shortopts> <longopts> [args]\n", argv[0]);
	printf("<shortopts>\tString of shortopts to pass to getopt()\n");
	printf("<longopts>\tSpace-separated list of long options.\n"
	       "\t\tsame : notation for arguments as short options\n");
	return EXIT_FAILURE;
    }

    shortopts = argv[1];

    /* Count the number of long options passed */
    cur = argv[2];
    count = 2;	    /* For {0,0,0,0} */
    while (*cur)
	if (*cur++ == ' ')
	    count++;
    longopts = malloc(sizeof(struct option) * count);
    if (!longopts) {
	fprintf(stderr, "Unable to allocate memory for long options.\n");
	return EXIT_FAILURE;
    }

    name = strtok(argv[2], " ");
    for (i=0; name != NULL; i++) {
	size_t namelen = strlen(name);
	if (namelen > 0 && name[namelen-1] == ':') {
	    if (namelen > 1 && name[namelen-2] == ':') {
		longopts[i].has_arg = optional_argument;
		name[namelen-2] = '\0';
	    } else {
		longopts[i].has_arg = required_argument;
		name[namelen-1] = '\0';
	    }
	} else
	    longopts[i].has_arg = no_argument;

	longopts[i].name = name;
	longopts[i].flag = 0;
	longopts[i].val = name[0];

	name = strtok(NULL, " ");
    }
    assert(i < count);
    longopts[i].name = 0;
    longopts[i].has_arg = 0;
    longopts[i].flag = 0;
    longopts[i].val = 0;

    while ((c = getopt_long(argc-3, argv+3, shortopts, longopts,
			    &longind)) != -1) {
	switch (c) {
	    case 0:
		printf("Encountered option '%s' with arg '%s', flag set.\n",
		       longopts[longind].name, optarg);
		break;
	    case ':':
		printf("Encountered option '%c' missing argument\n", optopt);
		break;
	    case '?':
		printf("Encountered unexpected option '%c'\n", optopt);
		break;
	    default:
		printf("Encountered option '%c' with argument '%s'\n",
		       c, optarg);
		break;
	}

	printf("optind=%d\topterr=%d\toptopt=%d\toptarg='%s'\tlongind='%d'\n",
	       optind, opterr, optopt, optarg, longind);

	for (i=3; i<argc; i++)
	    printf("argv[%d] = %s\n", i-3, argv[i]);
    }

    /* Print any remaining command line arguments (not options). */
    if (optind+3 < argc) {
      printf("Remaining arguments:  ");
      while (optind+3 < argc) {
        printf("'%s' ", argv[optind+3]);
	optind++;
      }
      putchar('\n');
    }

    free(longopts);

    return EXIT_SUCCESS;
}
