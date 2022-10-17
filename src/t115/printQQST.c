/*
 * Copyright (c) 2016,2022 IBM Corporation and other Contributors.
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

static char *qqstPart[] = { "FULL",   
                            "PARTIAL",
                             NULL};

static char *qqstDisp[] = { "PRIVATE",   
                            "SHARED",
                             NULL};

static char *qqstUncm[] = { "NO",      
                            "YES",
                            NULL};
/* Queue statistics */
void printQQST(qqst *p)
{
  char *c;
  unsigned int *u;
  uint32_t flags;

  SMFPRINTSTART("QQST", p, conv16(p->qqstll));

  ADDSTRENS("Queue",    p->qqstqnam);

  flags = conv32(p->qqstflag);
  if ((flags & QQSTDISP) == QQSTDISP) {
    c = qqstDisp[1];
  } else {
    c = qqstDisp[0];
  }
  ADDSTR("Queue_Disp",  c,fieldWidth(qqstDisp));

  /* When the record is partial the depth will be shown as 0 but might be incorrect */
  if ((flags & QQSTPART) == QQSTPART) {
    c = qqstPart[1];
  } else {
    c = qqstPart[0];
  }
  ADDSTR("Record",  c,fieldWidth(qqstPart));

  ADDSTRENS("QSG_Name", p->qqstqsgn);
  ADDSTRENS("Structure",p->qqstcfst);

  ADDS16("Pageset"  ,   p->qqstpsid); /* Pageset/Bufferpool will be -1 for shared queues */
  ADDS16("BufferPool",  p->qqstbpid); /*   and for unallocated private queues            */
  ADDS32("Current_Depth",p->qqstdpth);

  /* qqstopct was the first field added after the original structure */
  if (conv16(p->qqstll)>offsetof(qqst,qqstopct))
  {
    ADDS32("Output_Handles",p->qqstopct);
    ADDS32("Input_Handles" ,p->qqstipct);
    ADDS32("Max_Age"       ,p->qqstmage);
    ADDS32("QTime_Short"   ,p->qqstqtst);
    ADDS32("QTime_Long"    ,p->qqstqtlt);
    ADDTIME("Last_Put"     ,p->qqstlput);
    ADDTIME("Last_Get"     ,p->qqstlget);
    if ((flags & QQSTUNCM) == QQSTUNCM) {
      c = qqstUncm[1];
    } else {
      c = qqstUncm[0];
    }
    ADDSTR("Uncommitted",  c,fieldWidth(qqstUncm));
  }

  SMFPRINTSTOP;

  return;
}

