#ifndef _SCORE_H_
#define _SCORE_H_

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "utypes.h"
#include "keys.h"

/** @defgroup Score Score
 * @{
 *
 * Score related functions
 */
 
/** @name Score Limits
*/
//@{
#define NUM_HIGHSCORES 6
#define SCORE_NAME_LEN 6
#define SCORE_STR_LEN 3
//@}

/** Highscore save file
*/
extern const char HIGHSCORES_FILE[];

/** Score definition
*/
typedef struct {
	char* name; //< player name
	int score; //< pontuation
} Score;

/** Creates Score
	@return a pointer to the new score
*/
Score* newScore();

/** Creates a array of scores
	@return a pointer to the array of scores
*/
Score** newHighScores();

/** Deletes score
	@param s score to be deleted
*/
void deleteScore(Score* s);

/** Deletes a array of scores
	@param hs the array of scores to delete
*/
void deleteHighScores(Score** hs);

/** Draw score value
	Uses drawString
*/
void drawScoreValue(Score* s, int x, int y, int color, int scale, char* buffer);

/** Draw a array of scores
	Uses drawString
*/
void drawHighScores(Score** hs, int x, int y, int color, int scale, char* buffer);

/** Saves the array of scores to the file HIGHSCORES_FILE
	@param hs arrays of scores
*/
void saveHighScores(Score** hs);

/** Reads an array of scores from the file HIGHSCORES_FILE
*/
Score** readHighScores();

/** Puts score to arrays of scores in order.
	@param hs array of scores
	@param s score to insert
*/
Bool putScore(Score** hs, Score* s);

/** Checks if score enters in the array of scores.
	@param hs array of scores
	@param s score to insert
*/
Bool isHighScore(Score** hs, Score* s);

//@}

#endif
