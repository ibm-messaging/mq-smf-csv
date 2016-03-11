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
/* This formatter for strftime                                      */
/********************************************************************/
static char *fmt = "\"%Y/%m/%d\",\"%H:%M:%S";
static char stckTime[64] = {0};

/********************************************************************/
/* FUNCTION: convDate                                               */
/* DESCRIPTION:                                                     */
/*    Convert a STCK value into a formatted timestamp suitable      */
/*    for the CSV printing. Timestamp is returned in a fixed buffer.*/
/*    This routine does not cope with dates before 1970, but        */
/*    that's not likely to be a concern for processing MQ SMF.      */
/********************************************************************/
char * convDate(unsigned long long stck)
{
  time_t sec;
  unsigned int usec;
  unsigned long long int s;
  struct tm *t;
  size_t offset;

  stck = conv64(stck);   /* Always passed in z/OS endian; ensure converted */

  s = stck - EPOCH1970;  /* Make relative to our epoch instead of z/OS 1900*/
  s = s / 4096;                                /* Convert stck to microsecs*/
  sec  = (time_t)(s / 1000000);                     /* Split into two parts*/
  usec = s % 1000000;

  t = localtime(&sec);                 /* Turn seconds into tm structure...*/

  offset = strftime(stckTime,sizeof(stckTime)-1,fmt,t); /* ...and format it*/
  sprintf(&stckTime[offset],",%6.6d\"",usec);          /* Add on usec value*/

  return stckTime;
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

  if ((year % 4    == 0) &&
      (year % 100  != 0) &&
      (year % 400 == 0))
    isLeapYear = TRUE;

  if (isLeapYear)
    days[1]++;                                        /* Update Feb*/

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
