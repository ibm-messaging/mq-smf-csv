/*
 * Copyright (c) 2016,2024 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Andrew Mattingly - AMS records
 */

#include <stdio.h>
#include "mqsmf.h"
#include "t180/smf180.h"

SMFPRINTGLOB;


/* These constants can be found in the csq0ksmf.macro definitions */

/* Action codes */
#define RB41ACOP        1               /* Open   */
#define RB41ACGT        2               /* Get    */
#define RB41ACPT        3               /* Put    */
#define RB41ACP1        4               /* Put1   */
#define RB41ACCL        5               /* Close  */

/* Audit codes */
#define RB41AGOK        0               /* Get successful (Note: same value for Put) */
#define RB41AGQM        1               /* Get QoP mismatch      */
#define RB41AGSE        2               /* Get security error    */
#define RB41AGDE        3               /* Get decrypt error     */
#define RB41AGSN        4               /* Get sender map error  */
#define RB41AGSA        5               /* Get sender azn error  */
#define RB41AGHE        6               /* Get header error      */
#define RB41AGEV        7               /* Get environment error */
#define RB41AGSM        8               /* Get size mismatch     */
#define RB41AGEM        9               /* Get encrypt mismatch  */
#define RB41AGCV        10              /* Get conversion error  */
#define RB41AGAD        11              /* Get adddynq error     */
#define RB41AGMQ        12              /* Get MQ error          */
#define RB41APOK        0               /* Put successful        */
#define RB41APBD        32              /* Put build error       */
#define RB41APEV        33              /* Put environment error */
#define RB41APEN        34              /* Put encrypt error     */
#define RB41APMQ        35              /* Put MQ error          */

/* Access decision codes */
#define RB41DCAG        2               /* Access granted */
#define RB41DCAD        4               /* Access denied  */

/* Protection operation codes */
#define RB41PONO        1               /* None            */
#define RB41POIN        2               /* Integrity       */
#define RB41POPR        3               /* Privacy         */
#define RB41POCF        4               /* Confidentiality */

/* Signature algorithm codes */
#define RB41SGM2        1               /* MD2     */
#define RB41SGM5        2               /* MD5     */
#define RB41SGS1        3               /* SHA-1   */
#define RB41S224        4               /* SHA-224 */
#define RB41S256        5               /* SHA-256 */
#define RB41S384        6               /* SHA-384 */
#define RB41S512        7               /* SHA-512 */

/* encryption algorithm codes */
#define RB41ECWK        3               /* Weak (RC2)    */
#define RB41ECMD        2               /* Medium (DES)  */
#define RB41ECST        1               /* Strong (3DES) */
#define RB41ECA1        4               /* AES128        */
#define RB41ECA2        5               /* AES256        */

/* area to hold a (long) list of recipient distinguished names */
#define MAX_RECIPS_STRING       16383
static char recipsString[MAX_RECIPS_STRING+1];

static char emptyMsgId[24] = {0};

