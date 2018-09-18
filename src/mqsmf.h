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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifndef INCL_MQSMF_H
#define INCL_MQSMF_H

/********************************************************************/
/* True and false.                                                  */
/********************************************************************/
typedef      unsigned int BOOL;
#ifndef TRUE
#define TRUE            1
#define FALSE           0
#endif


/********************************************************************/
/* Some platform-specific definitions                               */
/********************************************************************/
#ifdef _AIX
#pragma pack(1)
#else
#pragma pack(push,1)
#endif

#ifdef _WIN32
#define snprintf _snprintf
#endif

/********************************************************************/
/* Include the base header for all the MQ SMF structures            */
/********************************************************************/
#include "mqsmfstruc.h"

#pragma pack(1)


/********************************************************************/
/* Layout of the SMF records. Standard header for all types of SMF. */
/********************************************************************/
typedef struct                         /* Layout of the SMF record header  */
{
  unsigned short SMFLEN;
  unsigned char SMFSEG[2];
  unsigned char SMFRECFLG;             /* Flags                            */
  unsigned char SMFRECRTY;             /* Record Type                      */
  unsigned char SMFRECTME[4];          /* Time record produced             */
  unsigned char SMFRECDTE[4];          /* Date record produced             */
  unsigned char SMFRECSID[4];          /* MVS System ID                    */
  unsigned char SMFRECSSID[4];         /* Subsystem ID                     */
  unsigned short SMFRECSTY;            /* Record Subtype                   */
  unsigned char SMFRECREL[3];          /* WMQ Release                      */
  char reserved[1];                    /* Padding                          */
} SMFHeader_t;

/********************************************************************/
/* SMF uses these triplet blocks a lot to say where each element is,*/
/* and how many repeats there are.                                  */
/********************************************************************/
typedef struct {
  unsigned int   offset;
  unsigned short l;
  unsigned short n;
} triplet_t;

/*********************************************************************/
/* The MQ SMF records contain the standard SMF header, and the usual */
/* SMF triplets identifying information within the SMF record.       */
/* Triplets always start at the same place but the actual number of  */
/* them will vary by record type/subtype.                            */
/* Define the maximum number here for convenience, but remember that */
/* the actual data may start before that max entry depending on      */
/* the number of triplets and the offset values contained in them.   */
/*********************************************************************/
typedef struct
{
  SMFHeader_t      Header;       /* SMF record header                */
  triplet_t  s[12];
} SMFRecord_t;

typedef struct columnHeader_s {
  char *name;
  int   idx;
  void *arrayLine;
} columnHeader_t;


/*********************************************************************/
/* This is the largest that an SMF record can be.                    */
/*********************************************************************/
#define MAX_SMF_DATA (32768)


/*********************************************************************/
/* Don't care about structure packing from here on - not referencing */
/* z/OS-defined structures                                           */
/*********************************************************************/
#pragma pack(pop)

/********************************************************************/
/* Items we may want to print on every element which come from the  */
/* SMF headers at the front of the record.                          */
/* The selected items are then used in the COMMON_BLOCK macro for   */
/* every line of output.                                            */
/********************************************************************/
typedef struct {
  unsigned long long int intstart;      /* Interval start for 115   */
  unsigned long long int intduration;   /* Duration for 115         */
  char recordDate[64];                  /* Timestamp record created */
  char recordTime[64];
  char systemId[4];
  char qMgr[4];
  char mqVer[3];
  char padding;
  char stckFormatDate[64];                  /* STCK as string date/time */
  char stckFormatTime[64];                  /* STCK as string date/time */
} commonFields_t;

