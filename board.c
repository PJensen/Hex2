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
**      /_____\\ \      FILE:    globals.h
**     /_____\ \\ /     AUTHOR:  Robin McNeil
**    /_____/ \/ / /    AUTHOR:  Pete Jensen
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /
**    \_____/ / /\ /
**     \_____/ \\ \
**      \_____\ \\
**       \_____\/
**/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>

#include "globals.h"
#include "board.h"
#include "graph.h"
#include "error.h"

// A Board is made up of (BOARD_WIDTH * BOARD_HEIGHT)
// TILE structures.
TILE board[BOARD_WIDTH][BOARD_HEIGHT];

// A Cursor consists of a x-component, a y-component and an offset.
CURSOR cursor;

static short int maxColors = TILE_YELLOW;

/* ************************************************************************ */
// MATCH matchStructure;
// A general structure used in storing the results of a match.  A few quick
// notes about the intended use of this structure.  How about a few examples?
//
// matchStructure.type - What type of tile is involved?
//
// matchStructure.matchType - What pattern is represented?  This can be one of
// 		MATCH_TYPE_NONE:	No match is represented.
//
//		MATCH_TYPE_THREE:	A match of three tiles, there are four possible
//							configuration for this match.
//
//		MATCH_TYPE_FOUR:	A match of four tiles.
//
//		MATCH_TYPE_SIX:		The flower pattern.
//
// matchStructure.match - Depending on what pattern is represented ...
//
//		matchStructure.match.three_match (or)
//		matchStructure.match.four_match (or)
//		matchStructure.match.six_match
//
// should be used.  All three of these structure members reference their own
// structure accordingly. @SEE: board.h for more.
//
// NOTE: matching precidence,
// 	MATCH_TYPE_FOUR MUST be checked before any matches of MATCH_TYPE_THREE
// 	are even looked for.  This is because a MATCH_TYPE_FOUR is (potentially)
// 	composed of two MATCH_TYPE_THREE matches.


/* Local Prototype(s) */
void tile_display(int x, int y);
void board_explodeTile(POS* tilePos);

/* ************************************************************************ */
// FUNCTION: 	board_init
// PURPOSE: To initialize most (if not all) board related data structures.
// PARAMATERS:	(None)
// RETURNS: 	(None)
// <Usage>		(void) board_init();
void board_init( void ) {

	int x, y;

#ifndef __DEBUG
	randomize();
#else
	srand(1);
#endif
	
	/* Give each tile a random color */
	for( x = 0; x < BOARD_WIDTH; x++ ) {
		for( y = 0; y < BOARD_HEIGHT; y++ ) {
			/* Select a random tile to put here */
			tile_generateRandom(&board[x][y], maxColors);
		}
	}

	/* Initialize the position of the cursor in the top left. */
	cursor.x = 0;
	cursor.y = 0;
	cursor.offset = 0;
	
	return;
}

/* ************************************************************************ */
// FUNCTION: 	board_render
// PURPOSE: The purpose of 'board_render' is somewhat self documenting.
// However, it should be noted that this isn't just ANY board;  there are
// offsets, even/odd colums etc.  NOTE: idealy (to reduce flicker) this
// function shouldn't be called all too often.
//
// PARAMETERS: 	(None)
// RETURNS: 	(None)
// <Usage:>		(void) board_render();
void board_render(){
	SPIN_COORDS coords;
	int x, y, xpos, ypos, index;
	
	for( x = 0; x < BOARD_WIDTH; x++ ) {
		for( y = 0; y < BOARD_HEIGHT; y++ ) {

			
			/* Are we at an even numbered tile? */
			if( x % 2 == 1 )
				if( y == BOARD_HEIGHT - 1 )
					break;

			/* Compute both the x-position and y-position of the tile that
			* is about to be drawn.  Take into account offsets.*/
			xpos = BOARD_BORDER_OFFSET + x * TILE_WIDTH;
			ypos = BOARD_BORDER_OFFSET + y * TILE_HEIGHT;

#if __BOARD_OFFSET
			/* Are we at an even numbered tile? */
 			if( x % 2 == 1 )
 				ypos += TILE_HEIGHT / 2;
#endif
			/* Render the tile */
			tile_render(xpos, ypos, &board[x][y]);
		}
	}
				
	// Now since we done drawing all of the tiles,
	// lets draw the cursor, makes sense no?
	(void) board_renderCursor();


// NOTE:
// This shows EXACTLY what tiles are selected;  particularly good for
// debugging.  Feel free to have a look at board_getSpinTiles since it
// references this very comment.
#ifdef __DEBUG

	(void) board_getSpinTiles(&coords, cursor.x, cursor.y, cursor.offset);

	xpos = BOARD_BORDER_OFFSET + coords.p1.x * (TILE_WIDTH) + TILE_WIDTH/2;
	ypos = BOARD_BORDER_OFFSET + coords.p1.y * (TILE_HEIGHT) + TILE_HEIGHT/2;

#if __BOARD_OFFSET
	if( coords.p1.x % 2 == 1 )
		ypos += TILE_HEIGHT / 2;
#endif
	// cyan
	
	putPixel(xpos, ypos, 11);
	putPixel(xpos + 1, ypos, 11);
	putPixel(xpos, ypos + 1, 11);

	xpos = BOARD_BORDER_OFFSET + coords.p2.x * (TILE_WIDTH) + TILE_WIDTH/2;
	ypos = BOARD_BORDER_OFFSET + coords.p2.y * (TILE_HEIGHT) + TILE_HEIGHT/2;
#if __BOARD_OFFSET
	if( coords.p2.x % 2 == 1 )
		ypos += TILE_HEIGHT / 2;
#endif
	// magenta
	putPixel(xpos, ypos, 5);
	putPixel(xpos + 1, ypos, 5);
	putPixel(xpos, ypos + 1, 5);

	xpos = BOARD_BORDER_OFFSET + coords.p3.x * (TILE_WIDTH)  + TILE_WIDTH/2;
	ypos = BOARD_BORDER_OFFSET + coords.p3.y * (TILE_HEIGHT) + TILE_HEIGHT/2;
	
#if __BOARD_OFFSET
	if( coords.p3.x % 2 == 1 )
		ypos += TILE_HEIGHT / 2;
#endif
	// brown
	putPixel(xpos, ypos, 6);
	putPixel(xpos + 1, ypos, 6);
	putPixel(xpos, ypos + 1, 6);
#endif

	return;
}

