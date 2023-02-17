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

void printQIST(qist *p)
{
  SMFPRINTSTART("QIST", p, conv16(p->qistll));

  ADDU32 ("Msg_Get"             ,p->qistmget);
  ADDU32 ("Msg_Put"             ,p->qistmput);
  ADDU32 ("Msg_Browse_Lock"     ,p->qistmblr);
  ADDU32 ("Object_Create"       ,p->qistdcre);
  ADDU32 ("Object_Put"          ,p->qistdput);
  ADDU32 ("Object_Delete"       ,p->qistddel);
  ADDU32 ("Object_Get"          ,p->qistdget);
  ADDU32 ("Object_Locate"       ,p->qistdloc);
  ADDU32 ("Msg_Count"           ,p->qistmcnt);
  ADDU32 ("Stgclass_Change"     ,p->qistalst);
  ADDU32 ("Lock_Marked_Msg"     ,p->qistlomm);
  ADDU32 ("Delete_Marked_Msg"   ,p->qistdlmm);
  ADDU32 ("Enumerates"          ,p->qistenum);
  /* ADDU32 ("ReadAhead_IO"        ,p->qistraio);*/  /* The ReadAhead fields are never filled in, */
  /* ADDU32 ("ReadAhead_BufferPool",p->qistrabp);*/  /* so not going to print them here           */
  ADDU32 ("Gets_From_Disk"      ,p->qistgetd);
  ADDU32 ("Gets_From_BufferPool",p->qistgetb);

  SMFPRINTSTOP;

  return;
}
