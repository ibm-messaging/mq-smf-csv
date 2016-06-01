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

void printQCTSSL(qct_ssl *p)
{
  SMFPRINTSTART("QCTSSL", p, sizeof(qct_ssl));

  ADDU32 ("TSKN",p->qcttskn);
  ADDU32 ("REQN",p->qctreqn);
  ADDSTCK("CPTM",p->qctcptm);
  ADDSTCK("ELTM",p->qcteltm);
  ADDSTCK("WTTM",p->qctwttm);
  ADDTIME("LSTM",p->qctlstm);
  ADDSTCK("LSDU",p->qctlsdu);

  SMFPRINTSTOP;

  return;
}
