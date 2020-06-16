/*
 * Copyright (c) 2016,2020 IBM Corporation and other Contributors.
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
#include "t123/smf123.h"

SMFPRINTGLOB;

void print123V1_Server(SMF123V1_Server_t *p)
{
  SMFPRINTSTART("ZCEE_V1_Server", p, sizeof(*p));

  ADDU32("Version",p->version);
  ADDSTRENS("System_Name",p->systemName);
  ADDSTRENS("Sysplex_Name",p->sysplexName);
  ADDSTRENS("Job_Id",p->jobid);
  ADDSTRENS("Job_Name",p->jobname);
  ADDSTRB("SToken",p->stoken,8);

  SMFPRINTSTOP;
  return;
}
