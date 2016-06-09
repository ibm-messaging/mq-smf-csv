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

  ADDS32("STOR" ,p->qsphstor);
  ADDS32("OSTOR",p->qsphostor);
  ADDS32("XSTOR",p->qsphxstor);
  ADDU16("BSIZE",p->qsphbsize);
  ADDU16("BCNT ",p->qsphbcnt );
  ADDU32("COUNT",p->qsphcount);
  ADDU64("DSPNM",p->qsphdspnm);
  ADDSTREN("NAME",p->qsphname,48);

  ADDSTR("FIXED", ((p->qsphflags & QSPHFIXED)?"Yes":"No"),3);
  ADDSTR("GLOB ", ((p->qsphflags & QSPHGLOB )?"Yes":"No"),3);
  ADDSTR("DSP  ", ((p->qsphflags & QSPHDSPC )?"Yes":"No"),3);
  ADDSTR("IVSA ", ((p->qsphflags & QSPHIVSA )?"Yes":"No"),3);
  ADDSTR("64BIT", ((p->qsphflags & QSPH64BIT)?"Yes":"No"),3);


  SMFPRINTSTOP;

  return;
}

