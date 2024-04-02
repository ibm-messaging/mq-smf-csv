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

  ADDS32("Open",p->qmstopen);
  ADDS32("Close",p->qmstclos);
  ADDS32("Get" ,p->qmstget );
  ADDS32("Put" ,p->qmstput );
  ADDS32("Put1",p->qmstput1);
  ADDS32("Inq" ,p->qmstinq );
  ADDS32("Inql",p->qmstinql);
  ADDS32("Set" ,p->qmstset );
  ADDS32("Endw",p->qmstendw);
  ADDS32("Close_Handles",p->qmstcalh);
  if (conv16(p->qmstll) >  offsetof(qmst,  qmstsub))/* Fields added for V7.0 API calls*/
  {
    ADDS32("Sub"   ,p->qmstsub);
    ADDS32("SubReq",p->qmstsubr);
    ADDS32("CB"    ,p->qmstcb);
    ADDS32("CTL"   ,p->qmstctl);
    ADDS32("Status",p->qmststus);
    ADDS32("Pubs"  ,p->qmstpubs);
  }
#if CSQDSMF_VERSION >= 903
  if (conv16(p->qmstll) >  offsetof(qmst,  qmstspp))/* Fields added for V9.0.3 */
  {
    ADDS64("PersPuts",    p->qmstspp);
    ADDS64("NonPersPuts", p->qmstsnp);
    ADDS64("PersPutBytes",    p->qmstpbp);
    ADDS64("NonPersPutBytes", p->qmstnbp);
  }
#endif

  SMFPRINTSTOP;

  return;
}

