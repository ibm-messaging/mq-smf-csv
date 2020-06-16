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

void print123V1_Data(SMF123V1_Data_t *p)
{

  SMFPRINTSTART("ZCEE_V1_Data", p, sizeof(*p));

  ADDS32("Version", p->version);
  ADDU32("DataType", p->dataType);

  ADDU32("Data_Version", p->dataVersion);
  ADDTIME("Entry",p->zcEntry);
  ADDTIME("Exit",p->zcExit);
  ADDSTCK("Entry_Clock",p->zcEntry);
  ADDSTCK("Exit_Clock",p->zcExit);
  ADDSTRENS("Request_Target_URI",p->reqTargetUri);
  ADDU32("Request_Payload_Length", p->reqPayloadLen);
  ADDSTRENS("Service_Name",p->serviceName);
  ADDSTRENS("Request_Method",p->reqMethod);
  ADDSTRENS("User_Name",p->userName);
  ADDSTRB("Request_ID",p->reqId,sizeof(p->reqId));
  ADDSTRENS("Service_Group",p->serviceGroup);
  ADDSTRENS("User_Name_Mapped",p->userNameMapped);

  SMFPRINTSTOP;
  return;
}
