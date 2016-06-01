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

void printQESD(qesd *p)
{
      SMFPRINTSTART("QESD",p,conv16(p->qesdll));

      ADDSTREN("STR ", p -> qesdstr,12);
      ADDU32  ("STRN", p -> qesdstrn);
      ADDU32  ("SMBT", p -> qesdsmbt);
      ADDU32  ("SMBS", p -> qesdsmbs);
      ADDU32  ("SMBD", p -> qesdsmbd);
      ADDU32  ("SMBU", p -> qesdsmbu);
      ADDU32  ("SMBF", p -> qesdsmbf);
      ADDU32  ("SMMC", p -> qesdsmmc);
      ADDU32  ("SMAR", p -> qesdsmar);
      ADDU32  ("SMFR", p -> qesdsmfr);
      ADDU32  ("SMRR", p -> qesdsmrr);
      ADDU32  ("SMCR", p -> qesdsmcr);
      ADDU32  ("SMAP", p -> qesdsmap);
      ADDU32  ("SMFP", p -> qesdsmfp);
      ADDU32  ("SMRP", p -> qesdsmrp);
      ADDU32  ("SMCP", p -> qesdsmcp);
      ADDU32  ("SMFL", p -> qesdsmfl);
      ADDU32  ("SMMM", p -> qesdsmmm);
      ADDU32  ("SMMU", p -> qesdsmmu);
      ADDU32  ("SMMF", p -> qesdsmmf);
      ADDU32  ("BFSZ", p -> qesdbfsz);
      ADDU32  ("BFTO", p -> qesdbfto);
      ADDU32  ("BFUS", p -> qesdbfus);
      ADDU32  ("BFUP", p -> qesdbfup);
      ADDU32  ("BFFS", p -> qesdbffs);
      ADDU32  ("BFFE", p -> qesdbffe);
      ADDU32  ("BFPW", p -> qesdbfpw);
      ADDU32  ("BFBW", p -> qesdbfbw);
      ADDU32  ("BFGB", p -> qesdbfgb);
      ADDU32  ("BFGV", p -> qesdbfgv);
      ADDU32  ("BFGM", p -> qesdbfgm);
      ADDU32  ("BFGF", p -> qesdbfgf);
      ADDU32  ("BFGL", p -> qesdbfgl);
      ADDU32  ("BFGN", p -> qesdbfgn);
      ADDU32  ("BFRR", p -> qesdbfrr);
      ADDU32  ("BFRS", p -> qesdbfrs);
      ADDU32  ("BFRP", p -> qesdbfrp);
      ADDU32  ("BFWR", p -> qesdbfwr);
      ADDU32  ("BFFB", p -> qesdbffb);
      ADDU32  ("BFDB", p -> qesdbfdb);
      ADDU32  ("BFWP", p -> qesdbfwp);
      ADDU32  ("BFWB", p -> qesdbfwb);
      ADDSTCK ("BFPT", p -> qesdbfpt);
      ADDU32  ("BFMU", p -> qesdbfmu);
      ADDU32  ("BFMF", p -> qesdbfmf);
      ADDU32  ("BFMP", p -> qesdbfmp);
      ADDU32  ("BFMB", p -> qesdbfmb);
      ADDU32  ("IOHA", p -> qesdioha);
      ADDU32  ("IOHU", p -> qesdiohu);
      ADDU32  ("IOCI", p -> qesdioci);
      ADDU32  ("IOCA", p -> qesdioca);

      ADDU32  ("IOFR", p -> qesdiofr);
      ADDU32  ("IOFP", p -> qesdiofp);

      ADDSTCK ("IOFT", p -> qesdioft);

      ADDU32  ("IOWR", p -> qesdiowr);
      ADDU32  ("IOWP", p -> qesdiowp);
      ADDSTCK ("IOWT", p -> qesdiowt);

      ADDU32  ("IORR", p -> qesdiorr);
      ADDU32  ("IORP", p -> qesdiorp);
      ADDSTCK ("IORT", p -> qesdiort);

      ADDU32  ("IOOR", p -> qesdioor);
      ADDU32  ("IOOP", p -> qesdioop);
      ADDSTCK ("IOOT", p -> qesdioot);

      SMFPRINTSTOP;
      return;
}
