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

	/* emalloc */
	p = emalloc(10);
	memset(p, 'A', 10);
	if (p[0] != 'A' || p[9] != 'A') {
		fprintf(stderr, "emalloc: failed to write memory\n");
		goto err_p;
	}
	free(p);

	/* emalloc 0 */
	p = emalloc(0);
	if (p)
		free(p);

	p = emalloc(10);
	memset(p, 'A', 10);

	/* erealloc */
	p = erealloc(p, 20);
	memset(p + 10, 'B', 10);
	if (p[0] != 'A' || p[9] != 'A' || p[10] != 'B' || p[19] != 'B') {
		fprintf(stderr, "erealloc: failed to reallocate or preserve memory\n");
		goto err_p;
	}
	free(p);
	p = NULL;

	/* ecalloc */
	q = ecalloc(5, 4);
	for (i = 0; i < 20; i++) {
		if (q[i] != '\0') {
			fprintf(stderr, "ecalloc: did not zero memory\n");
			goto err_q;
		}
	}
	free(q);
	q = NULL;

	/* ecalloc 0 */
	q = ecalloc(0, 0);
	if (q) free(q);

	q = ecalloc(0, 5);
	if (q) free(q);

	q = ecalloc(5, 0);
	if (q) free(q);

	/* estrdup */
	s = estrdup("test string");
	if (strcmp(s, "test string") != 0) {
		fprintf(stderr, "estrdup: failed to copy string\n");
		goto err_s;
	}
	free(s);

	/* estrdup empty */
	s = estrdup("");
	if (strcmp(s, "") != 0) {
		fprintf(stderr, "estrdup: failed to copy empty string\n");
		goto err_s;
	}
	free(s);

	printf("ok\n");
	return 0;

err_s:
	free(s);
	return 1;
err_q:
	free(q);
	return 1;
err_p:
	free(p);
	return 1;
}
