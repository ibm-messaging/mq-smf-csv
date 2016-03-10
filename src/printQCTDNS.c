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

void printQCTDNS(qct_dns *p)
{
  SMFPRINTSTART("QCTDNS", p, sizeof(qct_dns));

  ADDU32 ("TSKN",p->qcttskn);
  ADDU32 ("REQN",p->qctreqn);
  ADDSTCK("CPTM",p->qctcptm);
  ADDSTCK("ELTM",p->qcteltm);
  ADDSTCK("WTTM",p->qctwttm);
  ADDTIME("LGTM",p->qctlgtm);
  ADDSTCK("LGDU",p->qctlgdu);

  SMFPRINTSTOP;

  return;
}
