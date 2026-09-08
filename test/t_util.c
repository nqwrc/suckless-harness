#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

int
main(int argc, char *argv[])
{
	char *p, *q, *s;
	int i;

	if (argc > 1 && strcmp(argv[1], "die") == 0) {
		die("test:");
	}
	if (argc > 1 && strcmp(argv[1], "die_no_colon") == 0) {
		die("test");
	}

	/* test emalloc: allocate 10 bytes, expect successful write to boundaries */
	p = emalloc(10);
	memset(p, 'A', 10);
	if (p[0] != 'A' || p[9] != 'A') {
		fprintf(stderr, "emalloc: failed to write memory\n");
		free(p);
		return 1;
	}

	/* test erealloc: reallocate to 20 bytes, expect original memory preserved and successful write to new boundaries */
	p = erealloc(p, 20);
	memset(p + 10, 'B', 10);
	if (p[0] != 'A' || p[9] != 'A' || p[10] != 'B' || p[19] != 'B') {
		fprintf(stderr, "erealloc: failed to reallocate or preserve memory\n");
		free(p);
		return 1;
	}
	free(p);

	/* test ecalloc: allocate 5 elements of 4 bytes, expect all 20 bytes are zeroed */
	q = ecalloc(5, 4);
	for (i = 0; i < 20; i++) {
		if (q[i] != '\0') {
			fprintf(stderr, "ecalloc: did not zero memory\n");
			free(q);
			return 1;
		}
	}
	free(q);

	/* test estrdup: duplicate a string, expect identical contents */
	s = estrdup("test string");
	if (strcmp(s, "test string") != 0) {
		fprintf(stderr, "estrdup: failed to copy string\n");
		free(s);
		return 1;
	}
	free(s);

	printf("ok\n");
	return 0;
}
