#ifndef _GAME_H_
#define _GAME_H_

#include <dos.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

/** @defgroup Game Game
 * @{
 *
 * Game functions
 */

/** Game constant
*/
#define NUM_PCS 6

/** Game possible states
*/
typedef enum {PLAYING, END, HELP, SCORE} GameState;

/** Function that initializes the game and the objects
*/
void game_init();

/** Function used to update the objects of the game
*/
void update(int mili);

/** Draws the objects of the game
*/
void draw(char *buffer);

/** The game loop
	uses double buffering and controls the number of frames per second
	calls update, draw and game_init
*/
void game_loop(int fps);

/** Game end
	called in atexit
	frees and deletes all game objects
*/
void game_end();

/** Reinitialize game
*/
void reset_game();

/** @} end of game */

#endif
