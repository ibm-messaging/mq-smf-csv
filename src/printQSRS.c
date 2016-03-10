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

void printQSRS(qsrs *p)
{
  SMFPRINTSTART("QSRS", p, conv16(p->qsrsll));

  ADDS32("PHBGF",  p->qsrsphbgf);
  ADDS32("PHBGV",  p->qsrsphbgv);
  ADDS32("PHBLF",  p->qsrsphblf);
  ADDS32("PHBLV",  p->qsrsphblv);
  ADDS32("GMBT",   p->qsrsgmbt);
  ADDS32("STKT",   p->qsrsstkt);


  ADDS32("STRTA",  p->qsrsstrta);
  ADDS32("SIZA",   p->qsrssiza);
  ADDS32("CRGTP",  p->qsrscrgtp);
  ADDS32("LIMIT",  p->qsrslimit);
  ADDS32("LOAL",   p->qsrsloal);
  ADDS32("OLOAL",  p->qsrsoloal);
  ADDS32("HIAL",   p->qsrshial);
  ADDS32("OHIAL",  p->qsrsohial);
  ADDS32("SMFL",   p->qsrssmfl);
  ADDS32("SMFR",   p->qsrssmfr);


  ADDS32("ESTRA",  p->qsrsestra);
  ADDS32("ESIZA",  p->qsrsesiza);
  ADDS32("ERGTP",  p->qsrsergtp);
  ADDS32("ELIM",   p->qsrselim);
  ADDS32("ELOAL",  p->qsrseloal);
  ADDS32("OELOAL", p->qsrsoeloal);
  ADDS32("EHIAL",  p->qsrsehial);
  ADDS32("OEHIAL", p->qsrsoehial);
  ADDS32("SMFEL",  p->qsrssmfel);
  ADDS32("SMFER",  p->qsrssmfer);


  ADDU64("MEMLIM", p->qsrsmemlim);
  ADDU64("ABYTES", p->qsrsabytes);
  ADDU64("GBYTES", p->qsrsgbytes);
  ADDS32("NMOMB",  p->qsrsnmomb);
  ADDBYTE("MEMLIMS",p->qsrsmemlims);

  ADDS32("FMCT",   p->qsrsfmct);


  ADDS32("AVAL",   p->qsrsaval);
  ADDS32("MVS",    p->qsrsmvs);
  ADDS32("CRIT",   p->qsrscrit);
  ADDS32("SOS",    p->qsrssos);



  ADDU64("AVAL64", p->qsrsaval64);
  ADDU64("CUSH64", p->qsrscush64);
  ADDU64("SMDS64", p->qsrssmds64);


  ADDU64("PHBLGF", p->qsrsphblgf);
  ADDU64("PHBLGV", p->qsrsphblgv);
  ADDU64("PHBLLF", p->qsrsphbllf);
  ADDU64("PHBLLV", p->qsrsphbllv);
  ADDU64("GMBL",   p->qsrsgmbl);


  SMFPRINTSTOP;

  return;
}

