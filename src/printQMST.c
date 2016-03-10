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

void printQMST(qmst *p)
{
  SMFPRINTSTART("QMST", p, conv16(p->qmstll));

  ADDS32("OPEN",p->qmstopen);
  ADDS32("CLOSE",p->qmstclos);
  ADDS32("GET ",p->qmstget );
  ADDS32("PUT ",p->qmstput );
  ADDS32("PUT1",p->qmstput1);
  ADDS32("INQ ",p->qmstinq );
  ADDS32("INQL",p->qmstinql);
  ADDS32("SET ",p->qmstset );
  ADDS32("ENDW",p->qmstendw);
  ADDS32("CALH",p->qmstcalh);
  if (conv16(p->qmstll) >  offsetof(qmst,  qmstsub))/* Fields added for V7.0 API calls*/
  {
    ADDS32("SUB ",p->qmstsub);
    ADDS32("SUBR",p->qmstsubr);
    ADDS32("CB  ",p->qmstcb);
    ADDS32("CTL ",p->qmstctl);
    ADDS32("STUS",p->qmststus);
    ADDS32("PUBS",p->qmstpubs);
  }
  SMFPRINTSTOP;

  return;
}

