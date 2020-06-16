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

void print123V2_Data(SMF123V2_Data_t *p)
{

  SMFPRINTSTART("ZCEE_V2_Data", p, sizeof(*p));

  ADDBYTE("Version",p->version);
  ADDBYTE("Request_Type",p->reqType);
  ADDU16("Response_Code",p->respCode);
  ADDBYTEX("Response_Flags",p->respFlags);
  ADDSTRENS("User_Name",p->userName);
  ADDSTRENS("User_Name_Mapped",p->userNameMapped);
  ADDSTRENS("IP_Address",p->ipAddr);
  ADDSTRENS("API_Name",p->apiName);
  ADDSTRENS("API_Version",p->apiVersion);
  ADDSTRENS("Service_Name",p->serviceName);
  ADDSTRENS("Service_Version",p->serviceVersion);
  ADDSTRENS("Request_Method",p->reqMethod);
  ADDSTRENS("Request_Query_String",p->reqQueryStr);
  ADDSTRENS("Request_Target_URI",p->reqTargetUri);
  ADDU32("Request_Payload_Len",p->reqPayloadLen);
  ADDU32("Response_Payload_Len",p->respPayloadLen);
  ADDTIMESTCKE("Entry",p->zcEntry);
  ADDTIMESTCKE("Exit",p->zcExit);
  ADDSTCKE("Entry_Clock",p->zcEntry);
  ADDSTCKE("Exit_Clock",p->zcExit);
  ADDSTRENS("Sor_Sent",p->sorSent);

  ADDSTRENS("Sor_Received",p->sorRecv);
  ADDSTRENS("SP_Name",p->spName);
  ADDSTRENS("SOR_Reference",p->sorReference);
  ADDSTRENS("SOR_Identifier",p->sorIdentifier);
  ADDSTRENS("SOR_Resource",p->sorResource);
  ADDSTRB("Request_ID",p->reqId,sizeof(p->reqId));
  ADDSTRB("Tracking_Token",p->trackingToken,sizeof(p->trackingToken));
  ADDSTRENS("Request_Header_1",p->reqHdr1);
  ADDSTRENS("Request_Header_2",p->reqHdr2);
  ADDSTRENS("Request_Header_3",p->reqHdr3);
  ADDSTRENS("Request_Header_4",p->reqHdr4);
  ADDSTRENS("Response_Header_1",p->respHdr1);
  ADDSTRENS("Response_Header_2",p->respHdr2);
  ADDSTRENS("Response_Header_3",p->respHdr3);
  ADDSTRENS("Response_Header_4",p->respHdr4);

  SMFPRINTSTOP;
  return;
}
