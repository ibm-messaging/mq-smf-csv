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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "mqsmf.h"

#define INITIALEVENTBUFSIZE (1024*10*10)
char   jsonEscBuf[1024];    /* Value with special chars escaped */
char  *jsonPtr  = NULL;        /* Where to add next line to EventBuf */
char  *jsonOutputStart  = NULL;
size_t jsonOutputSize;

static char *jsonEscape(char *s, size_t l);
static char *removeSuffix(char *s);
static int   removeIndex(char *s);

static void jsonAddElement(columnHeader_t *h, char *fmt, ...);

#define MAXARRAYELEMENTS (64)
#define MAXARRAYDATABUFFER (1023)
typedef struct jsonArrayElement_s {
  char *name;
  int   offsetD;
  char  line[MAXARRAYDATABUFFER+1];
} jsonArrayElement_t;
static jsonArrayElement_t *jsonArray[MAXARRAYELEMENTS];
static int arrayElementCount = 0;

static char nameNoDup[COL_HEAD_LEN];
columnHeader_t *jsonFormatHeader(BOOL idx, char *name)
{
  columnHeader_t *ch;
  jsonArrayElement_t *j = NULL;
  char *i = name;
  char *o = nameNoDup;
  BOOL nextUpper=FALSE;
  char prev = '.';
  int arrayIndex;
  char c;

  /* Copy the string, removing problematic chars as we go */
  i = name;
  do
  {
    switch (*i)
    {
    case ' ':
    case '(':
    case ')':
    case '{':
    case '}':
    case '.':
    case '_':
      c = '\0';
      nextUpper=TRUE;
      break;

    default:
      if(nextUpper)
        c=toupper(*i);
      else
        c=tolower(*i);
      nextUpper=FALSE;
      break;
    }

    /* Get rid of repeated '_' characters */
    if ((c != prev || prev != '_') && c != '\0')
    {
      *o = c;
      o++;
    }

    i++;
    prev = c;
  } while (*i);

  *o = 0;         /* Make sure it's terminated */

  /* And remove any trailing _ */
  for (o = nameNoDup + strlen(nameNoDup) -1;
      (o >= nameNoDup) && ((*o == '_') || (*o == ' '));
      o--)
    *o = 0;


  if (idx)
  {
    char *suf;
    int x;
    suf = removeSuffix(nameNoDup);
    if (suf) {
      arrayIndex = removeIndex(nameNoDup);
      strcat(nameNoDup,suf);
    } else {
      arrayIndex = removeIndex(nameNoDup);
    }

    if (arrayIndex >0)
    {
      j = NULL;
      for (x=0;x<MAXARRAYELEMENTS;x++) {
        j = jsonArray[x];
        if (j && !strcmp(j->name,nameNoDup))
          break;
      }
      if (!j) {
        if (debugLevel >0)
        {
          fprintf(infoStream,"Allocating array element %d buffer %s\n",arrayIndex,nameNoDup);
        }
        j = malloc(sizeof(jsonArrayElement_t));
        j->offsetD = 0;
        jsonArray[arrayElementCount++] = j;
        memset(j->line,0,sizeof(j->line));
      }
    }
    else
    {
      if (debugLevel >0)
      {
        fprintf(infoStream,"Array index %d for %s\n",arrayIndex,nameNoDup);
      }
    }
  }

  ch = malloc(sizeof(columnHeader_t));
  if (debugLevel >0)
  {
     fprintf(infoStream,"Allocating buffer for column %s\n",nameNoDup);
  }
  ch->name = strdup(nameNoDup);
  ch->idx = idx?arrayIndex:-1;
  ch->arrayLine = j;
  if (j)
     j->name = ch->name;
  return ch;
}


