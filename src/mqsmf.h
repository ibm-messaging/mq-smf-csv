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
  char stckFormat[64];                  /* STCK as string date/time */
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
extern void closeDDL   (void);
extern void printDDL   (char *,int, int);


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
extern char  *convDate(unsigned long long s);  /* Convert stck to date/time*/
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
/* How long are the lines for data and headings.          */
/* These are fixed, and need to be long enough for        */
/* the contents. We do check whether data has overflowed  */
/* and abend if it has. It's not ideal but works well-    */
/* enough. One table (WQ) has a lot of columns because    */
/* of nested arrays, and it generates >10K chars for its  */
/* headings - hence picking numbers here that are much    */
/* more than that.                                        */
/**********************************************************/
#define HEADINGS_LEN (20000)
#define DATA_LEN     (20000)

/**********************************************************/
/* Datatypes that allow creation of SQL DDL statements    */
/**********************************************************/
#define DDL_I        (1)
#define DDL_I64      (2)
#define DDL_C        (3)
#define DDL_SUS      (4)
#define DDL_DATETIME (5)

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
#define ADDHEAD(h,type,len) \
  if (first && h)  \
  {                \
    offsetH += snprintf(&headings[offsetH],HEADINGS_LEN - offsetH, "%s,", h); \
    if (offsetH > HEADINGS_LEN) \
    {                               \
      printf("HEADINGS buffer needs to be bigger than %d bytes.\n",HEADINGS_LEN); \
      exit(1);                      \
    }                               \
    if (sqlMode) \
    printDDL(h,type,len); \
  }

#define ADDDATA(fmt,...)  \
  offsetD+=snprintf(&dataline[offsetD],DATA_LEN-offsetD,fmt,__VA_ARGS__); \
  if (offsetD > DATA_LEN)                \
  {                                      \
      printf("DATA buffer needs to be bigger than %d bytes.\n",DATA_LEN); \
      exit(1);                           \
  }

#define ADDS64(h,v) \
  ADDHEAD(h,DDL_I64,0); \
  ADDDATA("%lld,",conv64(v))

#define ADDU64(h,v) \
  ADDHEAD(h,DDL_I64,0); \
  ADDDATA("%llu,",conv64(v))

#define ADDS64IDX(h,idx,v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEAD(tmpHead,DDL_I64,0); \
  ADDDATA("%lld,",conv64(v))

#define ADDU64IDX(h,idx,v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEAD(tmpHead,DDL_I64,0); \
  ADDDATA("%llu,",conv64(v))

#define ADDS32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%d,",conv32(v))

#define ADDU32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%u,",conv32(v))

#define ADDX32(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%X,",conv32(v))

#define ADDS32IDX(h,idx, v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEAD(tmpHead,DDL_I,0); \
  ADDDATA("%d,",conv32(v))

#define ADDU32IDX(h,idx, v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDHEAD(tmpHead,DDL_I,0); \
  ADDDATA("%u,",conv32(v))

#define ADDS16(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%hd,",conv16(v))

#define ADDU16(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%hu,",conv16(v))

#define ADDBYTE(h,v) \
  ADDHEAD(h,DDL_I,0); \
  ADDDATA("%u,",(v))

#define ADDTIME(h,v) \
  sprintf(tmpHead,"%s (DATE),%s (TIME)",h,h); \
  ADDHEAD(tmpHead,DDL_DATETIME,0); \
  ADDDATA("%s,",convDate(v))

#define ADDTIMEIDX(h,idx,v) \
  sprintf(tmpHead,"%s{%s} (DATE),%s{%s} (TIME)",h,idx,h,idx); \
  ADDHEAD(tmpHead,DDL_DATETIME,0); \
  ADDDATA("%s,",convDate(v))

#define ADDSTCK(h,v) \
  sprintf(tmpHead,"%s(S),%s(US)",h,h); \
  ADDHEAD(tmpHead,DDL_SUS,0); \
  ADDDATA("%s,",convSecUSec(v))

