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

void printQCCT(qcct *p)
{
  SMFPRINTSTART("QCCT", p, conv16(p->qcctll));

  ADDSTREN("JOBN",p->qcctjobn,8);
  ADDSTREN("QSGN",p->qcctqsgn,4);
  ADDU32 ("NOCC",p->qcctnocc);
  ADDU32 ("MXCC",p->qcctmxcc);
  ADDU32 ("NOAC",p->qcctnoac);
  ADDU32 ("MXAC",p->qcctmxac);
  ADDU32 ("MXTP",p->qcctmxtp);
  ADDU32 ("MXLU",p->qcctmxlu);
  ADDU32 ("STUS",p->qcctstus);
  SMFPRINTSTOP;

  return;
}
