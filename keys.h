#ifndef _KEYS_H_
#define _KEYS_H_

#include "video.h"

/** @defgroup Keys Keys
 * @{
 *
 * Keys related functions
 */
 
/** @name Special Keys Scancodes
*/
//@{
#define RELEASED 		0x80 //< Released bit

#define ESC_KEY 		0x01
#define TAB_KEY 		0x0F
#define ENTER_KEY  		0x1C
#define SPACE_KEY  		0x39
#define BACKSPACE_KEY 	0x0E
#define SHIFT_KEY		0x2A

#define RIGHT_KEY 		0x4D
#define LEFT_KEY 		0x4B
#define UP_KEY 			0x48
#define DOWN_KEY 		0x50
//@}

/** @name Char Size on Screen
*/
//@ {
extern const int charWidth;
extern const int charHeight;
//@}

/** Gets the character of the scancode.
	@return the ascii character matching the scancode
*/
char scancodeToAscii(char sc);

/** Draws a character in the buffer.
*/
void drawChar(char c,int x, int y, int color, int scale, char *buffer);

/** Draws a string in the buffer.
	Uses drawChar()
*/
void drawString(char* str,int x, int y, int color, int scale, char *buffer);

//@}

#endif
