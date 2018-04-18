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
/*   Module Name : smfConv.c                                          */
/*                                                                    */
/*   Description : Various utility functions to convert data from     */
/*                 one format to another including                    */
/*                   big/little-endian                                */
/*                   EBCDIC/ASCII characters                          */
/*                   bytes to printable hex string                    */
/**********************************************************************/

#include <stdio.h>
#include <ctype.h>
#include "mqsmf.h"

extern void *realloc(void *,size_t);

/****************************************************************/
/* This is a basic, common subset, converter for EBCDIC values. */
/* It's good enough for strings such as MQ object names.        */
/****************************************************************/
unsigned char EBCDIC_TO_ASCII[256] =
{
  '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
  '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
  '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
  '.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
  ' ','.','.','.','.','.','.','.','.','.','$','.','<','(','+','|',
  '&','.','.','.','.','.','.','.','.','.','!','$','*',')',';','.',
  '-','/','.','.','.','.','.','.','.','.','.',',','%','_','>','?',
  '.','.','.','.','.','.','.','.','.','.',':','#','@','\'','=','.',
  '.','a','b','c','d','e','f','g','h','i','.','{','.','(','+','.',
  '.','j','k','l','m','n','o','p','q','r','.','}','.',')','.','.',
  '.','~','s','t','u','v','w','x','y','z','.','.','.','.','.','.',
  '.','.','.','.','.','.','.','.','.','.','[',']','.','.','.','-',
  '{','A','B','C','D','E','F','G','H','I','.','.','.','.','.','.',
  '}','J','K','L','M','N','O','P','Q','R','.','.','.','.','.','.',
  '.','.','S','T','U','V','W','X','Y','Z','.','.','.','.','.','.',
  '0','1','2','3','4','5','6','7','8','9','.','.','.','.','.','.'
};

#define BIG      1
#define LITTLE   2

int endian = 0;
int bitReverse = FALSE;

/**********************************************************************/
/* FUNCTION: convInit                                                 */
/* DESCRIPTION:                                                       */
/*   Decide whether this is big or little-endian process              */
/**********************************************************************/
void convInit()
{
  unsigned int TEST = 0x11223344;
  unsigned char c;
  c = ((char *)&TEST)[0];

  if (c == 0x11)
  {
    endian = BIG;
  }
  else if (c == 0x44)
  {
    fprintf(infoStream,"Swapping bytes in input records\n");
    endian = LITTLE;
  }
  else
  {
    fprintf(infoStream,"Can't work out endianness\n");
  }

  return;
}

/**********************************************************************/
/* FUNCTION: convStr                                                  */
/* DESCRIPTION:                                                       */
/*   Convert a set of EBCDIC bytes into a null-terminated ASCII       */
/*   string. A local buffer is allocated that is big enough (and      */
/*   reallocated if too small) for the given length. Pointer to the   */
/*   buffer is returned which means the caller must use it before     */
/*   calling again into this function.                                */
/**********************************************************************/
static char *convStrBuf =  NULL;
static int   convStrMaxLen = 0;
char *convStr(unsigned char *buf, int len)
{
  int i;

  if (len > convStrMaxLen)
  {
    convStrBuf = (char *)realloc(convStrBuf,len+2);
    convStrMaxLen = len;
  }

  for (i=0;i<len;i++)
  {
    convStrBuf[i] = EBCDIC_TO_ASCII[buf[i]];
  }
  convStrBuf[i]=0;
  return convStrBuf;

}

/**********************************************************************/
/* FUNCTION: convBin                                                  */
/* DESCRIPTION:                                                       */
/*   Convert a set of bytes into a null-terminated string containing  */
/*   the hex values. A local buffer is allocated that is big enough   */
/*   (and reallocated if too small) for the given length. Ptr to the  */
/*   buffer is returned which means the caller must use it before     */
/*   calling again into this function.                                */
/**********************************************************************/
static char *convBinBuf =  NULL;
static int   convBinMaxLen = 0;
char *convBin(unsigned char *inbuf, int origlen)
{
  int i;
  int len = 2*origlen;

  if (len > convBinMaxLen)
  {
    convBinBuf = (char *)realloc(convBinBuf,len+1);
    convBinMaxLen = len;
  }

  for (i=0;i<origlen;i++)
    sprintf(&convBinBuf[i*2],"%02X",inbuf[i]);
  convBinBuf[len] = 0;
  return convBinBuf;
}

