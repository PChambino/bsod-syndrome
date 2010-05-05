
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdio.h>

#include "kbc.h"

typedef struct {
	uchar *data;
	int yov, xov, ysig, xsig, mb, rb, lb, dx, dy;
} Mouse;

int parse_mouse_event(GQueue *mouseQueue, Mouse *mouse);

void print_mouse_event(Mouse *mouse);

#endif
