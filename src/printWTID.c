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

  ADDSTR  ("ATYP", strConnType(conv32(p->wtidatyp)),16);
  ADDSTREN("CCN ", p->wtidccn, 8);
  ADDSTREN("OPID", p->wtidopid,8);
  ADDSTRB ("NID ", p->wtidnid, 16);
  ADDSTRB ("CORI(HEX)", p->wtidcori, 12);
  ADDSTREN("CORI(CHAR)", p->wtidcori, 12);
  ADDSTRB ("UOWI", p->wtiduowi, 24);
  ADDSTRB ("ACCT", p->wtidacct, 22);
  ADDSTREN("CHL ", p->wtidchl, 20);
  ADDSTREN("CHLC", p->wtidchlc, 48);
  ADDSTRB ("CTXT", p->wtidctxt, 16);
  ADDSTREN("TRAN", p->wtidtran, 8);

  SMFPRINTSTOP;

  return;
}
