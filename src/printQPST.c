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

/*******************************************************************/
/* Bitfields on z/OS are implemented in the compiler with the      */
/* first bit being the top bit of a word. So we refer to           */
/* flags in that order.                                            */
/*******************************************************************/
#define QPSTLOC  (0x80000000)
#define QPSTF4KB (0x40000000)

SMFPRINTGLOB;

void printQPST(qpst *p)
{
  unsigned int flags;
  unsigned int *u;

  SMFPRINTSTART("QPST", p, conv16(p->qpstll));

  ADDS32("POOL",p->qpstpool);
  ADDS32("NBUF",p->qpstnbuf);
  ADDS32("CBSL",p->qpstcbsl);
  ADDS32("CBS ",p->qpstcbs );
  ADDS32("GETP",p->qpstgetp);
  ADDS32("GETN",p->qpstgetn);
  ADDS32("RIO ",p->qpstrio );
  ADDS32("STW ",p->qpststw );
  ADDS32("TPW ",p->qpsttpw );
  ADDS32("WIO ",p->qpstwio );
  ADDS32("IMW ",p->qpstimw );
  ADDS32("DWT ",p->qpstdwt );
  ADDS32("DMC ",p->qpstdmc );
  ADDS32("STL ",p->qpststl );
  ADDS32("STLA",p->qpststla);
  ADDS32("SOS ",p->qpstsos );

  /*****************************************************************/
  /* Extract the word containing bitfield flags. Make sure it's in */
  /* the right order, then test the flags.                         */
  /*****************************************************************/
  u = (unsigned int  *)&p->qpstsos;
  flags = conv32(*(u+1));

  if(flags & QPSTLOC)
  {
    ADDSTR("LOC","Above");
  }
  else
  {
    ADDSTR("LOC","Below");
  }

  if(flags & QPSTF4KB)
  {
    ADDSTR("FIX","Fixed");
  }
  else
  {
    ADDSTR("FIX","Paged");
  }

  SMFPRINTSTOP;

  return;
}
