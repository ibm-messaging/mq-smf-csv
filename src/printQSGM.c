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

void printQSGM(qsgm *p)
{
  SMFPRINTSTART("QSGM", p, conv16(p->qsgmll));

  ADDS32( "LVTOT" ,p->qsgmlvtot);
  ADDS32( "LVNEW" ,p->qsgmlvnew);
  ADDS32( "LVMAX" ,p->qsgmlvmax);
  ADDS32( "LVMIN" ,p->qsgmlvmin);
  ADDS32( "COUNT" ,p->qsgmcount);
  ADDBYTE("SCLS"  ,p->qsgmscls);
  ADDU64( "CSECT" ,p->qsgmcsect);
  ADDS32( "OFFSET",p->qsgmoffset);

  SMFPRINTSTOP;

  return;
}

