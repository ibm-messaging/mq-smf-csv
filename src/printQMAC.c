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


/*
 * This function is slightly different than most of the
 * printers because it dumps the contents of TWO separate
 * structures. Identification of the task is in the
 * QWHC structure, while the actual numbers follow in the QMAC
 * structure. Given the relationship between these, they
 * need to be printed at the same time. So both are in here.
 */
void printQMAC(qmac *p, qwhc *pqwhc)
{

  SMFPRINTSTART("QMAC",p,conv16(p->qmacll));
  if (debugLevel > 0)
    printDEBUG("QWHC",pqwhc,conv16(pqwhc->qwhclen));


  ADDSTREN("AID ",pqwhc->qwhcaid,8);
  ADDSTR  ("CCV ",convBin(pqwhc->qwhccv ,12));
  ADDSTREN("CCN ",pqwhc->qwhccn ,8 );
  ADDSTREN("OPID",pqwhc->qwhcopid ,8 );
  ADDSTR  ("ATYP",strConnType(conv32(pqwhc->qwhcatyp)));
  ADDSTR  ("TOKN",convBin(pqwhc->qwhctokn ,22));
  ADDSTR  ("NID ",convBin(pqwhc->qwhcnid  ,16));

  ADDSTCK("CPUT", p->qmaccput);

  ADDU32("PUTA" , p->qmacputl.qmacputa);
  ADDU32("PUTB" , p->qmacputl.qmacputb);
  ADDU32("PUTC" , p->qmacputl.qmacputc);
  ADDU32("PUTD" , p->qmacputl.qmacputd);
  ADDU32("GETA" , p->qmacgetl.qmacgeta);
  ADDU32("GETB" , p->qmacgetl.qmacgetb);
  ADDU32("GETC" , p->qmacgetl.qmacgetc);
  ADDU32("GETD" , p->qmacgetl.qmacgetd);

  SMFPRINTSTOP;

  return;
}
