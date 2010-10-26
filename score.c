#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#include "graph.h"
#include "error.h"
#include "board.h"
#include "sounds.h"
#include "globals.h"

#include "score.h"

// Score keeping (pseudo state) variables:
// NOTE: So we don't take all the time of incrementing potentially large score
// additions;  we'll increment score +5 at a time so long as
// score < intermediateScore;  when incrementing score; we should
// ONLY increment intermediateScore.
int score = 0;
int intermediateScore;

// The number of matches completed.
int matchesCompleted;

int SCORE_FIFTY_PTS_TEXTURE[8][11] = {
	
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 16, 16, 16, 0, 0, 0, 0, 0, 0, 0},
	{0, 16, 7, 7, 7, 0, 16, 16, 16, 0, 0},
	{0, 16, 16, 16, 7, 0, 16, 7, 16, 7, 0},
	{0, 0, 0, 16, 7, 0, 16, 7, 16, 7, 0},
	{0, 16, 16, 16, 7, 0, 16, 16, 16, 7, 0},
	{0, 0, 7, 7, 7, 0, 0, 7, 7, 7, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


void score_Score(MATCH scoredMatch){
	int xCoord, yCoord;	// Remove to new func.
	int xAccum = 0, yAccum = 0;
	int x, y;
	
	switch( scoredMatch.matchType )
	{
		case MATCH_TYPE_THREE:

			intermediateScore += SCORE_THREE_MATCH_VALUE;
			
			// What I should be doing is finding the center of three points.
			// should check some textbooks for this one perhaps.
			// {
			xAccum += scoredMatch.match.three_match.p1.x;
			yAccum += scoredMatch.match.three_match.p1.y;
			
			xAccum += scoredMatch.match.three_match.p2.x;
			yAccum += scoredMatch.match.three_match.p2.y;
			
			xAccum += scoredMatch.match.three_match.p3.x;
			yAccum += scoredMatch.match.three_match.p3.y;
			
			xCoord = (xAccum / 3) * TILE_WIDTH + BOARD_BORDER_OFFSET + 13;
			yCoord = (yAccum / 3) * TILE_WIDTH + BOARD_BORDER_OFFSET + 10;
			// }
			
			// Render the number.
			for (x = 0; x < 11; x++)
				for (y = 0; y < 8; y++)
					if (SCORE_FIFTY_PTS_TEXTURE[y][x] != 0x00)
						putPixel(x + xCoord,
								 y + yCoord, SCORE_FIFTY_PTS_TEXTURE[y][x]);
					
			
		break;
		
	}
	
	return;
}

void score_processScore(){
	
	while (score < intermediateScore){
		score += SCORE_DEFAULT_INCREMENT;
			
		score_displayScore();
		sound_shortHigh(score);
	}
}

void score_displayScore(){
	
	gotoxy(SCORE_DISPLAY_X_COORD, SCORE_DISPLAY_Y_COORD);
	printf("SCORE: %d", score);
	
	return;
}