static void jsonAddLine(columnHeader_t *h ,int type, const char *fmt,...)
{
  size_t offset;
  va_list args;
  int l;
  long long ll;

  /*
   * Trying to avoid printf variants as much as possible to
   * attempt to get best performance out of this routine. Profiling
   * shows this to be a key function for speed.
   */

  if (h)
  {
    memcpy(jsonPtr,"\"",1);    jsonPtr ++;
    l = strlen(h->name);
    memcpy(jsonPtr,h->name,l); jsonPtr += l;
    memcpy(jsonPtr,"\": ",3); jsonPtr += 3;
  }

  switch (type)
  {
  case ODT_I:
  case ODT_I64:
  case ODT_VAR:
    va_start(args,fmt);
    jsonPtr  += vsprintf(jsonPtr,fmt,args);
    va_end(args);
    break;
  case ODT_C:
    memcpy(jsonPtr,"\"",1); jsonPtr ++;
    l = strlen(fmt);
    memcpy(jsonPtr,fmt,l);  jsonPtr += l;
    memcpy(jsonPtr,"\"",1); jsonPtr ++;
    break;
  case ODT_USEC:
    l = strlen(fmt);
    memcpy(jsonPtr,fmt,l);  jsonPtr += l;
    break;
  }

  if (h)
  {
    *jsonPtr++ = ',';
  }
  *jsonPtr++ = '\n';
  *jsonPtr = 0;

  offset = jsonPtr - jsonOutputStart;

  /* Resize the buffer if we get 90% of the way through it */
  if (offset > (jsonOutputSize * 90 /100)) {
    jsonOutputSize = jsonOutputSize*2;
    if (debugLevel > 0)
       fprintf(infoStream,"Resizing JSON output buffer to %ld\n",(long)jsonOutputSize);
    jsonOutputStart = realloc(jsonOutputStart,jsonOutputSize);
    jsonPtr = jsonOutputStart + offset;
  }
}

static void jsonAddElement(columnHeader_t *h, char *fmt, ...)
{
  size_t offset;
  va_list args;
  /*
   * Array element processing is a lot less common than adding separate lines
   * so I'm not (for now) trying to avoid printf processing in here. Maybe
   * later if it turns out to be another bottleneck
   */

  jsonArrayElement_t *j = h->arrayLine;
  if (!j)
  {
    fprintf(stderr,"Have not allocated an element for %s\n",h->name);
    return;
  }

  va_start(args,fmt);
  j->offsetD  += vsnprintf(&(j->line[j->offsetD]),MAXARRAYDATABUFFER,fmt,args);
  va_end(args);

  j->line[j->offsetD]=0;
  if (j->offsetD >= sizeof(j->line)) {
    fprintf(stderr,"Buffer for JSON array is too small. Change MAXARRAYDATABUFFER and rebuild.\n");
  }
}

void jsonAddInt(columnHeader_t *h,int i)
{
  if (h && h->arrayLine)
    jsonAddElement(h,"%i, ",i);
  else
    jsonAddLine(h,ODT_I,"%i",i);
}

void jsonAddInt64(columnHeader_t *h,long long ll)
{
  if (h && h->arrayLine)
    jsonAddElement(h,"%lld, ",ll);
  else
    jsonAddLine(h,ODT_I64, "%lld",ll);
}
void jsonAddString(columnHeader_t *h,char *c,int l)
{
  char *p = jsonEscape(c,l); /* This is always null terminated copy */
  if (h && h->arrayLine)
    jsonAddElement(h,"\"%s\", ",p);
  else
    jsonAddLine(h,ODT_C, p);
}
void jsonAddNumericString(columnHeader_t *h,char *c,int l)
{
  char *p = jsonEscape(c,l);
  if (h && h->arrayLine)
    jsonAddElement(h," %s,",p);
  else
    jsonAddLine(h,ODT_USEC, p);
}

static void *removeComma(char *p)
{
  if (p)
  {
    char *c = p-2;
    if (*c == ',')
    {
      *c++ = '\n';
      *c = 0;
      p--;
    }
  }
  return p;
}

