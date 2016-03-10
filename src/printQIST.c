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

void printQIST(qist *p)
{
  SMFPRINTSTART("QIST", p, conv16(p->qistll));

  ADDU32 ("MGET",p->qistmget);
  ADDU32 ("MPUT",p->qistmput);
  ADDU32 ("MBLR",p->qistmblr);
  ADDU32 ("DCRE",p->qistdcre);
  ADDU32 ("DPUT",p->qistdput);
  ADDU32 ("DDEL",p->qistddel);
  ADDU32 ("DGET",p->qistdget);
  ADDU32 ("DLOC",p->qistdloc);
  ADDU32 ("MCNT",p->qistmcnt);
  ADDU32 ("ALST",p->qistalst);
  ADDU32 ("LOMM",p->qistlomm);
  ADDU32 ("DLMM",p->qistdlmm);
  ADDU32 ("ENUM",p->qistenum);
  ADDU32 ("RAIO",p->qistraio);
  ADDU32 ("RABP",p->qistrabp);
  ADDU32 ("GETD",p->qistgetd);
  ADDU32 ("GETB",p->qistgetb);

  SMFPRINTSTOP;

  return;
}
