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


static char *verb[] = {
  "Cmit",
  "Prep"
};

SMFPRINTGLOB;

void printWTAS(wtas *p)
{
  int i,j;
  cfstat *cf;
  char index[64];
  char indexet[64];

  SMFPRINTSTART("WTAS",p,conv16(p->wtaslen));

  ADDS32 ("Record_Version" , p->wtasver);
  ADDSTRB("Correl", (char *)&p->wtasstrt,16);
  ADDTIME("Start_Time", p->wtasstrt);
  ADDS32 ("Block_Count", p->wtaswqct);

  /*-----------------------------------------------------------*/
  /* STCK elapse waiting for a each latch class and the number */
  /* of times that a wait occurred.                            */
  /*-----------------------------------------------------------*/
  for (i = 0; i < 32; i++)
  {
    /* Latches are known by index from 1, not from zero  */
    sprintf(index,"%d",i+1);
    ADDSTCKIDX("Latch_Wait_Time",index,p->wtaslwet[i]);
    ADDS32IDX ("Latch_Wait_Count",index,p->wtaslwn[i]);
  }

  ADDSTRB("Longest_Latch", p->wtaslown,8);

  /*-----------------------------------------------------------*/
  /* other statistics                                          */
  /*-----------------------------------------------------------*/
  ADDSTCK("Other_MQI_ET"   , p->wtasotet);
  ADDSTCK("Other_MQI_CT"   , p->wtasotct);
  ADDS32 ("Other_MQI_Count", p->wtasotn);
  ADDSTCK("Max_Latch_Wait_Time", p->wtasmlw);
  ADDS32 ("Max_Latch_Wait_ID", p->wtasmlwn);

  /*-----------------------------------------------------------*/
  /* Commit statistics                                         */
  /*-----------------------------------------------------------*/
  ADDSTCK("Commit_ET"   , p->wtascmet);
  ADDSTCK("Commit_CT"   , p->wtascmct);
  ADDS32 ("Commit_Count", p->wtascmn);

  /*-----------------------------------------------------------*/
  /* Backout statistics                                        */
  /*-----------------------------------------------------------*/
  ADDSTCK("Backout_ET"   , p->wtasbaet);
  ADDSTCK("Backout_CT"   , p->wtasbact);
  ADDS32 ("Backout_Count", p->wtasban);

  /*-----------------------------------------------------------*/
  /* Journal and logging information                           */
  /*-----------------------------------------------------------*/
  ADDSTCK("Journal_Write_ET"   , p->wtasjwet);
  ADDU32 ("Journal_Write_Count", p->wtasjwn);
  ADDU32 ("Journal_Write_Bytes", p->wtasjwb);
  ADDSTCK("Journal_Forced_Write_ET", p->wtasjcet);
  ADDU32 ("Journal_Fored_Write_Count", p->wtasjcn);
  ADDU32 ("Task_Suspend_Count", p->wtassusn);
  ADDSTCK("Task_Suspend_Time", p->wtassuse);

  /*-----------------------------------------------------------*/
  /* Pageset 0 logging activity                                */
  /*-----------------------------------------------------------*/
  ADDSTCK("Pageset0_Log_ET"   , p->wtaspse0);
  ADDU32 ("Pageset0_Log_Count", p->wtaspsn0);

  /*-----------------------------------------------------------*/
  /* DB2 activity                                              */
  /*-----------------------------------------------------------*/
  ADDSTCK("DB2_Elapse_Thread", p->wtasdbet);
  ADDSTCK("DB2_Elapse_Server", p->wtasdbes);
  ADDSTCK("DB2_Max_Elapse_Thread", p->wtasdbmt);
  ADDSTCK("DB2_Max_Elapse_Server", p->wtasdbms);
  ADDU32 ("DB2_Request_Count", p->wtasdbct);

  ADDU64("DB2_Bytes_Written", p->wtasdbpt);
  ADDU64("DB2_Bytes_Read"   , p->wtasdbgt);

  /*-----------------------------------------------------------*/
  /* CF activity                                               */
  /*-----------------------------------------------------------*/
  ADDU32 ("CF_STE_Call_Count",p->wtascsec);
  ADDU32 ("CF_STM_Call_Count",p->wtascmec);
  ADDU32 ("CF_STE_Redrive_Count",p->wtasrsec);
  ADDU32 ("CF_STM_Redrive_Count",p->wtasrmec);
  ADDSTCK("CF_STE_Elapsed",p->wtassstc);
  ADDSTCK("CF_STM_Elapsed",p->wtasmstc);

  /*-----------------------------------------------------------*/
  /* Interval data and page counts                             */
  /*-----------------------------------------------------------*/
  ADDTIME("WTAS_Interval_Start", p->wtasints);
  ADDTIME("WTAS_Interval_End"  , p->wtasinte);
  ADDS32 ("Get_Pages_Old", p->wtasgpo);
  ADDS32 ("Get_Pages_New", p->wtasgpn);

  /*-----------------------------------------------------------*/
  /* Version 7 (WTAS_VER_4) inclusions                         */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_4)
  {
    ADDSTCK("Publish_High_Water", p->wtaspbhw);
    ADDSTCK("Publish_Total_ET"  , p->wtaspbtt);

    ADDSTCK("Topic_ET", p->wtastpet);
    ADDSTCK("Topic_CT", p->wtastpct);
    ADDU32 ("Topic_Count", p->wtastpn);

    ADDSTCK("Subscribe_ET", p->wtassuet);
    ADDSTCK("Subscribe_CT", p->wtassuct);
    ADDU32 ("Subscribe_Count", p->wtassun );
    ADDU32 ("Subscribe_Selector_Count", p->wtassusc);
    ADDU32 ("Subscribe_Max_Select_Len", p->wtassusl);

    ADDSTCK("SubReq_ET", p->wtassqet);
    ADDSTCK("SubReq_CT", p->wtassqct);
    ADDU32 ("SubReq_Count", p->wtassqn);

    ADDSTCK("MQCTL_ET", p->wtasctet);
    ADDSTCK("MQCTL_CT", p->wtasctct);
    ADDU32 ("MQCTL_Count", p->wtasctn);
    ADDSTCK("MQCTL_CPU", p->wtasctsr);

    ADDSTCK("MQSTAT_ET", p->wtasstet);
    ADDSTCK("MQSTAT_CT", p->wtasstct);
    ADDU32 ("MQSTAT_Count", p->wtasstn);

    for (i=0;i<2;i++)
    {
      if (i==0)
        cf = &p->commitcf;
      else
        cf = &p->prepcf;

      for (j=0;j<14;j++)/* Last few of the array are unused, so don't print them*/
      {
        if (j==9)
         continue;                                       /* An unused entry*/
        if (first) {
          /* Easier to understand if "ET" is at the end of the headings. */
          /* So there are two types of heading needed in this loop.      */
          sprintf(index,  "%s_%s"   ,verb[i],strCfStatType(j));
          sprintf(indexet,"%s_%s_ET",verb[i],strCfStatType(j));
        }
        ADDU32IDX("CFCount"   , index,  cf->type[j].cfcount);
        ADDU32IDX("CFSync"    , index,  cf->type[j].cfsyncn);
        ADDU32IDX("CFSync"    , indexet,cf->type[j].cfsyncet);
        ADDU32IDX("CFAsync"   , index,  cf->type[j].cfasyncn);
        ADDU32IDX("CFAsync"   , indexet,cf->type[j].cfasyncet);
      }
    }


  }                                                  /* End if (WTAS_VER_4)*/

  /*-----------------------------------------------------------*/
  /* Version 7.1 (WTAS_VER_5) inclusions                       */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_5)
  {
    ADDU32 ("SMDS_Write_Bytes", p -> wtassmwb);
    ADDU32 ("SMDS_Read_Bytes", p -> wtassmrb);
    ADDU32 ("SMDS_Write_Pages", p -> wtassmwp);
    ADDU32 ("SMDS_Read_Pages", p -> wtassmrp);
    ADDU32 ("SMDS_Saved_Reads", p -> wtassmrs);

    ADDSTCK("SMDS_IO_Wait_Time", p -> wtassmwt);
  }

  /*-----------------------------------------------------------*/
  /* Version 8   (WTAS_VER_8) inclusions                       */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_8)
  {
    ADDSTCK("Prepare_ET", p -> wtaspret);
    ADDSTCK("Prepare_CT", p -> wtasprct);
    ADDU32 ("Prepare_Count", p -> wtasprn);
    for (i = 0; i < 32; i++)
    {
      sprintf(index,"%d",i+1);
      ADDSTCKIDX("Longest_Latch", index, p -> wtaslmax[i]);
    }
  }

  SMFPRINTSTOP;
  return;
}
