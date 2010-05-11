
#include "hammer.h"

Hammer* newHammer() {
	Hammer* hammer = malloc(sizeof(Hammer));
	hammer->state = UP;
	
	char **maps[] = {hammer_up, hammer_down};
	hammer->sprite = newSprite(HRES/2, VRES/2, maps, sizeof(maps)/sizeof(char*));
	
	hammer->sprite->x -= hammer->sprite->width/2;
	hammer->sprite->y -= hammer->sprite->height/2;
	
	return hammer;
}

void drawHammer(Hammer *hammer, char* buffer) {
	drawSpriteT(hammer->sprite, 15, buffer);
}

void updateHammer(Hammer *hammer, double sec, char key, Mouse *mouse) {
	static int time = 0;
	int dx = 0, dy = 0;
	
	switch (hammer->state) {
		case UP:
			switch (key) {
				case 0x4d: // RIGHT
					dx = HAMMER_DELTA;
					break;
				case 0x4b: // LEFT
					dx = -HAMMER_DELTA;
					break;
				case 0x48: // UP
					dy = HAMMER_DELTA;
					break;
				case 0x50: // DOWN
					dy = -HAMMER_DELTA;
					break;
				case 0x1c: // ENTER
					hammer->state = HIT;
					break;
				default:
					break;
			}
			
			if (mouse) {
				dx = mouse->dx;
				dy = mouse->dy;
				if (mouse->lb)
					hammer->state = HIT;
			}

			moveSprite(hammer->sprite, dx, dy);
			break;
		case HIT:
			hammer->sprite->imgIndex = (hammer->sprite->imgIndex + 1) % hammer->sprite->imgs;
			time = HAMMER_DELAY;
			hammer->state = DOWN;
			break;
		case DOWN:
			if (time-- <= 0) {
				hammer->sprite->imgIndex = (hammer->sprite->imgIndex + 1) % hammer->sprite->imgs;
				hammer->state = UP;
			}
			break;
		default:
			break;
	}
}

void deleteHammer(Hammer* hammer) {
	deleteSprite(hammer->sprite);
	free(hammer);
}

/*void updateSprite(Sprite *sprite, double sec, char key, Mouse *mouse) {
	static int down = 0;
	static int time = 10;
	if (down) {
		if (time-- == 0) {
			sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
			down = 0;
			time = 100;
		}
	}

	if (key)
	 	switch (key) {
			case 0x4d: // RIGHT
				sprite->x++;
				break;
			case 0x4b: // LEFT
				sprite->x--;
				break;
			case 0x48: // UP
				sprite->y--;
				break;
			case 0x50: // DOWN
				sprite->y++;
				break;
			case 0x1c: // ENTER
				down = 1;
				sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
				break;
			default:
				break;
	 	}
	
	if (mouse) {
		//print_mouse_event(mouse);
		moveSprite(sprite, mouse->dx, mouse->dy);
		if (mouse->lb) {
			down = 1;
			sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
		}
	}
}
*/
