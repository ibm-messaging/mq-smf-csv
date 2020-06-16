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

/*******************************************************************/
/* Bitfields on z/OS are implemented in the compiler with the      */
/* first bit being the top bit of a word. So we refer to           */
/* flags in that order.                                            */
/*******************************************************************/
#define QESDENCF (0x80000000)

SMFPRINTGLOB;

void printQESD(qesd *p)
{
  unsigned int  flags;
  unsigned int  *u;


      SMFPRINTSTART("QESD",p,conv16(p->qesdll));

      ADDSTREN("Structure"        , p -> qesdstr,12);
      ADDU32  ("Structure_Number" , p -> qesdstrn);
      ADDU32  ("Blocks_SMDS"      , p -> qesdsmbt);
      ADDU32  ("Blocks_Space_Map" , p -> qesdsmbs);
      ADDU32  ("Blocks_Msg_Data"  , p -> qesdsmbd);
      ADDU32  ("Blocks_In_Use"    , p -> qesdsmbu);
      ADDU32  ("Blocks_Free"      , p -> qesdsmbf);
      ADDU32  ("Messages_In_Dataset", p -> qesdsmmc);
      ADDU32  ("Space_Allocated"  , p -> qesdsmar);
      ADDU32  ("Space_Free"       , p -> qesdsmfr);
      ADDU32  ("Space_Reallocated", p -> qesdsmrr);
      ADDU32  ("Space_Cleaned_Up" , p -> qesdsmcr);
      ADDU32  ("Pages_Allocated"  , p -> qesdsmap);
      ADDU32  ("Pages_Freed"      , p -> qesdsmfp);
      ADDU32  ("Pages_Reallocated", p -> qesdsmrp);
      ADDU32  ("Pages_Cleaned_Up" , p -> qesdsmcp);
      ADDU32  ("Failed_Allocates" , p -> qesdsmfl);
      ADDU32  ("Max_Msg_Count"    , p -> qesdsmmm);
      ADDU32  ("Max_Used_Blocks"  , p -> qesdsmmu);
      ADDU32  ("Min_Free_Blocks"  , p -> qesdsmmf);
      ADDU32  ("Buffer_Size"      , p -> qesdbfsz);
      ADDU32  ("Buffer_Total"     , p -> qesdbfto);
      ADDU32  ("Buffer_Shared_Used", p -> qesdbfus);
      ADDU32  ("Buffer_Private_Used", p -> qesdbfup);
      ADDU32  ("Free_Saved_Buffers", p -> qesdbffs);
      ADDU32  ("Free_Empty_Buffers", p -> qesdbffe);
      ADDU32  ("Pool_Waiters"     , p -> qesdbfpw);
      ADDU32  ("Buffer_Waiters"   , p -> qesdbfbw);
      ADDU32  ("Buffer_Get_Count", p -> qesdbfgb);
      ADDU32  ("Buffer_Get_Valid", p -> qesdbfgv);
      ADDU32  ("Buffer_Get_Empty", p -> qesdbfgm);
      ADDU32  ("Buffer_Get_Free" , p -> qesdbfgf);
      ADDU32  ("Buffer_Stole"    , p -> qesdbfgl);
      ADDU32  ("Buffer_No_Get"   , p -> qesdbfgn);
      ADDU32  ("Total_Read_Requests", p -> qesdbfrr);
      ADDU32  ("Read_Saved"      , p -> qesdbfrs);
      ADDU32  ("Read_Partial"    , p -> qesdbfrp);
      ADDU32  ("Total_Write_Requests", p -> qesdbfwr);
      ADDU32  ("Buffer_Free_Valid_Data", p -> qesdbffb);
      ADDU32  ("Buffer_Empty_Discarded", p -> qesdbfdb);
      ADDU32  ("Waited_For_Pool"  , p -> qesdbfwp);
      ADDU32  ("Waited_For_Buffer", p -> qesdbfwb);
      ADDSTCK ("Pool_Wait_Time"   , p -> qesdbfpt);
      ADDU32  ("Buffer_Wait_Time" , p -> qesdbfmu);
      ADDU32  ("Buffer_Max_Used"  , p -> qesdbfmf);
      ADDU32  ("Buffer_Min_Free"  , p -> qesdbfmp);
      ADDU32  ("Buffer_Max_Waiters", p -> qesdbfmb);

      ADDU32  ("High_Allocated_Ctl_Interval", p -> qesdioha);
      ADDU32  ("High_Used_Ctl_Interval", p -> qesdiohu);
      ADDU32  ("Ctl_Interval_Size", p -> qesdioci);
      ADDU32  ("Ctl_Area_Size", p -> qesdioca);

      ADDU32  ("Format_Write_Requests", p -> qesdiofr);
      ADDU32  ("Format_Total_Pages", p -> qesdiofp);
      ADDSTCK ("Format_IO_Time", p -> qesdioft);
      ADDSTCK ("Format_IO_Wait", p -> qesdiofw);

      ADDU32  ("Write_Requests", p -> qesdiowr);
      ADDU32  ("Write_Total_Pages", p -> qesdiowp);
      ADDSTCK ("Write_IO_Time", p -> qesdiowt);
      ADDSTCK ("Write_IO_Wait", p -> qesdioww);

      ADDU32  ("Read_Requests", p -> qesdiorr);
      ADDU32  ("Read_Total_Pages", p -> qesdiorp);
      ADDSTCK ("Read_IO_Time", p -> qesdiort);
      ADDSTCK ("Read_IO_Wait", p -> qesdiorw);

      ADDU32  ("Other_Read_Requests", p -> qesdioor);
      ADDU32  ("Other_Read_Total_Pages", p -> qesdioop);
      ADDSTCK ("Other_Read_IO_Time", p -> qesdioot);
      ADDSTCK ("Other_Read_IO_Wait", p -> qesdioow);

#if CSQDSMF_VERSION >= 914
      if (conv16(p->qesdll)>offsetof(qesd,qesdflag)) {
        u = (unsigned int *)&p->qesdflag;
        flags = conv32(*(u+1));

        if(flags & QESDENCF) {
          ADDSTR("Encrypted","Yes",3);
        }
        else
        {
          ADDSTR("Encrypted","No",3);
        }
      }
#endif

      SMFPRINTSTOP;
      return;
}
