
#include "game.h"

static Mouse mouse;

static Sprite *spriteBG;
static Hammer *hammer;
static Song *song;

void game_init() {
	//disable();
	
	srand(time(NULL));
	
	char **mapsBG[] = {BG};
	spriteBG = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

	hammer = newHammer();
	
	Note notes[] = {{Sol6, 100}, {Mi6, 50}, {Sol6, 50}, {Mi6, 25}}; 
	song = newSong(10, notes, sizeof(notes)/sizeof(Note));
	
	//enable();
}

void game_end() {
	deleteSong(song);
	deleteHammer(hammer);
	deleteSprite(spriteBG);
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
	
	updateHammer(hammer, sec, c, (parse_mouse_event(mouseQueue, &mouse) ? &mouse : NULL));
}

void draw(char *buffer) {
	drawSpriteBG(spriteBG, buffer);
	drawHammer(hammer, buffer);
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
