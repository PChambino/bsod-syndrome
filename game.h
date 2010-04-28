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
#include "pixmap.h"
#include "music.h"


void game_init();

void update(double sec);

void draw(char *buffer);

void game_loop(int fps);

void game_end();

#endif
