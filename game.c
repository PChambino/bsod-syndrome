
#include "game.h"

static Sprite *sprite;
static Song *song;

void updateSprite(Sprite *sprite, double sec, char key) {
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
				play_song(song);
				break;
			case 0x50: // DOWN
				sprite->y++;
				break;
			default:
				break;
	 	}
}

void game_init() {
	srand(time(NULL));
	
	char **maps[] = {penguin};
	sprite = newSprite(HRES/2, VRES/2, maps, 1, updateSprite);
		
	sprite->x -= sprite->width/2;
	sprite->y -= sprite->height/2;
	
	Note notes[] = {{Sol6, 100}, {Mi6,50}, {Sol6, 50}, {Mi6, 25}}; 
	song = malloc(sizeof(Song));
	song->lenght = sizeof(notes)/sizeof(Note);
	song->pause = 10;
	song->notes = notes;
}

void game_end() {
	deleteSprite(sprite);
	free(song);
}

void update(double sec) {
	char c = 0;
	if (!queueEmpty(&keys))
		switch (c = queueGet(&keys)) {
			case 1: // ESC
				exit(0);
			default:
				break;
		}
		
	sprite->update(sprite, sec, c);	
}

void draw(char *buffer) {
	int i = 0;
	for (i = 0; i < VRES; i++)
		memset(buffer+i*HRES, i,HRES);
	
	drawSprite(sprite, buffer);
}

void game_loop(int fps) {
	game_init();
	atexit(game_end);

	// creates a buffer for double-buffering
	char buffer[HRES*VRES];
	
	// calculates time to wait per frame
	int waitPerFrame = 1000 / fps;
	
	extern int time_tick;
	int lastTime = time_tick;
	int timePerFrame;
	
	while (1) {
		disable();
		
		// calculates last frame time
		timePerFrame = time_tick - lastTime;
		lastTime = time_tick;
		
		update(timePerFrame / 1000);
		draw(buffer);
		
		// copies from buffer to screen
		memcpy(base, buffer, HRES * VRES * sizeof(char));
				
		enable();
		
		// waits for a while
		mili_sleep(waitPerFrame);
	}
}
