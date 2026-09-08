#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

#define main lc_main
#include "lc.c"
#undef main

int
main(void)
{
	FILE *fp;
	unsigned long n;
	clock_t start, end;
	struct rusage r_start, r_end;
	int i;
	char *buf;
	size_t len;
	const char *line;
	size_t linelen;

	line = "This is a line of text to count for benchmarking.\n";
	linelen = strlen(line);
	len = 1000000 * linelen;
	buf = malloc(len);
	if (!buf)
		die("malloc:");

	/* Write 1M lines to the buffer */
	for (i = 0; i < 1000000; i++) {
		memcpy(buf + (i * linelen), line, linelen);
	}

	fp = fmemopen(buf, len, "r");
	if (!fp)
		die("fmemopen:");

	getrusage(RUSAGE_SELF, &r_start);
	start = clock();
	n = count_lines(fp, "tmpfile");
	end = clock();
	getrusage(RUSAGE_SELF, &r_end);

	printf("lines: %lu\n", n);
	printf("time: %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
	printf("memory: %ld max RSS (KB)\n", r_end.ru_maxrss);

	fclose(fp);
	free(buf);
	return 0;
}
