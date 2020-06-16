/*
 * Copyright (c) 2016,2020 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Andrew Mattingly - AMS parser/formatter
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef INCL_SMF180_H
#define INCL_SMF180_H


/********************************************************************/
/* Note that AMS can emit SMF records with a configurablt record    */
/* type but the default is 180.                                     */
/********************************************************************/

/********************************************************************/
/* The AMS control blocks are much less likely to change than the   */
/* rest of the MQ SMF structures so this file is derived from,      */
/* instead of being generated from, the product header file.        */
/********************************************************************/
/********************************************************************/
/* AMS uses doublet blocks to hold the offset and length of its     */
/* Product and Data sections, unlike other SMF records :)           */
/********************************************************************/
typedef struct {
        unsigned int    offset;
        unsigned int    l;
} doublet_t;

/*********************************************************************/
/* The AMS SMF records contain the standard SMF header, and unusual  */
/* SMF doublets identifying information within the SMF record.       */
/* see: mqhlq.SCSQMACS(CSQ0KSMF)                                     */
/*********************************************************************/
typedef struct
{
        SMFHeader_t     Header;       /* SMF record header                */
        /* AMS record does not store MQ v.r.m after record subtype */
        /* It stores number of doublets as a halfword, then another*/
        /* reserved halfword, but number of doublets is always 2   */
        /* beyond those 4 bytes that don't follow the MQ standard  */
        /* header mapping, there are another 4 reserved fullwords  */
        uint32_t        smfb4rs1[4];
        doublet_t       s[2];
} AMSRecord_t;

typedef struct {
        /* Product Section */
        uint8_t         smfb4ver;
        uint8_t         smfb4rel;
        uint8_t         smfb4mod;
        uint8_t         smfb4rs2;
        uint8_t         smfb4fmi[8];
        uint8_t         smfb4mvs[34];
        uint8_t         smfb3rs3[2];
        uint32_t        smfb4rs4[4];
} AMSProduct_t;

typedef struct {
        /* Data Section */
        uint8_t         rb41jbn[8];             /* Job name                            */
        uint8_t         rb41usr[8];             /* User identification                 */
        uint8_t         rb41sys[8];             /* System name                         */
        uint8_t         rb41plx[8];             /* Sysplex name                        */
        uint32_t        rb41act;                /* Action being performed              */
        uint32_t        rb41rsn;                /* MQ related reason code              */
        uint32_t        rb41aud;                /* Audit code                          */
        uint32_t        rb41dec;                /* Access decision                     */
        uint8_t         rb41pop;                /* Protection operation                */
        uint8_t         rb41sga;                /* Signature alogorithm                */
        uint8_t         rb41enc;                /* Encryption strength                 */
        uint8_t         rb41rs1;                /* Reserved                            */
        doublet_t       rb41ob;                 /* Object name                         */
        doublet_t       rb41qn;                 /* Queue name                          */
        doublet_t       rb41qm;                 /* Queue manager name                  */
        doublet_t       rb41op;                 /* Operation codes                     */
        doublet_t       rb41id;                 /* Sender identity                     */
        uint32_t        rb41nrc;                /* Number of entries in recipient list */
        doublet_t       rb41rc;                 /* Recipient list                      */
        doublet_t       rb41ms;                 /* Message id                          */
        uint8_t         rb41fmt[8];             /* Format name                         */
        uint32_t        rb41kru;                /* Key reuse count                     */
        uint32_t        rb41rs2[3];             /* Reserved                            */
} AMSData_t;

/* AMS recipient entry */
typedef struct {
        uint32_t        rb41rcre;               /* Length of recipient entry           */
        uint8_t         rb41rcrp[1];    /* Recipient name                      */
} AMSRecipient_t;

extern void printAMS(AMSProduct_t*, AMSData_t *, int);

#endif
