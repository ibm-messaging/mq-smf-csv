/*
 * Copyright (c) 2018 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Mark Taylor - Initial Contribution
 */

/**********************************************************************/
/*                                                                    */
/*   IBM MQ                                                           */
/*                                                                    */
/*   Module Name : smfPrint.c                                         */
/*                                                                    */
/*   Build a line of data and associated column headers before        */
/*   printing it. Each line is built completely for a record, so      */
/*   "global" state in the function is needed only for the duration   */
/*   between smfPrintStart and smfPrintStop                           */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include "mqsmf.h"

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
#define HEADINGS_LEN (20000) /* Total for all columns     */
#define DATA_LEN     (20000)

int offsetH=0;
int offsetD=0;
char *tableName = NULL;

columnHeader_t **columnHeadersTmp;
int   columnCount = 0;

char tmpHead[64];
char headings[HEADINGS_LEN];    /* Ensure these are big enough for any line*/
static char dataline[DATA_LEN];

void smfAddHead(BOOL first,BOOL idx, char *h,int type,int len)
{
  columnHeader_t *ch;
  if (first && h)
  {
    /******************************************************************/
    /* For JSON output, we need to take a copy of the column heading  */
    /* modified to a suitable style. Other output formats do not      */
    /* need that copy.                                                */
    /******************************************************************/
    switch (outputFormat)
    {
    case OF_JSON:
      ch = jsonFormatHeader(idx,h);
      columnHeadersTmp[columnCount++] = ch;

      if (columnCount >= HEADINGS_COUNT)
      {
        fprintf(stderr,"Column headings buffer needs to be bigger than %d.\n",HEADINGS_COUNT);
        exit(1);
      }
      break;
    case OF_SQL:
      printDDL(h,type,len); /* Drop through */
    default:
      offsetH += snprintf(&headings[offsetH],HEADINGS_LEN - offsetH, "%s,", h);
      if (offsetH > HEADINGS_LEN)
      {
        fprintf(stderr,"HEADINGS buffer needs to be bigger than %d bytes.\n",HEADINGS_LEN);
        exit(1);
      }
      break;
    }
  }
}

BOOL localFirst;
void smfAddData(int datatype,char *fmt,...)
{
  long long ll;
  long      l;
  char *c;
  columnHeader_t *    h;

  va_list args;
  va_start(args,fmt);

  switch (outputFormat)
  {
  case OF_CSV:
  case OF_SQL:
    offsetD+=vsnprintf(&dataline[offsetD],DATA_LEN-offsetD,fmt,args);
    break;
  case OF_JSON:
    if (localFirst)
      columnCount--;

    h = columnHeadersTmp[columnCount];
    switch (datatype)
    {
    case ODT_I:
      l = va_arg(args,int);
      jsonAddInt(h,l);
      break;
    case ODT_I64:
      ll = va_arg(args,long long);
      jsonAddInt64(h,ll);
      break;
    case ODT_C:
      c  = va_arg(args,char *);
      jsonAddString(h,c,strlen(c));
      break;
    case ODT_USEC:
      c  = va_arg(args,char *);
      jsonAddNumericString(h,c,strlen(c));
      break;
    case ODT_FIXEDC:
      c = va_arg(args,char*);
      l = va_arg(args,int);
      l = va_arg(args,int);
      c = va_arg(args,char*);
      jsonAddString(h,c,l);
      break;
    default:
      fprintf(stderr,"Found unknown datatype %d for \"%s\"\n",datatype,h->name);
      break;
    }
    columnCount++;
    break;
  default:
    fprintf(stderr,"Unknown output format type %d\n",outputFormat);
    break;
  }

  va_end(args);

  if (offsetD > DATA_LEN)
  {
      fprintf(stderr,"DATA buffer needs to be bigger than %d bytes.\n",DATA_LEN);
      exit(1);
  }
}

void smfAddString(int l,char *v)
{
  char *equ=""; \
  if (looksLikeNum(l,v) && addEquals)
  {
    equ="=";
  }
  ADDDATA(ODT_FIXEDC,"%s\"%-*.*s\",",equ,l,l,v);
}

FILE *smfPrintStart(FILE *fp, char *name, void *p, size_t l, BOOL *f, BOOL *newFile,columnHeader_t **h)
{
  FILE *fpNew = fp;
  BOOL first = *f;
  int i;

  offsetH=0;
  offsetD=0;
  columnCount=0;
  tableName = name;
  columnHeadersTmp = h;
  localFirst = first;
  /* Dump the entire record in hex */
  if (debugLevel >=1 )
  {
    printDEBUG(name,p,(l));
  }

  if (first) {
    switch (outputFormat)
    {
    case OF_CSV:
    case OF_SQL:
      fpNew = fopencsv(name,newFile);
      if (outputFormat == OF_SQL)
        openDDL(name);

      if (!fpNew)
      {
        exit(1);
      }
      break;
    case OF_JSON:
      /* Only want a single output file for all the JSON */
      if (!fpJson)
      {
        fpJson = fopenext("MQSMF","json",newFile);
        if (!fpJson)
        {
          exit(1);
        }
      }
      fpNew = fpJson;
      break;
    }
    for(i=0;i<HEADINGS_COUNT;i++) {
      h[i] = 0;
    }
  }
  if (outputFormat == OF_JSON)
    jsonNew(fpNew,name);

  ADDSTR ("Date",commonF.recordDate,8);
  ADDSTR ("Time",commonF.recordTime,16);
  ADDSTRN("LPAR",commonF.systemId,4,4);
  if (recordType == SMFTYPE_MQ_STAT || recordType == SMFTYPE_MQ_ACCT)
  {
    ADDSTRN("QMgr",commonF.qMgr,4,4);
    ADDSTRN("MQ_Version",commonF.mqVer,3,3);

    if (recordType == SMFTYPE_MQ_STAT && commonF.intstart != 0)
    {
      char *dt[2];
      unsigned long long du = conv64(commonF.intduration)/1000000L;
      ADDTIME("Interval_Start",commonF.intstart);
      ADDHEAD("Interval_Duration",DDL_I,0);
      /* Not documented, but this subtype uses a different scale for */
      /* measuring the duration.                                     */
      if (recordSubType == 231)
      {
        ADDDATA(ODT_I64,"%llu,",du/4096L);
      } else {
        ADDDATA(ODT_I64,"%llu,",du);
      }
    }
  } else {
    ADDSTRN("SubsystemID",commonF.qMgr,4,4);
  }
  *f = first;

  return fpNew;
}

void smfPrintStop(FILE *fp, BOOL newFile, BOOL *first, columnHeader_t **h)
{
  if (*first && newFile && printHeaders && outputFormat != OF_JSON)
  {
    fprintf(fp,"%s",headings);
    fprintf(fp,"\n");
  }

  if(*first && (outputFormat == OF_SQL))
  {
    closeDDL(tableName);
  }

  *first=FALSE;

  switch (outputFormat)
  {
  case OF_CSV:
  case OF_SQL:
    fprintf(fp,"%s",dataline);
    fprintf(fp,"\n");
    break;
  case OF_JSON:
    jsonDump(fp,h);
    break;
  }
}
