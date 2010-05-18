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
	cscreen->crash = newSong(50, notes, sizeof(notes)/sizeof(Note));
	
	Note notes1[] = {{Mi3, 150}, {Fa3, 150}, {Sol3, 150}, {Re3, 150}, 
					 {Do6, 250}, {Do6, 250}};
	cscreen->reinstall = newSong(50, notes1, sizeof(notes1)/sizeof(Note));
	
	cscreen->state = WINDOWS;
	
	cscreen->nextCrashTime = -1;
	
	return cscreen;
}

void deleteCScreen(CScreen* cscreen){
	deleteSprite(cscreen->sprite);
	deleteSong(cscreen->crash);
	deleteSong(cscreen->reinstall);
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
					play_song(cscreen->crash);
				}
			}
			// verificar colisoes com o cd
			if (hammer->state == CD_HIT && collidesSprite(hammer->sprite, cscreen->sprite))
				cscreen->state = REINSTALLED;
			break;
		case BSOD:
			cscreen->deathTime -= mili;
			if (cscreen->deathTime <= 0) {
				cscreen->state = DEATH;
				cscreen->sprite->imgIndex += 3;
				*score -= scoreDeath;
			}
			else if (hammer->state == HIT
			&& collidesSpriteRect(cscreen->sprite,
			hammer->sprite->x, hammer->sprite->y + hammer->sprite->height/2,
			hammer->sprite->width/2, hammer->sprite->height/2))
			{
				cscreen->state = WINDOWS;
				cscreen->sprite->imgIndex--;
				*score += scoreSaved;
				countedSavesUntilCD--;
				if (countedSavesUntilCD <= 0) {
					countedSavesUntilCD = savesUntilCD;
					hammer->state = GET_CD;
				}
			} else if (hammer->state == CD_HIT && collidesSprite(hammer->sprite, cscreen->sprite))
				cscreen->state = REINSTALLED;
			break;
		case REINSTALLED:
			cscreen->sprite->imgIndex = 2 + rand() % 2;
			play_song(cscreen->reinstall);
			hammer->state = GET_HAMMER;
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
