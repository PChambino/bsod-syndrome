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

/** @defgroup Setup Setup
 * @{
 *
 * Setup related functions
 */
 
/** frames per second the game loop while run.
*/
extern int fps;

/** Reads and process arguments.
*/
void process_args(int argc, char **argv);

/** Logger file
*/
extern FILE* logger;

/** Setup log file.
	Opens log file and dups to stdout
*/
void setup_log();

/** video address base
*/
extern char *base;
/** Setup video
	Starts graphic mode.
*/
void setup_video();

/** Keyboard keys queue
*/
extern Queue keys;
/** Mouse queue
*/
extern GQueue *mouseQueue;
/** Setup mouse and keyboard
*/
void setup_kbc();

/** Counts milisec using rtc_isr
*/
extern int mili_tick;
/** Setup rtc
*/
void setup_rtc();

/** Setup timer
*/
void setup_timer();

/** Reinstalls and undo all setups
*/
void tear_down();

//@}

#endif
