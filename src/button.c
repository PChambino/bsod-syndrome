
#include "button.h"

Button* newButton(int x, int y, char** over) {
	Button *b = malloc(sizeof(Button));
	
	char** map[] = {over};
	b->sprite = newSprite(x, y, map, 1);
	
	b->state = NORMAL;
	
	return b;
}

void deleteButton(Button *button) {
	deleteSprite(button->sprite);
	free(button);
}

void drawButton(Button *button, char* buffer) {
	if (button->state == OVER)
		drawSprite(button->sprite, buffer);
}

void updateButton(Button *button, Hammer *hammer) {
	if (collidesSprite(button->sprite, hammer->sprite)) {
		button->state = OVER;
		if (hammer->state == HIT || hammer->state == CD_HIT)
			button->state = CLICKED;
	} else
		button->state = NORMAL;
}
