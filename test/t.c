#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

char *argv0;

static void
usage(void)
{
	fprintf(stderr, "usage: %s [-v] [-f file]\n", argv0);
	exit(1);
}

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

	if (fflush(stdout) == EOF || ferror(stdout)) {
		fprintf(stderr, "stdout: error\n");
		exit(1);
	}

	return 0;
}
