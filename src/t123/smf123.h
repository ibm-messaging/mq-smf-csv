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
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/*
See https://www.ibm.com/support/knowledgecenter/SS4SVW_3.0.0/monitoring/smf_provider_v2.html
and https://www.ibm.com/support/knowledgecenter/SS4SVW_3.0.0/monitoring/smf_v1.html

Note that the V1 structure is the default.

*/

#ifndef INCL_SMF123_H
#define INCL_SMF123_H

/* Space for the subtypeVersion is actually included in the SMFHeader type but is in the
   same place as the MQ Version field. I could use something like a union for a more
   general way but don't want to rewrite the working MQ code at the moment.
   So we get the field with a manual memcpy.
*/
typedef struct {
  SMFHeader_t Header;
  /* uint32_t    SubtypeVersion; */
  uint32_t    tripletCount;
  uint32_t    recordIndex;
  uint32_t    recordCount;
  char        recordContinuation[8];
  widetriplet_t   triplets[2];
} SMF123V1_t;

typedef struct {
  SMFHeader_t  	Header;       /* SMF record header                */
  /* uint32_t      SubtypeVersion; */
  unsigned char sectionsCount; /* Points at a set of tripets */
  unsigned char sectionsOffset;
  unsigned char reserved[2];
  uint32_t      TZOffset;
} SMF123V2_t;

typedef struct {
  triplet_t server;
  triplet_t data;
} SMF123Triplets_t;


typedef struct {
  uint32_t      version;
  char          systemName[8];
  char          sysplexName[8];
  char          jobid[8];
  char          jobname[8];
  unsigned char stoken[8];
  char          reserved[36];
} SMF123V1_Server_t;

typedef struct {
  unsigned char version;
  unsigned char reserved[3];
  char          systemName[8];
  char          sysplexName[8];
  char          jobid[8];
  char          jobname[8];
  unsigned char stoken[8];
  char          configDir[128];
  char          serverVersion[8];
} SMF123V2_Server_t;

typedef struct {
  uint32_t version;
  uint32_t dataType;
  uint32_t dataLen; /* should be sizeof(the rest of this structure) */
  uint32_t dataVersion;
  uint64_t    zcEntry; /* STCK - given as char[8] in original docs */
  uint64_t    zcExit;
  char     reqTargetUri[64];
  uint32_t reqPayloadLen;
  char     serviceName[64];
  char     reqMethod[8];
  uint32_t respPayloadLen;
  char     userName[64];
  unsigned char reqId[8];
  char     reserved2[15];
  char     reserved3[1];
  char     serviceGroup[64];
  char     userNameMapped[8];
} SMF123V1_Data_t;


typedef struct {
  char          version;
  char          reqType;
  uint16_t      respCode;
  char          respFlags;
  char          reserved[3];
  char          userName[64];
  char          userNameMapped[8];
  char          ipAddr[48];
  char          apiName[64];
  char          apiVersion[8];
  char          serviceName[64];
  char          serviceVersion[8];
  char          reqMethod[8];
  char          reqQueryStr[128];
  char          reqTargetUri[256];
  uint32_t      reqPayloadLen;
  uint32_t      respPayloadLen;
  char          zcEntry[16]; /* STCKE */
  char          zcExit[16];
  char          sorSent[16];
  char          sorRecv[16];
  char          spName[16];
  char          sorReference[32];
  char          sorIdentifier[64];
  char          sorResource[128];
  unsigned char reqId[8];
  unsigned char trackingToken[64];
  char reqHdr1[64];
  char reqHdr2[64];
  char reqHdr3[64];
  char reqHdr4[64];
  char respHdr1[64];
  char respHdr2[64];
  char respHdr3[64];
  char respHdr4[64];
}SMF123V2_Data_t;

extern void print123V1_Data(SMF123V1_Data_t *p);
extern void print123V2_Data(SMF123V2_Data_t *p);
extern void print123V1_Server(SMF123V1_Server_t *p);
extern void print123V2_Server(SMF123V2_Server_t *p);



#endif