/* ************************************************************************ */
// FUNCTION:	board_cursorMoveUp
// PURPOSE: To move the cursor up.
// 	NOTE: The display is not updated here.
// 	@see board_renderCursor
// PARAMETERS:	(None)
// RETURNS:  Success or Failure
short int board_cursorMoveUp(  ){

	if (cursor.y <= 0)
	{
		if (cursor.offset == 1 )
			cursor.offset = 0;
		else
			return BOARD_CURSOR_MOVE_FAILED;
	}
	else
	{
		if (cursor.offset == 0 )
		{
			cursor.y--;
			cursor.offset = 1;
		}
		else
			cursor.offset = 0;

	}

	return BOARD_CURSOR_MOVE_SUCCESS;
}

/* ************************************************************************ */
// FUNCTION:	board_cursorMoveDown
// PURPOSE: To move the cursor down
// 	NOTE: The display is not updated here.
// 	@see board_renderCursor
// PARAMETERS:	(None)
// RETURNS:  Success or Failure
short int board_cursorMoveDown(  ){

	if (cursor.y >= BOARD_HEIGHT - 2)
	{
		return BOARD_CURSOR_MOVE_FAILED;
	}
	else
	{
		if (cursor.offset == 1 )
		{
			cursor.y++;
			cursor.offset = 0;
		}
		else
			cursor.offset = 1;
	}

	return BOARD_CURSOR_MOVE_SUCCESS;
}

/* ************************************************************************ */
// FUNCTION:	board_cursorMoveRight
// PURPOSE: To move the cursor Right
// 	NOTE: The display is not updated here.
// 	@see board_renderCursor
// PARAMETERS:	(None)
// RETURNS:  Success or Failure
short int board_cursorMoveRight( ){

	if (cursor.x >= BOARD_WIDTH - 2)
		return BOARD_CURSOR_MOVE_FAILED;
	else
		cursor.x++;

	return BOARD_CURSOR_MOVE_SUCCESS;
}

/* ************************************************************************ */
// FUNCTION:	board_cursorMoveLeft
// PURPOSE: To move the cursor left.
// 	NOTE: The display is not updated here.
// 	@see board_renderCursor
// PARAMETERS:	(None)
// RETURNS:  Success or Failure
short int board_cursorMoveLeft( ){

	if (cursor.x <= 0)
		return BOARD_CURSOR_MOVE_FAILED;
	else
		cursor.x--;

	return BOARD_CURSOR_MOVE_SUCCESS;
}

