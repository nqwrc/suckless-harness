/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

static void usage(void);

char *argv0;

int
main(int argc, char *argv[])
{
	char *file = NULL;
	int v = 0;
	int i;

	ARGBEGIN {
	case 'f':
		file = EARGF(usage());
		break;
	case 'v':
		v = 1;
		break;
	default:
		usage();
	} ARGEND;

	printf("v=%d file=%s rest=%d", v, file ? file : "(null)", argc);
	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
	return 0;
}

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-v] [-f file]\n", argv0);
	exit(1);
}
