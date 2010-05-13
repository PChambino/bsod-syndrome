#ifndef _GAME_H_
#define _GAME_H_

#include <dos.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "video.h"
#include "queue.h"
#include "timer.h"
#include "setup.h"
#include "sprite.h"
#include "music.h"
#include "mouse.h"
#include "hammer.h"
#include "keys.h"
#include "cscreen.h"
#include "bg.h"

#define NUM_PCS 6

void game_init();

void update(int mili);

void draw(char *buffer);

void game_loop(int fps);

void game_end();

#endif
