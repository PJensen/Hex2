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
**      /_____\\ \      FILE:    sounds.h
**     /_____\ \\ /     AUTHOR:  Robin McNeil
**    /_____/ \/ / /    AUTHOR:  Pete Jensen
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#ifndef __BOARD_H
#define __BOARD_H

#include "tile.h"

#define BOARD_WIDTH					0x0B
#define BOARD_HEIGHT				0x0B

#define BOARD_BORDER_OFFSET			0x0C

#define BOARD_CURSOR_MOVE_FAILED	0x01
#define BOARD_CURSOR_MOVE_SUCCESS	0x00

#define BOARD_CURSOR_COLOR			0x05

#define BOARD_EMPTY					0x01

typedef struct
{
	int x;
	int y;
	
	/* If 1, indicates that the y value is offset by .5 (8 pixels) */
	int offset;
} CURSOR;

typedef struct
{
	int x;
	int y;
	int star;
} POS;

typedef struct
{
	POS p1, p2 ,p3;
	
	enum {
		SHAPE_TOP_LEFT,		// Top Left
		SHAPE_TOP_RIGHT,		// Top Right
		SHAPE_BOTTOM_RIGHT,	// Bottom Right
		SHAPE_BOTTOM_LEFT		// Bottom L3ft
	} shapeType;
	
} SPIN_COORDS;

/* The three_match structure */
// NOTE: Ideally this should only be initialized from within a MATCH data
// structure.
typedef struct
{
	POS p1, p2, p3;
	int shapeType;
} THREE_MATCH;

/* The four_match structure */
// NOTE: Ideally this should only be initialized from within a MATCH data
// structure.
typedef struct
{
	POS p1, p2, p3, p4;
	int shapeType;
} FOUR_MATCH;

/* The six_match structure */
// NOTE: Ideally this should only be initialized from within a MATCH data
// structure.
typedef struct
{
	POS p1, p2, p3, p4, p5, p6, center;
	int shapeType;
} SIX_MATCH;

typedef struct
{
	struct
	{
		THREE_MATCH three_match;
		FOUR_MATCH four_match;
		SIX_MATCH six_match;
		
	} match;
	
	enum {
			MATCH_TYPE_NONE,
			MATCH_TYPE_THREE,
			MATCH_TYPE_FOUR,
			MATCH_TYPE_SIX
		} matchType;
	
	int type;
	
} MATCH;

/* Board Prototypes */
void board_init( void );
int board_hasEmpty( void );
void board_render( void );

/* Board-Cursor Prototypes */
short int board_cursorMoveUp( void );
short int board_cursorMoveDown( void );
short int board_cursorMoveRight( void );
short int board_cursorMoveLeft( void );

/* Board Rotation (right) Prototypes*/
void board_rotateRight( void );
void board_rotateRightAtCoords(int x, int y, int offset);

/* Board Rotation (left) Prototypes*/
void board_rotateLeft( void );
void board_rotateLeftAtCoords(int x, int y, int offset);

/* AUX board operations */
void board_getSpinTiles(SPIN_COORDS *, int x, int y, int offset);
void board_renderCursor( void );
int board_applyGravity( void );

int board_check3Matches(int x, int y, MATCH* matchStructure);
int board_check4Matches(int x, int y, MATCH* matchStructure);

/* Board Pattern Checking Prototypes */
int board_checkPatterns( MATCH* matchStructure );

void board_killTile(int x, int y);
void board_makeStar(int x, int y);

void board_checkCoords(int x, int y);
void board_dropTiles( void );

void board_killMatch(MATCH *matchStructure);
void board_explodeMatch(MATCH *matchStructure);

#endif