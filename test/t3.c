/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

static char *pad_(const char *s);
static void run(int argc, char *argv[]);
static void usage(void);

char *argv0;

int
main(void)
{
	char *av[5];

	av[0] = pad_("prog");
	av[1] = pad_("-f");
	av[2] = pad_("Y");
	av[3] = pad_("z");
	av[4] = NULL;

	printf("expected: file=Y rest=1 : z\n");
	printf("actual:   ");
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
pad_(const char *s)
{
	/* zero-filled slack after the NUL: what a hardened/zeroing
	 * allocator or a differently-laid-out stack would give you */
	char *p = calloc(1, 16);
	memcpy(p, s, strlen(s));
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

	printf("file=%s rest=%d :", file ? file : "(null)", argc);
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
}