/* ************************************************************************ */
// FUNCTION:	board_renderCursor
// PURPOSE: 	The purpose of this function is strictly to render (via
// 	drawing pixels) the cursor.  A note that currently the cursor consists
// 	of four pixels.  We may at a later date display a nicer cursor around
// 	whichever tiles are currently selected based on the cursor position.
// PARAMETERS:	(None)
// RETURNS:  	(None)
void board_renderCursor(){

	// Calculate the exact x & y coordinate(s) according to the size of an
	// individual tile and the size of the border.
	int x = (cursor.x + 1) * TILE_WIDTH + BOARD_BORDER_OFFSET;
	int y = (cursor.y + 1) * TILE_HEIGHT + BOARD_BORDER_OFFSET;
	
	// Do the same old'e offset calculation
	if( cursor.offset == 1 )
		y += TILE_HEIGHT / 2;

	// Draw the four pixels that represent the cursor.
	putPixel(x, y, BOARD_CURSOR_COLOR);
	putPixel(x + 1, y, BOARD_CURSOR_COLOR);
	putPixel(x, y + 1, BOARD_CURSOR_COLOR);
	putPixel(x + 1, y + 1, BOARD_CURSOR_COLOR);

	return;
}
/* ************************************************************************ */
// FUNCTION:	board_rotateRight
// PURPOSE: 	The purpose of this function is to rotate 3 tiles at the
//	current cursor position.  A quick note that this function really calls
// 	a more dynamic function (board_rotateRightAtCoords)
//
// @SEE: 	board_rotateRightAtCoords
//
// PARAMETERS:	(None)
// RETURNS:  	(None)
void board_rotateRight(){
	board_rotateRightAtCoords(cursor.x, cursor.y, cursor.offset);
}

/* ************************************************************************ */
// FUNCTION:	board_rotateLeft
// PURPOSE: 	The purpose of this function is to rotate 3 tiles at the
//	current cursor position.  A quick note that this function really calls
// 	a more dynamic function (board_rotateLeftAtCoords).
//
// @SEE: 	board_rotateLeftAtCoords
//
// PARAMETERS:	(None)
// RETURNS:  	(None)
void board_rotateLeft(){
	board_rotateLeftAtCoords(cursor.x, cursor.y, cursor.offset);
}

/* ************************************************************************ */
// FUNCTION:	board_rotateRightAtCoords
// PURPOSE: 	The purpose of this function is to rotate 3 tiles given ...
//
// PARAMETERS:	x - The x-coordinate.
//				y - The y-coordinate.
//				offset - The offset.
//
// RETURNS:  	(None)
void board_rotateRightAtCoords(int x, int y, int offset){
	SPIN_COORDS coords;
	TILE temp;				// A temporary tile where the 'ripple' starts/

	/* Get the tiles that we're going to rotate. */
	board_getSpinTiles(&coords, x, y, offset);

	switch(coords.shapeType) {

		case SHAPE_TOP_RIGHT:	// Intentional Fall Through Mechanism
		case SHAPE_TOP_LEFT:
			temp.type = board[coords.p1.x][coords.p1.y].type;
			temp.star = board[coords.p1.x][coords.p1.y].star;

			board[coords.p1.x][coords.p1.y].type =
				board[coords.p2.x][coords.p2.y].type;
			board[coords.p1.x][coords.p1.y].star =
				board[coords.p2.x][coords.p2.y].star;

			board[coords.p2.x][coords.p2.y].type =
				board[coords.p3.x][coords.p3.y].type;
			board[coords.p2.x][coords.p2.y].star =
				board[coords.p3.x][coords.p3.y].star;

			board[coords.p3.x][coords.p3.y].type = temp.type;
			board[coords.p3.x][coords.p3.y].star = temp.star;

		break;

		case SHAPE_BOTTOM_LEFT:		// Intentional Fall Through Mechanism
		case SHAPE_BOTTOM_RIGHT:

			temp.type = board[coords.p3.x][coords.p3.y].type;
			temp.star = board[coords.p3.x][coords.p3.y].star;

			board[coords.p3.x][coords.p3.y].type =
				board[coords.p2.x][coords.p2.y].type;

			board[coords.p3.x][coords.p3.y].star =
				board[coords.p2.x][coords.p2.y].star;

			board[coords.p2.x][coords.p2.y].type =
				board[coords.p1.x][coords.p1.y].type;
			board[coords.p2.x][coords.p2.y].star =
				board[coords.p1.x][coords.p1.y].star;

			board[coords.p1.x][coords.p1.y].type = temp.type;
			board[coords.p1.x][coords.p1.y].star = temp.star;

		break;
	}

	return;
}

