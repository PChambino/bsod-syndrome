#ifndef _HAMMER_H_
#define _HAMMER_H_

#include "sprite.h"

#include "hammer_p.h"

#define HAMMER_DELTA 100
#define HAMMER_DELAY 200

typedef enum {UP, HIT, DOWN} HammerState;

typedef struct {
	Sprite* sprite;
	HammerState state;
} Hammer;

Hammer* newHammer();

void drawHammer(Hammer *hammer, char* buffer);

void updateHammer(Hammer *hammer, double sec, char key, Mouse *mouse);

void deleteHammer(Hammer* hammer);

#endif
