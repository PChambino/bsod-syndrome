#ifndef _CSCREEN_H_
#define _CSCREEN_H_

#include "sprite.h"
#include "music.h"
#include "cscreenp.h"
#include "hammer.h"

/** @defgroup ComputerScreen ComputerScreen
 * @{
 *
 * Computer Screen functions
 */

 /** @name CScreen constants
 */
 // @{
#define crashTimeMaxInitial 10000
#define crashTimeDecrease 1000

#define BSoDTime 2000

#define savesUntilCDInitial 3

#define scoreSaved 10
#define scoreDeath 15
 //@}
 
 /** Computer Screen possible states
 */
typedef enum {WINDOWS, REINSTALLED, BSOD, DEATH, INACTIVE} CScreenState;

/** Computer Screen definition
*/
typedef struct {
	Sprite * sprite;	/**< computer screen sprite*/
	Song * crash;		/**< song played when computer crashes*/
	Song * reinstall;	/**< song when computer is reinstalled*/
	CScreenState state;	/**< state of the computer screen*/
	int nextCrashTime;	/**< next crash time*/
	int deathTime;		/**< death time*/
} CScreen;

/** Creates a new computer screen
	@param x position x of computer on game screen
	@param y position y of computed on game screen
	@param small if small is 1 draw small screen, otherwise draw big screen
	
	@return returns a pointer to the computer screen itself
*/
CScreen * newCScreen(int x, int y, int small);// small -> 1 for small screen, otherwise draw big screen

/**	Deletes the computer screen
	@param CScreen the computer screen itself
*/
void deleteCScreen(CScreen*);

/** Updates the state of the computer screen based on the position of the hammer
	@param cscreen computer screen to be updated
	@param hammer the hammer
	@param numberPCs number os pcs that aren't death
	@param score update score when computer is hit
	@param mili time of a frame
*/
void updateCScreen(CScreen *cscreen, Hammer *hammer, int *numberPCs, int *score, double mili);

/** Draw computer screen
	@param cscreen computer screen to be drawn
	@param buffer buffer to where the image will be drawn
*/
void drawCScreen(CScreen *cscreen, char *buffer);

/** Reset computer Screen
	@param cscreen computer screen to be reset
*/
void resetCScreen(CScreen* cscreen);

/** @} end of ComputerScreen */

#endif