void jsonNew(FILE * outFp, char *tableName)
{
  int i;

  if (!jsonPtr) {
      jsonOutputSize = INITIALEVENTBUFSIZE;
      jsonPtr  = (char *)malloc(jsonOutputSize);
      jsonOutputStart = jsonPtr;
   } else {
      jsonPtr = jsonOutputStart;
   }
   *jsonPtr = 0;

   for (i=0;i<MAXARRAYELEMENTS;i++) {
     jsonArray[i]=NULL;
   }

   arrayElementCount=0;

   jsonAddLine(NULL,ODT_VAR,"{");
   jsonAddLine(NULL,ODT_VAR,"\"recordType\" : %d,",recordType);
   jsonAddLine(NULL,ODT_VAR,"\"recordSubType\" : %d,",recordSubType);
   jsonAddLine(NULL,ODT_VAR,"\"structure\" : \"%s\",",tableName);
   return;
}

void jsonDump(FILE *fp, columnHeader_t **columnHeaders)
{
  int i;
  columnHeader_t *h;
  jsonArrayElement_t *j;

  for (i=0;i<HEADINGS_COUNT;i++) {
    h = columnHeaders[i];
    if (h && h->arrayLine)
    {
      char *p;
      /* Trim the line removing trailing spaces and any final comma, to make valid JSON */
      j = (jsonArrayElement_t *)h->arrayLine;
      p = &j->line[j->offsetD-1];
      while (*p == ' ' || *p == ',' && j->offsetD >0)
      {
        *p=0;
        j->offsetD--;
        p--;
      }

      if (j->offsetD > 0)
      {
        jsonAddLine(NULL,ODT_VAR, "\"%s\" : [ %*.*s ],",h->name,j->offsetD,j->offsetD,j->line);
        j->offsetD = 0;
      }
    }
  }

  jsonPtr = removeComma(jsonPtr);

  jsonAddLine(NULL,ODT_VAR,"}");
  if (jsonOutputStart) {
    fwrite(jsonOutputStart,1,jsonPtr-jsonOutputStart,fp);
    fflush(fp);
  }
  jsonPtr = jsonOutputStart;
  *jsonPtr = 0;
  return;
}

static char *jsonEscape(char *s, size_t l)
{
  size_t i,j;

  for (i=0,j=0;i<l;i++)
  {
    switch (s[i])
    {
    case '\"':
    case '\\':
      jsonEscBuf[j++]='\\';
      break;
    default:
      break;
    }

    jsonEscBuf[j++] = s[i];
  }
  jsonEscBuf[j] = 0;
  return jsonEscBuf;
}

/* Modify string, removing the suffix and returning new suffix if it was there */
static char *removeSuffix(char *s)
{
  char *c = NULL;
  int i;
  size_t l;
  char *p;

  /* By the time this is called, we already have the mixed case version */
  char *list[]     = {"Date","Time","Us"};

  for (i=0;i<sizeof(list)/sizeof(list[0]);i++)
  {
    l = strlen(s);
    p = &s[l-strlen(list[i])];
    if (!strncmp(p,list[i],strlen(list[i]))) {
      *p = 0;
      c=list[i];
      break;
    }
  }
  if (c && !strcmp(c,"Us"))
    c = "US"; // This looks better for distinguishing the unit
  return c;
}

static int removeIndex(char *s) {
  /* We know that the index is at most 2 digits */
  int i;
  int idx = 0;
  char *p;
  int l = strlen(s)-2;
  if(debugLevel >=1)
    fprintf(infoStream,"Working on index %s for %s\n",&s[l],s);
  for (i=0;i<2;i++)
  {
    p = &s[l+i];
    if (isdigit(*p))  {
      if (!strncmp(s,"io",2)) /* We know the "io*" columns are 2x2 arrays */
        idx = (2*i)*idx + (*p - '1') ;
      else                    /* Other columns are simple vectors */
        idx = (10*i)*idx + (*p - '0'); /* since it will never get into 100s */
      *p = 0;
    }
  }

  if (!strncmp(s,"io",2)) /* Want to get it indexed from 1 */
   idx++;

  return idx;
}

