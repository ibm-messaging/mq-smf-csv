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

/*******************************************************************/
/* Print out buffer manager (QPST) records.                        */
/*******************************************************************/
void printQSST(qsst *p)
{
  SMFPRINTSTART("QSST", p, conv16(p->qsstlen));

  ADDU32("Fixed_Pools_Alloc"       ,p->qsstdata.qsstgplf);
  ADDU32("Fixed_Pools_Freed"       ,p->qsstdata.qsstfplf);
  ADDU32("Fixed_Pools_Seg_Freed"   ,p->qsstdata.qsstfref);
  ADDU32("Fixed_Pools_Seg_Expand"  ,p->qsstdata.qsstexpf);
  ADDU32("Fixed_Pools_Seg_Contract",p->qsstdata.qsstconf);
  ADDU32("Var_Pools_Alloc"         ,p->qsstdata.qsstgplv);
  ADDU32("Var_Pools_Freed"         ,p->qsstdata.qsstfplv);
  ADDU32("Var_Pools_Seg_Freed"     ,p->qsstdata.qsstfrev);
  ADDU32("Var_Pools_Seg_Expand"    ,p->qsstdata.qsstexpv);
  ADDU32("Var_Pools_Seg_Contract"  ,p->qsstdata.qsstconv);
  ADDU32("Getmain_Count"           ,p->qsstdata.qsstgetm);
  ADDU32("Freemain_Count"          ,p->qsstdata.qsstfrem);
  ADDU32("Nonzero_Return_Code"     ,p->qsstdata.qsstrcnz);
  ADDU32("SOS_Contractions"        ,p->qsstdata.qsstcont);
  ADDU32("SOS_Set"                 ,p->qsstdata.qsstcrit);
  ADDU32("SOS_Abend"               ,p->qsstdata.qsstabnd);

  SMFPRINTSTOP;

  return;
}