/* ************************************************************************ */
// FUNCTION:	board_rotateLeftAtCoords
// PURPOSE: 	The purpose of this function is to rotate 3 tiles given ...
//
// PARAMETERS:	x - The x-coordinate.
//				y - The y-coordinate.
//				offset - The offset.
//
// RETURNS:  	(None)
void board_rotateLeftAtCoords(int x, int y, int offset){
	SPIN_COORDS coords;
	TILE temp;				// A temporary tile where the 'ripple' starts/

	/* Get the tiles that we're going to rotate. */
	board_getSpinTiles(&coords, x, y, offset);

	switch(coords.shapeType) {

		case SHAPE_BOTTOM_LEFT: // Intentional Fall Through Mechanism
		case SHAPE_BOTTOM_RIGHT:

			temp.type = board[coords.p1.x][coords.p1.y].type;
			temp.star = board[coords.p1.x][coords.p1.y].star;

			board[coords.p1.x][coords.p1.y].type =
				board[coords.p2.x][coords.p2.y].type;
			board[coords.p1.x][coords.p1.y].star =
				board[coords.p2.x][coords.p2.y].star;

			board[coords.p2.x][coords.p2.y].type =
				board[coords.p3.x][coords.p3.y].type;
			board[coords.p2.x][coords.p2.y].star =
				board[coords.p3.x][coords.p3.y].star;

			board[coords.p3.x][coords.p3.y].type = temp.type;
			board[coords.p3.x][coords.p3.y].star = temp.star;

		break;

		case SHAPE_TOP_RIGHT:	// Intentional Fall Through Mechanism
		case SHAPE_TOP_LEFT:

			temp.type = board[coords.p3.x][coords.p3.y].type;
			temp.star = board[coords.p3.x][coords.p3.y].star;

			board[coords.p3.x][coords.p3.y].type =
				board[coords.p2.x][coords.p2.y].type;

			board[coords.p3.x][coords.p3.y].star =
				board[coords.p2.x][coords.p2.y].star;

			board[coords.p2.x][coords.p2.y].type =
				board[coords.p1.x][coords.p1.y].type;
			board[coords.p2.x][coords.p2.y].star =
				board[coords.p1.x][coords.p1.y].star;

			board[coords.p1.x][coords.p1.y].type = temp.type;
			board[coords.p1.x][coords.p1.y].star = temp.star;

		break;
	}

	return;
}

/* ************************************************************************ */
// FUNCTION:	board_getSpinTiles
// PURPOSE:	The purpose of this function is simply to fill the SPIN_COORDS
// 	structure with the appropriate data based upon the parameters.
//
// DEBUGGERS NOTE:  When debugging is enabled expect that this function has the
// effect of printing exactly what shape was selected to fill the SPIN_COORDS
// data structure.  Also note that this is directly connected to the
// board_render function in that one pixel is placed in the 3 POS's returned.
//
// @SEE: 	board_render
//
// PARAMETERS:	ret - The spin-coord structure to modify.
//				x - The x coordinate
//				y - The y coordinate
//				offset - The offset to use.
void board_getSpinTiles(SPIN_COORDS *ret, int x, int y, int offset) {

	if( x % 2 == 1 )
	{
		if( offset == 1 )
		{
			ret->p1.x = x;
			ret->p1.y = y;

			ret->p2.x = x+1;
			ret->p2.y = y+1;

			ret->p3.x = x;
			ret->p3.y = y+1;

			ret->shapeType = SHAPE_BOTTOM_LEFT;

#ifdef __DEBUG
			gotoxy(1,1);
			printf("BOTTOM LEFT\t\t");
#endif
		}
		else
		{
			ret->p1.x = x;
			ret->p1.y = y;

			ret->p2.x = x+1;
			ret->p2.y = y+1;

			ret->p3.x = x+1;
			ret->p3.y = y;
			ret->shapeType = SHAPE_TOP_RIGHT;

#ifdef __DEBUG
			gotoxy(1,1);
			printf("TOP RIGHT\t\t");
#endif
		}
	}
	else
	{
		if( offset == 1 )
		{
			ret->p1.x = x;
			ret->p1.y = y+1;

			ret->p2.x = x+1;
			ret->p2.y = y;

			ret->p3.x = x+1;
			ret->p3.y = y+1;

			ret->shapeType = SHAPE_BOTTOM_RIGHT;

#ifdef __DEBUG
			gotoxy(1,1);
			printf("BOTTOM RIGHT\t\t");
#endif
		}
		else
		{
			ret->p1.x = x;
			ret->p1.y = y;

			ret->p2.x = x;
			ret->p2.y = y+1;

			ret->p3.x = x+1;
			ret->p3.y = y;
			ret->shapeType = SHAPE_TOP_LEFT;

#ifdef __DEBUG
			gotoxy(1,1);
			printf("TOP LEFT\t\t");
#endif
		}
	}

	return;
}

