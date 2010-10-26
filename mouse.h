/** 
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
**
**        ______
**       /_____/\
**      /_____\\ \      FILE:    mouse.h
**     /_____\ \\ /     AUTHOR:  Pete Jensen
**    /_____/ \/ / /    DATE:    Feb 7th, 2005
**   /_____/ /   \//\   VERSION: 3.0.0
**   \_____\//\   / /   
**    \_____/ / /\ /    
**     \_____/ \\ \     HTTP://WWW.HAIRYLOGIC.COM
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

#ifndef __MOUSE_H
#define __MOUSE_H

char IfMouse (void);
void ShowMouse (void);
void HideMouse (void);
void ReadMouse (void);
void SetMouseXY (unsigned int X, unsigned int Y);
void SetMinMaxX (unsigned int Min, unsigned int Max);
void SetMinMaxY (unsigned int Min, unsigned int Max);

unsigned int MouseX, MouseY, MouseB;

char IfMouse ()
{
  unsigned int Result;
  asm mov ax, 0
  asm int 0x33
  asm mov Result, ax
  return Result;
}

void ShowMouse ()
{
  asm mov ax, 0x1
  asm int 0x33
}

void HideMouse ()
{
  asm{ mov ax, 0x2
  int 0x33
  }
}

void ReadMouse ()
{
  asm mov ax, 0x3
  asm int 0x33
  asm mov MouseB, bx
  asm mov MouseX, cx
  asm mov MouseY, dx
}

void SetMouseXY (unsigned int X, unsigned int Y)
{
  asm mov ax, 0x4
  asm mov cx, X
  asm mov dx, Y
  asm int 0x33
}

void SetMinMaxX (unsigned int Min, unsigned int Max)
{
  asm mov ax, 0x7
  asm mov cx, Min
  asm mov dx, Max
  asm int 0x33
}

void SetMinMaxY (unsigned int Min, unsigned int Max)
{
  asm mov ax, 0x8
  asm mov cx, Min
  asm mov dx, Max
  asm int 0x33
}

#endif
