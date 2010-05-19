
#include "game.h"

static Mouse mouse;

static Sprite *spriteBG;
static Hammer *hammer;
static CScreen **cscreens;

static int numPCs;
static Score score;
static Score *highScores;

static GameState state;

static Button *helpButton;
static Button *exitButton;

static Sprite *helpBoard;
static Sprite *scoreBoard;

void game_init() {
	srand(time(NULL));
	
	char **mapsBG[] = {BG};
	spriteBG = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

//	mapsBG[0] = HELP_BOARD;
//	helpBoard = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

	mapsBG[0] = SCORE_BOARD;
	scoreBoard = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

	helpButton = newButton(0, 160, HELP_OVER);
	exitButton = newButton(196, 40, EXIT_OVER);

	hammer = newHammer();
	
	numPCs = NUM_PCS;
	
	cscreens = malloc(NUM_PCS * sizeof(CScreen));
	cscreens[0] = newCScreen(241, 367, 0);
	cscreens[1] = newCScreen(435, 363, 0);
	cscreens[2] = newCScreen(626, 363, 0);
	cscreens[3] = newCScreen(388, 224, 1);
	cscreens[4] = newCScreen(532, 216, 1);
	cscreens[5] = newCScreen(672, 209, 1);
	
	score.score = 0;
	
	highScores = readHighScores();
	
	state = PLAYING;
}

void game_end() {
	saveHighScores(highScores);
	deleteHighScores(highScores);
	
	deleteHammer(hammer);

	int i;
	for (i = 0; i < NUM_PCS; i++)
		deleteCScreen(cscreens[i]);
	free(cscreens);
	
	deleteButton(helpButton);
	deleteButton(exitButton);
	
//	deleteSprite(helpBoard);
	deleteSprite(scoreBoard);
	deleteSprite(spriteBG);
}

void reset_game() {
	fprintf(logger, "Reset Game\n");
	score.score = 0;
	numPCs = NUM_PCS;

	hammer->state = GET_HAMMER;

	int i;
	for (i = 0; i < NUM_PCS; i++)
		resetCScreen(cscreens[i]);
}

void update(int mili) {
	char c = 0;
	if (!queueEmpty(&keys)) {
		c = queueGet(&keys);
		if (c == ESC_KEY)
			exit(0);
	}
	Bool mouseEvent = parse_mouse_event(mouseQueue, &mouse);

	updateHammer(hammer, mili, c, (mouseEvent ? &mouse : NULL));

	int i;
	switch (state) {
		case PLAYING:
			for (i = 0; i < NUM_PCS; i++)
				updateCScreen(cscreens[i], hammer, &numPCs, &score.score, mili);	

			if (numPCs == 0) {
				state = END;
				hammer->state = GET_HAMMER;
			}
			
			updateButton(helpButton, hammer);
			if (helpButton->state == CLICKED)
				state = HELP;
				
			updateButton(exitButton, hammer);
			if (exitButton->state == CLICKED)
				exit(0);
			
			break;
		case END:
			updateButton(helpButton, hammer);
			if (helpButton->state == CLICKED)
				state = HELP;
				
			updateButton(exitButton, hammer);
			if (exitButton->state == CLICKED)
				exit(0);
				
			if (c == ENTER_KEY || c == SPACE_KEY)
				state = SCORE;
			break;
		case SCORE:
			// check score for highscore
			// input name for score (if highscore)
			if (c == ENTER_KEY) {
				// save highscores...			
				reset_game();
				state = PLAYING;
			}
			break;
		case HELP:
			if (c == ENTER_KEY || c == SPACE_KEY)
				state = PLAYING;
			break;
		default:
			break;
	}
}

void draw(char *buffer) {
	int i;
	switch (state) {
		case END:
		case PLAYING:
			drawSpriteBG(spriteBG, buffer);
		
			drawScoreValue(&score, 695, 17, 0, 3, buffer);
		
			drawHighScores(highScores, 368, 85, 0, 1, buffer);
			
			for (i = 0; i < NUM_PCS; i++)
				drawCScreen(cscreens[i], buffer);
				
			drawButton(helpButton, buffer);
			drawButton(exitButton, buffer);
			
			break;
		case SCORE:
			drawSpriteBG(scoreBoard, buffer);
			break;
		case HELP:
//			drawSpriteBG(helpBoard, buffer);
			break;
		default:
			break;
	}
	
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
