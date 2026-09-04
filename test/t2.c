#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arg.h"
#include "t_setup.h"

char *argv0;

static char *
dup_(const char *s)
{
	char *p = malloc(strlen(s) + 1);
	memcpy(p, s, strlen(s) + 1);
	return p;
}

int
main(void)
{
	char *av[5];

	/* simulates:  prog -f Y z   with each string separately allocated */
	av[0] = dup_("prog");
	av[1] = dup_("-f");
	av[2] = dup_("Y");
	av[3] = dup_("z");
	av[4] = NULL;

	run(4, av, 0);
	return 0;
}
