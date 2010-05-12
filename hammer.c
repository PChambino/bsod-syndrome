
#include "hammer.h"

Hammer* newHammer() {
	Hammer* hammer = malloc(sizeof(Hammer));
	
	char **maps[] = {hammer_up, hammer_down};
	hammer->sprite = newSprite(HRES/2, VRES/2, maps, sizeof(maps)/sizeof(char*));
	
	hammer->sprite->x -= hammer->sprite->width/2;
	hammer->sprite->y -= hammer->sprite->height/2;
	
	Note notes[] = {{Fas3, 30}, {Fa2, 50}};
	hammer->sound = newSong(10, notes, sizeof(notes)/sizeof(Note));
	
	hammer->state = UP;
	
	return hammer;
}

void drawHammer(Hammer *hammer, char* buffer) {
	drawSpriteT(hammer->sprite, 15, buffer);
}

void updateHammer(Hammer *hammer, double mili, char key, Mouse *mouse) {
	static int delay = 0;
	int dx = 0, dy = 0;
	
	switch (hammer->state) {
		case UP:
			switch (key) {
				case RIGHT_KEY:
					dx = HAMMER_DELTA;
					break;
				case LEFT_KEY:
					dx = -HAMMER_DELTA;
					break;
				case UP_KEY:
					dy = HAMMER_DELTA;
					break;
				case DOWN_KEY:
					dy = -HAMMER_DELTA;
					break;
				case SPACE_KEY:
				case ENTER_KEY:
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
			play_song(hammer->sound);
			hammer->sprite->imgIndex = (hammer->sprite->imgIndex + 1) % hammer->sprite->imgs;
			delay = HAMMER_DELAY;
			hammer->state = DOWN;
			break;
		case DOWN:
			delay -= mili;
			if (delay <= 0) {
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
	deleteSong(hammer->sound);
	free(hammer);
}
