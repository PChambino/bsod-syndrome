
#include "game.h"

static Mouse mouse;

static Sprite *spriteBG;
static Hammer *hammer;
static CScreen **cscreens;

static int numPCs;

static Score *score;
static Score **highScores;
static Bool highScore;

static GameState state;

static Button *helpButton;
static Button *exitButton;

static Sprite *helpBoard;
static Sprite *scoreBoard;

void game_init() {
	disable_irq(MOUSE_IRQ);
	disable_irq(KBD_IRQ);

	srand(time(NULL));
	
	char **mapsBG[] = {BG};
	spriteBG = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

	mapsBG[0] = HELP_BOARD;
	helpBoard = newSprite(0, 0, mapsBG, sizeof(mapsBG)/sizeof(char*));

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
	
	score = newScore();
	
	highScores = readHighScores();
	
	state = PLAYING;

	enable_irq(KBD_IRQ);
	enable_irq(MOUSE_IRQ);
}

void game_end() {
	deleteScore(score);

	saveHighScores(highScores);
	deleteHighScores(highScores);
	
	deleteHammer(hammer);

	int i;
	for (i = 0; i < NUM_PCS; i++)
		deleteCScreen(cscreens[i]);
	free(cscreens);
	
	deleteButton(helpButton);
	deleteButton(exitButton);
	
	deleteSprite(helpBoard);
	deleteSprite(scoreBoard);
	deleteSprite(spriteBG);
}

void reset_game() {
	fprintf(logger, "Reset Game\n");
	score->score = 0;
	numPCs = NUM_PCS;

	hammer->state = GET_HAMMER;

	int i;
	for (i = 0; i < NUM_PCS; i++)
		resetCScreen(cscreens[i]);
}

void update(int mili) {
	char c = 0, k = 0;
	if (!queueEmpty(&keys)) {
		c = queueGet(&keys);
		if (c == ESC_KEY)
			exit(0);
	}
	Bool mouseEvent = parse_mouse_event(mouseQueue, &mouse);

	updateHammer(hammer, mili, c, (mouseEvent ? &mouse : NULL));

	int i, len;
	switch (state) {
		case PLAYING:
			for (i = 0; i < NUM_PCS; i++)
				updateCScreen(cscreens[i], hammer, &numPCs, &score->score, mili);	

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
				
			if (hammer->state == HIT) {
				state = SCORE;
				highScore = isHighScore(highScores, score);
			}
			break;
		case SCORE:
			if (highScore) {
				if (c != 0) {
					if (c == BACKSPACE_KEY) {
						len = strlen(score->name);
						if (len > 0) {
							score->name[len - 1] = NULL;
						}
						break;
					}
					
					k = scancodeToAscii(c);
					if (k != 0) {
						len = strlen(score->name);
						if (len < SCORE_NAME_LEN) {
							score->name[len] = k;
							score->name[len + 1] = NULL;
						}
					}
				}
			}

			if (hammer->state == HIT && (!highScore || (c != SPACE_KEY && score->name[0] != NULL))) {
				if (highScore) {
					putScore(highScores, score);
					saveHighScores(highScores);
				}
				reset_game();
				state = PLAYING;
			}
			break;
		case HELP:
			if (hammer->state == HIT)
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
		
			drawScoreValue(score, 695, 17, 0, 3, buffer);
		
			drawHighScores(highScores, 368, 85, 0, 1, buffer);
			
			for (i = 0; i < NUM_PCS; i++)
				drawCScreen(cscreens[i], buffer);
				
			drawButton(helpButton, buffer);
			drawButton(exitButton, buffer);
			
			break;
		case SCORE:
			drawSpriteBG(scoreBoard, buffer);
			
			drawScoreValue(score, 150, 200, 15, 10, buffer);
			
			if (highScore) {
				drawString("Name:", 40, 415, 15, 5, buffer);
				drawString(score->name, 250, 415, 15, 5, buffer);
			}
			break;
		case HELP:
			drawSpriteBG(helpBoard, buffer);
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
