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

void printWTID(wtid *p)
{
  SMFPRINTSTART("WTID", p, conv16(p->wtidlen));

  ADDSTR  ("Appl_Type", strConnType(conv32(p->wtidatyp)),16);
  ADDSTREN("Connection_Name", p->wtidccn, 8);
  ADDSTREN("Operator_ID", p->wtidopid,8);
  ADDSTRB ("NID ", p->wtidnid, 16);
  ADDSTRB ("Correl(HEX)", p->wtidcori, 12);
  ADDSTREN("Correl(CHAR)", p->wtidcori, 12);
  ADDSTRB ("UOW_ID", p->wtiduowi, 24);
  ADDSTRB ("Accounting_Token", p->wtidacct, 22);
  ADDSTREN("Channel_Name", p->wtidchl, 20);
  ADDSTREN("Channel_Connection_Name", p->wtidchlc, 48);
  ADDSTRB ("Context_Token", p->wtidctxt, 16);
  ADDSTREN("MVS_UserId", p->wtidtran, 8);

  SMFPRINTSTOP;

  return;
}