int board_check6Matches(int x, int y, MATCH* matchStructure){
	
	POS p1, p2, p3, p4, p5, p6, center;
	int temp, index;
	
	p1.x = x;
	p1.y = y;
	
	temp = board[p1.x][p1.y].type;
	
	if (temp == TILE_EMPTY)
		return 0;
	
	for (index = 0; index < 2; index++){
		switch(index)
		{
			//    0  1  2
			// 0 [ ][6][ ]
			// 1 [1][c][5]
			// 2 [2][3][4]
			case 0:
				if( !(x < BOARD_WIDTH - 2 && y > 0 && y < BOARD_HEIGHT - 1) )
					continue;	
				if( x % 2 == 1 )
					continue;
					
				p2.x = x;
				p2.y = y + 1;
				
				p3.x = x + 1;
				p3.y = y + 1;
				
				p4.x = x + 2;
				p4.y = y + 1;
				
				p5.x = x + 2;
				p5.y = y;
				
				p6.x = x + 1;
				p6.y = y - 1;
				
				center.x = x + 1;
				center.y = y;
			break;
			
			//    1  2  3
			// 0 [1][6][5]
			// 1 [2][c][4]
			// 2 [ ][3][ ]
			case 1:
				if( !(x < BOARD_WIDTH - 2 && y < BOARD_HEIGHT - 2) )
					continue;
				if( x % 2 == 0 )
					continue;
					
				p2.x = x;
				p2.y = y + 1;
				
				p3.x = x + 1;
				p3.y = y + 2;
				
				p4.x = x + 2;
				p4.y = y + 1;
				
				p5.x = x + 2;
				p5.y = y;
				
				p6.x = x + 1;
				p6.y = y;
				
				center.x = x + 1;
				center.y = y + 1;
					
			break;
		}
		
		if (board[p2.x][p2.y].type == temp
			&& board[p3.x][p3.y].type == temp 
				&& board[p4.x][p4.y].type == temp
					&& board[p5.x][p5.y].type == temp
						&& board[p6.x][p6.y].type == temp ){
			// Set the first POS structure
			matchStructure->match.six_match.p1.x = p1.x;
			matchStructure->match.six_match.p1.y = p1.y;
			
			// Set the second POS structure
			matchStructure->match.six_match.p2.x = p2.x;
			matchStructure->match.six_match.p2.y = p2.y;
			
			// Set the third POS structure
			matchStructure->match.six_match.p3.x = p3.x;
			matchStructure->match.six_match.p3.y = p3.y;
			
			// Set the fourth POS structure
			matchStructure->match.six_match.p4.x = p4.x;
			matchStructure->match.six_match.p4.y = p4.y;
			
			matchStructure->match.six_match.p5.x = p5.x;
			matchStructure->match.six_match.p5.y = p5.y;
			
			matchStructure->match.six_match.p6.x = p6.x;
			matchStructure->match.six_match.p6.y = p6.y;
			
			matchStructure->match.six_match.center.x = center.x;
			matchStructure->match.six_match.center.y = center.y;
			
			// Set the match type:  Note; any later logic should
			// basically check the match type before making any
			// other assumtions about what match was made: (e.g)
			// 		switch( matchStructure.matchType  ...
			matchStructure->matchType = MATCH_TYPE_SIX;
			matchStructure->type = temp;
			
			matchStructure->match.six_match.p1.star = board[p1.x][p1.y].star;
			matchStructure->match.six_match.p2.star = board[p2.x][p2.y].star;
			matchStructure->match.six_match.p3.star = board[p3.x][p3.y].star;
			matchStructure->match.six_match.p4.star = board[p4.x][p4.y].star;
			matchStructure->match.six_match.p5.star = board[p5.x][p5.y].star;
			matchStructure->match.six_match.p6.star = board[p6.x][p6.y].star;
			
			return 1;
		}
	}

	return 0;
}

