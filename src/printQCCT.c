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

  ADDSTREN("Job_Name"             ,p->qcctjobn,8);
  ADDSTREN("QSG_Name"             ,p->qcctqsgn,4);
  ADDU32 ("Current_Chl_High_Water",p->qcctnocc);
  ADDU32 ("Current_Chl_Max"       ,p->qcctmxcc);
  ADDU32 ("Active_Chl_High_Water" ,p->qcctnoac);
  ADDU32 ("Active_Chl_Max"        ,p->qcctmxac);
  ADDU32 ("Max_TCP_Chl"           ,p->qcctmxtp);
  ADDU32 ("Max_LU62_Chl"          ,p->qcctmxlu);
  ADDU32 ("Storage_Usage"         ,p->qcctstus);
  SMFPRINTSTOP;

  return;
}
