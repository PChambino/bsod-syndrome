#ifndef _HAMMER_H_
#define _HAMMER_H_

#include "music.h"
#include "sprite.h"
#include "keys.h"

#include "hammer_p.h"

#define HAMMER_DELTA 50
#define HAMMER_DELAY 200

typedef enum {UP, HIT, DOWN} HammerState;

typedef struct {
	Sprite* sprite;
	HammerState state;
	Song* sound;
} Hammer;

Hammer* newHammer();

void drawHammer(Hammer *hammer, char* buffer);

void updateHammer(Hammer *hammer, double mili, char key, Mouse *mouse);

void deleteHammer(Hammer* hammer);

#endif
