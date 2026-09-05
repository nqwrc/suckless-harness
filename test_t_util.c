#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int
main(void)
{
	char *p, *q, *s;
	int i;

	/* emalloc */
	p = emalloc(10);
	memset(p, 'A', 10);
	if (p[0] != 'A' || p[9] != 'A') {
		fprintf(stderr, "emalloc: failed to write memory\n");
		return 1;
	}

	/* erealloc */
	p = erealloc(p, 20);
	memset(p + 10, 'B', 10);
	if (p[0] != 'A' || p[9] != 'A' || p[10] != 'B' || p[19] != 'B') {
		fprintf(stderr, "erealloc: failed to reallocate or preserve memory\n");
		return 1;
	}
	free(p);

	/* ecalloc */
	q = ecalloc(5, 4);
	if (q[0] != '\0' || q[19] != '\0') {
		fprintf(stderr, "ecalloc: did not zero memory\n");
		return 1;
	}
	free(q);

	/* estrdup */
	s = estrdup("test string");
	if (strcmp(s, "test string") != 0) {
		fprintf(stderr, "estrdup: failed to copy string\n");
		return 1;
	}
	free(s);

	printf("ok\n");
	return 0;
}
