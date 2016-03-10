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

void printQJST(qjst *p)
{
  int i,j;
  char index[32];

  SMFPRINTSTART("QJST", p, conv16(p->qjstll));

  ADDS32("WRW  ",p->qjstwrw );
  ADDS32("WRNW ",p->qjstwrnw);
  ADDS32("WRF  ",p->qjstwrf );
  ADDS32("WTB  ",p->qjstwtb );
  ADDS32("RBUF ",p->qjstrbuf);
  ADDS32("RACT ",p->qjstract);
  ADDS32("RARH ",p->qjstrarh);
  ADDS32("TVC  ",p->qjsttvc );
  ADDS32("BSDS ",p->qjstbsds);
  ADDS32("BFFL ",p->qjstbffl);
  ADDS32("BFWR ",p->qjstbfwr);
  ADDS32("ALR  ",p->qjstalr );
  ADDS32("ALW  ",p->qjstalw );
  ADDS32("CIOF ",p->qjstciof);
  ADDS32("LLCP ",p->qjstllcp);
  ADDS32("WUR  ",p->qjstwur );
  ADDS32("LAMA ",p->qjstlama);
  ADDS32("LAMS ",p->qjstlams);
  ADDS32("LSUS ",p->qjstlsus);
  ADDS32("LOGW ",p->qjstlogw);
  ADDS32("CIWR ",p->qjstciwr);
  ADDS32("SERW ",p->qjstserw);
  ADDS32("THRW ",p->qjstthrw);
  ADDS32("BPAG ",p->qjstbpag);

  if (conv32(p->qjstll)>=offsetof(qjst,qjstio))
  {
    for (i=0;i<3;i++)
    {
      if (first)
        sprintf(index,"%d",i+1);

      ADDS32IDX("CMPREQ",index,p->qjstcompr[i].qjstcmpreq);
      ADDS32IDX("CMPFAIL  ", index,p->qjstcompr[i].qjstcmpfail);
      ADDS64IDX("CMPUNCMP ", index,p->qjstcompr[i].qjstcmpuncmp);
      ADDS64IDX("CMPCOMP  ", index,p->qjstcompr[i].qjstcmpcomp);
      ADDS32IDX("DECREQ   ", index,p->qjstcompr[i].qjstdecreq);
      ADDS32IDX("DECFAIL  ", index,p->qjstcompr[i].qjstdecfail);
      ADDS64IDX("DECUNCMP ", index,p->qjstcompr[i].qjstdecuncmp);
      ADDS64IDX("DECCOMP  ", index, p->qjstcompr[i].qjstdeccomp);
    }
  }

  if (conv32(p->qjstll)>=sizeof(qjst))
  {
    for (i=0;i<2;i++)
    {
      for (j=0;j<2;j++)
      {
        if (first)
          sprintf(index,"%d.%d",i+1,j+1);

        ADDU32IDX("IOCOUNT   ", index , p->qjstio[i].qjstiotype[j].qjstiocount);
        ADDU32IDX("IOCI      ", index , p->qjstio[i].qjstiotype[j].qjstioci);
        ADDSTCKIDX("IOTOTIO  ", index , p->qjstio[i].qjstiotype[j].qjstiototio);
        ADDSTCKIDX("IOTOTSUS ", index , p->qjstio[i].qjstiotype[j].qjstiototsus);

        ADDSTCKIDX("IOMAXIO  ", index , p->qjstio[i].qjstiotype[j].qjstiomaxio);
        ADDTIMEIDX("IOMAXIOT  ", index ,p->qjstio[i].qjstiotype[j].qjstiomaxioT);
        ADDSTRENIDX("IOMAXIOL", index , p->qjstio[i].qjstiotype[j].qjstiomaxiol,8);

        ADDSTCKIDX("IOMAXSUS  ", index, p->qjstio[i].qjstiotype[j].qjstiomaxsus);
        ADDTIMEIDX("IOMAXSUST  ", index, p->qjstio[i].qjstiotype[j].qjstiomaxsusT);
        ADDSTRENIDX("IOMAXSUSL", index, p->qjstio[i].qjstiotype[j].qjstiomaxsusl,8);
      }
    }
  }

  SMFPRINTSTOP;

  return;
}
