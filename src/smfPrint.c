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


int offsetH=0;
int offsetD=0;
char *tableName = NULL;

char headings[HEADINGS_LEN];    /* Ensure these are big enough for any line*/
char dataline[DATA_LEN];
char tmpHead[64];                     /* Working space for a column heading*/

void smfAddHead(BOOL first,char *h,int type,int len)
{
  if (first && h)
  {
    offsetH += snprintf(&headings[offsetH],HEADINGS_LEN - offsetH, "%s,", h);
    if (offsetH > HEADINGS_LEN)
    {
      printf("HEADINGS buffer needs to be bigger than %d bytes.\n",HEADINGS_LEN);
      exit(1);
    }

    if (outputFormat == OF_SQL)
    {
      printDDL(h,type,len);
    }
  }
}

void smfAddData(char *fmt,...)
{
  va_list args;
  va_start(args,fmt);
  offsetD+=vsnprintf(&dataline[offsetD],DATA_LEN-offsetD,fmt,args);
  va_end(args);

  if (offsetD > DATA_LEN)
  {
      printf("DATA buffer needs to be bigger than %d bytes.\n",DATA_LEN);
      exit(1);
  }
}

FILE *smfPrintStart(char *name, void *p, size_t l, BOOL *f, BOOL *newFile)
{
  FILE *fp;
  BOOL first = *f;

  offsetH=0;
  offsetD=0;
  tableName = name;

  /* Dump the entire record in hex */
  if (debugLevel >=1 )
  {
    printDEBUG(name,p,(l));
  }

  if (first) {
    fp = fopencsv(name,newFile);
    if (outputFormat == OF_SQL)
      openDDL(name);
  }
  if (!fp)
  {
    exit(1);
  }

  ADDSTR ("Date",commonF.recordDate,8);
  ADDSTR ("Time",commonF.recordTime,16);
  ADDSTRN("LPAR",commonF.systemId,4,4);
  ADDSTRN("QMgr",commonF.qMgr,4,4);
  ADDSTRN("MQ_Version",commonF.mqVer,3,3)

  if (recordType == 115 && commonF.intstart != 0) {
    unsigned long long du = conv64(commonF.intduration)/1000000L;
    ADDTIME("Interval_Start",commonF.intstart);
    ADDHEAD("Interval_Duration",DDL_I,0);
    if (recordSubType == 231) {
      ADDDATA("%llu,",du/4096L);
    } else {
      ADDDATA("%llu,",du);
    }
  }
  *f = first;

  return fp;
}

void smfPrintStop(FILE *fp, BOOL newFile, BOOL *first)
{
  if (*first && newFile && printHeaders)
  {
    fprintf(fp,"%s",headings);
    fprintf(fp,"\n");
  }

  if(*first && (outputFormat == OF_SQL))
  {
    closeDDL(tableName);
  }

  *first=FALSE;

  fprintf(fp,"%s",dataline);
  fprintf(fp,"\n");
}
