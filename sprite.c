
#include "sprite.h"

Sprite* newSprite(int x, int y, char **map[], int maps)
{
	Sprite *sprite;
	sprite = malloc(sizeof(Sprite));
	
	sprite->x = x;
	sprite->y = y;
	
	sprite->imgs = maps;
	sprite->img = malloc(sprite->imgs * sizeof(char*));
	for (sprite->imgIndex = 0; sprite->imgIndex < sprite->imgs; sprite->imgIndex++) {
		sprite->img[sprite->imgIndex] = read_xpm(map[sprite->imgIndex],
			&sprite->width, &sprite->height);
	}
	sprite->imgIndex = 0;
	
	return sprite;
}

void drawSprite(Sprite *sprite, char *buffer) {
	int i;
	for (i = 0; i < sprite->height; i++)
		memcpy(buffer+HRES*(i+sprite->y)+sprite->x,
			sprite->img[sprite->imgIndex]+i*sprite->width,
			sprite->width);
}

void drawSpriteT(Sprite *sprite, char t, char *buffer) {
	int i, j;
	char pix;
	for (i = 0; i < sprite->height; i++)
		for (j = 0; j < sprite->width; j++)
			if ((pix = sprite->img[sprite->imgIndex][i*sprite->width+j]) != t)
				set_pixelb(j+sprite->x, i+sprite->y, pix, buffer);
}

void drawSpriteBG(Sprite *sprite, char *buffer) {
	if (sprite->width == HRES && sprite->height == VRES)
		memcpy(buffer, sprite->img[sprite->imgIndex], HRES * VRES * sizeof(char));
}

void moveSprite(Sprite *sprite, int dx, int dy) {
	sprite->x += dx;
	sprite->y -= dy;
	if (sprite->x < 0)
		sprite->x = 0;
	if (sprite->y < 0)
		sprite->y = 0;
	if (sprite->x + sprite->width > HRES)
		sprite->x = HRES - sprite->width;
	if (sprite->y + sprite->height > VRES)
		sprite->y = VRES - sprite->height;
}

void deleteSprite(Sprite *sprite) {
	for (sprite->imgIndex = 0; sprite->imgIndex < sprite->imgs; sprite->imgIndex++)
		free(sprite->img[sprite->imgIndex]);
	free(sprite->img);
	free(sprite);
}
