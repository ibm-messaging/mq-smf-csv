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



void printQCST(qcst *p)
{
  SMFPRINTSTART("QCST", p, conv16(p->qcstll));

  ADDTIME ("Collection_Time" ,p->qcstcltm);
  ADDSTREN("Chl_Name"        ,p->qcstchnm,20);
  ADDSTR  ("Disposition"     ,strMQCHLD((int)p->qcstchdp),20);
  ADDSTR  ("Type"            ,strMQCHT((int)p->qcstchty),20);
  ADDSTR  ("Status"          ,strMQCHS((int)p->qcstchst),20);
  ADDSTR  ("Stat_Setting"    ,strMQMON((int)p->qcststcl),20);
  ADDSTRENM("Connection_Name" ,p->qcstcnnm,48,48);
  ADDTIME ("Start"           ,p->qcststrt);
  ADDTIME ("Stop"            ,p->qcstludt);
  ADDTIME ("Last_Msg"        ,p->qcstlmst);
  ADDU32  ("BatchSize"       ,p->qcstcbsz);
  ADDU32  ("Msg_All_Count"   ,p->qcstnmsg);
  ADDU32  ("Msg_Persistent_Count",p->qcstnpmg);
  ADDU32  ("Batches"         ,p->qcstbatc);
  ADDU32  ("Full_Batches"    ,p->qcstfuba);
  ADDU32  ("Buffers_Sent"    ,p->qcstbfst);
  ADDU32  ("Buffers_Rcvd"    ,p->qcstbfrc);
  ADDU32  ("Current_Shared_Convs",p->qcstcscv);
  ADDU64  ("Number_Bytes"    ,p->qcstnbyt);
  ADDU64  ("Number_Persistent_Bytes",p->qcstnpby);
  ADDU64  ("Bytes_Sent"      ,p->qcstbyst);
  ADDU64  ("Bytes_Received"  ,p->qcstbyrc);
  ADDU32  ("Compression_Rate",p->qcstcpra);
  ADDU32  ("Exit_Time_Avg"   ,p->qcstetav);
  if (conv32(p->qcstetmn) == 0x8FFFFFFF)
     p->qcstetmn = conv32(0);
  ADDU32  ("Exit_Time_Min"   ,p->qcstetmn);
  ADDU32  ("Exit_Time_Max"   ,p->qcstetmx);
  ADDTIME ("Exit_Time_Max_Date",p->qcstetdt);
  ADDU32  ("DNS_Resolution_Time",p->qcstdnrt);
  ADDU32  ("Net_Time_Avg"    ,p->qcstntav);
  if (conv32(p->qcstntmn) == 0x8FFFFFFF)
     p->qcstntmn = conv32(0);
  ADDU32  ("Net_Time_Min"    ,p->qcstntmn);
  ADDU32  ("Net_Time_Max"    ,p->qcstntmx);
  ADDTIME ("Net_Time_Max_Date",p->qcstntdt);
  ADDSTREN("Remote_QMgr_Or_App"     ,p->qcstrqmn,48);
  ADDSTRB ("SSL_Peer_Serial",p->qcstslsn,8);
  ADDSTREN("SSL_CERTI"       ,p->qcstslcn,16);
  ADDX32  ("SSL_CipherSuite" ,p->qcstslcs);
  ADDU32  ("Put_Retry_Count" ,p->qcstptrc);
  ADDU32  ("XmitQ_Empty_Count",p->qcstqetc);
  ADDU32  ("Batch_Interval"  ,p->qcstcbit);
  ADDU32  ("Batch_Data_Limit",p->qcstcbdl);
  ADDU16  ("Dispatcher_Number",p->qcstdspn);

  SMFPRINTSTOP;
  return;
}