int board_check4Matches(int x, int y, MATCH* matchStructure){
	POS p1, p2, p3, p4;
	int temp, index;
	
	p1.x = x;
	p1.y = y;
	
	temp = board[p1.x][p1.y].type;
	
	if (temp == TILE_EMPTY)
		return 0;
	
	for (index = 0; index < 3; index++){
		switch(index)
		{
			// [1][2]
			// [3][4]
			case 0:
				if( !(x < BOARD_WIDTH - 1 && y < BOARD_HEIGHT - 1) )
					continue;
				
				p2.x = x + 1;
				p2.y = y;
				p3.x = x;
				p3.y = y + 1;
				p4.x = x + 1;
				p4.y = y + 1;
				
				break;
			
			//  Column:	0  1  2
			//    		  [2]
			// 		   [1][3][4]
			case 1:
				if( !(x < BOARD_WIDTH - 2 && y > 0) )
					continue;
				if( x % 2 == 1 )
					continue;
					
				p2.x = x + 1;
				p2.y = y - 1;
				p3.x = x + 1;
				p3.y = y;
				p4.x = x + 2;
				p4.y = y;
				
				break;
				
			// Column: 1  2  3
			// 		  [1][2][3]
			//    	     [4]
			case 2:
				if( !(x < BOARD_WIDTH - 2 && y < BOARD_HEIGHT - 1) )
					continue;
				if( x % 2 == 0 )
					continue;
				
				p2.x = x + 1;
				p2.y = y;
				p3.x = x + 2;
				p3.y = y;
				p4.x = x + 1;
				p4.y = y + 1;
				
				break;
		}
		
		
		if (board[p2.x][p2.y].type == temp ){
			if (board[p3.x][p3.y].type == temp ){
				if (board[p4.x][p4.y].type == temp ){
					// Set the first POS structure
					matchStructure->match.four_match.p1.x = p1.x;
					matchStructure->match.four_match.p1.y = p1.y;
					
					// Set the second POS structure
					matchStructure->match.four_match.p2.x = p2.x;
					matchStructure->match.four_match.p2.y = p2.y;
					
					// Set the third POS structure
					matchStructure->match.four_match.p3.x = p3.x;
					matchStructure->match.four_match.p3.y = p3.y;
					
					// Set the fourth POS structure
					matchStructure->match.four_match.p4.x = p4.x;
					matchStructure->match.four_match.p4.y = p4.y;
					
					// Set the match type:  Note; any later logic should
					// basically check the match type before making any
					// other assumtions about what match was made: (e.g)
					// 		switch( matchStructure.matchType  ...
					matchStructure->matchType = MATCH_TYPE_FOUR;
					matchStructure->type = temp;
					
					matchStructure->match.four_match.p1.star = board[p1.x][p1.y].star;
					matchStructure->match.four_match.p2.star = board[p2.x][p2.y].star;
					matchStructure->match.four_match.p3.star = board[p3.x][p3.y].star;
					matchStructure->match.four_match.p4.star = board[p4.x][p4.y].star;
					
					return 1;
				}
			}
		}
	}
	
	return 0;
}

/* ************************************************************************ */
// FUNCTION: 	board_check3Matches
// PURPOSE: The purpose of this function is to calculate not only weather or
// not there was a three-match.  But it is also responsible for setting
// matchType, matchStructure.match.three_match and matchStructure.type that is
// according to the current state of the board.
//
// PARAMETERS:	(None)
// RETURNS:		(None)
//
// <Usage:> 	board_check3Matches();
//
// DEVELOPERS NOTE:
// I've got no aversion to checking all four types of three matches here.
int board_check3Matches(int x, int y, MATCH* matchStructure) {

	int type, star, i;
	POS p1, p2, p3;
	
	// Initialize P1
	p1.x = x;
	p1.y = y;

	
	// For any pattern type; the code below will always be the same,
	// as a matter of fact we really don't know what pattern we've got
	// until we've got it in our hands.  Ergo, the last step;  after
	// initializing the variables below, is to set the appriate
	// matchType: but like stated, above; this can only be done once
	// we;ve found the pattern.
	type = board[p1.x][p1.y].type;
	star = board[p1.x][p1.y].star;
	if( star == 0 )
		star = -1;
	
	if (type == TILE_EMPTY)
		return 0;

	// Check for the first of four possible arrangements of tiles
	// for three matches.
	for( i = SHAPE_TOP_LEFT; i <= SHAPE_BOTTOM_LEFT; i++ ) {
				
		switch(i)
		{
		case SHAPE_TOP_LEFT:
			if( !(x < BOARD_WIDTH - 1 && y < BOARD_HEIGHT - 1) )
				continue;
			if( x % 2 == 1 )
				continue;
				
			p2.x = x+1;
			p2.y = y;
				
			p3.x = x;
			p3.y = y+1;
			
			break;
			
		case SHAPE_TOP_RIGHT:
			if( !(x > 0 && y < BOARD_HEIGHT - 1) )
				continue;
			/* SHAPE_TOP_RIGHT can never appear in an even row */
			if( x % 2 == 1 )
				continue;
					
			p2.x = x-1;
			p2.y = y;
				
			p3.x = x;
			p3.y = y+1;
			
			break;
			
		case SHAPE_BOTTOM_RIGHT:
			if( !(x > 0 && y > 0) )
				continue;
			if( x % 2 == 0 )
				continue;
				
			p2.x = x - 1;
			p2.y = y;
				
			p3.x = x;
			p3.y = y-1;
			
			break;
			
		case SHAPE_BOTTOM_LEFT:
			if( !(x < BOARD_WIDTH - 1 && y > 0) )
				continue;
			/* SHAPE_BOTTOM_LEFT can never appear in an even row */
			if( x % 2 == 0 )
				continue;
			
			p2.x = x + 1;
			p2.y = y;
				
			p3.x = x;
			p3.y = y-1;
			break;
		}
		
	
		if (board[p2.x][p2.y].type == type && board[p3.x][p3.y].type == type ||
				board[p2.x][p2.y].star == star && board[p3.x][p3.y].star == star ) {
			// Set the first POS structure
			matchStructure->match.three_match.p1.x = p1.x;
			matchStructure->match.three_match.p1.y = p1.y;

			// Set the second POS structure
			matchStructure->match.three_match.p2.x = p2.x;
			matchStructure->match.three_match.p2.y = p2.y;

			// Set the third POS structure
			matchStructure->match.three_match.p3.x = p3.x;
			matchStructure->match.three_match.p3.y = p3.y;
			
			// Set star stuff in the struct.
			matchStructure->match.three_match.p1.star = board[p1.x][p1.y].star;
			matchStructure->match.three_match.p2.star = board[p2.x][p2.y].star;
			matchStructure->match.three_match.p3.star = board[p3.x][p3.y].star;
			
			// Set the match type:  Note; any later logic should
			// basically check the match type before making any
			// other assumtions about what match was made: (e.g)
			// 		switch( matchStructure.matchType  ...
			matchStructure->matchType = MATCH_TYPE_THREE;
			matchStructure->type = type;
			
			return 1;
		}
	}
	
	return 0;
}

