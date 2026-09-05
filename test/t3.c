#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

char *argv0;

static void
usage(void)
{
	fprintf(stderr, "usage\n");
	exit(1);
}

static char *
padstr(const char *s)
{
	/* zero-filled slack after the NUL: what a hardened/zeroing
	 * allocator or a differently-laid-out stack would give you */
	char *p;
	p = calloc(1, 16);
	if (p == NULL) {
		perror("calloc");
		exit(1);
	}
	memcpy(p, s, strlen(s));
	return p;
}

static void
run(int argc, char *argv[])
{
	char *file;
	int i;

	file = NULL;

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

int
main(void)
{
	char *av[5];

	av[0] = padstr("prog");
	av[1] = padstr("-f");
	av[2] = padstr("Y");
	av[3] = padstr("z");
	av[4] = NULL;

	printf("expected: file=Y rest=1 : z\n");
	printf("actual:   ");
	run(4, av);
	return 0;
}
