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
**      /_____\\ \      FILE:    error.h
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    
**   /_____/ /   \//\   VERSION: 1.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     
**      \_____\ \\ 
**       \_____\/
**/

#ifndef __ERROR_H
#define __ERROR_H

#define ERRORLOG  "error.log"

#define ERROR_SEVERE	0x00
#define ERROR_WARNING	0x01
#define ERROR_DEBUG		0x02

void errorHandler(char *message);
void errorHandlerLvl(char *message, int level);

#endif