/********************************************************************/
/* Formatting functions - one for each element type                 */
/********************************************************************/
extern void printDEBUG (char *title, void *,int);
extern void printQ5ST  (q5st *);
extern void printQCCT  (qcct *);
extern void printQCST  (qcst *);
extern void printQCTADP(qct_adp *);
extern void printQCTDNS(qct_dns *);
extern void printQCTDSP(qct_dsp *);
extern void printQCTSSL(qct_ssl *);
extern void printQESD  (qesd *);
extern void printQEST  (qest *);
#ifdef QIS1IDV
extern void printQIS1  (qis1 *);
#endif
extern void printQIST  (qist *);
extern void printQJST  (qjst *);
extern void printQLST  (qlst *);
extern void printQMAC  (qmac *,qwhc *);
extern void printQMST  (qmst *);
extern void printQPST  (qpst *);
extern void printQSGM  (qsgm *);
extern void printQSPH  (qsph *);
extern void printQSRS  (qsrs *);
extern void printQSST  (qsst *);
extern void printQTST  (qtst *);
extern void printWQ    (wq   *);
extern void printWTAS  (wtas *);
extern void printWTID  (wtid *, unsigned char *);

extern void openDDL    (char *);
extern void closeDDL   (char *);
extern void closeFinalDDL   (void);
extern void printDDL   (char *,int, int);
extern char *formatDDL (char *);
extern char *formatDDLMaxLength (char *,int);
extern void addIndex   (char *);

extern void  jsonNew(FILE *,char *);
extern void  jsonDump(FILE *, columnHeader_t **);
extern columnHeader_t *jsonFormatHeader(BOOL,char *);
extern void  jsonAddInt(columnHeader_t *,int);
extern void  jsonAddInt64(columnHeader_t *,long long);
extern void  jsonAddString(columnHeader_t *,char *,int);
extern void  jsonAddNumericString(columnHeader_t *,char *,int);

/*******************************************************/
/* Windows has some efficient macros to reverse bytes. */
/* Elsewhere, we can do it by hand                     */
/* Also declare the other formatting and conversion    */
/* functions.                                          */
/*******************************************************/
#ifdef _WIN32
#define conv16(x) _byteswap_ushort(x)
#define conv32(x) _byteswap_ulong(x)
#define conv64(x) _byteswap_uint64(x)
#else
extern unsigned short conv16(const unsigned short);
extern unsigned int   conv32(const unsigned int);
extern unsigned long  long conv64(const unsigned long long);
#endif

extern void   convInit();              /* Initialise endianness            */
extern char  *convBin(unsigned char *inbuf, int length );/* Print as hex string*/
extern char  *convStr(unsigned char *,int);/* Convert EBCDIC string to ASCII*/
extern char  *convSecUSec(unsigned long long s);/* Convert stck durations to string containing sec,usec*/
extern void   convDate(unsigned long long s,char *dt[2]);  /* Convert stck to date/time*/
extern int    looksLikeNum(int l, char *c);

extern char  *stckConv(unsigned long long stck, char * output);
extern void   calcYMD(int y,int dY,int *m,int *dM);

extern FILE * fopencsv(const char *, BOOL *);
extern FILE * fopenext(const char *, const char *, BOOL *);

/*
 * Convert some MQI values into the corresponding string
 */
extern char *strConnType(int v);
extern char *strMQIT (int v);
extern char *strMQQT (int v);
extern char *strMQQSGD  (int v);
extern char *strMQMON (int v);
extern char *strMQCHT (int v);
extern char *strMQCHS  (int v);
extern char *strMQCHLD  (int v);
extern char *strCfStatType  (int v);

/**********************************************************/
/* Information about the column headings.                 */
/* These are fixed, and need to be long enough for        */
/* the contents. We do check whether data has overflowed  */
/* and abend if it has.                                   */
/**********************************************************/
#define COL_HEAD_LEN (64)    /* Max len of a column name  */
#define HEADINGS_COUNT (512) /* Max columns per table     */

/**********************************************************/
/* Datatypes that allow creation of SQL DDL statements    */
/**********************************************************/
#define DDL_I        (1)
#define DDL_I64      (2)
#define DDL_C        (3)
#define DDL_SUS      (4)
#define DDL_DATE     (5)
#define DDL_TIME     (6)

