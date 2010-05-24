#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "video.h"
#include "mouse.h"
#include "utypes.h"

/** @defgroup Sprite Sprite
 * @{
 *
 * Sprite related functions
 */

/** Reads a xpm-like sprite defined in "map" (look at pixmap.h for
 * examples). Returns the address of the allocated memory where the
 * sprite was read. Updates "width" and "heigh" with the sprite
 * dimension.  Return NULL on error.
 * Assumes that VRES and HRES (the screen vertical and horizontal resolution)
 * are externaly defined.
 * 
 * Usage example, using the defined sprite in pixmap.h:
 * <pre>
 *   #include "pixmap.h" // defines  pic1, pic2, etc 
 *   int wd, hg;
 *   char *sprite = read_xpm(pic1, &wd, &hg);
 * </pre>
*/
char *read_xpm(char *map[], int *width, int *height);

/** Sprite definition
*/
typedef struct Sprite Sprite;

/** Sprite definition
*/
struct Sprite {
	int x, y; //< position on screen
	char **img; //< array of images
	int imgs, imgIndex; //< images count and index of current image
	int width, height; //< images sizes
};

/** Creates a sprite
	@param map arrays of pixmaps
	@param maps size of map
	@return a pointer to a sprite
*/
Sprite* newSprite(int x, int y, char **map[], int maps);

/** @name Sprite Draw Functions
*/
//@{
/** Draws a sprite line by line using memcpy
*/
void drawSprite(Sprite *sprite, char *buffer);
/** Draws a sprite pixel by pixel, not drawing pixel of color t
	@param t transparent color
*/
void drawSpriteT(Sprite *sprite, char t, char *buffer);
/** Draws a sprite entirely to the buffer using mempy.
	Image should be at position 0,0 and have a size equal to the screen.
*/
void drawSpriteBG(Sprite *sprite, char *buffer);
//@}

/** Moves x and y variables of sprite by dx and dy.
*/
void moveSprite(Sprite *sprite, int dx, int dy);

/** @name Sprite Collisions Functions
*/
//@{
/** Checks collision between a sprite and a rectangle
*/
Bool collidesSpriteRect(Sprite *sprite, int x, int y, int width, int height);
/** Checks collision between two sprites
*/
Bool collidesSprite(Sprite *sprite, Sprite *other);
//@}

/** Deletes a sprite
*/
void deleteSprite(Sprite *sprite);

/** @} end of Sprite */

#endif
