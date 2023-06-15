/*
 * Copyright (c) 2016,2023 IBM Corporation and other Contributors.
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

void printQCTADP(qct_adp *p,uint32_t idx)
{
  SMFPRINTSTART("QCTADP", p, sizeof(qct_adp));

  ADDU32 ("Task_Number"      ,p->qcttskn);
  ADDU32NC("Task_Index"      ,idx);
  ADDU32 ("Request_Count"    ,p->qctreqn);
  ADDSTCK("Task_CPU_Time"    ,p->qctcptm);
  ADDSTCK("Task_Elapsed_Time",p->qcteltm);
  ADDSTCK("Task_Wait_Time"   ,p->qctwttm);
  SMFPRINTSTOP;

  return;
}
