#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "sprite.h"
#include "hammer.h"

/** @defgroup Button Button
 * @{
 *
 * Button related functions
 */

 /** Button States
 */
typedef enum {NORMAL, OVER, CLICKED} ButtonState;

/** Button definition
*/
typedef struct {
	Sprite* sprite;		/**< sprite when hammer is over */
	ButtonState state;	/**< state of the button */
} Button;

/** Creates new button
	@param x position x on screen
	@param y position y on screen
	@param over pixmap when hammer is over
	
	@return returns a pointer to the button itself
*/
Button* newButton(int x, int y, char** over);

/**	Draws a button
	@param button button to draw
	@param buffer buffer where the image is to be drawn
*/
void drawButton(Button *button, char* buffer);

/** Updates the hammer button based on the position of the hammer
	@param button the button to be updated
	@param hammer the hammer
*/
void updateButton(Button *button, Hammer *hammer);

/** Deletes the button
	@param button the button itself
*/
void deleteButton(Button *button);

/** @} end of Button */

#endif
