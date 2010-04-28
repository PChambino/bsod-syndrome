#ifndef _SETUP_H_
#define _SETUP_H_

#include <stdio.h>

#include "ints.h"
#include "rtc.h"
#include "video.h"
#include "game.h"
#include "music.h"

extern int fps;

void process_args(int argc, char **argv);

void setup_timer();

void setup_rtc();

extern Queue keys;
void setup_kbd();

void setup_mouse();

extern char *base;
void setup_video();

void tear_down();

#endif
