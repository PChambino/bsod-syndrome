#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "sprite.h"
#include "hammer.h"

typedef enum {NORMAL, OVER, CLICKED} ButtonState;

typedef struct {
	Sprite* sprite;
	ButtonState state;
} Button;

Button* newButton(int x, int y, char** over);

void drawButton(Button *button, char* buffer);

void updateButton(Button *button, Hammer *hammer);

void deleteButton(Button *button);

#endif
