#ifndef _VIDEO_H_
#define _VIDEO_H_

#include <dpmi.h>
#include <sys/nearptr.h>
#include <unistd.h>
#include <stdlib.h>
#include <go32.h>
#include <pc.h>

#include "utypes.h"

/** @defgroup Video Video 
 * @{
 *
 * Entering/leaving/utility video functions
 */

#define BIT14ON 0x4000

#define INPUT_STATUS        0x03da
#define VRETRACE            0x08

extern int HRES;  /**< global variable with the horizontal resolution in pixels */
extern int VRES;  /**< global variable with the vertical resolution in pixels */

/** Enter graphics mode, enabling near pointers and mapping video physical memory
 * to program virtual address.
 *
 * Returns a generic pointer pointing to video memory address or NULL on error. 
 * "mode" specifies the VESA graphics mode to use, and
 * the mapping information is returned through "map".
 *
 * Also initializes two global variables, VRES and HRES,
 */
char * enter_graphics(int mode);

/** Unmap video memory, disable near pointer and returns to text mode
 */
void leave_graphics();

/** @name Set Pixel Functions
	Draws a pixel of color "color" at screen coordinates x, y at memory address "base"
*/
//@{
void set_pixelb(int x, int y, int color, char *base);
void set_pixel(int x, int y, int color);
//@}

/** @name Get Pixel Functions
	Returns the pixel color at screen coordinates x, y at memory address "base"
*/
//@{
int get_pixelb(int x, int y, char *base);
int get_pixel(int x, int y);
//@}

/** @name Clear Screen Functions
	Set graphics memory at memory address "base" to "color".
*/
//@{
void clear_screenb(char color, char *base);
void clear_screen(char color);
//@}

/** Draw a line of color "color" between point (xi,yi) and (xf,yf) at memory address "base"
*/
void draw_line(int xi, int yi, int xf, int yf, int color);

/** Waits for the screen vertical retrace.
*/
void wait_for_retrace();

/** @} end of video */ 

#endif
