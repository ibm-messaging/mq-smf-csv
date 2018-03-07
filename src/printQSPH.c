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

void printQSPH(qsph *p)
{
  SMFPRINTSTART("QSPH", p, conv16(p->qsphll));

  ADDS32("Current_Size" ,p->qsphstor);
  ADDS32("Previous_Size",p->qsphostor);
  ADDS32("Expansion_Size",p->qsphxstor);
  ADDU16("Record_Size",p->qsphbsize);
  ADDU16("Records_per_Segment",p->qsphbcnt );
  ADDU32("PHB_Count",p->qsphcount);
  if (p->qsphflags & QSPHDSPC) {
    ADDSTREN("Dataspace_Name",(char *)&p->qsphdspnm,8);
  } else {
    ADDSTR("Dataspace_Name","        ",8);
  }
  ADDSTREN("Name",p->qsphname,48);

  ADDSTR("Attr_Fixed", ((p->qsphflags & QSPHFIXED)?"Yes":"No"),3);
  ADDSTR("Attr_Glob", ((p->qsphflags & QSPHGLOB )?"Yes":"No"),3);
  ADDSTR("Attr_DSP  ", ((p->qsphflags & QSPHDSPC )?"Yes":"No"),3);
  ADDSTR("Attr_IVSA ", ((p->qsphflags & QSPHIVSA )?"Yes":"No"),3);
  ADDSTR("Attr_64BIT", ((p->qsphflags & QSPH64BIT)?"Yes":"No"),3);


  SMFPRINTSTOP;

  return;
}

