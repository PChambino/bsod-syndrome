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

typedef struct Sprite Sprite;
struct Sprite {
	int x, y;
	char **img;
	int imgs, imgIndex;
	int width, height;
	//void (*update) (Sprite *sprite, double sec, char key, Mouse *mouse);
};

Sprite* newSprite(int x, int y, char **map[], int maps);

void drawSprite(Sprite *sprite, char *buffer);
void drawSpriteT(Sprite *sprite, char t, char *buffer);
void drawSpriteBG(Sprite *sprite, char *buffer);

void moveSprite(Sprite *sprite, int dx, int dy);

Bool collidesSpriteRect(Sprite *sprite, int x, int y, int width, int height);
Bool collidesSprite(Sprite *sprite, Sprite *other);

void deleteSprite(Sprite *sprite);

/** @} end of Sprite */

#endif
