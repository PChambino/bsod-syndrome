
#include "hammer.h"

Hammer* newHammer() {
	Hammer* hammer = malloc(sizeof(Hammer));
	
	char **maps[] = {hammer_up, hammer_down, CD_pixmap};
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
	int dx = 0, dy = 0, hit_flag = 0;
	
	switch (hammer->state) {
		case GET_HAMMER:
			hammer->sprite->imgIndex = 0;
			hammer->state = UP;
			break;
		case GET_CD:
			hammer->sprite->imgIndex += 2;
			hammer->state = CD;
			break;
		case CD:
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
				case ENTER_KEY:
				case SPACE_KEY:
					hit_flag = 1;
					break;
				default:
					break;
			}
			
			if (mouse) {
				dx = mouse->dx;
				dy = mouse->dy;
				if (mouse->lb)
					hit_flag = 1;
			}

			if (hit_flag) {
				play_song(hammer->sound);

				if (hammer->state == UP) {
					hammer->state = HIT;
				} else {
					hammer->state = CD_HIT;
				}				
			}
			
			moveSprite(hammer->sprite, dx, dy);
			break;
		case HIT:
			hammer->sprite->imgIndex++;
			delay = HAMMER_DELAY;
			hammer->state = DOWN;
			break;
		case DOWN:
			delay -= mili;
			if (delay <= 0) {
				hammer->sprite->imgIndex--;
				hammer->state = UP;
			}
			break;
		case CD_HIT:
			delay = HAMMER_DELAY;
			hammer->state = CD_DOWN;
			break;
		case CD_DOWN:
			delay -= mili;
			if (delay <= 0) {
				hammer->state = CD;
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