/**********************************************************************/
/* FUNCTION: convSecUSec                                              */
/* DESCRIPTION:                                                       */
/*   Convert a STCK value into a string showing duration in seconds   */
/*   and microseconds. The two numbers are comma-separated for use in */
/*   the CSV output as some spreadsheets had problems with 64-bit     */
/*   values. But in SQL mode, just print a single number as databases */
/*   are able to handle a BIGINT datatype.                            */
/*   The input STCK is assumed to be in z/OS                          */
/*   big-endian format, so must be converted to local endianness first*/
/**********************************************************************/
static char usecBuf[64];
char *convSecUSec(unsigned long long s)
{
  int sec = 0;
  int usec = 0;

  s = conv64(s);

  if (s > 0)
  {
    s = s / 4096;                          /* Convert stck to microsecs*/
    sec  = s / 1000000;
    usec = s % 1000000;
  }

  /* Separate fields by ',' for CSV formats */
  switch (outputFormat)
  {
  case OF_SQL:
    sprintf(usecBuf,"%llu ",s);
    break;
  case OF_JSON:
    sprintf(usecBuf,"%llu",s);
    break;
  default:
    sprintf(usecBuf,"%u, %u ",sec,usec);
    break;
  }

  return usecBuf;
}



/**********************************************************************/
/* Set of functions to do the byte-swapping if necessary. Do not need */
/* these on Windows as there are efficient macros that we can use     */
/* instead.                                                           */
/**********************************************************************/
#ifndef _WIN32
unsigned short conv16(const unsigned short i)
{
  if (endian == BIG)
    return i;
  else
  {
    union {
    unsigned short l;
    unsigned char  c[2];
    } Uin,Uout;
    Uin.l = i;
    Uout.c[0] = Uin.c[1];
    Uout.c[1] = Uin.c[0];
    return Uout.l;
  }
}

unsigned int   conv32(const unsigned int   i)
{
  if (endian == BIG)
    return i;
  else
  {
    union {
    unsigned int   l;
    unsigned char  c[4];
    } Uin,Uout;
    Uin.l = i;
    Uout.c[0] = Uin.c[3];
    Uout.c[1] = Uin.c[2];
    Uout.c[2] = Uin.c[1];
    Uout.c[3] = Uin.c[0];
    return Uout.l;
  }
}

unsigned long long conv64(const unsigned long long i)
{
  if (endian == BIG)
    return i;
  else
  {
    union {
    unsigned long long l;
    unsigned char  c[8];
    } Uin,Uout;
    Uin.l = i;
    Uout.c[0] = Uin.c[7];
    Uout.c[1] = Uin.c[6];
    Uout.c[2] = Uin.c[5];
    Uout.c[3] = Uin.c[4];
    Uout.c[4] = Uin.c[3];
    Uout.c[5] = Uin.c[2];
    Uout.c[6] = Uin.c[1];
    Uout.c[7] = Uin.c[0];
    return Uout.l;
  }
}
#endif

/**********************************************************************/
/* FUNCTION: looksLikeNum                                             */
/* DESCRIPTION:                                                       */
/*   Returns 1 if all characters in a string are hex digits or space  */
/*   Otherwise return 0.                                              */
/**********************************************************************/
int looksLikeNum(int l, char *c)
{
  int rc = 1;
  int i;
  for (i=0;i<l;i++)
  {
    if (!isxdigit(c[i]) && !isspace(c[i]))
      return 0;
  }
  return rc;
}


/**********************************************************************/
/* FUNCTIONS: strXXXX                                                 */
/* DESCRIPTION: Convert an MQI value into a printable string.         */
/*   I could have used cmqstrc.h for most of these, but it's a small  */
/*   enough set that it was simpler to use these and do the basic     */
/*   readability changes (eg mixed case) in here.                     */
/**********************************************************************/
char *strConnType(int v)
{
  char *c="";
  switch (v)
  {
  case 1: c = "CICS"    ;break;
  case 2: c = "TSO "    ;break;
  case 3: c = "IMS Ctl ";break;
  case 4: c = "IMS MPP/BMP";break;
  case 5: c = "Cmd Svr" ;break;
  case 6: c = "CHIN"    ;break;
  case 7: c = "RRS "    ;break;
  }
  return c;
}