int board_checkPatterns( MATCH* matchStructure ){

	int x, y;
	
	for (x = 0; x < BOARD_WIDTH; x++){
		for (y = 0; y < BOARD_HEIGHT; y++){
			// Six-matches have highest precedence
			if (board_check6Matches(x, y, matchStructure))
				return 1;
				
			// Four-matches have higher precedence than three matches: so they
			// come first (naturally).
			if (board_check4Matches(x, y, matchStructure))
				return 1;
				
			if (board_check3Matches(x, y, matchStructure))
				return 1;
		}
	}

	return 0;
}

/* ************************************************************************ */
// FUNCTION: 	board_hasEmpty
// PURPOSE: The purpose of this function is to check weather or not the board
// 	in its current state is empty.
// PARAMETERS:	(None)
// RETURNS:	BOARD_EMPTY (0x01) - If the board is indeed empty.
//		   !BOARD_EMPTY (0x00) - If the board is not empty.
//
// <Usage:> 	while ( board_hasEmpty() ...
int board_hasEmpty( void ) {

	int x, y;	// xCoord, and yCoord to index through.

	// Index through the entire board looking for a tile of type
	// empty.  IF we find said tile; return that fact otherwise
	// continue on until we've indexed through the entire board.
	for (x = 0; x < BOARD_WIDTH; x++)
	{
		for (y = 0; y < BOARD_HEIGHT; y++)
		{
			if( y == BOARD_HEIGHT - 1 && x % 2 == 1 )
				continue;
				
			if (board[x][y].type == TILE_EMPTY)
				return BOARD_EMPTY;
		}
	}
	
	return !BOARD_EMPTY;
}

void board_dropTiles(){

	int col;
	
	for	(col = 0; col <= BOARD_WIDTH - 1; col++){
		if (board[col][0].type == TILE_EMPTY)	
			tile_generateRandom(&board[col][0], maxColors);
	}
}

/* ************************************************************************ */
// FUNCTION: board_applyGravity
// PURPOSE: As the name suggests this function applies gravity to the whole
// board.  We expect that this function has NO side effects apart from
// modifying the board data structure.
//
// PARAMETERS: 	(None)
// RETURNS:		(None)
// <Usage:>		board_applyGravity();
//
// TODO:
//
int board_applyGravity(){
	
	register int x, y;
	int ret = 0;
	
	for (y = BOARD_HEIGHT - 1; y > 0 ; y--){
		for (x = 0; x < BOARD_WIDTH; x++){
			if( y == BOARD_HEIGHT - 1 &&  x % 2 == 1 )
				continue;
				
			if (board[x][y].type == TILE_EMPTY &&
				board[x][y-1].type != TILE_EMPTY){
					
					tile_swap(&board[x][y], &board[x][y - 1]);
					ret = 1;
			}
		}
	}

	return ret;
}

void board_killTile(int x, int y){
	
	// Don't do anything if the coords are out of range
	if( x < 0 || y < 0 || x > BOARD_WIDTH - 1 || y > BOARD_HEIGHT - 1 )
		return;
	
	// Kill the tile by setting type to TILE_EMPTY
	board[x][y].type = TILE_EMPTY;
	board[x][y].star = 0;
}

