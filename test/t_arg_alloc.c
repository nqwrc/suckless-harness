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
alloc_(const char *s, int pad)
{
	char *p;

	if (pad) {
		/* zero-filled slack after the NUL: what a hardened/zeroing
		 * allocator or a differently-laid-out stack would give you */
		p = calloc(1, 16);
		memcpy(p, s, strlen(s));
	} else {
		p = malloc(strlen(s) + 1);
		memcpy(p, s, strlen(s) + 1);
	}
	return p;
}

static void
run(int argc, char *argv[], int pad)
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

	if (pad)
		printf("file=%s rest=%d :", file ? file : "(null)", argc);
	else
		printf("file=%s rest=%d", file ? file : "(null)", argc);

	for (i = 0; i < argc; i++)
		printf(" %s", argv[i]);
	printf("\n");
}

int
main(int argc, char *argv[])
{
	char *av[5];
	int i;
	int pad;

	pad = (argc > 1 && strcmp(argv[1], "pad") == 0);

	/* simulates:  prog -f Y z   with each string separately allocated */
	av[0] = alloc_("prog", pad);
	av[1] = alloc_("-f", pad);
	av[2] = alloc_("Y", pad);
	av[3] = alloc_("z", pad);
	av[4] = NULL;

	if (pad) {
		printf("expected: file=Y rest=1 : z\n");
		printf("actual:   ");
	}

	run(4, av, pad);

	for (i = 0; i < 4; i++) {
		free(av[i]);
	}
	return 0;
}
