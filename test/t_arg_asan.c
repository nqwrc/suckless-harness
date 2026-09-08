/* See LICENSE file for copyright and license details. */
#include "t_arg_fixture.h"

static char *
dupstr(const char *s)
{
	char *p = malloc(strlen(s) + 1);
	memcpy(p, s, strlen(s) + 1);
	return p;
}

int
main(void)
{
	/* simulates:  prog -f Y z   with each string separately allocated */
	run_with_allocator(dupstr);
	return 0;
}
