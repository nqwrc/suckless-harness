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
dup_(const char *s)
{
	char *p;

	p = malloc(strlen(s) + 1);
	if (!p) {
		perror("malloc");
		exit(1);
	}
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

int
main(void)
{
	char *av[5];
	int i;

	/* simulates:  prog -f Y z   with each string separately allocated */
	av[0] = dup_("prog");
	av[1] = dup_("-f");
	av[2] = dup_("Y");
	av[3] = dup_("z");
	av[4] = NULL;

	run(4, av);

	for (i = 0; i < 4; i++)
		free(av[i]);

	return 0;
}
