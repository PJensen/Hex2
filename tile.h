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
**      /_____\\ \      FILE:    tile.h
**     /_____\ \\ /     AUTHOR:  Robin McNeil
**    /_____/ \/ / /    AUTHOR:  Pete Jensen
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#ifndef __TILE_H
#define __TILE_H

/* Basic tile definitions */
#define TILE_WIDTH			16
#define TILE_HEIGHT			16

#define TILE_LEVELS			7

typedef struct
{
	enum TILE_COLOR
	{
		TILE_EMPTY = -1,
		TILE_RED = 0,
		TILE_BLUE = 1,
		TILE_GREEN = 2,
		TILE_INDIGO = 3,
						// Add new tiles BEFORE yellow.
		TILE_YELLOW = 4,
		
		TILE_SUPERSTAR = 5,
	} type;
	
	int star;
	
} TILE;

//typedef void *TILE_TEXTURE_PTR;
//typedef int** TILE_TEXTURE_PTR;
typedef int (*TILE_TEXTURE_PTR)[TILE_HEIGHT][TILE_WIDTH];

/* Tile data(s) */
extern int TILE_GREEN_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_RED_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_YELLOW_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_BLUE_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_INDIGO_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_STAR_TEXTURE[TILE_HEIGHT][TILE_WIDTH];
extern int TILE_SUPERSTAR_TEXTURE[TILE_WIDTH][TILE_HEIGHT];

void tile_initializeTileLevels( void );
void tile_render(int x, int y, TILE *tile);
void tile_swap(TILE *a, TILE *b);

void tile_generateRandom(TILE* tile, int maxColor);

#endif
