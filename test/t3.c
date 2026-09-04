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

int
main(void)
{
	char *av[5];
	char *orig[5];
	int i;

	orig[0] = av[0] = pad_("prog");
	orig[1] = av[1] = pad_("-f");
	orig[2] = av[2] = pad_("Y");
	orig[3] = av[3] = pad_("z");
	orig[4] = av[4] = NULL;

	printf("expected: file=Y rest=1 : z\n");
	printf("actual:   ");
	run(4, av);
	for (i = 0; i < 4; i++)
		free(orig[i]);
	return 0;
}
