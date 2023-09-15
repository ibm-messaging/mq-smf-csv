/*
 * Copyright (c) 2016 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Mark Taylor - Initial Contribution
 */

#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;
static char *hex = "0123456789ABCDEF";

/*******************************************************************/
/* NAME: printDEBUG                                                */
/*                                                                 */
/* FUNCTION: Debug function to print a buffer in hex               */
/*******************************************************************/
void printDEBUG(char *title, void *buf,int length)
{
  int  i, j;
  unsigned char *p = (unsigned char*)buf;
  int rows;
  int o;
  char line[80];

  if (first)
  {
    fp = fopenext("DEBUG","txt",&newFile);
    first = FALSE;
  }
  if (!fp)
    return;

  fprintf(fp,"-- %s -- (%d bytes) --------------------\n",title,length);

  rows = (length + 15) / 16;

  for (i=0;i<rows;i++)
  {

    memset(line,' ',sizeof(line));
    o=sprintf(line,"%08.8X : ",i *16);

    for (j=0;j<16 && (j + (i*16) < length);j++)
    {
      line[o++] = hex[p[j]  >> 4 ];
      line[o++] = hex[p[j] & 0x0F];
      if (j % 4 == 3)
        line[o++] = ' ';
    }

    o = 48;
    line[o++] = '|';
    for (j=0;j<16 && (j + (i*16) < length);j++)
    {
      line[o++] = EBCDIC_TO_ASCII[p[j]];
    }

    o = 65;
    line[o++] = '|';
    line[o++] = 0;


    fprintf(fp,"%s\n",line);
    p += 16;
  }
  return;
}

FILE *printDEBUGStream()
{
  return fp;
}
