
#include "sprite.h"

Sprite* newSprite(int x, int y, char **map[], int maps,
	void (*update)(Sprite *sprite, double sec, char key))
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
	
	sprite->update = update;
	
	return sprite;
}

void drawSprite(Sprite *sprite, char *buffer) {
	int i, j;
	char pix;
	for (i = 0; i < sprite->height; i++)
		for (j = 0; j < sprite->width; j++)
			if ((pix = sprite->img[sprite->imgIndex][i*sprite->width+j]))
				set_pixelb(j+sprite->x, i+sprite->y, pix, buffer);
}

void deleteSprite(Sprite *sprite) {
	for (sprite->imgIndex = 0; sprite->imgIndex < sprite->imgs; sprite->imgIndex++)
		free(sprite->img[sprite->imgIndex]);
	free(sprite->img);
	free(sprite);
}
