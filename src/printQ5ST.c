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

void printQ5ST(q5st *p)
{
  SMFPRINTSTART("Q5ST", p, conv16(p->q5stll));

  ADDU32("Server_Tasks"       ,p->numtask );
  ADDU32("Active_Server_Tasks",p->acttask );
  ADDU32("Connect_Requests"   ,p->conncnt );
  ADDU32("Disconnect_Requests",p->disccnt );
  ADDU32("Max_Request_Queue_Depth", p->dhigmax );
  ADDU32("DB2_Task_Abends"    ,p->abndcnt );
  ADDU32("Request_Requeues"   ,p->requcnt );
  ADDU32("Deadlock_Timeouts"  ,p->deadcnt );
  ADDU32("Delete_Requests"    ,p->delecnt );
  ADDU32("List_Requests"      ,p->listcnt );
  ADDU32("Read_Requests"      ,p->readcnt );
  ADDU32("Update_Requests"    ,p->updtcnt );
  ADDU32("Write_Requests"     ,p->writcnt );
  ADDU32("DB2_Blob_Selects",p->lmssel  );
  ADDU32("DB2_Blob_Inserts",p->lmsins  );
  ADDU32("DB2_Blob_Updates",p->lmsupd  );
  ADDU32("DB2_Blob_Deletes",p->lmsdel  );
  ADDU32("DB2_Blob_Lists"  ,p->lmslis  );
  ADDU32("SCST_Selects"    ,p->scssel  );
  ADDU32("SCST_Inserts"    ,p->scsins  );
  ADDU32("SCST_Updates"    ,p->scsupd  );
  ADDU32("SCST_Deletes"    ,p->scsdel  );
  ADDU32("SCST_Buff_Too_Small",p->scsbfts );
  ADDU32("SCST_Max_Rows"   ,p->scsmaxr );
  ADDU32("SSKT_Selects"    ,p->ssksel  );
  ADDU32("SSKT_Inserts"    ,p->sskins  );
  ADDU32("SSKT_Deletes"    ,p->sskdel  );

  ADDSTCK("Thread_Delete_Elapsed",p->deletcuw);
  ADDSTCK("Thread_Delete_Max"    ,p->deletmxw);
  ADDSTCK("SQL_Delete_Elapsed"   ,p->delescuw);
  ADDSTCK("SQL_Delete_Max"       ,p->delesmxw);
  ADDSTCK("Thread_List_Elapsed"  ,p->listtcuw);
  ADDSTCK("Thread_List_Max"      ,p->listtmxw);
  ADDSTCK("SQL_List_Elapsed"     ,p->listscuw);
  ADDSTCK("SQL_List_Max"         ,p->listsmxw);
  ADDSTCK("Thread_Read_Elapsed"  ,p->readtcuw);
  ADDSTCK("Thread_Read_Max"      ,p->readtmxw);
  ADDSTCK("SQL_Read_Elapsed"     ,p->readscuw);
  ADDSTCK("SQL_Read_Max"         ,p->readsmxw);
  ADDSTCK("Thread_Update_Elapsed",p->updttcuw);
  ADDSTCK("Thread_Update_Max"    ,p->updttmxw);
  ADDSTCK("SQL_Update_Elapsed"   ,p->updtscuw);
  ADDSTCK("SQL_Update_Max"       ,p->updtsmxw);
  ADDSTCK("Thread_Write_Elapsed" ,p->writtcuw);
  ADDSTCK("Thread_Write_Max"     ,p->writtmxw);
  ADDSTCK("SQL_Write_Elapsed"    ,p->writscuw);
  ADDSTCK("SQL_Write_Max"        ,p->writsmxw);
  ADDSTCK("SCST_Thread_Select_Elapsed",p->scsstcuw);
  ADDSTCK("SCST_Thread_Select_Max"    ,p->scsstmxw);
  ADDSTCK("SCST_SQL_Select_Elapsed"   ,p->scssscuw);
  ADDSTCK("SCST_SQL_Select_Max"       ,p->scsssmxw);
  ADDSTCK("SCST_Thread_Insert_Elapsed",p->scsitcuw);
  ADDSTCK("SCST_Thread_Insert_Max"    ,p->scsitmxw);
  ADDSTCK("SCST_SQL_Insert_Elapsed"   ,p->scsiscuw);
  ADDSTCK("SCST_SQL_Insert_Max"       ,p->scsismxw);
  ADDSTCK("SCST_Thread_Update_Elapsed",p->scsutcuw);
  ADDSTCK("SCST_Thread_Update_Max"    ,p->scsutmxw);
  ADDSTCK("SCST_SQL_Update_Elapsed"   ,p->scsuscuw);
  ADDSTCK("SCST_SQL_Update_Max"       ,p->scsusmxw);
  ADDSTCK("SCST_Thread_Delete_Elapsed",p->scsdtcuw);
  ADDSTCK("SCST_Thread_Delete_Max"    ,p->scsdtmxw);
  ADDSTCK("SCST_SQL_Delete_Elapsed"   ,p->scsdscuw);
  ADDSTCK("SCST_SQL_Delete_Max"       ,p->scsdsmxw);
  ADDSTCK("SSKT_Thread_Select_Elapsed",p->sskstcuw);
  ADDSTCK("SSKT_Thread_Select_Max"    ,p->sskstmxw);
  ADDSTCK("SSKT_SQL_Select_Elapsed"   ,p->ssksscuw);
  ADDSTCK("SSKT_SQL_Select_Max"       ,p->sskssmxw);
  ADDSTCK("SSKT_Thread_Insert_Elapsed",p->sskitcuw);
  ADDSTCK("SSKT_Thread_Insert_Max"    ,p->sskitmxw);
  ADDSTCK("SSKT_SQL_Insert_Elapsed"   ,p->sskiscuw);
  ADDSTCK("SSKT_SQL_Insert_Max"       ,p->sskismxw);
  ADDSTCK("SSKT_Thread_Update_Elapsed",p->sskdtcuw);
  ADDSTCK("SSKT_Thread_Update_Max"    ,p->sskdtmxw);
  ADDSTCK("SSKT_SQL_Update_Elapsed"   ,p->sskdscuw);
  ADDSTCK("SSKT_SQL_Update_Max"       ,p->sskdsmxw);
  ADDSTCK("BLOB_Thread_Select_Elapsed",p->lmsstcuw);
  ADDSTCK("BLOB_Thread_Select_Max"    ,p->lmsstmxw);
  ADDSTCK("BLOB_SQL_Select_Elapsed"   ,p->lmssscuw);
  ADDSTCK("BLOB_SQL_Select_Max"       ,p->lmsssmxw);
  ADDSTCK("BLOB_Thread_Insert_Elapsed",p->lmsitcuw);
  ADDSTCK("BLOB_Thread_Insert_Max"    ,p->lmsitmxw);
  ADDSTCK("BLOB_SQL_Insert_Elapsed"   ,p->lmsiscuw);
  ADDSTCK("BLOB_SQL_Insert_Max"       ,p->lmsismxw);
  ADDSTCK("BLOB_Thread_Update_Elapsed",p->lmsutcuw);
  ADDSTCK("BLOB_Thread_Update_Max"    ,p->lmsutmxw);
  ADDSTCK("BLOB_SQL_Update_Elapsed"   ,p->lmsuscuw);
  ADDSTCK("BLOB_SQL_Update_Max"       ,p->lmsusmxw);
  ADDSTCK("BLOB_Thread_Delete_Elapsed",p->lmsdtcuw);
  ADDSTCK("BLOB_Thread_Delete_Max"    ,p->lmsdtmxw);
  ADDSTCK("BLOB_SQL_Delete_Elapsed"   ,p->lmsdscuw);
  ADDSTCK("BLOB_SQL_Delete_Max"       ,p->lmsdsmxw);
  ADDSTCK("BLOB_Thread_List_Elapsed"  ,p->lmsltcuw);
  ADDSTCK("BLOB_Thread_List_Max"      ,p->lmsltmxw);
  ADDSTCK("BLOB_SQL_List_Elapsed"     ,p->lmslscuw);
  ADDSTCK("BLOB_SQL_List_Max"         ,p->lmslsmxw);

  SMFPRINTSTOP;

  return;
}