#define ADDSTCKIDX(h,idx,v) \
  sprintf(tmpHead,"%s{%s}(S),%s{%s}(US)",h,idx,h,idx); \
  ADDHEAD(tmpHead,DDL_SUS,0); \
  ADDDATA("%s,",convSecUSec(v))

/* Add ASCII string, known length - underpins the other ADDSTRxx macros */
#define ADDSTRN(h,v,l,maxlen) \
  ADDHEAD(h,DDL_C,maxlen); \
  { char *equ=""; \
  if (looksLikeNum(l,v) && addEquals) equ="="; \
  ADDDATA("%s\"%-*.*s\",",equ,l,l,v) \
  }

#define ADDSTR(h,v,maxlen) \
  ADDSTRN(h,v,strlen(v),maxlen)             /* ASCII string null terminated*/

#define ADDSTRB(h,v,len) \
  ADDSTR(h,convBin(v,len),len*2+1)                         /* Binary string*/

#define ADDSTRIDX(h,idx, v) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDSTRN(tmpHead,v,strlen(v),)

#define ADDSTREN(h,v,l) \
  ADDSTRN(h,convStr(v,l),l,l)                /* EBCDIC string, known length*/

#define ADDSTRENIDX(h,idx, v,l) \
  if (first) sprintf(tmpHead,"%s {%s}",h,idx); \
  ADDSTREN(tmpHead,v,l)                      /* EBCDIC string, known length*/

#define COMMON_BLOCK \
  ADDSTR ("Date",commonF.recordDate,8); \
  ADDSTR ("Time",commonF.recordTime,16); \
  ADDSTRN("LPAR",commonF.systemId,4,4); \
  ADDSTRN("QMgr",commonF.qMgr,4,4); \
  ADDSTRN("MQ_Version",commonF.mqVer,3,3) \
  if (recordType == 115 && commonF.intstart != 0) { \
    unsigned long long du = conv64(commonF.intduration)/1000000L; \
    ADDTIME("Interval_Start",commonF.intstart) \
    ADDHEAD("Interval_Duration",DDL_I,0); \
    if (recordSubType == 231) { \
    ADDDATA("%llu,",du/4096L); \
    } else { \
    ADDDATA("%llu,",du); \
    } \
  }

#define SMFPRINTSTART(n,p,l) \
  int offsetH=0;  \
  int offsetD=0;  \
  if (debugLevel >=1 ) printDEBUG(n,p,(l));\
  if (first) {    \
    fp = fopencsv(n,&newFile);\
    if (sqlMode) \
      openDDL(n);   \
  } \
  if (!fp)        \
  {               \
    exit(1);      \
  } \
  COMMON_BLOCK

#define SMFPRINTSTOP \
  if (first && newFile && printHeaders)             \
  {                      \
    fprintf(fp,"%s",headings);\
    fprintf(fp,"\n");    \
  }                      \
  if(first && sqlMode) \
    closeDDL(); \
  first=FALSE;         \
  fprintf(fp,"%s",dataline);\
  fprintf(fp,"\n")

#define SMFPRINTGLOB \
  static BOOL first = TRUE;\
  static BOOL newFile = TRUE;\
  static FILE *fp = NULL

/*****************************************/
/* This isn't currently used. But it was */
/* useful during initial debug.          */
/*****************************************/
#define CHECKEYE(ptr,e) \
  { \
    if (ptr && memcmp(e,convStr(ptr,4),4)) \
    { \
      printf("Bad eyecatcher for %s\n",e); \
      exit(1); \
    } \
  };

/*****************************************/
/* Global variables - defined in mqsmf.c */
/*****************************************/
extern char headings[];
extern char tmpHead[];
extern char dataline[];
extern unsigned char  EBCDIC_TO_ASCII[];
extern int  debugLevel;
extern BOOL  addEquals;
extern BOOL  printHeaders;
extern BOOL  sqlMode;
extern unsigned int   recordType;
extern unsigned short recordSubType;
extern commonFields_t commonF;

#endif
