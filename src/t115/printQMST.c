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

void printQMST(qmst *p)
{
  SMFPRINTSTART("QMST", p, conv16(p->qmstll));

  ADDS32("Open",p->qmstopen);
  ADDS32("Close",p->qmstclos);
  ADDS32("Get" ,p->qmstget );
  ADDS32("Put" ,p->qmstput );
  ADDS32("Put1",p->qmstput1);
  ADDS32("Inq" ,p->qmstinq );
  ADDS32("Inql",p->qmstinql);
  ADDS32("Set" ,p->qmstset );
  ADDS32("Endw",p->qmstendw);
  ADDS32("Close_Handles",p->qmstcalh);
  if (conv16(p->qmstll) >  offsetof(qmst,  qmstsub))/* Fields added for V7.0 API calls*/
  {
    ADDS32("Sub"   ,p->qmstsub);
    ADDS32("SubReq",p->qmstsubr);
    ADDS32("CB"    ,p->qmstcb);
    ADDS32("CTL"   ,p->qmstctl);
    ADDS32("Status",p->qmststus);
    ADDS32("Pubs"  ,p->qmstpubs);
  }
#if CSQDSMF_VERSION >= 903
  if (conv16(p->qmstll) >  offsetof(qmst,  qmstspp))/* Fields added for V9.0.3 */
  {
    ADDS64("PersPuts",    p->qmstspp);
    ADDS64("NonPersPuts", p->qmstsnp);
    ADDS64("PersPutBytes",    p->qmstpbp);
    ADDS64("NonPersPutBytes", p->qmstnbp);
  }
#endif

#if CSQDSMF_VERSION >= 943
  // MQW 943 introduced OpenTelemetry tracing for applications via
  // SMF. As part of that, there are lots of new metrics.
  if (conv16(p->qmstll) > offsetof(qmst,qmstocpa))
  {
    ADDU64("Otel_Prop_Put",       p->qmstocpa);
    ADDU64("Otel_Prop_Get",       p->qmstocga);
    ADDU64("Otel_Disc_Put",       p->qmstoahc);
    ADDU64("Otel_Inval_Put",      p->qmstocpd);
    ADDU64("Otel_Max_Props_Put",  p->qmstocmp);
    ADDU64("Otel_Inval_Get",      p->qmstocgd);
    ADDU64("Otel_State_Trunc_Put",p->qmstocps);
    ADDU64("Otel_State_Trunc_Get",p->qmstocgs);
    ADDU64("Otel_Spans_Consumer", p->qmstoscs);
    ADDU64("Otel_Spans_Producer", p->qmstosps);
    ADDU64("Otel_Nosample_Put",   p->qmstosns);
    ADDU64("Otel_Disc_Get",       p->qmstocda);
    ADDU64("Otel_Span_Limit_Producer", p->qmstospl);
    ADDU64("Otel_Span_Limit_Consumer", p->qmstoscl);
    ADDU64("Otel_Span_Limit_Storage",  p->qmstosfs);
    ADDU64("Otel_Span_Success",        p->qmstosok);
    ADDU64("Otel_Span_Failed",         p->qmstosfr);
    ADDU64("Otel_Span_Failed_Capacity",p->qmstosfc);
    ADDU64("Otel_SMF_Success",         p->qmstotso);
    ADDU64("Otel_SMF_Failed",          p->qmstotsr);
    ADDU64("Otel_SMF_Failed_Capacity", p->qmstotsc);
    ADDSTCK("Otel_CPU_Time",           p->qmstotpu);
    ADDSTCK("Otel_Wait_Work_Time",     p->qmstotpw);
    ADDSTCK("Otel_Wait_SMF_Time",      p->qmstotpd);
    ADDS32("Otel_EmissionTasks",       p->qmstotsk);
  }
#endif

  SMFPRINTSTOP;

  return;
}

