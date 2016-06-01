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



void printQCST(qcst *p)
{
  SMFPRINTSTART("QCST", p, conv16(p->qcstll));

  ADDTIME ("CLTM",p->qcstcltm);
  ADDSTREN("CHNM",p->qcstchnm,20);
  ADDSTR  ("CHDP",strMQCHLD((int)p->qcstchdp));
  ADDSTR  ("CHTY",strMQCHT((int)p->qcstchty));
  ADDSTR  ("CHST",strMQCHS((int)p->qcstchst));
  ADDSTR  ("STCL",strMQMON((int)p->qcststcl));
  ADDSTREN("CNNM",p->qcstcnnm,48);
  ADDTIME ("STRT",p->qcststrt);
  ADDTIME ("LUDT",p->qcstludt);
  ADDTIME ("LMST",p->qcstlmst);
  ADDU32  ("CBSZ",p->qcstcbsz);
  ADDU32  ("NMSG",p->qcstnmsg);
  ADDU32  ("NPMG",p->qcstnpmg);
  ADDU32  ("BATC",p->qcstbatc);
  ADDU32  ("FUBA",p->qcstfuba);
  ADDU32  ("BFST",p->qcstbfst);
  ADDU32  ("BFRC",p->qcstbfrc);
  ADDU32  ("CSCV",p->qcstcscv);
  ADDU64  ("NBYT",p->qcstnbyt);
  ADDU64  ("NPBY",p->qcstnpby);
  ADDU64  ("BYST",p->qcstbyst);
  ADDU64  ("BYRC",p->qcstbyrc);
  ADDU32  ("CPRA",p->qcstcpra);
  ADDU32  ("ETAV",p->qcstetav);
  ADDU32  ("ETMN",p->qcstetmn);
  ADDU32  ("ETMX",p->qcstetmx);
  ADDTIME ("ETDT",p->qcstetdt);
  ADDU32  ("DNRT",p->qcstdnrt);
  ADDU32  ("NTAV",p->qcstntav);
  ADDU32  ("NTMN",p->qcstntmn);
  ADDU32  ("NTMX",p->qcstntmx);
  ADDTIME ("NTDT",p->qcstntdt);
  ADDSTREN("RQMN",p->qcstrqmn,48);
  ADDSTR  ("SLSN",convBin(p->qcstslsn,8));
  ADDSTREN("SLCN",p->qcstslcn,16);
  ADDX32  ("SLCS",p->qcstslcs);
  ADDU32  ("PTRC",p->qcstptrc);
  ADDU32  ("QETC",p->qcstqetc);
  ADDU32  ("CBIT",p->qcstcbit);
  ADDU32  ("CBDL",p->qcstcbdl);
  ADDU32  ("DSPN",p->qcstdspn);

  SMFPRINTSTOP;
  return;
}
