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

void printQEST(qest *p)
{
  int i;
  int saveDebug = debugLevel;

  /************************************************************************/
  /* Each SMF element actually contains 64 copies of the interesting data */
  /* for each structure.                                                  */
  /************************************************************************/
  for (i=0;i<QESTSNUM;i++)
  {
    /**************************************************************/
    /* Only print the line if something appears to be in use      */
    /* ie if the structure name is assigned. '.' means it is not. */
    /*                                                            */
    /* Override the debug setting for subsequent loops so we only */
    /* dump the data once when debug is enabled; but then reset   */
    /* the value to its original setting on exit from function.   */
    /**************************************************************/
    if (convStr(p->qeststuc[i].qeststr,12)[0] != '.')
    {
      SMFPRINTSTART("QEST",p,conv16(p->qestll));
      debugLevel = 0;

      ADDSTREN("Structure_Name", p->qeststuc[i].qeststr,sizeof(p->qeststuc[i].qeststr));
      ADDU32 ("Structure_Number", p->qeststuc[i].qeststrn);
      ADDU32 ("IXLLSTE_Calls", p->qeststuc[i].qestcsec);
      ADDU32 ("IXLLSTM_Calls", p->qeststuc[i].qestcmec);
      ADDU32 ("IXLLSTE_Redrives", p->qeststuc[i].qestrsec);
      ADDU32 ("IXLLSTM_Redrives", p->qeststuc[i].qestrmec);
      ADDU32 ("Structure_Full", p->qeststuc[i].qestsful);
      ADDU32 ("Max_In_Use_Entries", p->qeststuc[i].qestmnus);
      ADDU32 ("Max_In_Use_Elements", p->qeststuc[i].qestmlus);
      ADDSTCK("IXLLSTE_Calls_Time", p->qeststuc[i].qestsstc);
      ADDSTCK("IXLLSTM_Calls_Time", p->qeststuc[i].qestmstc);

      SMFPRINTSTOP;
    }
  }

  debugLevel = saveDebug;
  return;
}
