/* 
** Copyright 1998-2008, Pete J. Jensen
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
**      /_____\\ \      FILE:    error.c
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#ifndef __GRAPH_H
#define __GRAPH_H

/* Defines for graphics module */
#define VIDEO_BIOS      10H
#define VIDEO_RAM       0xA000 

/* Screen size, width & height defined */
#define SCREEN_SIZE     (320 * 200)
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200

/* Typedefs for byte & word */
typedef unsigned char byte;
typedef unsigned short word;

/* Prototypes for Graphics Module */
void initializeGraphicsMode(void);
void endGraphicsMode(void);
void putPixel(word X, word Y, byte color);
byte getPixel(word X, word Y);

void loadPaletteData(char *palFileName);
void setVGApalette(unsigned char *buffer);

void displayHFGFile(char *fileName, unsigned char x, 
                    unsigned char y, signed char transparentByte);
                    
void displaySplash(char *fileName, signed char transparentColor);


#endif

