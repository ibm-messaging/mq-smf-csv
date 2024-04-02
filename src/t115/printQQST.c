/*
 * Copyright (c) 2016,2024 IBM Corporation and other Contributors.
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

static char *qqstPart[] = { "FULL",
                            "PARTIAL",
                             NULL};

static char *qqstDisp[] = { "PRIVATE",
                            "SHARED",
                             NULL};

static char *qqstUncm[] = { "NO",
                            "YES",
                            NULL};
/* Queue statistics */
void printQQST(qqst *p)
{
  char *c;
  unsigned int *u;
  uint32_t flags;

  SMFPRINTSTART("QQST", p, conv16(p->qqstll));

  ADDSTRENS("Queue",    p->qqstqnam);

  flags = conv32(p->qqstflag);
  if ((flags & QQSTDISP) == QQSTDISP) {
    c = qqstDisp[1];
  } else {
    c = qqstDisp[0];
  }
  ADDSTR("Queue_Disp",  c,fieldWidth(qqstDisp));

  /* When the record is partial the depth will be shown as 0 but might be incorrect */
  if ((flags & QQSTPART) == QQSTPART) {
    c = qqstPart[1];
  } else {
    c = qqstPart[0];
  }
  ADDSTR("Record",  c,fieldWidth(qqstPart));

  ADDSTRENS("QSG_Name", p->qqstqsgn);
  ADDSTRENS("Structure",p->qqstcfst);

  ADDS16("Pageset"  ,   p->qqstpsid); /* Pageset/Bufferpool will be -1 for shared queues */
  ADDS16("BufferPool",  p->qqstbpid); /*   and for unallocated private queues            */
  ADDS32("Current_Depth",p->qqstdpth);

  /* qqstopct was the first field added after the original structure */
  if (conv16(p->qqstll)>offsetof(qqst,qqstopct))
  {
    ADDS32("Output_Handles",p->qqstopct);
    ADDS32("Input_Handles" ,p->qqstipct);
    ADDS32("Max_Age"       ,p->qqstmage);
    ADDS32("QTime_Short"   ,p->qqstqtst);
    ADDS32("QTime_Long"    ,p->qqstqtlt);
    ADDTIME("Last_Put"     ,p->qqstlput);
    ADDTIME("Last_Get"     ,p->qqstlget);
    if ((flags & QQSTUNCM) == QQSTUNCM) {
      c = qqstUncm[1];
    } else {
      c = qqstUncm[0];
    }
    ADDSTR("Uncommitted",  c,fieldWidth(qqstUncm));
  }

#if CSQDSMF_VERSION > 932
  /* Lots more fields added in MQ 9.3.3  */
  if (conv16(p->qqstll)>offsetof(qqst,qqstdphi))
  {
    ADDU32("Queue_Depth_High_Water",p->qqstdphi);
    ADDU32("Queue_Depth_Low_Water", p->qqstdplo);
    ADDU64("Put",                   p->qqstputs);
    ADDU64("Put1",                  p->qqstput1);
    ADDU64("Put_NP",                p->qqstnppt);
    ADDU64("Put_P",                 p->qqstppt);
    ADDU64("Put1_NP",               p->qqstnpp1);
    ADDU64("Put1_P",                p->qqstpp1);
    ADDU64("Put_Bytes",             p->qqstputb);
    ADDU64("Put1_Bytes",            p->qqstpt1b);
    ADDU64("Put_Bytes_NP",          p->qqstnppb);
    ADDU64("Put_Bytes_P",           p->qqstppb);
    ADDU64("Put1_Bytes_NP",         p->qqstnp1b);
    ADDU64("Put1_Bytes_P",          p->qqstp1b);
    ADDU64("Msg_Length_Min",        p->qqstmsmi);
    ADDU64("Msg_Length_Max",        p->qqstmsma);
    ADDU64("Msg_Length_Ave",        p->qqstmsav);
    ADDU64("Get",                   p->qqstgets);
    ADDU64("Get_P",                 p->qqstnpdg);
    ADDU64("Get_NP",                p->qqstpdg);
    ADDU64("Get_Bytes",             p->qqstgetb);
    ADDU64("Get_Bytes_NP",          p->qqstnpdb);
    ADDU64("Get_Bytes_P",           p->qqstpdb);
    ADDU64("Browse",                p->qqstbrws);
    ADDU64("Browse_P",              p->qqstnpbr);
    ADDU64("Browse_NP",             p->qqstpbr);
    ADDU64("Browse_Bytes",          p->qqstbrwb);
    ADDU64("Browse_Bytes_NP",       p->qqstnpbb);
    ADDU64("Browse_Bytes_P",        p->qqstpbb);
    ADDU64("Get_Fail",              p->qqstflgt);
    ADDU64("Get_Fail_No_Msg",       p->qqstnmag);
    ADDU64("Get_Fail_Trunc",        p->qqsttmfg);
    ADDU64("Browse_Fail",           p->qqstflbr);
    ADDU64("Browse_Fail_No_Msg",    p->qqstnmab);
    ADDU64("Browse_Fail_Trunc",     p->qqsttmfb);
    ADDU64("Get_Wait_Fail",         p->qqstflgw);
    ADDU64("Get_Wait_Retry",        p->qqstrdgw);
    ADDU64("Browse_Wait_Fail",      p->qqstflbw);
    ADDU64("Browse_Wait_Retry",     p->qqstrdbw);
    ADDU32("Input_Count_Max",       p->qqstiphi);
    ADDU32("Input_Count_Min",       p->qqstiplo);
    ADDU32("Output_Count_Max",      p->qqstophi);
    ADDU32("Output_Count_Min",      p->qqstoplo);
    ADDU64("Open",                  p->qqstopen);
    ADDU64("Close",                 p->qqstclos);
    ADDU64("Inq",                   p->qqstinqr);
    ADDU64("Set",                   p->qqstset);
    ADDU64("Expired",               p->qqstexpr);
    ADDU64("Fast_Put_Messages",     p->qqstfptc);
    ADDU64("Fast_Put_Bytes",        p->qqstfptb);
    ADDU64("Rollback_Put",          p->qqstrbpt);
    ADDU64("Rollback_Get",          p->qqstrbgt);

    ADDU64("Put_Fail",              p->qqstflpt);
    ADDU64("Put1_Fail",             p->qqstflp1);
    ADDU64("Streamed",              p->qqststrm);
    ADDU64("Get_Signal_Accepted",   p->qqstsagt);
    ADDU64("Browse_Signal_Accepted",p->qqstsabr);

  }
#endif

  SMFPRINTSTOP;

  return;
}


