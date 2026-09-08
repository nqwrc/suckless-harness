#include <X11/Xlib.h>
#include <stdlib.h>

Pixmap XCreatePixmap(Display *d, Drawable dr, unsigned int w, unsigned int h, unsigned int depth) {
    (void)d; (void)dr; (void)w; (void)h; (void)depth;
    return (Pixmap)1;
}

int XFreePixmap(Display *d, Pixmap p) {
    (void)d; (void)p;
    return 0;
}

GC XCreateGC(Display *d, Drawable dr, unsigned long valuemask, void *values) {
    (void)d; (void)dr; (void)valuemask; (void)values;
    return (GC)1;
}

int XFreeGC(Display *d, GC gc) {
    (void)d; (void)gc;
    return 0;
}

int XSetLineAttributes(Display *d, GC gc, unsigned int line_width, int line_style, int cap_style, int join_style) {
    (void)d; (void)gc; (void)line_width; (void)line_style; (void)cap_style; (void)join_style;
    return 0;
}

int XSetForeground(Display *d, GC gc, unsigned long foreground) {
    (void)d; (void)gc; (void)foreground;
    return 0;
}

int XFillRectangle(Display *d, Drawable dr, GC gc, int x, int y, unsigned int w, unsigned int h) {
    (void)d; (void)dr; (void)gc; (void)x; (void)y; (void)w; (void)h;
    return 0;
}

int XDrawRectangle(Display *d, Drawable dr, GC gc, int x, int y, unsigned int w, unsigned int h) {
    (void)d; (void)dr; (void)gc; (void)x; (void)y; (void)w; (void)h;
    return 0;
}

int XCopyArea(Display *d, Drawable src, Drawable dst, GC gc, int src_x, int src_y, unsigned int w, unsigned int h, int dst_x, int dst_y) {
    (void)d; (void)src; (void)dst; (void)gc; (void)src_x; (void)src_y; (void)w; (void)h; (void)dst_x; (void)dst_y;
    return 0;
}

int XSync(Display *d, Bool discard) {
    (void)d; (void)discard;
    return 0;
}

/* drw.h declares these but expects the project to define them.
 * We must provide mocks for them to link. */
#include "drw.h"

Fnt *drw_fontset_create(Drw *drw, const char *fonts[], size_t fontcount) {
    (void)drw; (void)fonts; (void)fontcount;
    return NULL;
}

void drw_fontset_free(Fnt *set) {
    (void)set;
}

Clr *drw_scm_create(Drw *drw, const char *clrnames[], size_t clrcount) {
    (void)drw; (void)clrnames; (void)clrcount;
    return NULL;
}

int drw_text(Drw *drw, int x, int y, unsigned int w, unsigned int h, unsigned int lpad, const char *text, int invert) {
    (void)drw; (void)x; (void)y; (void)w; (void)h; (void)lpad; (void)text; (void)invert;
    return 0;
}
