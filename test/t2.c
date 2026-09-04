/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

static char *dup_(const char *s);
static void run(int argc, char *argv[]);
static void usage(void);

char *argv0;

int
main(void)
{
	char *av[5];

	/* simulates:  prog -f Y z   with each string separately allocated */
	av[0] = dup_("prog");
	av[1] = dup_("-f");
	av[2] = dup_("Y");
	av[3] = dup_("z");
	av[4] = NULL;

	run(4, av);
	return 0;
}

static void
usage(void)
{
	fprintf(stderr, "usage\n");
	exit(1);
}

static char *
dup_(const char *s)
{
	char *p = malloc(strlen(s) + 1);
	memcpy(p, s, strlen(s) + 1);
	return p;
}

static void
run(int argc, char *argv[])
{
	char *file = NULL;
	int i;

	ARGBEGIN {
	case 'f':
		file = EARGF(usage());
		break;
	default:
		usage();
	} ARGEND;

	printf("file=%s rest=%d", file ? file : "(null)", argc);
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
}
