
#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <stdio.h>

#include "kbc.h"

/** @defgroup Mouse Mouse
 * @{
 *
 * Mouse related functions
 */

/** Mouse Event definition
*/
typedef struct {
	uchar *data; //< raw bytes read
	int yov, xov, ysig, xsig, mb, rb, lb, dx, dy; //< information parsed
} Mouse;

/** Parses mouse event
*/
int parse_mouse_event(GQueue *mouseQueue, Mouse *mouse);

/** Prints mouse event
*/
void print_mouse_event(Mouse *mouse);

//@}

#endif
