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

  ADDU32 ("Pageset"            ,p->qis1psno);
  ADDU32 ("BufferPool"         ,p->qis1bpno);
  ADDBYTE("Pageset_Status"     ,p->qis1stat);
  ADDU32 ("Total_Pages"        ,p->qis1topg);
  ADDU32 ("Unused_Pages"       ,p->qis1unpg);
  ADDU32 ("Persistent_Pages "  ,p->qis1ppg);
  ADDU32 ("Nonpersistent_Pages",p->qis1nppg);
  ADDU32 ("Full_Count"         ,p->qis1full);
  ADDU32 ("Put_Cursor_High"    ,p->qis1pchi);
  ADDU32 ("Page_Write_IO"      ,p->qis1dwpn);
  ADDU32 ("Pages_Written"      ,p->qis1dwpp);
  ADDSTCK("Page_Write_IO_Time" ,p->qis1dwpt);
  ADDU32 ("IMW_Pages_IO"       ,p->qis1imwn);
  ADDU32 ("IMW_Pages"          ,p->qis1imwp);
  ADDSTCK("IMW_Pages_IO_Time"  ,p->qis1imwt);
  ADDU32 ("Pages_Read_IO"      ,p->qis1getn);
  ADDU32 ("Pages_Read"         ,p->qis1getp);
  ADDSTCK("Pages_Read_IO_Time" ,p->qis1gett);
  ADDU32 ("Checkpoint_Pages_Written",p->qis1ckpp );
  ADDBYTE("Stripes"            ,p->qis1strn);
  ADDS16 ("Expansions"         ,p->qis1expn);
  ADDBYTE("Expansion_Method"   ,p->qis1expm);

  /*****************************************************************/
  /* Extract the word containing bitfield flags. Make sure it's in */
  /* the right order, then test the flags.                         */
  /*****************************************************************/
  u = (unsigned int *)&p->qis1stat;
  flags = conv32(*(u+1));

  if(flags & QIS1EXPF)
  {
    ADDSTR("Expand","Yes",3);
  }
  else
  {
    ADDSTR("Expand","No",3);
  }

  SMFPRINTSTOP;

  return;
}
#endif
