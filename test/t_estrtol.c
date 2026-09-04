#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int
main(int argc, char *argv[])
{
	long n;

	if (argc != 2) {
		fprintf(stderr, "usage: %s string\n", argv[0]);
		return 1;
	}

	n = estrtol(argv[1], 10);
	printf("%ld\n", n);
	return 0;
}
