#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;

void printWTID(wtid *p)
{
  SMFPRINTSTART("WTID", p, conv16(p->wtidlen));

  ADDSTR  ("ATYP", strConnType(conv32(p->wtidatyp)));
  ADDSTREN("CCN ", p->wtidccn, 8);
  ADDSTREN("OPID", p->wtidopid,8);
  ADDSTR  ("NID ", convBin(p->wtidnid, 16));
  ADDSTR  ("CORI(HEX)", convBin(p->wtidcori, 12));
  ADDSTREN("CORI(CHAR)", p->wtidcori, 12);
  ADDSTR  ("UOWI", convBin(p->wtiduowi, 24));
  ADDSTR  ("ACCT", convBin(p->wtidacct, 22));
  ADDSTREN("CHL ", p->wtidchl, 20);
  ADDSTREN("CHLC", p->wtidchlc, 48);
  ADDSTR  ("CTXT", convBin(p->wtidctxt, 16));
  ADDSTREN("TRAN", p->wtidtran, 8);

  SMFPRINTSTOP;

  return;
}
