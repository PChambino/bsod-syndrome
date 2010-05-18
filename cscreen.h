#ifndef _CSCREEN_H_
#define _CSCREEN_H_

#include "sprite.h"
#include "music.h"
#include "cscreenp.h"
#include "hammer.h"

#define crashTimeMaxInitial 10000
#define crashTimeDecrease 1000

#define BSoDTime 2000

#define savesUntilCDInitial 3

#define scoreSaved 10
#define scoreDeath 15

typedef enum {WINDOWS, REINSTALLED, BSOD, DEATH, INACTIVE} CScreenState;

typedef struct {
	Sprite * sprite;
	Song * crash;
	Song * reinstall;
	CScreenState state;
	int nextCrashTime;
	int deathTime;
} CScreen;

CScreen * newCScreen(int x, int y, int small);// small -> 1 for small screen, otherwise draw big screen
void deleteCScreen(CScreen*);
void updateCScreen(CScreen *cscreen, Hammer *hammer, int *numberPCs, int *score, double mili);
void drawCScreen(CScreen *cscreen, char *buffer);

#endif
