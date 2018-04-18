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

/**********************************************************************/
/*                                                                    */
/*   IBM MQ                                                           */
/*                                                                    */
/*   Module Name : smfDate.c                                          */
/*                                                                    */
/*   Description : Functions to handle formatting dates and times.    */
/*                 In particular, taking STCK and converting it to    */
/*                 a usable format.                                   */
/**********************************************************************/

#include <stdio.h>
#include <time.h>
#include "mqsmf.h"


/*********************************************************/
/* How many days normally in each month - leap year gets */
/* handled explicitly.                                   */
/*********************************************************/
static int days[12] =
{
  31, 28, 31, 30,
  31, 30, 31, 31,
  30, 31, 30, 31
};

/********************************************************************/
/* Useful values come from the z Principles of Operation manual     */
/********************************************************************/
#define STCK1972  0x8126D60E46000000ull  /* Value at 1972/01/01     */
#define STCKYR365 0x01CAE8C13E000000ull  /* How many ticks per year */

/********************************************************************/
/* For Unix and Windows the key date, however, is 1970. So work     */
/* out the STCK time that corresponds to.                           */
/********************************************************************/
static unsigned long long int EPOCH1970 = (STCK1972 - (2*STCKYR365));

/********************************************************************/
/* Formatters for strftime and some fixed styles depending on       */
/* output format.                                                   */
/********************************************************************/
struct timeFormats_s {
  char *fmtDate;
  char *fmtTime;
  char *epochDate;
  char *epochTime;
  char *emptyDate;
  char *emptyTime;
  char *decimal;
} timeFormats[OF_JSON+1] =
 {
   { /* CSV */
     "\"%Y/%m/%d\"",
     "\"%H:%M:%S",    /* no closing quote on this as it's added later */
     "\"1970/01/01\"",
     "\"00:00:00,000000\"",
     "\"\"",
     "\"\"",
     ","
   },
   { /* SQL */
     "\"%Y-%m-%d\"",
     "\"%H:%M:%S",    /* no closing quote on this as it's added later */
     "\"1970-01-01\"",
     "\"00:00:00,000000\"",
     "\"1970-01-01\"",
     "\"00:00:00,000000\"",
     ","
   },
   { /* JSON - no embedded quotes in the strings */
     "%Y-%m-%d",
     "%H:%M:%S",
     "1970-01-01",
     "00:00:00.000000",
     "1970-01-01",
     "00:00:00.000000",
     "."
   }
 };
static char stckDate[64] = {0};
static char stckTime[64] = {0};

/********************************************************************/
/* FUNCTION: convDate                                               */
/* DESCRIPTION:                                                     */
/*    Convert a STCK value into a formatted timestamp suitable      */
/*    for the printing. Timestamp is returned in a fixed buffer.    */
/*    This routine does not cope with dates before 1970, but        */
/*    that's not likely to be a concern for processing MQ SMF.      */
/********************************************************************/
void convDate(unsigned long long stcki, char *dt[2])
{
  time_t sec;
  unsigned long long stck;
  unsigned int usec;
  unsigned long long int s;
  struct tm *t;
  size_t offset1;
  size_t offset2;
  struct timeFormats_s tf;

  tf = timeFormats[outputFormat];

  stck = conv64(stcki);   /* Always passed in z/OS endian; ensure converted */

  if (stck == 0 )
  {
    /***********************************************************************/
    /* Put a hardcoded value                                               */
    /***********************************************************************/
    strcpy(stckTime,tf.emptyTime);
    strcpy(stckDate,tf.emptyDate);
  }
  else
  {
  s = stck - EPOCH1970;  /* Make relative to our epoch instead of z/OS 1900*/
  s = s / 4096;                                /* Convert stck to microsecs*/
  sec  = (time_t)(s / 1000000);                     /* Split into two parts*/
  usec = s % 1000000;

  t = localtime(&sec);                 /* Turn seconds into tm structure...*/

  offset1 = strftime(stckDate,sizeof(stckDate)-1,tf.fmtDate,t); /* ...and format it*/
  offset2 = strftime(stckTime,sizeof(stckTime)-1,tf.fmtTime,t); /* ...and format it*/
  if (offset1 == 0 || offset2 == 0 || stck < EPOCH1970)
  {
    /***********************************************************************/
    /* There seem to be occasions where the timestamp is not a proper      */
    /* value - perhaps when there has been no work done for the activity   */
    /* being recorded. So we put a hardcoded value into the string to make */
    /* those times (MQ bugs?) easy to recognise.                           */
    /***********************************************************************/
    strcpy(stckTime,tf.epochTime);
    strcpy(stckDate,tf.epochDate);
  }
  else
    sprintf(&stckTime[offset2],"%s%6.6d%s",tf.decimal,usec,(outputFormat==OF_JSON)?"":"\"");          /* Add on usec value*/
  }
  dt[0] = stckDate;
  dt[1] = stckTime;
  return;
}

/********************************************************************/
/* FUNCTION: calcYMD                                                */
/* DESCRIPTION:                                                     */
/*    Given a dayOfYear value (1-366), convert it to the month      */
/*    (1-12) and day (1-31) values which are returned via ptrs.     */
/********************************************************************/
void calcYMD(int year,int dayOfYear,int *mon,int *day)
{
  int isLeapYear = FALSE;
  int i;

  if (((year % 4    == 0) &&
      (year % 100  != 0)) ||
      (year % 400 == 0))
    isLeapYear = TRUE;

  if (isLeapYear)
    days[1] = 29;                                        /* Update Feb*/
  else
    days[1] = 28;

  for (i=0;i<12;i++)
  {
    if(dayOfYear <= days[i])
    {
      *mon = i+1;                         /* Jan is month 1, not 0 */
      *day = dayOfYear;
      break;
    }
    else
    {
      dayOfYear -= days[i];
    }
  }

  return;
}

