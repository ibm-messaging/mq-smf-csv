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

void printQTST(qtst *p)
{

  SMFPRINTSTART("QTST", p, conv16(p->qtstll));

  ADDU32("Total_Subs_Count"  , p->qtstzero.qtststot);
  ADDU32("Durable_Subs_Count", p->qtstzero.qtstsdur);

  ADDU32("Subs_High_Water_API"  , p->qtstzero.qtstshig[0]);
  ADDU32("Subs_High_Water_Admin", p->qtstzero.qtstshig[1]);
  ADDU32("Subs_High_Water_Proxy", p->qtstzero.qtstshig[2]);
  ADDU32("Subs_Low_Water_API"   ,  p->qtstzero.qtstslow[0]);
  ADDU32("Subs_Low_Water_Admin" ,  p->qtstzero.qtstslow[1]);
  ADDU32("Subs_Low_Water_Proxy" ,  p->qtstzero.qtstslow[2]);

  ADDU32("Subs_Expired"         , p->qtstzero.qtstsexp);
  ADDU32("Total_Published_Msg"  , p->qtstzero.qtsttmsg);
  ADDU32("Single_Sub_High_Water", p->qtstzero.qtstsphw);

  ADDU32("Pubs_Total_API"  , p->qtstzero.qtstptot[0]);
  ADDU32("Pubs_Total_Admin", p->qtstzero.qtstptot[1]);
  ADDU32("Pubs_Total_Proxy", p->qtstzero.qtstptot[2]);

  ADDU32("Pubs_High_Water"   , p->qtstzero.qtstphig);
  ADDU32("Pubs_Low_Water"    , p->qtstzero.qtstplow);
  ADDU32("Pubs_No_Subscriber", p->qtstzero.qtstpnos);
  ADDSTCK("Publish_Elapsed_High_Water", p->qtstzero.qtstethw);
  ADDSTCK("Publish_Elapsed_Total", p->qtstzero.qtstetto);

  SMFPRINTSTOP;

  return;
}
