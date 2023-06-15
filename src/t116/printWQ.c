/*
 * Copyright (c) 2016, 2023 IBM Corporation and other Contributors.
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
  "Open",
  "Close",
  "Get",
  "Put",
  "Put1"
};

SMFPRINTGLOB;


/**********************************************************/
/* Print out the contents of the WQ block                 */
/**********************************************************/
void printWQ(wq *p)
{
  char index[64];
  char indexet[64];
  int i,j;

  SMFPRINTSTART("WQ", p, conv16(p->wqll));

  ADDSTRB ("Correlation"    ,p->correl, 16);
  ADDINDEX("Correlation");
  ADDSTREN("Open_Name",p->objname,48);
  ADDSTREN("Base_Name",p->basename,48);
  ADDINDEX("Base_Name");
  ADDTIME ("Open"     ,p->opentime);
  ADDTIME ("Close"    ,p->clostime);

  /*--------------------------------------------------------*/
  /* Object information                                     */
  /*--------------------------------------------------------*/
  ADDSTR("Queue" ,strMQQT(conv32(p->qtype)),16);
  ADDSTR("Index",strMQIT(conv32(p->indxtype)),16);
  ADDSTR("Scope",strMQQSGD(conv32(p->qsgdisp)),16);

  /*--------------------------------------------------------*/
  /* Open   - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Open_ET", p->openet);
  ADDSTCK("Open_CT", p->openct);
  ADDU32 ("Open_Count", p->openn);
  /*--------------------------------------------------------*/
  /* Close  - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Close_ET", p->closeet);
  ADDSTCK("Close_CT", p->closect);
  ADDU32 ("Close_Count" ,p->closen);

  /*--------------------------------------------------------*/
  /* Get    - Information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Get_ET", p->getet);
  ADDSTCK("Get_CT", p->getct);
  ADDU32 ("Get_Count",p->getn);
  ADDU32 ("Get_Browse_Any_Count",p->getbrwa);
  ADDU32 ("Get_Browse_Specific_Count",p->getbrws);
  ADDU32 ("Get_Dest_Any_Count",p->geta);
  ADDU32 ("Get_Dest_Specific_Count",p->gets);
  ADDU32 ("Get_Invalid",p->geterr);
  ADDSTCK("Get_Log_UNF_ET",p->getjwet);
  ADDU32 ("Get_Log_UNF_Count",p->getjwn);
  ADDSTCK("Get_Pageset_ET",p->getpset);
  ADDU32 ("Get_Pageset_Count",p->getpsn);
  ADDSTCK("Get_Suspend_ET",p->getsuset);
  ADDU32 ("Get_Suspend_Count",p->getsusn);
  ADDU32 ("Get_Pages_Skipped_Count",p->getepage);
  ADDU32 ("Get_Messages_Skipped_Count",p->getsmsg);
  ADDU32 ("Get_Messages_Expired_Count",p->getexmsg);

  /*--------------------------------------------------------*/
  /* Put    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Put_ET"            ,p->putet);
  ADDSTCK("Put_CT"            ,p->putct);
  ADDU32 ("Put_Count"         ,p->putn);
  ADDU32 ("Put_IGQ_Count"     ,p->putigq);
  ADDSTCK("Put_Log_UNF_ET"    ,p->putjwet);
  ADDU32 ("Put_Log_UNF_Count" ,p->putjwn);
  ADDSTCK("Put_Suspend_ET"    ,p->putsuset);
  ADDU32 ("Put_Suspend_Count" ,p->putsusn);
  ADDSTCK("Put_Pageset_Access_ET"    ,p->putpset);
  ADDU32 ("Put_Pageset_Access_Count" ,p->putpsn);

  /*--------------------------------------------------------*/
  /* Put1   - information                                   */
  /*-------------------------------------------------------*/
  ADDSTCK("Put1_ET"            ,p->put1et);
  ADDSTCK("Put1_CT"            ,p->put1ct);
  ADDU32 ("Put1_Count"         ,p->put1n);
  ADDU32 ("Put1_IGQ_Count"     ,p->put1igq);
  ADDSTCK("Put1_Log_UNF_ET"    ,p->put1jwet);
  ADDU32 ("Put1_Log_UNF_Count" ,p->put1jwn);
  ADDSTCK("Put1_Suspend_ET"    ,p->put1suset);
  ADDU32 ("Put1_Suspend_Count" ,p->put1susn);
  ADDSTCK("Put1_Pageset_Access_ET"    ,p->put1pset);
  ADDU32 ("Put1_Pageset_Access_Count" ,p->put1psn);

  /*--------------------------------------------------------*/
  /* Inq    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Inq_ET"   ,p->inqet);
  ADDSTCK("Inq_CT"   ,p->inqct);
  ADDU32 ("Inq_Count",p->inqn);

  /*--------------------------------------------------------*/
  /* set    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("Set_ET"        ,p->setet);
  ADDSTCK("Set_CT"        ,p->setct);
  ADDU32 ("Set_Count"     ,p->setn);
  ADDSTCK("Set_UNF_Log_ET",p->setjwet);
  ADDU32 ("Set_UNF_Log_Count",p->setjwn);

  /*--------------------------------------------------------*/
  /* other  - information                                   */
  /*--------------------------------------------------------*/
  ADDU32  ("Pageset_ID",p->nps);
  ADDSTREN("CF_Structure",p->cfstrucname,12);
  ADDU32  ("BufferPool_ID",p->nbuffpool);

  /*--------------------------------------------------------*/
  /* Byte counts and persistent data information            */
  /* The MINMS values for put/get are initially set to      */
  /* 0x0FFFFFFF, and appear that way in the SMF report if   */
  /* no puts/gets have touched that queue.  But that looks  */
  /* ugly, so I'm turning them to -1.                       */
  /*--------------------------------------------------------*/
  if (conv32(p->getminms) == 0x0FFFFFFF)
    p->getminms = conv32(-1);
  if (conv32(p->putminms) == 0x0FFFFFFF)
    p->putminms = conv32(-1);

  ADDS64 ("Total_Bytes_Put",p->putbytes);
  ADDS64 ("Total_Bytes_Get",p->getbytes);
  ADDS32 ("Total_Valid_Puts",p->validput);
  ADDS32 ("Total_Valid_Gets",p->validget);
  ADDS32 ("Generated_Msgs",p->ngen);

  ADDS32 ("Get_Max_Msg_Size",p->getmaxms);
  ADDS32 ("Get_Min_Msg_Size",p->getminms);
  ADDS32 ("Put_Max_Msg_Size",p->putmaxms);
  ADDS32 ("Put_Min_Msg_Size",p->putminms);

  ADDSTCK("Max_Time_On_Queue"  ,p->maxlatnt);
  ADDSTCK("Min_Time_On_Queue"  ,p->minlatnt);
  ADDSTCK("Total_Time_On_Queue",p->totlatnt);
  ADDS32("Use_Count",p->use_count);
  ADDS32("Total_Use",p->total_use);

  /*********************************************************/
  /* This is one structure that has been extended with     */
  /* every version of MQ. So we test the version and only  */
  /* add columns where they might apply. Note that the     */
  /* structure version is not the same number as the MQ    */
  /* product version.                                      */
  /*********************************************************/

  if (conv32(p->wqver) > 1)
  {
    ADDS32("Get_Persistent_Count" ,p->getpmsg);
    ADDS32("Put_Persistent_Count" ,p->putpmsg);
    ADDS32("Put1_Persistent_Count",p->put1pmsg);
    ADDS32("Max_Depth",p->maxqdpth);
  }

  if (conv32(p->wqver) > 3)
  {
    ADDU32 ("Get_Valid",p->getdval);
    ADDSTCK("Get_LogForce_ET",p->getjcet);
    ADDU32 ("Get_LogForce_Count" ,p->getjcn);
    ADDU32 ("Put_Waiting_Getter_Count",p->putpwg);
    ADDSTCK("Put_LogForce_ET",p->putjcet);
    ADDU32 ("Put_LogForce_Count ",p->putjcn);
    ADDU32 ("Put1_Waiting_Getter_Count",p->put1pwg);
    ADDSTCK("Put1_LogForce_ET",p->put1jcet);
    ADDU32 ("Put1_LogForce_Count",p->put1jcn);
    ADDSTCK("Set_LogForce_ET", p->setjcet);
    ADDU32 ("Set_LogForce_Count",p->setjcn);
    ADDU32 ("Put_Shared_Queue",p->putdsq);
  }

  if (conv32(p->wqver) > 4)
  {
    ADDU32("Select_Count",p->selcount);
    ADDU32("Select_Max_Length",p->selmaxln);
    /*----------------------------------------------------*/
    /* CB     - Information                               */
    /*----------------------------------------------------*/
    ADDSTCK("CallBack_ET",p->cbet);
    ADDSTCK("CallBack_CT",p->cbct);
    ADDU32 ("CallBack_Count",p->cbn);
  }

  if (conv32(p->wqver) > 5)
  {
    ADDSTCK("Open_Suspend_Time" ,p->opensuet);
    ADDU32 ("Open_Suspend_Count" ,p->opensun);
    ADDSTCK("Close_Suspend_Time",p->closesuet);
    ADDU32 ("Close_Suspend_Count",p->closesun);
    ADDU32 ("Open_NoCF",p->opencf0);
    ADDU32 ("Close_NoCF",p->closecf0);

    for (i=0;i<5;i++)
    {
      for (j=0;j<14;j++)/* Last few of the array are unused, so don't print them*/
      {
        if (first) {
          /* Easier to understand if "ET" is at the end of the headings. */
          /* So there are two types of heading needed in this loop.      */
          sprintf(index,  "%s_%s"   ,verb[i],strCfStatType(j));
          sprintf(indexet,"%s_%s_ET",verb[i],strCfStatType(j));
        }
        ADDU32IDX("CFCount"   , index,  p->mqcfreq[i].type[j].cfcount);
        ADDU32IDX("CFSync"    , index,  p->mqcfreq[i].type[j].cfsyncn);
        ADDU32IDX("CFSync"    , indexet,p->mqcfreq[i].type[j].cfsyncet);
        ADDU32IDX("CFAsync"   , index,  p->mqcfreq[i].type[j].cfasyncn);
        ADDU32IDX("CFAsync"   , indexet,p->mqcfreq[i].type[j].cfasyncet);
      }
    }

    ADDU32 ("Published_Count" , p->publishedn);
    ADDSTCK("TopicOnSRB_Open" , p->topicopensrb);
    ADDSTCK("TopicOnSRB_Put"  , p->topicputsrb);
    ADDSTCK("TopicOnSRB_Put1" , p->topicput1srb);
    ADDSTCK("TopicOnSRB_Close", p->topicclosesrb);
  }

  if (conv32(p->wqver) >= 8)
  {
     ADDU32("Put_Delay_Count",p->putddly);
  }

  if (conv32(p->wqver) >= 9)
  {
     ADDU32("Streamed_Msgs",p->streamedn);
  }

  SMFPRINTSTOP;

  return;
}

