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
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    AUTHOR:  Robin McNeil
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#ifndef __GLOBALS_H
#define __GLOBALS_H

/* Keys */
#define LEFT_ARROW_KEY     	75
#define RIGHT_ARROW_KEY    	77
#define DOWN_ARROW_KEY     	80
#define UP_ARROW_KEY       	72
#define F1_KEY             	59
#define ESC_KEY            	0x1B
#define ROTATE_RIGHT_KEY	'd'
#define ROTATE_LEFT_KEY		'a'

#define ANIM_DELAY_ROTATION			200
#define ANIM_DELAY_GRAVITY			120
#define GENERAL_ANIM_DELAY			130

#define SCORE_DISPLAY_X_COORD		25
#define SCORE_DISPLAY_Y_COORD		10

// Leave this defined if we want debugging turned ON.
#define __DEBUG	
#define __BOARD_OFFSET 				1

#endif