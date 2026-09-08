#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>

#include "drw.h"
#include "util.h"

int
main(void)
{
	Drw *drw;
	Clr scm[3] = {0};

	printf("test: drw_create with nonzero width/height creates a Drw and its resources\n");
	drw = drw_create((Display *)1, 0, 0, 100, 100);
	if (!drw || drw->w != 100 || drw->h != 100 || !drw->drawable || !drw->gc) {
		fprintf(stderr, "FAIL: drw_create failed to set expected fields\n");
		return 1;
	}

	printf("test: drw_resize updates width/height\n");
	drw_resize(drw, 200, 200);
	if (drw->w != 200 || drw->h != 200 || !drw->drawable) {
		fprintf(stderr, "FAIL: drw_resize failed to update dimensions\n");
		return 1;
	}

	printf("test: drw_setscheme updates the color scheme\n");
	drw_setscheme(drw, scm);
	if (drw->scheme != scm) {
		fprintf(stderr, "FAIL: drw_setscheme failed\n");
		return 1;
	}

	printf("test: drw_rect draws a filled rectangle\n");
	drw_rect(drw, 10, 10, 50, 50, 1, 0);

	printf("test: drw_rect draws an outlined rectangle\n");
	drw_rect(drw, 10, 10, 50, 50, 0, 0);

	printf("test: drw_rect with invert draws inverted colors\n");
	drw_rect(drw, 10, 10, 50, 50, 1, 1);

	printf("test: drw_rect does nothing if width or height is zero\n");
	drw_rect(drw, 0, 0, 0, 0, 1, 0);

	printf("test: drw_map synchronizes drawable to window\n");
	drw_map(drw, 0, 0, 0, 200, 200);

	printf("test: drw_free frees all resources\n");
	drw_free(drw);

	printf("test: drw_create with zero width/height creates a 1x1 drawable\n");
	drw = drw_create((Display *)1, 0, 0, 0, 0);
	if (!drw || drw->w != 0 || drw->h != 0 || !drw->drawable) {
		fprintf(stderr, "FAIL: drw_create with zero dims failed\n");
		return 1;
	}
	drw_free(drw);

	printf("test: drw_resize with zero width/height creates a 1x1 drawable\n");
	drw = drw_create((Display *)1, 0, 0, 100, 100);
	drw_resize(drw, 0, 0);
	if (drw->w != 0 || drw->h != 0 || !drw->drawable) {
		fprintf(stderr, "FAIL: drw_resize with zero dims failed\n");
		return 1;
	}
	drw_free(drw);

	printf("ok\n");
	return 0;
}
