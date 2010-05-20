#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "utypes.h"
#include "keys.h"

#define NUM_HIGHSCORES 6
#define SCORE_NAME_LEN 6
#define SCORE_STR_LEN 3

extern const char HIGHSCORES_FILE[];

typedef struct {
	char* name;
	int score;
} Score;

Score* newScore();

Score** newHighScores();

void deleteScore(Score* s);

void deleteHighScores(Score** hs);

void drawScoreValue(Score* s, int x, int y, int color, int scale, char* buffer);

void drawHighScores(Score** hs, int x, int y, int color, int scale, char* buffer);

void saveHighScores(Score** hs);

Score** readHighScores();

Bool putScore(Score** hs, Score* s);

Bool isHighScore(Score** hs, Score* s);

#endif
