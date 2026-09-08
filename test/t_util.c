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
	if (p[0] != 'A' || p[9] != 'A')
		die("emalloc: failed to write memory");

	/* erealloc */
	p = erealloc(p, 20);
	memset(p + 10, 'B', 10);
	if (p[0] != 'A' || p[9] != 'A' || p[10] != 'B' || p[19] != 'B')
		die("erealloc: failed to reallocate or preserve memory");
	free(p);

	/* ecalloc */
	q = ecalloc(5, 4);
	for (i = 0; i < 20; i++) {
		if (q[i] != '\0')
			die("ecalloc: did not zero memory");
	}
	free(q);

	/* estrdup */
	s = estrdup("test string");
	if (strcmp(s, "test string") != 0)
		die("estrdup: failed to copy string");
	free(s);

	printf("ok\n");

	if (fflush(stdout) == EOF || ferror(stdout))
		die("stdout:");

	return 0;
}