void printAMS(AMSProduct_t* p, AMSData_t *q, int len)
{
        char    temp[64], *pt;
        int             recipLength, recipsLength, nRecips, recipsStrLength;
        AMSRecipient_t  *pRecip;

        SMFPRINTSTART("AMS", p, len);

        /*******************/
        /* Product Section */
        /*******************/
        ADDBYTE("Product_Version", p->smfb4ver);
        ADDBYTE("Product_Release", p->smfb4rel);
        ADDBYTE("Product_Modification", p->smfb4mod);
        ADDSTREN("Product_FMID", p->smfb4fmi, 8);
        ADDSTREN("Operating_System", p->smfb4mvs, 34);

        /*******************/
        /* Data Section    */
        /*******************/
        ADDSTREN("Job_Name", q->rb41jbn, 8);
        ADDSTREN("User_ID", q->rb41usr, 8);
        ADDSTREN("System_Name", q->rb41sys, 8);
        ADDSTREN("Sysplex_Name", q->rb41plx, 8);
        
        switch (conv32(q->rb41act)) {
        case RB41ACOP:
                pt = "Open";
                break;
        case RB41ACGT:
                pt = "Get";
                break;
        case RB41ACPT:
                pt = "Put";
                break;
        case RB41ACP1:
                pt = "Put1";
                break;
        case RB41ACCL:
                pt = "Close";
                break;
        default:
                sprintf(temp, "Unknown (%u)", conv32(q->rb41act));
                pt = temp;
                break;
        }
        ADDSTR("Action_Code", pt, 64);
        ADDU32("MQ_Reason_Code", q->rb41rsn);

        switch (conv32(q->rb41aud)) {
        case RB41AGOK:
                pt = "Operation successful";
                break;
        case RB41AGQM:
                pt = "Get QoP mismatch";
                break;
        case RB41AGSE:
                pt = "Get security error";
                break;
        case RB41AGDE:
                pt = "Get decrypt error";
                break;
        case RB41AGSN:
                pt = "Get sender map error";
                break;
        case RB41AGSA:
                pt = "Get sender azn error";
                break;
        case RB41AGHE:
                pt = "Get header error";
                break;
        case RB41AGEV:
                pt = "Get environment error";
                break;
        case RB41AGSM:
                pt = "Get size mismatch";
                break;
        case RB41AGEM:
                pt = "Get encrypt mismatch";
                break;
        case RB41AGCV:
                pt = "Get conversion error";
                break;
        case RB41AGAD:
                pt = "Get adddynq error";
                break;
        case RB41AGMQ:
                pt = "Get MQ error";
                break;
        case RB41APBD:
                pt = "Put build error";
                break;
        case RB41APEV:
                pt = "Put environment error";
                break;
        case RB41APEN:
                pt = "Put encrypt error";
                break;
        case RB41APMQ:
                pt = "Put MQ error";
                break;
        default:
                sprintf(temp, "Unknown (%u)", conv32(q->rb41aud));
                pt = temp;
                break;
        }
        ADDSTR("Audit_Code", pt, 64);

        switch (conv32(q->rb41dec)) {
        case RB41DCAG:
                pt = "Access granted";
                break;
        case RB41DCAD:
                pt = "Access denied";
                break;
        default:
                sprintf(temp, "Unknown (%u)", conv32(q->rb41dec));
                pt = temp;
        }
        ADDSTR("Access_Decision", pt, 64);

        switch (q->rb41pop) {
        case RB41PONO:
                pt = "None";
                break;
        case RB41POIN:
                pt = "Integrity";
                break;
        case RB41POPR:
                pt = "Privacy";
                break;
        case RB41POCF:
                pt = "Confidentiality";
                break;
        default:
                sprintf(temp, "Unknown (%u)", (uint32_t)q->rb41pop);
                pt = temp;
        }
        ADDSTR("Protection_Operation", pt, 64);

        /* signature algorithm is populated in the SMF record, even when the */
        /* protection operation doesn't call for signature generation or     */
        /* verification, so test the protection operation for relevance of   */
        /* a signature algorithm value (i.e. Integrity or Privacy).          */
        if (q->rb41pop == RB41POIN ||
                q->rb41pop == RB41POPR) {
                switch (q->rb41sga) {
                case RB41SGM2:
                        pt = "MD2";
                        break;
                case RB41SGM5:
                        pt = "MD5";
                        break;
                case RB41SGS1:
                        pt = "SHA-1";
                        break;
                case RB41S224:
                        pt = "SHA-224";
                        break;
                case RB41S256:
                        pt = "SHA-256";
                        break;
                case RB41S384:
                        pt = "SHA-384";
                        break;
                case RB41S512:
                        pt = "SHA-512";
                        break;
                default:
                        sprintf(temp, "Unknown (%u)", (uint32_t)q->rb41sga);
                        pt = temp;
                        break;
                }
        }
        else {
                pt = "N/A";
        }
        ADDSTR("Signature_Algorithm", pt, 64);

        /* encryption algorithm is populated in the SMF record, even when the */
        /* protection operation doesn't call for encryption or decryption, so */
        /* test the protection operation for relevance of an encryption       */
        /* algorithm value (i.e. Confidentiality or Privacy).                 */
        if (q->rb41pop == RB41POCF ||
                q->rb41pop == RB41POPR) {
                switch (q->rb41enc) {
                case RB41ECWK:
                        pt = "Weak (RC2)";
                        break;
                case RB41ECMD:
                        pt = "Medium (DES)";
                        break;
                case RB41ECST:
                        pt = "Strong (3DES)";
                        break;
                case RB41ECA1:
                        pt = "AES-128";
                        break;
                case RB41ECA2:
                        pt = "AES-256";
                        break;
                default:
                        sprintf(temp, "Unknown (%u)", q->rb41enc);
                        pt = temp;
                }
        }
        else {
                pt = "N/A";
        }
        ADDSTR("Encryption_Algorithm", pt, 64);

        ADDSTRENM("Object_Name", ((unsigned char*)q) + q->rb41ob.offset, q->rb41ob.l,48);
        ADDSTRENM("Queue_Name", ((unsigned char*)q) + q->rb41qn.offset, q->rb41qn.l,48);
        ADDSTRENM("QManager_Name", ((unsigned char*)q) + q->rb41qm.offset, q->rb41qm.l,4);

        /* operation codes are not documented, but might be in the future, so convert */
        /* to a native trimmed string, so we can walk thru the characters, if needed. */
        memset(temp, '\0', sizeof(temp));
        memcpy(temp, convStr(((unsigned char*)q) + q->rb41op.offset, q->rb41op.l), q->rb41op.l);
        ADDSTR("Operation_Codes", temp, 64);

        ADDSTRENM("Sender_Identity", ((unsigned char*)q) + q->rb41id.offset, q->rb41id.l,1024);
        ADDU32("Recipient_Count", q->rb41nrc);

        recipsLength = 0;
        nRecips = 0;
        recipsStrLength = 0;
        while (recipsLength < q->rb41rc.l) {
                nRecips++;
                pRecip = (AMSRecipient_t*)(((char*)q) + q->rb41rc.offset + recipsLength);
                recipLength = conv32(pRecip->rb41rcre);
                if (nRecips > 1 && recipsStrLength < MAX_RECIPS_STRING) {
                        recipsString[recipsStrLength++] = ';';
                }
                if (recipsStrLength + recipLength <= MAX_RECIPS_STRING) {
                        memcpy(recipsString + recipsStrLength,
                                convStr(pRecip->rb41rcrp,
                                        recipLength - 1), /* exclude nul terminator */
                                recipLength - 1);
                        recipsStrLength += recipLength - 1;
                }
                recipsLength += recipLength + sizeof(uint32_t);
        }
        recipsString[recipsStrLength] = '\0';
        ADDSTR("Recipients", recipsString, MAX_RECIPS_STRING);

        if (q->rb41ms.l > 0) {
                ADDSTRBN("Message_ID", ((unsigned char*)q) + q->rb41ms.offset, q->rb41ms.l,48);
        }
        else {
                ADDSTRBN("Message_ID", ((unsigned char*)emptyMsgId),24, 48);
        }

        ADDSTREN("Format_Name", q->rb41fmt, 8);
        ADDU32("Reuse_Count", q->rb41kru);

        SMFPRINTSTOP;

        return;
}
