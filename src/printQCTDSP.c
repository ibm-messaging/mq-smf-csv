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

void printQCTDSP(qct_dsp *p)
{
  SMFPRINTSTART("QCTDSP", p, sizeof(qct_dsp));

  ADDU32 ("TSKN",p->qcttskn);
  ADDU32 ("REQN",p->qctreqn);
  ADDSTCK("CPTM",p->qctcptm);
  ADDSTCK("ELTM",p->qcteltm);
  ADDSTCK("WTTM",p->qctwttm);
  ADDS32 ("CHLN",p->qctchln);
  SMFPRINTSTOP;

  return;
}
