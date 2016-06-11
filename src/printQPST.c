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

/*******************************************************************/
/* Bitfields on z/OS are implemented in the compiler with the      */
/* first bit being the top bit of a word. So we refer to           */
/* flags in that order.                                            */
/*******************************************************************/
#define QPSTLOC  (0x80000000)
#define QPSTF4KB (0x40000000)

SMFPRINTGLOB;

void printQPST(qpst *p)
{
  unsigned int flags;
  unsigned int *u;

  SMFPRINTSTART("QPST", p, conv16(p->qpstll));

  ADDS32("BufferPool",p->qpstpool);
  ADDS32("Buffer_Count",p->qpstnbuf);
  ADDS32("Lowest_Stealable",p->qpstcbsl);
  ADDS32("Current_Stealable",p->qpstcbs );
  ADDS32("Getp_Old_Requests",p->qpstgetp);
  ADDS32("Getp_New_Requests",p->qpstgetn);
  ADDS32("DASD_Read",p->qpstrio );
  ADDS32("Set_Write_Pages",p->qpststw );
  ADDS32("Pages_Written",p->qpsttpw );
  ADDS32("DASD_Write",p->qpstwio );
  ADDS32("Sync_Writes",p->qpstimw );
  ADDS32("Defer_Write_THold_Reached",p->qpstdwt );
  ADDS32("Sync_Write_THold_Reached",p->qpstdmc );
  ADDS32("Buffer_Steals",p->qpststl );
  ADDS32("Buffer_Steals_Hash_Changes",p->qpststla);
  ADDS32("Suspend_No_Buffers",p->qpstsos );

  /*****************************************************************/
  /* Extract the word containing bitfield flags. Make sure it's in */
  /* the right order, then test the flags.                         */
  /*****************************************************************/
  u = (unsigned int  *)&p->qpstsos;
  flags = conv32(*(u+1));

  if(flags & QPSTLOC)
  {
    ADDSTR("LOC","Above",6);
  }
  else
  {
    ADDSTR("LOC","Below",6);
  }

  if(flags & QPSTF4KB)
  {
    ADDSTR("FIX","Fixed",6);
  }
  else
  {
    ADDSTR("FIX","Paged",6);
  }

  SMFPRINTSTOP;

  return;
}
