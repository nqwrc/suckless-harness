#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"

char *argv0;

#include "t_arg_util.h"

static char *
pad_(const char *s)
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
	char *av[5];

	av[0] = pad_("prog");
	av[1] = pad_("-f");
	av[2] = pad_("Y");
	av[3] = pad_("z");
	av[4] = NULL;

	printf("expected: file=Y rest=1 : z\n");
	printf("actual:   ");
	run(4, av, " :");
	return 0;
}
