#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int
main(void)
{
	char *p, *q, *s;
	int i;

	/* emalloc: test allocating 10 bytes and writing to the bounds. Expected: memory is allocated and writable. */
	p = emalloc(10);
	memset(p, 'A', 10);
	if (p[0] != 'A' || p[9] != 'A') {
		fprintf(stderr, "emalloc: failed to write memory (expected 'A' at bounds, got p[0]='%c', p[9]='%c')\n", p[0], p[9]);
		return 1;
	}

	/* erealloc: test reallocating to 20 bytes and writing to new bounds while preserving old data. Expected: old data is preserved, new data is writable. */
	p = erealloc(p, 20);
	memset(p + 10, 'B', 10);
	if (p[0] != 'A' || p[9] != 'A' || p[10] != 'B' || p[19] != 'B') {
		fprintf(stderr, "erealloc: failed to reallocate or preserve memory (expected 'A' at 0/9, 'B' at 10/19, got p[0]='%c', p[9]='%c', p[10]='%c', p[19]='%c')\n", p[0], p[9], p[10], p[19]);
		return 1;
	}
	free(p);

	/* ecalloc: test allocating 5 elements of 4 bytes. Expected: memory is allocated and zero-initialized. */
	q = ecalloc(5, 4);
	for (i = 0; i < 20; i++) {
		if (q[i] != '\0') {
			fprintf(stderr, "ecalloc: did not zero memory at index %d (expected '\\0', got '%c')\n", i, q[i]);
			return 1;
		}
	}
	free(q);

	/* estrdup: test duplicating a string. Expected: new string contains exact copy of original. */
	s = estrdup("test string");
	if (strcmp(s, "test string") != 0) {
		fprintf(stderr, "estrdup: failed to copy string (expected \"test string\", got \"%s\")\n", s);
		return 1;
	}
	free(s);

	printf("ok\n");
	return 0;
}
