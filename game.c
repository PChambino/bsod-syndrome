
#include "game.h"

static Mouse mouse;

static Sprite *sprite, *spriteBG;
static Song *song;

void updateSprite(Sprite *sprite, double sec, char key, Mouse *mouse) {
	static int down = 0;
	static int time = 10;
	if (down) {
		if (time-- == 0) {
			sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
			down = 0;
			time = 10;
		}
	}

	if (key)
	 	switch (key) {
			case 0x4d: // RIGHT
				sprite->x++;
				break;
			case 0x4b: // LEFT
				sprite->x--;
				break;
			case 0x48: // UP
				sprite->y--;
				break;
			case 0x50: // DOWN
				sprite->y++;
				break;
			case 0x1c: // ENTER
				down = 1;
				sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
				break;
			default:
				break;
	 	}
	
	if (mouse) {
		//print_mouse_event(mouse);
		moveSprite(sprite, mouse->dx, mouse->dy);
		if (mouse->lb) {
			down = 1;
			sprite->imgIndex = (sprite->imgIndex + 1) % sprite->imgs;
		}
	}
}

void game_init() {
	//disable();
	
	srand(time(NULL));
	
	char **maps[] = {hammer_up, hammer_down};
	sprite = newSprite(HRES/2, VRES/2, maps, sizeof(maps)/sizeof(char*), updateSprite);
		
	sprite->x -= sprite->width/2;
	sprite->y -= sprite->height/2;
	
	char **mapsBG[] = {BG};
	spriteBG = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*), NULL);
	
	Note notes[] = {{Sol6, 100}, {Mi6, 50}, {Sol6, 50}, {Mi6, 25}}; 
	song = newSong(10, notes, sizeof(notes)/sizeof(Note));
	
	//enable();
}

void game_end() {
	deleteSprite(sprite);
	deleteSprite(spriteBG);
	deleteSong(song);
}

void update(double sec) {
	char c = 0;
	if (!queueEmpty(&keys))
		switch (c = queueGet(&keys)) {
			case 1: // ESC
				exit(0);
			case 0x0F: // TAB
				play_song(song);
			default:
				break;
		}
	
	sprite->update(sprite, sec, c, (parse_mouse_event(mouseQueue, &mouse) ? &mouse : NULL));
}

void draw(char *buffer) {
/*	int i = 0;
	for (i = 0; i < VRES; i++)
		memset(buffer+i*HRES, i,HRES);*/

	drawSpriteBG(spriteBG, buffer);
	
	drawSpriteT(sprite, 15, buffer);
	//drawSprite(sprite, buffer);
}

void game_loop(int fps) {
	fprintf(logger, "Game Init\n");
	game_init();
	atexit(game_end);
	
	// creates a buffer for double-buffering
	char buffer[HRES*VRES];
	
	// calculates time to wait per frame
	/*int waitPerFrame = 1000 / fps;
	
	extern int time_tick;
	int lastTime = time_tick;
	int timePerFrame;*/

	fprintf(logger, "Game Loop\n");
	while (1) {
		//disable();
		disable_irq(MOUSE_IRQ);
		disable_irq(KBD_IRQ);
		
		// calculates last frame time
		/*timePerFrame = time_tick - lastTime;
		lastTime = time_tick;*/
		
		update(1);//timePerFrame / 1000);
		draw(buffer);
		
		// copies from buffer to screen
		memcpy(base, buffer, HRES * VRES * sizeof(char));
		
		//enable();
		enable_irq(KBD_IRQ);
		enable_irq(MOUSE_IRQ);
		
		// waits for a while
		//mili_sleep(waitPerFrame);
	}
}
