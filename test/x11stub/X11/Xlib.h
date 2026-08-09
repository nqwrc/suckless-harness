#ifndef FAKE_XLIB_H
#define FAKE_XLIB_H
typedef struct Display Display;
typedef unsigned long XID, Window, Drawable, Pixmap, Colormap;
typedef struct Visual Visual;
typedef struct _GC *GC;
typedef int Bool;
#define False 0
#define LineSolid 0
#define CapButt 1
#define JoinMiter 0
#define DefaultVisual(d,s) ((Visual *)0)
#define DefaultColormap(d,s) ((Colormap)0)
#define DefaultDepth(d,s) (24)
extern Pixmap XCreatePixmap(Display *, Drawable, unsigned int, unsigned int, unsigned int);
extern int XFreePixmap(Display *, Pixmap);
extern GC XCreateGC(Display *, Drawable, unsigned long, void *);
extern int XFreeGC(Display *, GC);
extern int XSetLineAttributes(Display *, GC, unsigned int, int, int, int);
extern int XSetForeground(Display *, GC, unsigned long);
extern int XFillRectangle(Display *, Drawable, GC, int, int, unsigned int, unsigned int);
extern int XDrawRectangle(Display *, Drawable, GC, int, int, unsigned int, unsigned int);
extern int XCopyArea(Display *, Drawable, Drawable, GC, int, int, unsigned int, unsigned int, int, int);
extern int XSync(Display *, Bool);
#endif