/**********************************************************/
/* Datatypes for formatting in JSON. keep them distinct   */
/* from the DDL datatypes above as they have different    */
/* reasons to be used.                                    */
/**********************************************************/
#define ODT_I        (21)
#define ODT_I64      (22)
#define ODT_C        (23)
#define ODT_FIXEDC   (24)
#define ODT_USEC     (25)
#define ODT_VAR      (26)

/************************************************************************/
/* Some "unsigned 64-bit" in the SMF are really pointers or correlators */
/* No real counter is likely to reach the top bit. But treating them as */
/* unsigned values in these cases is causing overflow when loading into */
/* DB2 because SQL 64-bit quantities are always signed with no option   */
/* for the C datatype. So as an expediency, we will drop the sign bit   */
/* which should not affect any real calculations.                       */
/************************************************************************/
#define DROPSIGN 0x7FFFFFFFFFFFFFFF

/**********************************************************/
/* Macros to build the printable lines for CSV output.    */
/* Separate macros for each data type and when we need    */
/* to "dynamically" build column headers based off an     */
/* index loop variable.                                   */
/*                                                        */
/* Basic pattern in each formatting function is           */
/*   SMFPRINTGLOB;                                        */
/*   printXXX(qxxx *p)                                    */
/*   {                                                    */
/*     SMFPRINTSTART(title string, pointer to buf, len);  */
/*     ADDxxxx(column name, data);                        */
/*     ADDyyyy(column name, data);                        */
/*     ...                                                */
/*     SMFPRINTSTOP;                                      */
/*   }                                                    */
/**********************************************************/
extern FILE *smfPrintStart(FILE *,char *,void *,size_t,BOOL *,BOOL *,columnHeader_t **);
extern void  smfPrintStop(FILE *,BOOL, BOOL *,columnHeader_t **);
extern void  smfAddHead(BOOL first,BOOL,char *h,int type,int len);
extern void  smfAddData(int datatype,char *fmt,...);
extern void  smfAddString(int, char *);

#define SMFPRINTGLOB \
  static BOOL first = TRUE;\
  static BOOL newFile = TRUE;\
  static FILE *fp = NULL;\
  static char *dt[2];\
  static columnHeader_t *columns[HEADINGS_COUNT]

#define SMFPRINTSTART(n,p,l) \
  fp = smfPrintStart(fp,n,p,l,&first,&newFile,&columns[0]);

#define SMFPRINTSTOP \
  smfPrintStop(fp, newFile, &first, &columns[0]);

#define ADDHEAD(a,b,c) smfAddHead(first,FALSE,a,b,c)
#define ADDHEADIDX(a,b,c) smfAddHead(first,TRUE,a,b,c)
#define ADDDATA(t,f,...)    smfAddData(t,f,__VA_ARGS__)

#define ADDS64(h,v) \
  ADDHEAD(h,DDL_I64,0); \
  ADDDATA(ODT_I64,"%lld,",conv64(v))

#define ADDU64(h,v) \
  ADDHEAD(h,DDL_I64,0); \
  ADDDATA(ODT_I64,"%lld,",(DROPSIGN & conv64(v)))

#define ADDS64IDX(h,idx,v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEADIDX(tmpHead,DDL_I64,0); \
  ADDDATA(ODT_I64,"%lld,",conv64(v))

#define ADDU64IDX(h,idx,v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEADIDX(tmpHead,DDL_I64,0); \
  ADDDATA(ODT_I64,"%lld,",(DROPSIGN & conv64(v)))

#define ADDS32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%d,",conv32(v))

#define ADDU32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%u,",conv32(v))

#define ADDX32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%X,",conv32(v))

#define ADDS32IDX(h,idx, v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEADIDX(tmpHead,DDL_I,0); \
  ADDDATA(ODT_I,"%d,",conv32(v))

#define ADDU32IDX(h,idx, v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEADIDX(tmpHead,DDL_I,0); \
  ADDDATA(ODT_I,"%u,",conv32(v))

#define ADDS16(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%hd,",conv16(v))

#define ADDU16(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%hu,",conv16(v))

#define ADDBYTE(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA(ODT_I,"%u,",(v))

