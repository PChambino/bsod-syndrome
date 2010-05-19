
#include "score.h"

const char HIGHSCORES_FILE[] = "high.txt";

static char scoreStr[SCORE_STR_LEN];

Score* newHighScores() {
	Score *hs = malloc(NUM_HIGHSCORES * sizeof(Score));
	
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		hs[i].name = NULL;
		hs[i].score = 0;
	}
	
	return hs;
}

void deleteHighScores(Score* hs) {
	// free names ?
	
	free(hs);
}

void drawScoreValue(Score* s, int x, int y, int color, int scale, char* buffer) {
	sprintf(scoreStr, "%d", s->score);
	drawString(scoreStr, x, y, color, scale, buffer);
}

void drawHighScores(Score* hs, int x, int y, int color, int scale, char* buffer) {
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		if (hs[i].name == NULL)
			break;
		drawString(hs[i].name, x, y+(charHeight*i), color, scale, buffer); 
		drawScoreValue(&hs[i], x+(charWidth*scale*(1+SCORE_NAME_LEN)), y+(charHeight*i),
			color, scale, buffer);
	}
}

void saveHighScores(Score* hs) {
	int file = open(HIGHSCORES_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	
	if (file == -1)
		return;
		
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		if (hs[i].name == NULL)
			break;
		write(file, hs[i].name, SCORE_NAME_LEN * sizeof(char));
		write(file, &hs[i].score, sizeof(int));
	}
	
	close(file);
}

Score* readHighScores() {
	Score *hs = newHighScores();
	
	int file = open(HIGHSCORES_FILE, O_RDONLY);

	if (file >= 0) {
		int i;
		for (i = 0; i < NUM_HIGHSCORES; i++) {
			char *name = malloc(SCORE_NAME_LEN * sizeof(char));
			if (read(file, name, SCORE_NAME_LEN * sizeof(char)) == 0) {
				free(name);
				break;
			}
			hs[i].name = name;
			if (read(file, &hs[i].score, sizeof(int)) == 0) break;
		}
		
		close(file);
	}
	
	return hs;
}


