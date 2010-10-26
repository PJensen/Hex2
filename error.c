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

#include "error.h"
#include "graph.h"
#include "globals.h"


/*
 * Function: errorHandler
 * Purpose : To handler errors for the program; and log all erorrs to the
 * errorLog file as defined in the header file.
 * Inputs  : The error message.
 * Returns : Void
 * Modifies: errorlog.
 * Error Checking : Failed closing file.
 *                  Failed logging error.
 * Sample Call    : errorHandler("Could not go to sleep");
 */
void errorHandlerLvl(char *message, int level)
{
    FILE *fp;
     
    if ((fp = fopen(ERRORLOG, "a")) != NULL)
    { 
        fprintf(fp, "\nERROR: %s\n", message);
        fprintf(stderr, "\n\nERROR: %s\n", message);

        if (fclose(fp) == EOF)
            fprintf(stderr, "\n\nFailed to close file.");
    }
    else
    { 
        /* Display Errors To StdErr since we could not log to a file. */
        fprintf(stderr, "\a\nERROR *** : %s\n", message);
        fprintf(stderr, "\n\nFATAL ERROR: Failed logging Error\n");        
    }
    
    if (level == ERROR_SEVERE){
	    
	    /* This Happens Weather we logged the error or not. */
    	fprintf(stderr, "(press any key to continue)\n");
    	getch();
    
	    // End the graphics mode
	    endGraphicsMode();
	    
	    // Clear the screen 
     	clrscr();
     	
    	exit(EXIT_FAILURE);
	}
}



/*
 * Function: errorHandler
 * Purpose : To handler errors for the program; and log all erorrs to the
 * errorLog file as defined in the header file.
 * Inputs  : The error message.
 * Returns : Void
 * Modifies: errorlog.
 * Error Checking : Failed closing file.
 *                  Failed logging error.
 * Sample Call    : errorHandler("Could not go to sleep");
 */
void errorHandler(char *message)
{
    FILE *fp;
    
    // End the graphics mode
    endGraphicsMode();
    
    // Clear the screen 
 	clrscr();
     
    if ((fp = fopen(ERRORLOG, "a")) != NULL)
    { 
        fprintf(fp, "\nERROR: %s\n", message);
        fprintf(stderr, "\n\nERROR: %s\n", message);

        if (fclose(fp) == EOF)
            fprintf(stderr, "\n\nFailed to close file.");
    }
    else
    { 
        /* Display Errors To StdErr since we could not log to a file. */
        fprintf(stderr, "\a\nERROR *** : %s\n", message);
        fprintf(stderr, "\n\nFATAL ERROR: Failed logging Error\n");        
    }
    
	    
	    /* This Happens Weather we logged the error or not. */
    	fprintf(stderr, "(press any key to continue)\n");
    	getch();
    
	exit(EXIT_FAILURE);
}
