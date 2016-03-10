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

void printQLST(qlst *p)
{
  SMFPRINTSTART("QLST", p, conv16(p->qlstll));

  ADDU32("GETL",p->qlstgetl);
  ADDU32("HLDL",p->qlsthldl);
  ADDU32("RELL",p->qlstrell);

  SMFPRINTSTOP;
  return;
}

