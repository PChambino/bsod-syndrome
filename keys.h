#ifndef _KEYS_H_
#define _KEYS_H_

#include "video.h"

#define RELEASED 		0x80

#define ESC_KEY 		0x01
#define TAB_KEY 		0x0F
#define ENTER_KEY  		0x1C
#define SPACE_KEY  		0x39
#define BACKSPACE_KEY 	0x0E

#define RIGHT_KEY 		0x4D
#define LEFT_KEY 		0x4B
#define UP_KEY 			0x48
#define DOWN_KEY 		0x50

extern const int charWidth;
extern const int charHeight;

char scancodeToAscii(char sc);

void drawChar(char c,int x, int y, int color, int scale, char *buffer);

void drawString(char* str,int x, int y, int color, int scale, char *buffer);

#endif