void board_makeStar(int x, int y){
	// Check the coords.
	board_checkCoords(x, y);
	
	// Create the star by setting type to superStar.
	board[x][y].type = TILE_SUPERSTAR;	
}

void board_checkCoords(int x, int y){
	
	char buff[256];
	
		// Sanity
	if (x >= BOARD_WIDTH || x < 0)
	{
		sprintf(buff, "xCoord: %d", x);
		errorHandlerLvl(buff, ERROR_DEBUG);	
		errorHandler("board_killTile: xCoord was out of range.");
	}
	
	// Sanity
	if (y >= BOARD_WIDTH || y < 0)
	{
		sprintf(buff, "yCoord: %d", y);
		errorHandlerLvl(buff, ERROR_DEBUG);	
		errorHandler("board_killTile: yCoord was out of range.");
	}
}

void board_killMatch(MATCH *matchStructure){
	
	switch( matchStructure->matchType )
	{
	case MATCH_TYPE_THREE:
		board_killTile(matchStructure->match.three_match.p1.x,
			matchStructure->match.three_match.p1.y);
		board_killTile(matchStructure->match.three_match.p2.x,
			matchStructure->match.three_match.p2.y);
		board_killTile(matchStructure->match.three_match.p3.x,
			matchStructure->match.three_match.p3.y);
		
		break;
	
	case MATCH_TYPE_FOUR:
		board_killTile(matchStructure->match.four_match.p1.x,
			matchStructure->match.four_match.p1.y);
		board_killTile(matchStructure->match.four_match.p2.x,
			matchStructure->match.four_match.p2.y);
		board_killTile(matchStructure->match.four_match.p3.x,
			matchStructure->match.four_match.p3.y);
		board_killTile(matchStructure->match.four_match.p4.x,
			matchStructure->match.four_match.p4.y);
		 
		break;
		
	case MATCH_TYPE_SIX:
		board_killTile(matchStructure->match.six_match.p1.x,
			matchStructure->match.six_match.p1.y);
		board_killTile(matchStructure->match.six_match.p2.x,
			matchStructure->match.six_match.p2.y);
		board_killTile(matchStructure->match.six_match.p3.x,
			matchStructure->match.six_match.p3.y);
		board_killTile(matchStructure->match.six_match.p4.x,
			matchStructure->match.six_match.p4.y);
		board_killTile(matchStructure->match.six_match.p5.x,
			matchStructure->match.six_match.p5.y);
		board_killTile(matchStructure->match.six_match.p6.x,
			matchStructure->match.six_match.p6.y);
			
		break;
	}
}

void board_explodeMatch(MATCH *matchStructure)
{
	switch(matchStructure->matchType)
	{
		case MATCH_TYPE_THREE:
		
			// Explode the three, and around the three.
			board_explodeTile(&matchStructure->match.three_match.p1);
			board_explodeTile(&matchStructure->match.three_match.p2);
			board_explodeTile(&matchStructure->match.three_match.p3);
		
			break;
		
		case MATCH_TYPE_FOUR:
		
			// Explode the four, and around the four.
			board_explodeTile(&matchStructure->match.four_match.p1);
			board_explodeTile(&matchStructure->match.four_match.p2);
			board_explodeTile(&matchStructure->match.four_match.p3);
			board_explodeTile(&matchStructure->match.four_match.p4);
			
			
			break;
	}
}

////////////////////////////////////
// This 
void board_explodeTile(POS* tilePos){

	// Even odd.
	if (tilePos->x % 2 == 1){
		// Center
		
		if (board[tilePos->x][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y);
		
		if (board[tilePos->x][tilePos->y - 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y - 1);
			
		if (board[tilePos->x + 1][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x + 1, tilePos->y);
			
		if (board[tilePos->x + 1][tilePos->y + 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x + 1, tilePos->y +1);
			
		if (board[tilePos->x][tilePos->y + 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y + 1);
			
		if (board[tilePos->x - 1][tilePos->y + 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x - 1, tilePos->y + 1);
			
		if (board[tilePos->x - 1][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x - 1, tilePos->y);
	}
	else {
		
		// Center
		if (board[tilePos->x][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y);
			
		if (board[tilePos->x - 1][tilePos->y - 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x - 1, tilePos->y - 1);
			
		if (board[tilePos->x][tilePos->y - 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y - 1);
			
		if (board[tilePos->x + 1][tilePos->y - 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x + 1, tilePos->y - 1);
			
		if (board[tilePos->x + 1][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x + 1, tilePos->y);
			
		if (board[tilePos->x - 1][tilePos->y].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x - 1, tilePos->y);
			
		if (board[tilePos->x][tilePos->y + 1].type != TILE_SUPERSTAR)
			board_killTile(tilePos->x, tilePos->y + 1);
	}
}
