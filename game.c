
#include "game.h"

static Mouse mouse;

static Sprite *spriteBG;
static Hammer *hammer;
static CScreen **cscreens;
static Song *song;

static int numPCs;
static int score;

void game_init() {
	srand(time(NULL));
	
	char **mapsBG[] = {BG};
	spriteBG = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

	hammer = newHammer();

	numPCs = NUM_PCS;
	score = 0;
	
	cscreens = malloc(numPCs * sizeof(CScreen));
	cscreens[0] = newCScreen(241, 367, 0);
	cscreens[1] = newCScreen(435, 366, 0);
	cscreens[2] = newCScreen(626, 363, 0);
	cscreens[3] = newCScreen(388, 224, 1);
	cscreens[4] = newCScreen(532, 216, 1);
	cscreens[5] = newCScreen(672, 209, 1);
	
	Note notes[] = {{Sol6, 100}, {Mi6, 50}, {Sol6, 50}, {Mi6, 25}}; 
	song = newSong(10, notes, sizeof(notes)/sizeof(Note));	
}

void game_end() {
	deleteSong(song);
	deleteHammer(hammer);

	int i;
	for (i = 0; i < NUM_PCS; i++)
		deleteCScreen(cscreens[i]);
	free(cscreens);
		
	deleteSprite(spriteBG);
}

void update(int mili) {
	char c = 0;
	if (!queueEmpty(&keys))
		switch (c = queueGet(&keys)) {
			case ESC_KEY:
				exit(0);
			case TAB_KEY:
				play_song(song);
			default:
				break;
		}
	
	updateHammer(hammer, mili, c, (parse_mouse_event(mouseQueue, &mouse) ? &mouse : NULL));
	
	int i;
	for (i = 0; i < NUM_PCS; i++)
		updateCScreen(cscreens[i], hammer, &numPCs, &score, mili);
		
	if (numPCs <= 0) {
		// termina!
	}	
}

void draw(char *buffer) {
	drawSpriteBG(spriteBG, buffer);

	static char scoreStr[3];
	sprintf(scoreStr, "%d", score);
	drawString(scoreStr, 700, 17, 0, 3, buffer);

	drawString("Eva     80", 368, 85, 0, 1, buffer);
	
	int i;
	for (i = 0; i < NUM_PCS; i++)
		drawCScreen(cscreens[i], buffer);
	
	drawHammer(hammer, buffer);
}

void game_loop(int fps) {
	fprintf(logger, "Game Init\n");
	game_init();
	atexit(game_end);
	
	// creates a buffer for double-buffering
	char buffer[HRES*VRES];
	
	// calculates time to wait per frame
	int waitPerFrame = 1000 / fps;
	
	int lastTime = mili_tick;
	int timePerFrame, end;

	fprintf(logger, "Game Loop\n");
	while (1) {
		disable_irq(MOUSE_IRQ);
		disable_irq(KBD_IRQ);
		
		// calculates last frame time
		timePerFrame = mili_tick - lastTime;
		lastTime = mili_tick;
		//fprintf(logger, "Frame Time: %d\n", timePerFrame);		
		
		update(timePerFrame);
		draw(buffer);
		
		// copies from buffer to screen
		memcpy(base, buffer, HRES * VRES * sizeof(char));
		
		enable_irq(KBD_IRQ);
		enable_irq(MOUSE_IRQ);
		
		// waits for a while
		end = mili_tick + waitPerFrame - (mili_tick - lastTime);
		while (mili_tick < end) {}
	}
}
