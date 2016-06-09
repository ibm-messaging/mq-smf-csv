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
#define QIS1EXPF (0x80000000)

SMFPRINTGLOB;

/*******************************************************************/
/* Only include this function if building from an MQ V9 header file*/
/*******************************************************************/
#ifdef QIS1IDV
void printQIS1(qis1 *p)
{
  unsigned int  flags;
  unsigned int  *u;

  SMFPRINTSTART("QIS1", p, sizeof(qis1));

  ADDU32 ("PSNO",p->qis1psno);
  ADDU32 ("BPNO",p->qis1bpno);
  ADDBYTE("STAT",p->qis1stat);
  ADDU32 ("TOPG",p->qis1topg);
  ADDU32 ("UNPG",p->qis1unpg);
  ADDU32 ("PPG ",p->qis1ppg);
  ADDU32 ("NPPG",p->qis1nppg);
  ADDU32 ("FULL",p->qis1full);
  ADDU32 ("PCHI",p->qis1pchi);
  ADDU32 ("DWPN",p->qis1dwpn);
  ADDU32 ("DWPP",p->qis1dwpp);
  ADDSTCK("DWPT",p->qis1dwpt);
  ADDU32 ("IMWN",p->qis1imwn);
  ADDU32 ("IMWP",p->qis1imwp);
  ADDSTCK("IMWT",p->qis1imwt);
  ADDU32 ("GETN",p->qis1getn);
  ADDU32 ("GETP",p->qis1getp);
  ADDSTCK("GETT",p->qis1gett);
  ADDU32 ("CKPP",p->qis1ckpp );
  ADDBYTE("STRN",p->qis1strn);
  ADDBYTE("EXPM",p->qis1expm);
  ADDS16 ("EXPN",p->qis1expn);

  /*****************************************************************/
  /* Extract the word containing bitfield flags. Make sure it's in */
  /* the right order, then test the flags.                         */
  /*****************************************************************/
  u = (unsigned int *)&p->qis1stat;
  flags = conv32(*(u+1));

  if(flags & QIS1EXPF)
  {
    ADDSTR("EXPAND","Yes",3);
  }
  else
  {
    ADDSTR("EXPAND","No",3);
  }

  SMFPRINTSTOP;

  return;
}
#endif
