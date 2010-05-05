
#include "mouse.h"

int parse_mouse_event(GQueue *mouseQueue, Mouse *mouse) {
	mouse->data = getGQueue(mouseQueue);
	
	if (mouse->data == NULL)
		return 0;
		
	mouse->yov = Y_OVERFLOW(mouse->data[0]);
	mouse->xov = X_OVERFLOW(mouse->data[0]);
	mouse->ysig = Y_SIGN(mouse->data[0]);
	mouse->xsig = X_SIGN(mouse->data[0]);
	mouse->mb = MIDDLE_BUTTON(mouse->data[0]);
	mouse->lb = LEFT_BUTTON(mouse->data[0]);
	mouse->rb = RIGHT_BUTTON(mouse->data[0]);
	mouse->dx = (mouse->xsig == 1)? mouse->data[1] : (mouse->data[1] - 256);
	mouse->dy = (mouse->ysig == 1)? mouse->data[2] : (mouse->data[2] - 256);
	
	return 1;
}

void print_mouse_event(Mouse *mouse) {
	printf("%x %x %x LB=%d MB=%d RB=%d XOV=%d YOV=%d dX=%d dY=%d\n",
		mouse->data[0], mouse->data[1], mouse->data[2],
		mouse->lb, mouse->mb, mouse->rb,
		mouse->xov, mouse->yov, mouse->dx, mouse->dy);
}
