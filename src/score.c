
#include "score.h"

const char HIGHSCORES_FILE[] = "high.txt";

static char scoreStr[SCORE_STR_LEN];

Score* newScore() {
	Score* s = malloc(sizeof(Score));
	s->name = malloc((SCORE_NAME_LEN + 1) * sizeof(char));
	s->name[0] = NULL;
	s->score = 0;
	
	return s;
}

Score** newHighScores() {
	Score **hs = malloc(NUM_HIGHSCORES * sizeof(Score*));
	
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		hs[i] = newScore();		
	}
	return hs;
}

void deleteScore(Score* s) {
	free(s->name);
	free(s);
}

void deleteHighScores(Score** hs) {
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		deleteScore(hs[i]);
	}
	free(hs);
}

void drawScoreValue(Score* s, int x, int y, int color, int scale, char* buffer) {
	sprintf(scoreStr, "%d", s->score);
	drawString(scoreStr, x, y, color, scale, buffer);
}

void drawHighScores(Score** hs, int x, int y, int color, int scale, char* buffer) {
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		if (hs[i]->name[0] == NULL)
			break;
		drawString(hs[i]->name, x, y+(charHeight*i), color, scale, buffer); 
		drawScoreValue(hs[i], x+(charWidth*scale*(1+SCORE_NAME_LEN)), y+(charHeight*i),
			color, scale, buffer);
	}
}

void saveHighScores(Score** hs) {
	int file = open(HIGHSCORES_FILE, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	
	if (file == -1)
		return;
		
	int i;
	for (i = 0; i < NUM_HIGHSCORES; i++) {
		if (hs[i]->name[0] == NULL)
			break;
		write(file, hs[i]->name, (SCORE_NAME_LEN + 1) * sizeof(char));
		write(file, &hs[i]->score, sizeof(int));
	}
	
	close(file);
}

Score** readHighScores() {
	Score **hs = newHighScores();
	
	int file = open(HIGHSCORES_FILE, O_RDONLY);

	if (file >= 0) {
		int i;
		for (i = 0; i < NUM_HIGHSCORES; i++) {
			if (read(file, hs[i]->name, (SCORE_NAME_LEN + 1) * sizeof(char)) == 0) {
				break;
			}
			if (read(file, &hs[i]->score, sizeof(int)) == 0) break;
		}
		
		close(file);
	}
	
	return hs;
}

Bool putScore(Score** hs, Score* s) {
	int i;
	for (i = NUM_HIGHSCORES - 1; i >= 0; i--) {
		if (hs[i]->name[0] == NULL)
			continue;
		
		if (hs[i]->score < s->score) {
			if (i == NUM_HIGHSCORES - 1)
				continue;
			strcpy(hs[i+1]->name, hs[i]->name);
			hs[i+1]->score = hs[i]->score;
			continue;
		}
		
		if (i == NUM_HIGHSCORES - 1)
			return false;
		
		break;
	}

	strcpy(hs[i+1]->name, s->name);
	hs[i+1]->score = s->score;
	
	return true;
}

Bool isHighScore(Score** hs, Score* s) {
	return (hs[NUM_HIGHSCORES - 1]->name[0] == NULL || hs[NUM_HIGHSCORES - 1]->score < s->score); 
}
