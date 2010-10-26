/* 
** Copyright 1998-2008, Pete J. Jensen
** Copyright 1998-2008, Robin McNeil
** All Rights Reserved.
** 
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of Hairylogic, Inc.
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of Hairylogic, Inc.
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
**      /_____\\ \      FILE:    graph.c
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    DATE:    Feb 6th, 2002
**   /_____/ /   \//\   VERSION: 3.0.5
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     HTTP://WWW.HAIRYLOGIC.COM
**      \_____\ \\ 
**       \_____\/
**
** This module handles all low level video operations, such as initializing a
** graphics mode, ending a graphics mode, puting pixels etc.
** 
** CONTENTS:
**    initializeGraphicsMode();
**    endGraphicsMode();
**    putPixel();
**    getPixel();
**    loadPaletteData();
**    setPaletteData();
**/
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <dir.h>
#include <alloc.h>
#include <time.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <io.h>
#include <limits.h>

/* Graphics Header File*/
#include "graph.h"
#include "error.h"
#include "globals.h"

/**
 * Function: initializeGraphicsMode
 * Purpose : To initialize the graphics mode into mode 13H; it has the 
 *           properties of 320x200 screensize & 8 bits per pixel.
 * Inputs  : Void
 * Returns : Void
 * Modifies: The current graphics mode.
 * Sample Call : initializeGraphicsMode();
 */
void initializeGraphicsMode()
{
    asm MOV AL, 13H;
    asm MOV AH, 00H;
    asm INT VIDEO_BIOS;
}

/*
 * Function: endGraphicsMode
 * Purpose : To take us out of the 13H graphics mode and back into dos text
 *           mode. This should be run anytime the program hits an error. 
 *           and displays an error message.
 * Inputs  : Void
 * Returns : Void.
 * Modifies: The Current Graphics Mode.
 * Sample Call : endGraphicsMode();
 */
void endGraphicsMode()
{
    asm MOV AL, 3H;
    asm MOV AH, 00H;
    asm INT VIDEO_BIOS;
}

/*
 * Function: putPixel
 * Purpose : To put a pixel @ x,y with given colorByte.
 * Inputs  : word X: The X location of the pixel.
             word Y: The Y location of the pixel.
 * Returns : Void
 * Modifies: Adds one pixel to the graphics screen.
 * Sample Call : putPixel(10, 10, 14);
 */
void putPixel(word X, word Y, byte color)
{
    asm mov DI, VIDEO_RAM;
    asm mov ES, DI;
    asm mov AX, Y;
    asm mov BX, Y;
    asm shl AX, 8;
    asm shl BX, 6;
    asm add AX, BX;
    asm add AX, X;
    asm mov DI, AX;
    asm mov AL, color;
    asm mov ES:[DI], AL;
}

/*
 * Function: getPixel
 * Purpose : To get a pixel @ x,y with & return colorByte.
 * Inputs  : word X: The X location of the pixel.
             word Y: The Y location of the pixel.
 * Returns : The byteColor @ x,y.
 * Sample Call : byteColor = getPixel(10, 10);
 */
byte getPixel(word X, word Y)
{
    byte getColor;
    
    asm MOV AH,0DH
    asm MOV CX,X 
    asm MOV DX,Y 
    asm INT VIDEO_BIOS
    asm MOV getColor, AL

    return getColor;
}

/*
 * Function: loadPaletteData
 * Purpose : To load palette Data
 * Inputs  : The filename where the palette data is stored.
 * Returns : Void
 * Sample Call : loadPaletteData("palette//pal.pal");
 */
void loadPaletteData(char *palFileName)
{
    unsigned char palette_data[256][3];
    FILE *fp;
    unsigned int w, h;
    char errorBuffer[70];

    if ((fp = fopen(palFileName, "rb")) != NULL)
    {
        for (h = 0; h < 256; h++)
            for (w = 0; w < 3; w++)
                if (fscanf(fp,"%d", &palette_data[h][w]) != 1)
                    errorHandler("Unable to read palette data!");
                else continue;

        setVGApalette(&palette_data[0][0]);
        
        if (fclose(fp) == NULL)
            errorHandler("Unable to close palette file!");
    }
    else
    {
        sprintf(errorBuffer, "Could not open palette file: %s", palFileName);
        errorHandler(errorBuffer);
    }
}

/*
 * Function: setVGApalette
 * Purpose : To set the Vga palette; pass this function palette data.
 * Inputs  : Arrary of unsigned chars.
 * Returns : Void
 * Sample Call : setVGApalette(paletteData);
 */
void setVGApalette(unsigned char *buffer)
{
	union REGS reg;
	struct SREGS inreg;

	reg.x.ax = 0x1012;
	
   	segread(&inreg);
	inreg.es = inreg.ds;
   
	reg.x.bx = 0;
	reg.x.cx = 256;
	reg.x.dx = (int)&buffer[0];
   
	int86x(0x10,&reg,&reg,&inreg);
}

void displayHFGFile(char *fileName, unsigned char x, 
                    unsigned char y, signed char transparentByte)
{
    /* File Pointer */
    FILE *fp;
    
    /* Width; Height & Color Information */
    unsigned short int width;
    unsigned short int height;
    unsigned int w, h;
    unsigned int colorByte;

    /* The error buffer to store compound strings. */
    char errorBuffer[80];

    /* Try to open the HFG file. */
    if ((fp = fopen(fileName, "rb")) != NULL)
    {
        /* Try to read a valid image size; if not goto error handler */
        if ( (fscanf(fp, "%d", &width) + fscanf(fp, "%d", &height)) != 2)
            errorHandler("Unable to Read HFG File Width & Height.");

        /* Iterate through the image data. */
        for (h = 0; h < height; h++) 
            for (w = 0; w < width; w++)
                if ( fscanf(fp, "%d", &colorByte) != 1)
                    /* We did not read valid value. */
                    errorHandler("Invalid HFG File format."); 
                else
                    if (colorByte != transparentByte)
                        /* Move to proper location, put colorByte @ x+w, y+h */
                        putPixel((x + w), (y + h), colorByte);

        /* Make 100% sure we close the file, I ran into a bug here because In
         * displaying several pictures one after another I would be unable to
         * open another file. */
        if (fclose(fp) == EOF)
            errorHandler("Unable to close an open HFG file!");
    }
    else
    {
        /* Display */
        sprintf(errorBuffer, "Unable to open a file named: %s", fileName);
        errorHandler(errorBuffer);
    }
}

void displaySplash(char *fileName, signed char transparentColor)
{
    FILE *fp;
    unsigned short int garbage;
    unsigned int w, h;
    const SPLASH_WIDTH = 320;
    const SPLASH_HEIGHT = 200;
    unsigned int colorByte;
    
    if ((fp = fopen(fileName,"rb")) != NULL)
    {
        fscanf(fp,"%d", &garbage);
        fscanf(fp,"%d", &garbage);
        
        for (h = 0; h < SPLASH_HEIGHT; h++)
            for (w = 0; w < SPLASH_WIDTH; w++)
            {
                fscanf(fp, "%d", &colorByte);
                
                if(colorByte != transparentColor)
                    putPixel(w ,h ,colorByte);
            }
        fclose(fp);
    }
    else
        errorHandler("Could not open splash file.");
}
