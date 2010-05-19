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
#include "score.h"
#include "cscreen.h"
#include "button.h"
#include "button_p.h"
#include "bg.h"

#define NUM_PCS 6

typedef enum {PLAYING, END, HELP, SCORE} GameState;

void game_init();

void update(int mili);

void draw(char *buffer);

void game_loop(int fps);

void game_end();

void reset_game();

#endif
