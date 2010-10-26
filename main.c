/* 
** Copyright 1998-2008, Pete J. Jensen
** Copyright 1998-2008, Robin McNeil
** All Rights Reserved.
** 
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of the authors
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of the authors.
** 
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
**
**        ______
**       /_____/\
**      /_____\\ \      FILE:    main.c
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    AUTHOR:  Robin McNeil
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>

#include "error.h"
#include "graph.h"
#include "globals.h"
#include "board.h"
#include "sounds.h"
// #include "score.h"

/* Prototypes */
void gameLoop( void );
short int getKeyPress(void);
int handleKeyPress( short int );
void score_render( void );
void game_render( void );
void score_addToScore(unsigned long int value);

#define TEMPORARY_ANIM_DELAY	150
#define FORCED_ROTATIONS		1

enum {STATE_NORMAL, STATE_ROTATION,
		STATE_GRAVITY} gameState;

enum {ROTATION_RIGHT, ROTATION_LEFT} rotationDirection;
		
// Rotation state variables.
short int rotationCounter;
short int direction;

// Score variables.
static unsigned long int score = 0;
static int intermediateScore;		// For anim

// The number of matches completed.
int matchesCompleted;

int main(int argc, char **argv){
		
	initializeGraphicsMode();
	
	tile_initializeTileLevels();
	board_init();
	
	//SPLASHSCREEN
	// MAIN MENU

	board_render();
	
#ifdef __DEBUG	
	getch();
#endif
		
	gameLoop();

	endGraphicsMode();
	
	return EXIT_SUCCESS;
}

void gameLoop(){
	
	for ( ; ; ) {
		
		switch( gameState )
		{
		case STATE_ROTATION:
		
#if FORCED_ROTATIONS
			if( rotationCounter++ >= 3 )
#else
			if( rotationCounter++ >= 1 )
#endif
			{
				sound_badSound();	
				rotationCounter = 0;
				gameState = STATE_NORMAL;
				break;
			}
			
			switch(rotationDirection)
			{
			case ROTATION_RIGHT:
				board_rotateRight();
				break;
			case ROTATION_LEFT:
				board_rotateLeft();
				break;
			}
			
			gameState = STATE_NORMAL;
			break;
			
		case STATE_GRAVITY:
			board_applyGravity();
			board_dropTiles();
			gameState = STATE_NORMAL;
			delay(ANIM_DELAY_GRAVITY);
			
			break;
			
		case STATE_NORMAL:
			game_render();
							
			//score_displayScore();
			
			if( board_hasEmpty() )
			{
				gameState =	STATE_GRAVITY;
				break;
			}
			
			if(rotationCounter < 3 && rotationCounter > 0)
				delay(ANIM_DELAY_ROTATION);
			
			if( matchesCompleted = processMatches() )
			{
				/* NOTE: If we had a real animation engine, we wouldn't need this */
				game_render();
				delay(ANIM_DELAY_GRAVITY);
				
				rotationCounter = 0;
				sound_marioSound();
				gameState = STATE_GRAVITY;
				break;
			}
			
			if(rotationCounter)
				gameState = STATE_ROTATION;
			else
				if( !handleKeyPress( getKeyPress() ) ) return;
		}
	}

	return;	
}

/* ***************************************************************************
 * Function: getKeyPress
 * Purpose : Wait for the player to press a key; only then do something; if we
 *           did not do this the game would go flickering when we were nooping
 *           
 * Inputs  : Void
 * Returns : The (int) key value that the player pressed.
 * Sample Call : ch = getKeyPress();
 */
short int getKeyPress()
{
    register int keyPressed;

    /* 1) Check if a key has been pressed.	 */
    /* 2) Store the key that was indeed pressed. */
    for(;;) {
	    	    
        if ( kbhit() ) {
	        
	        // Store the key pressed. 
            keyPressed = getch();
            
            // Return the key pressed back to the calling function.
            return keyPressed;
        } 
        else { 
	        
	        // NOTE: placing a break here modifies the behavior of user
	        // input and the game significantly.  Instead of waiting, 
	        // eg (continue) for a keypress, it will continue on with
	        // the rest of the program.
	        
	         //break;
        	 continue;
    	}
    }

    return -1;
}

/* ***************************************************************************
 * Function: handleKeyPress
 * Purpose : Designed to work in conjunction with getKeyPress, except this
 *           fuction handles the dissemination of tasks; to map to pressed
 *           keys on the keyboard.
 *           
 * Inputs  : int inkey - the pressed key.
 * Returns : void
 * Sample Call : handleKeyPress( getKeyPress() ); //Waits for key, pass int.
 */	
