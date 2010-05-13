#include "cscreen.h"

CScreen * newCScreen(int x, int y, int small){
	CScreen* cscreen = malloc(sizeof(CScreen));
	
	char **maps[] = {WINDOWS_BIG, BSOD_BIG, LINUX_BIG, MAC_BIG, DEATH_BIG};
	if (small) {
		maps[0] = WINDOWS_SMALL;
		maps[1] = BSOD_SMALL;
		maps[2] = LINUX_SMALL;
		maps[3] = MAC_SMALL;
		maps[4] = DEATH_SMALL;
	}
	
	cscreen->sprite = newSprite(x, y, maps, sizeof(maps)/sizeof(char*));
	
	Note notes[] = {{Mi6, 275},{Mi6, 250},{Mi6, 225},{Mi6, 200},
					{Mi6, 175},{Mi6, 150},{Mi6, 125},{Mi6, 100},
					{Mi6, 75},{Mi6, 50},{Mi6, 25},
					{Mi6, 1000}};
	cscreen->sound = newSong(50, notes, sizeof(notes)/sizeof(Note));
	
	cscreen->state = WINDOWS;
	
	cscreen->nextCrashTime = -1;
	
	return cscreen;
}

void deleteCScreen(CScreen* cscreen){
	deleteSprite(cscreen->sprite);
	deleteSong(cscreen->sound);
	free(cscreen);
}

void updateCScreen(CScreen *cscreen, Hammer *hammer, int *numberPCs, int *score, double mili){
	static int crashTimeMax = crashTimeMaxInitial;
	static int bsodTime = BSoDTime;
	static int savesUntilCD = savesUntilCDInitial;
	static int countedSavesUntilCD = savesUntilCDInitial;
	
	switch (cscreen->state) {
		case WINDOWS:
			if(cscreen->nextCrashTime == -1)
				cscreen->nextCrashTime = rand() % crashTimeMax;
			else {
				cscreen->nextCrashTime -= mili;
				if (cscreen->nextCrashTime <= 0) {
					cscreen->nextCrashTime = -1;
					cscreen->state = BSOD;
					cscreen->deathTime = bsodTime;
					cscreen->sprite->imgIndex++;
					play_song(cscreen->sound);
				}
			}
			// verificar colisoes com o cd
			break;
		case BSOD:
			cscreen->deathTime -= mili;
			if (cscreen->deathTime <= 0) {
				cscreen->state = DEATH;
				cscreen->sprite->imgIndex += 3;
				*score -= scoreDeath;
			}
			else if (hammer->state == HIT
				&& hammer->sprite->x >= cscreen->sprite->x && hammer->sprite->x <= cscreen->sprite->x + cscreen->sprite->width
				&& hammer->sprite->y + hammer->sprite->height >= cscreen->sprite->y
				&& hammer->sprite->y + hammer->sprite->height <= cscreen->sprite->y + cscreen->sprite->height) {
					cscreen->state = WINDOWS;
					cscreen->sprite->imgIndex--;
					*score += scoreSaved;
					countedSavesUntilCD--;
					if (countedSavesUntilCD <= 0) {
						countedSavesUntilCD = savesUntilCD;
						//hammer->state = CD;
					}
				}
			break;
		case REINSTALLED:
		case DEATH:
			numberPCs--;
			// aumentar dificuldade
			savesUntilCD++;
			crashTimeMax -= crashTimeDecrease;
			cscreen->state = INACTIVE;
			break;
		case INACTIVE:
			break;
		default:
			break;
	}
}

void drawCScreen(CScreen *cscreen, char *buffer){
	drawSprite(cscreen->sprite, buffer);
}
