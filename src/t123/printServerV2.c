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

void print123V2_Server(SMF123V2_Server_t *p)
{
  SMFPRINTSTART("ZCEE_V2_Server", p, sizeof(*p));

  ADDBYTE("Version",p->version);
  ADDSTREN("System_Name",p->systemName,sizeof(p->systemName));
  ADDSTREN("Sysplex_Name",p->sysplexName,sizeof(p->sysplexName));
  ADDSTREN("Job_Id",p->jobid,sizeof(p->jobid));
  ADDSTREN("Job_Name",p->jobname,sizeof(p->jobname));
  ADDSTRB("SToken",p->stoken,sizeof(p->stoken));

  ADDSTREN("Config_Directory",p->configDir,sizeof(p->configDir));
  ADDSTREN("Server_Version",p->serverVersion,sizeof(p->serverVersion));

  SMFPRINTSTOP;
  return;
}
