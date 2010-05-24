#ifndef _HAMMER_H_
#define _HAMMER_H_

#include "music.h"
#include "sprite.h"
#include "keys.h"

#include "hammer_p.h"

/** @defgroup Hammer Hammer
 * @{
 *
 * Hammer related functions
 */
 
/** @name Hammer Constants
*/
//@{
#define HAMMER_DELTA 50
#define HAMMER_DELAY 200
//@}

/** Hammer States
*/
typedef enum {GET_HAMMER, UP, HIT, DOWN, GET_CD, CD, CD_HIT, CD_DOWN} HammerState;

/** Hammer Definition
*/
typedef struct {
	Sprite* sprite; //< Hammer sprite
	HammerState state; //< Hammer state
	Song* sound; //< Hammer hit sound
} Hammer;

/** Creates a Hammer.
	@return a pointer to a Hammer
*/
Hammer* newHammer();

/** Draws a hammer to the buffer
	@param hammer Hammer to be draw
	@param buffer buffer to where the image will be drawn
*/
void drawHammer(Hammer *hammer, char* buffer);

/** Updates the hammer.
	Moves it based on key and/or mouse.
	On click changes state to HIT
	...
	
	@param hammer hammer to be updated
	@param mili time of frame to consider
	@param key key received from keyboard
	@param mouse mouse event received
*/
void updateHammer(Hammer *hammer, double mili, char key, Mouse *mouse);

/** Deletes Hammer
	@param hammer Hammer to be deleted
*/
void deleteHammer(Hammer* hammer);

//@}

#endif
