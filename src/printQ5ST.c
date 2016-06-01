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

  ADDU32("NUMTASK",p->numtask );
  ADDU32("ACTTASK",p->acttask );
  ADDU32("CONNCNT",p->conncnt );
  ADDU32("DISCCNT",p->disccnt );
  ADDU32("DHIGMAX",p->dhigmax );
  ADDU32("ABNDCNT",p->abndcnt );
  ADDU32("REQUCNT",p->requcnt );
  ADDU32("DEADCNT",p->deadcnt );
  ADDU32("DELECNT",p->delecnt );
  ADDU32("LISTCNT",p->listcnt );
  ADDU32("READCNT",p->readcnt );
  ADDU32("UPDTCNT",p->updtcnt );
  ADDU32("WRITCNT",p->writcnt );
  ADDU32("LMSSEL ",p->lmssel  );
  ADDU32("LMSINS ",p->lmsins  );
  ADDU32("LMSUPD ",p->lmsupd  );
  ADDU32("LMSDEL ",p->lmsdel  );
  ADDU32("LMSLIS ",p->lmslis  );
  ADDU32("SCSSEL ",p->scssel  );
  ADDU32("SCSINS ",p->scsins  );
  ADDU32("SCSUPD ",p->scsupd  );
  ADDU32("SCSDEL ",p->scsdel  );
  ADDU32("SSKSEL ",p->ssksel  );
  ADDU32("SSKINS ",p->sskins  );
  ADDU32("SSKDEL ",p->sskdel  );
  ADDU32("SCSBFTS",p->scsbfts );
  ADDU32("SCSMAXR",p->scsmaxr );

  ADDSTCK("DELETCUW",p->deletcuw);
  ADDSTCK("DELETMXW",p->deletmxw);
  ADDSTCK("DELESCUW",p->delescuw);
  ADDSTCK("DELESMXW",p->delesmxw);
  ADDSTCK("LISTTCUW",p->listtcuw);
  ADDSTCK("LISTTMXW",p->listtmxw);
  ADDSTCK("LISTSCUW",p->listscuw);
  ADDSTCK("LISTSMXW",p->listsmxw);
  ADDSTCK("READTCUW",p->readtcuw);
  ADDSTCK("READTMXW",p->readtmxw);
  ADDSTCK("READSCUW",p->readscuw);
  ADDSTCK("READSMXW",p->readsmxw);
  ADDSTCK("UPDTTCUW",p->updttcuw);
  ADDSTCK("UPDTTMXW",p->updttmxw);
  ADDSTCK("UPDTSCUW",p->updtscuw);
  ADDSTCK("UPDTSMXW",p->updtsmxw);
  ADDSTCK("WRITTCUW",p->writtcuw);
  ADDSTCK("WRITTMXW",p->writtmxw);
  ADDSTCK("WRITSCUW",p->writscuw);
  ADDSTCK("WRITSMXW",p->writsmxw);
  ADDSTCK("SCSSTCUW",p->scsstcuw);
  ADDSTCK("SCSSTMXW",p->scsstmxw);
  ADDSTCK("SCSSSCUW",p->scssscuw);
  ADDSTCK("SCSSSMXW",p->scsssmxw);
  ADDSTCK("SCSITCUW",p->scsitcuw);
  ADDSTCK("SCSITMXW",p->scsitmxw);
  ADDSTCK("SCSISCUW",p->scsiscuw);
  ADDSTCK("SCSISMXW",p->scsismxw);
  ADDSTCK("SCSUTCUW",p->scsutcuw);
  ADDSTCK("SCSUTMXW",p->scsutmxw);
  ADDSTCK("SCSUSCUW",p->scsuscuw);
  ADDSTCK("SCSUSMXW",p->scsusmxw);
  ADDSTCK("SCSDTCUW",p->scsdtcuw);
  ADDSTCK("SCSDTMXW",p->scsdtmxw);
  ADDSTCK("SCSDSCUW",p->scsdscuw);
  ADDSTCK("SCSDSMXW",p->scsdsmxw);
  ADDSTCK("SSKSTCUW",p->sskstcuw);
  ADDSTCK("SSKSTMXW",p->sskstmxw);
  ADDSTCK("SSKSSCUW",p->ssksscuw);
  ADDSTCK("SSKSSMXW",p->sskssmxw);
  ADDSTCK("SSKITCUW",p->sskitcuw);
  ADDSTCK("SSKITMXW",p->sskitmxw);
  ADDSTCK("SSKISCUW",p->sskiscuw);
  ADDSTCK("SSKISMXW",p->sskismxw);
  ADDSTCK("SSKDTCUW",p->sskdtcuw);
  ADDSTCK("SSKDTMXW",p->sskdtmxw);
  ADDSTCK("SSKDSCUW",p->sskdscuw);
  ADDSTCK("SSKDSMXW",p->sskdsmxw);
  ADDSTCK("LMSSTCUW",p->lmsstcuw);
  ADDSTCK("LMSSTMXW",p->lmsstmxw);
  ADDSTCK("LMSSSCUW",p->lmssscuw);
  ADDSTCK("LMSSSMXW",p->lmsssmxw);
  ADDSTCK("LMSITCUW",p->lmsitcuw);
  ADDSTCK("LMSITMXW",p->lmsitmxw);
  ADDSTCK("LMSISCUW",p->lmsiscuw);
  ADDSTCK("LMSISMXW",p->lmsismxw);
  ADDSTCK("LMSUTCUW",p->lmsutcuw);
  ADDSTCK("LMSUTMXW",p->lmsutmxw);
  ADDSTCK("LMSUSCUW",p->lmsuscuw);
  ADDSTCK("LMSUSMXW",p->lmsusmxw);
  ADDSTCK("LMSDTCUW",p->lmsdtcuw);
  ADDSTCK("LMSDTMXW",p->lmsdtmxw);
  ADDSTCK("LMSDSCUW",p->lmsdscuw);
  ADDSTCK("LMSDSMXW",p->lmsdsmxw);
  ADDSTCK("LMSLTCUW",p->lmsltcuw);
  ADDSTCK("LMSLTMXW",p->lmsltmxw);
  ADDSTCK("LMSLSCUW",p->lmslscuw);
  ADDSTCK("LMSLSMXW",p->lmslsmxw);

  SMFPRINTSTOP;

  return;
}
