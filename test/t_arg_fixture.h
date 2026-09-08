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

static void
run_with_allocator(char *(*alloc)(const char *))
{
	char *av[5];
	int i;

	av[0] = alloc("prog");
	av[1] = alloc("-f");
	av[2] = alloc("Y");
	av[3] = alloc("z");
	av[4] = NULL;

	run(4, av);

	for (i = 0; i < 4; i++)
		free(av[i]);
}
