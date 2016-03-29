
#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;

void printQTST(qtst *p)
{

  SMFPRINTSTART("QTST", p, conv16(p->qtstll));

  ADDU32("STOT", p->qtstzero.qtststot);
  ADDU32("SDUR", p->qtstzero.qtstsdur);

  ADDU32("SHIGH{1}", p->qtstzero.qtstshig[0]);
  ADDU32("SHIGH{2}", p->qtstzero.qtstshig[1]);
  ADDU32("SHIGH{3}", p->qtstzero.qtstshig[2]);
  ADDU32("SLOW{1}",  p->qtstzero.qtstslow[0]);
  ADDU32("SLOW{2}",  p->qtstzero.qtstslow[1]);
  ADDU32("SLOW{3}",  p->qtstzero.qtstslow[2]);

  ADDU32("SEXP", p->qtstzero.qtstsexp);
  ADDU32("TMSG", p->qtstzero.qtsttmsg);
  ADDU32("SPHW", p->qtstzero.qtstsphw);

  ADDU32("PTOT{1}", p->qtstzero.qtstptot[0]);
  ADDU32("PTOT{2}", p->qtstzero.qtstptot[1]);
  ADDU32("PTOT{3}", p->qtstzero.qtstptot[2]);

  ADDU32("SHIGH", p->qtstzero.qtstphig);
  ADDU32("PLOW ", p->qtstzero.qtstplow);
  ADDU32("PNOS ", p->qtstzero.qtstpnos);
  ADDSTCK("ETHW", p->qtstzero.qtstethw);
  ADDSTCK("ETTO", p->qtstzero.qtstetto);

  SMFPRINTSTOP;

  return;
}
