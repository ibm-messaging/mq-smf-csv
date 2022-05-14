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
    c = "SHARED";
  } else {
    c = "PRIVATE";
  }
  ADDSTR("Queue_Disp",  c,7); /* 7 = max length of SHARED/PRIVATE */

  /* When the record is partial the depth will be shown as 0 but might be incorrect */
  if ((flags & QQSTPART) == QQSTPART) { 
    c = "PARTIAL";
  } else {
    c = "FULL";
  }
  ADDSTR("Record",  c,7); /* 7 = max length of PARTIAL/FULL */

  ADDSTRENS("QSG_Name", p->qqstqsgn);
  ADDSTRENS("Structure",p->qqstcfst);

  ADDS16("Pageset"  ,   p->qqstpsid); /* Pageset/Bufferpool will be -1 for shared queues */
  ADDS16("BufferPool",  p->qqstbpid); /*   and for unallocated private queues            */
  ADDS32("Current_Depth",p->qqstdpth);

  SMFPRINTSTOP;

  return;
}