int handleKeyPress(short int inkey)
{
	unsigned char done;		// Are we done yet?

	
    switch ( inkey )
    {
        /* Handle directional keys. */
        case UP_ARROW_KEY:       board_cursorMoveUp(); break;
        case DOWN_ARROW_KEY:     board_cursorMoveDown(); break;
        case RIGHT_ARROW_KEY:    board_cursorMoveRight(); break;    
        case LEFT_ARROW_KEY:     board_cursorMoveLeft(); break;

        /* Handle function and escape keys. */
        case F1_KEY:             break;    
        case ESC_KEY:            return 0;
       
        case 'r':
        	board_init();
        	break;  
       
        case 'g':
        	(void) board_applyGravity();
        	break;

        case ROTATE_LEFT_KEY:
        	rotationDirection = ROTATION_LEFT;
    	    gameState = STATE_ROTATION;
        	break;
        case ROTATE_RIGHT_KEY:
        	rotationDirection = ROTATION_RIGHT;
        	gameState = STATE_ROTATION;
        	break;
    }
    
    return 1;
}

/* ***************************************************************************
 * Function: processMatches
 * Purpose:
 */	
int processMatches()
{
	MATCH matchStructure = {0};
	int counter = 0;
	        
	while( board_checkPatterns( &matchStructure ) )
	{
		int scoreBase = 5;
		enum {
			SCORE_TYPE_THREE = 3,
			SCORE_TYPE_FOUR = 4,
			SCORE_TYPE_SIX = 6,
		} scoreType;
		
		int numStars = 0;
		int superStar = 0;
		int scoreValue = 0;
		
    	counter++;
    	switch( matchStructure.matchType )
    	{
		case MATCH_TYPE_THREE:
			if (matchStructure.match.three_match.p1.star)
			 	numStars++;
			if (matchStructure.match.three_match.p2.star)
			 	numStars++;
			if (matchStructure.match.three_match.p3.star)
			 	numStars++;
			 	
			scoreType = SCORE_TYPE_THREE;
			if (matchStructure.type == TILE_SUPERSTAR)
			 	superStar = 1;
			
			break;
		
		case MATCH_TYPE_FOUR:
			if (matchStructure.match.four_match.p1.star)
				numStars++;
			if (matchStructure.match.four_match.p2.star)
			 	numStars++;
			if (matchStructure.match.four_match.p3.star)
			 	numStars++;
			if (matchStructure.match.four_match.p4.star)
			 	numStars++;
			 	
			scoreType = SCORE_TYPE_FOUR;
			if( matchStructure.type == TILE_SUPERSTAR )
				superStar = 1;
			
			break;
			
		case MATCH_TYPE_SIX:
			// Actually we don't kill the center tile; we only change it.
			// TODO: board_makeStar(POS)
			board_makeStar(matchStructure.match.six_match.center.x, 
						matchStructure.match.six_match.center.y);
			
			scoreType = SCORE_TYPE_SIX;
			if( matchStructure.type == TILE_SUPERSTAR ){
				// TODO: board_makeBlackDiamond( @ center)
				superStar = 1;
			}
		
			
			break;
    	}

    	
    	
	   	scoreValue = scoreBase * scoreType * (numStars ? numStars + 1 : 1) * (superStar ? 100 : 1);
    	score_addToScore(scoreValue);
    	
    	board_killMatch(&matchStructure);
    	
    	if( numStars >= 3){
    		//sound_explosion(numStars);
    		//sound_nested(numStars, scoreValue);
    		//sound_fart(numStars);
    		//delay(300);
    		sound_xplo(numStars);
    		board_explodeMatch(&matchStructure);
		}
    		
    	// Grrrrrr ... This could/should be rearranged and refactored.
    	//score_Score( matchStructure );
    	//score_processScore();
	} 
	    	
    return counter;
}

void game_render()
{
	board_render();
	score_render();
}

void score_addToScore(unsigned long int value)
{
	
	// Okay for now.
	score += value;
}



void score_render(){
	
	gotoxy(SCORE_DISPLAY_X_COORD, SCORE_DISPLAY_Y_COORD);
	printf("%u", score);
	
	gotoxy(SCORE_DISPLAY_X_COORD, SCORE_DISPLAY_Y_COORD + 1);
	printf("%i     ", matchesCompleted);
	
	return;
}