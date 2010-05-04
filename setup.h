#ifndef _SETUP_H_
#define _SETUP_H_

#include <stdio.h>

#include "ints.h"
#include "rtc.h"
#include "video.h"
#include "game.h"
#include "music.h"
#include "GQueue.h"
#include "kbc.h"

extern int fps;

void process_args(int argc, char **argv);

extern FILE* logger;

void setup_log();

extern char *base;
void setup_video();

extern Queue keys;
extern GQueue *mouseQueue;
void setup_kbc();

void setup_rtc();

void setup_timer();

void tear_down();

#endif
