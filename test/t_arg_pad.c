#include "t_arg_fixture.h"

static char *
padstr(const char *s)
{
	/* zero-filled slack after the NUL: what a hardened/zeroing
	 * allocator or a differently-laid-out stack would give you */
	char *p = calloc(1, 16);
	memcpy(p, s, strlen(s));
	return p;
}

int
main(void)
{
	printf("expected: file=Y rest=1 : z\n");
	printf("actual:   ");
	run_with_allocator(padstr);
	return 0;
}