#define ADDTIME(h,v) \
  convDate(v,dt); \
  sprintf(tmpHead,"%s (DATE)",h); \
  ADDHEAD(tmpHead,DDL_DATE,0); \
  ADDDATA(ODT_C,"%s,",dt[0]);\
  sprintf(tmpHead,"%s (TIME)",h); \
  ADDHEAD(tmpHead,DDL_TIME,0); \
  ADDDATA(ODT_C,"%s,",dt[1])

#define ADDTIMEIDX(h,idx,v) \
  convDate(v,dt); \
  sprintf(tmpHead,"%s{%s} (DATE)",h,idx); \
  ADDHEADIDX(tmpHead,DDL_DATE,0); \
  ADDDATA(ODT_C,"%s,",dt[0]); \
  sprintf(tmpHead,"%s{%s} (TIME)",h,idx); \
  ADDHEADIDX(tmpHead,DDL_TIME,0); \
  ADDDATA(ODT_C,"%s,",dt[1])

#define ADDSTCK(h,v) \
  if (outputFormat == OF_CSV) {\
    sprintf(tmpHead,"%s(S)",h); \
    ADDHEAD(tmpHead,DDL_C,0); \
    sprintf(tmpHead,"%s(US)",h); \
    ADDHEAD(tmpHead,DDL_C,0); \
  } else{ \
   sprintf(tmpHead,"%s(US)",h); \
    ADDHEAD(tmpHead,DDL_SUS,0); \
  } \
  ADDDATA(ODT_USEC,"%s,",convSecUSec(v))

#define ADDSTCKIDX(h,idx,v) \
  if (outputFormat != OF_CSV) \
  sprintf(tmpHead,"%s{%s}(US)",h,idx); \
  else \
  sprintf(tmpHead,"%s{%s}(S),%s{%s}(US)",h,idx,h,idx); \
  ADDHEADIDX(tmpHead,DDL_SUS,0); \
  ADDDATA(ODT_USEC,"%s,",convSecUSec(v))

/* Add ASCII string, known length - underpins the other ADDSTRxx macros */
#define ADDSTRN(h,v,l,maxlen) \
   ADDHEAD(h,DDL_C,maxlen); \
   smfAddString(l,v)

#define ADDSTR(h,v,maxlen) \
  ADDSTRN(h,v,strlen(v),maxlen)             /* ASCII string null terminated*/

#define ADDSTRB(h,v,len) \
  ADDSTR(h,convBin(v,len),len*2+1)                         /* Binary string*/

#define ADDSTREN(h,v,l) \
  ADDSTRN(h,convStr(v,l),l,l)                /* EBCDIC string, known length*/

#define ADDSTRENIDX(h,idx, v,l) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEADIDX(tmpHead,DDL_C,l);\
  smfAddString(l,convStr(v,l))               /* EBCDIC string, known length*/

#define ADDINDEX(n) \
  if (first) { \
    addIndex(n); \
  }

/*****************************************/
/* This isn't currently used. But it was */
/* useful during initial debug.          */
/*****************************************/
#define CHECKEYE(ptr,e) \
   \
    if (ptr && memcmp(e,convStr(ptr,4),4)) \
    { \
      printf("Bad eyecatcher for %s\n",e); \
      exit(1); \
    } \
  };

/*****************************************/
/* Global variables - defined in mqsmf.c */
/*****************************************/
extern unsigned char  EBCDIC_TO_ASCII[];
extern int  debugLevel;
extern BOOL  addEquals;
extern BOOL  printHeaders;
extern unsigned int   recordType;
extern unsigned short recordSubType;
extern commonFields_t commonF;
extern char tmpHead[];
extern FILE *infoStream;
extern char *ddlTemplateOpen;
extern char *ddlTemplateClose;
extern char *ddlQuote;
extern BOOL  ddlFold;

enum outputFormat_e { OF_CSV=0, OF_SQL=1, OF_JSON=2 };
extern enum outputFormat_e outputFormat;

#endif