char *strMQIT (int v)
{
  char *c;
  switch (v)
  {
  case    0: c = "None"; break;
  case    1: c = "MsgId"; break;
  case    2: c = "CorrelId"; break;
  case    4: c = "MsgToken"; break;
  case    5: c = "GroupId"; break;
  default: c = ""; break;
  }
  return c;
}

char *strMQQT (int v)
{
  char *c;
  switch (v)
  {
  case     1: c = "Local"; break;
  case     2: c = "Model"; break;
  case     3: c = "Alias"; break;
  case     6: c = "Remote"; break;
  case     7: c = "Cluster"; break;
  case     8: c = "Topic"; break; /* not really a qtype but unique MQOT value */
  case  1001: c = "All"; break;
  default: c = ""; break;
  }
  return c;
}

char *strMQQSGD  (int v)
{
  char *c;
  switch (v)
  {
  case   -1: c = "All"; break;
  case    0: c = "QMgr"; break;
  case    1: c = "Copy"; break;
  case    2: c = "Shared"; break;
  case    3: c = "Group"; break;
  case    4: c = "Private"; break;
  case    6: c = "Live"; break;
  default: c = ""; break;
  }
  return c;
}


char *strMQMON (int v)
{
   char *c;
   switch (v)
   {
   case         -3: c = "QMgr"; break;
   case         -1: c = "None"; break;
   case          0: c = "Off"; break;
   case          1: c = "On"; break;
   case         17: c = "Low"; break;
   case         33: c = "Medium"; break;
   case         65: c = "High"; break;
   default: c = ""; break;
   }
   return c;
}

char *strMQCHT (int v)
{
   char *c;
   switch (v)
   {
   case          1: c = "Sender"; break;
   case          2: c = "Server"; break;
   case          3: c = "Receiver"; break;
   case          4: c = "Requester"; break;
   case          5: c = "All"; break;
   case          6: c = "ClntConn"; break;
   case          7: c = "SvrConn"; break;
   case          8: c = "ClusRcvr"; break;
   case          9: c = "ClusSdr"; break;
   case         10: c = "MQTT"; break;
   case         11: c = "AMQP"; break;
   default: c = ""; break;
   }
   return c;
}

char *strMQCHS  (int v)
{
   char *c;
   switch (v)
   {
   case          0: c = "Inactive"; break;
   case          1: c = "Binding"; break;
   case          2: c = "Starting"; break;
   case          3: c = "Running"; break;
   case          4: c = "Stopping"; break;
   case          5: c = "Retrying"; break;
   case          6: c = "Stopped"; break;
   case          7: c = "Requesting"; break;
   case          8: c = "Paused"; break;
   case          9: c = "Disconnected"; break;
   case         13: c = "Initializing"; break;
   case         14: c = "Switching"; break;
   default: c = ""; break;
   }
   return c;
}

char *strMQCHLD  (int v)
{
   char *c;
   switch (v)
   {
   case         -1: c = "All"; break;
   case          1: c = "Default"; break;
   case          2: c = "Shared"; break;
   case          4: c = "Private"; break;
   case          5: c = "Fixshared"; break;
   default: c = ""; break;
   }
   return c;
}

char *strCfStatType(int v)
{
  char *c;
  switch (v)
  {
  case  0: c = "Lock"        ; break;
  case  1: c = "Unlock"      ; break;
  case  2: c = "WrLCCtl"     ; break;
  case  3: c = "SigXCF"      ; break;
  case  4: c = "SigCF"       ; break;
  case  5: c = "Read"        ; break;
  case  6: c = "Write"       ; break;
  case  7: c = "StartMon"    ; break;
  case  8: c = "StopMon"     ; break;
  case  9: c = "Unused"      ; break;
  case 10: c = "New"         ; break;
  case 11: c = "Move"        ; break;
  case 12: c = "MoveEntry"   ; break;
  case 13: c = "Delete"      ; break;
  case 14: c = "Unused"      ; break;
  case 15: c = "Unused"      ; break;
  case 16: c = "Unused"      ; break;
  case 17: c = "Unused"      ; break;
  case 18: c = "Unused"      ; break;
  case 19: c = "Unused"      ; break;
  default: c = ""; break;
  }
  return c;
}

