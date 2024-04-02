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


  ADDSTREN("Authorisation_ID",pqwhc->qwhcaid,8);
  ADDSTRB ("Correl_ID",pqwhc->qwhccv ,12);
  ADDSTREN("Connection_Name",pqwhc->qwhccn ,8 );
  ADDSTREN("Operator_ID",pqwhc->qwhcopid ,8 );
  ADDSTR  ("Application_Type",strConnType(conv32(pqwhc->qwhcatyp)),20);
  ADDSTRB ("Accounting_Token",pqwhc->qwhctokn ,22);
  ADDSTRB ("Network_ID",pqwhc->qwhcnid  ,16);

  ADDSTCK("CPU_Time", p->qmaccput);

  ADDU32("Puts_Len_0to99"      , p->qmacputl.qmacputa);
  ADDU32("Puts_Len_100to999"   , p->qmacputl.qmacputb);
  ADDU32("Puts_Len_1000to9999" , p->qmacputl.qmacputc);
  ADDU32("Puts_Len_10000"     , p->qmacputl.qmacputd);
  ADDU32("Gets_Len_0to99"      , p->qmacgetl.qmacgeta);
  ADDU32("Gets_Len_100to999"   , p->qmacgetl.qmacgetb);
  ADDU32("Gets_Len_1000to9999" , p->qmacgetl.qmacgetc);
  ADDU32("Gets_Len_10000"     , p->qmacgetl.qmacgetd);

  SMFPRINTSTOP;

  return;
}
