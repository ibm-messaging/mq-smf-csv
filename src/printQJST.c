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

void printQJST(qjst *p)
{
  int i,j;
  char index[32];

  SMFPRINTSTART("QJST", p, conv16(p->qjstll));

  ADDS32("WRT_Request_Wait"      ,p->qjstwrw );
  ADDS32("WRT_Request_NoWait"    ,p->qjstwrnw);
  ADDS32("WRT_Request_Force"     ,p->qjstwrf );
  ADDS32("Unavailable_Buffer_Count",p->qjstwtb );
  ADDS32("Log_Read_Output_Buffer",p->qjstrbuf);
  ADDS32("Log_Read_Active_Log"   ,p->qjstract);
  ADDS32("Log_Read_Archive_Log"  ,p->qjstrarh);
  ADDS32("Tape_Contention_Delays",p->qjsttvc );
  ADDS32("BSDS_Access "          ,p->qjstbsds);
  ADDS32("Active_Log_CI"         ,p->qjstbffl);
  ADDS32("Buffer_Writes"         ,p->qjstbfwr);
  ADDS32("Archive_Read"          ,p->qjstalr );
  ADDS32("Archive_Write"         ,p->qjstalw );
  ADDS32("CI_Offloads"           ,p->qjstciof);
  ADDS32("Checkpoints"           ,p->qjstllcp);
  ADDS32("Read_Delayed"          ,p->qjstwur );
  ADDS32("Lookahead_Mount_Attempts",p->qjstlama);
  ADDS32("Lookahead_Mount_Succeed",p->qjstlams);
  ADDS32("Log_Request_Suspends"  ,p->qjstlsus);
  ADDS32("Log_Writes"            ,p->qjstlogw);
  ADDS32("Log_CI"                ,p->qjstciwr);
  ADDS32("Log_CI_Serial"         ,p->qjstserw);
  ADDS32("Log_Write_Threshold"   ,p->qjstthrw);
  ADDS32("Log_Write_Buffer_Paged",p->qjstbpag);

  if (conv16(p->qjstll)>offsetof(qjst,qjstcompr))
  {
    /* Array of 1 entry! Reserved space follows for 2 more, but they   */
    /* are not currently used. If the others are ever used, change the */
    /* macro to use ADDSxxIDX                                          */
    for (i=0;i<1;i++)
    {
      if (first)
        sprintf(index,"%d",i+1);

      ADDS32("Comp_Requests"            , p->qjstcompr[i].qjstcmpreq);
      ADDS32("Comp_Fail"                , p->qjstcompr[i].qjstcmpfail);
      ADDS64("Comp_Uncompressed_Bytes"  , p->qjstcompr[i].qjstcmpuncmp);
      ADDS64("Comp_Compressed_Bytes"    , p->qjstcompr[i].qjstcmpcomp);
      ADDS32("Decomp_Requests"          , p->qjstcompr[i].qjstdecreq);
      ADDS32("Decomp_Fail"              , p->qjstcompr[i].qjstdecfail);
      ADDS64("Decomp_Uncompressed_Bytes", p->qjstcompr[i].qjstdecuncmp);
      ADDS64("Decomp_Compressed_Bytes"  , p->qjstcompr[i].qjstdeccomp);
    }
  }

  if (conv16(p->qjstll)>offsetof(qjst,qjstio[0]))
  {
    for (i=0;i<2;i++)
    {
      for (j=0;j<2;j++)
      {
        if (first)
          sprintf(index,"%d.%d",i+1,j+1);

        ADDU32IDX("IO_Count"      , index , p->qjstio[i].qjstiotype[j].qjstiocount);
        ADDU32IDX("IO_CI"         , index , p->qjstio[i].qjstiotype[j].qjstioci);
        ADDSTCKIDX("IO_Total_Time", index , p->qjstio[i].qjstiotype[j].qjstiototio);
        ADDSTCKIDX("IO_Total_Suspend_Time", index , p->qjstio[i].qjstiotype[j].qjstiototsus);

        ADDSTCKIDX("IO_Max_Duration" , index , p->qjstio[i].qjstiotype[j].qjstiomaxio);
        ADDTIMEIDX("IO_Max_Time"     , index ,p->qjstio[i].qjstiotype[j].qjstiomaxioT);
        ADDSTRENIDX("IO_Max_Log_ID"  , index , p->qjstio[i].qjstiotype[j].qjstiomaxiol,8);

        ADDSTCKIDX("IO_Max_Suspend_Dur"     , index, p->qjstio[i].qjstiotype[j].qjstiomaxsus);
        ADDTIMEIDX("IO_Max_Suspend_Time"    , index, p->qjstio[i].qjstiotype[j].qjstiomaxsusT);
        ADDSTRENIDX("IO_Max_Suspend_Log_ID" , index, p->qjstio[i].qjstiotype[j].qjstiomaxsusl,8);
      }
    }

    ADDU64 ("Writer_Idle_Time"     , p->qjstslptu);
    ADDU64 ("IO_Time_Sum_Squares_1", p->qjstiosqu[0]);
    ADDU64 ("IO_Time_Sum_Squares_2", p->qjstiosqu[1]);
  }

#if CSQDSMF_VERSION >= 912
  /* MQ 9.1.2 added support for zHyperwrite logging */
  if (conv16(p->qjstll)>offsetof(qjst,qjstcp1n))
  {
      ADDS32("Copy1_New_Logs"           , p->qjstcp1n);
      ADDS32("Copy2_New_Logs"           , p->qjstcp2n);
      ADDS32("New_Logs_ZHW_Capable"     , p->qjsthwc);
      ADDS32("New_Logs_ZHW_Enabled"     , p->qjsthwe);
  }
#endif

#if CSQDSMF_VERSION >= 914
  /* MQ 9.1.4 added support for encrypted datasets */
  if (conv16(p->qjstll)>offsetof(qjst,qjstencr)) {
      ADDS32("New_Logs_Encrypted",p->qjstencr);
  }
#endif

  SMFPRINTSTOP;

  return;
}

