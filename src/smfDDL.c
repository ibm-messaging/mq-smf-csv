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
#include <string.h>
#include <ctype.h>
#include "mqsmf.h"

static FILE *fp = NULL;
char *comma = "";
char *schema = "MQSMF";

#define INDEXSETSIZE 128
char *indexSet[INDEXSETSIZE] = {0};
int   nextIndex = 0;

/*
 * The DDL generated here is very simple. The
 * table is created with a fixed name and relevant
 * datatypes. If any index columns have been named
 * then those are added to the DDL with each table.
 * There is no "Primary key" defined.
 */

void openDDL(char *name)
{
   BOOL newFile;
   int i;

   for (i=0;i<INDEXSETSIZE;i++)
   {
     indexSet[i] = NULL;
   }
   nextIndex = 0;

   if (!fp)
   {
     fp = fopenext("MQTABLES","ddl", &newFile);
     if (!fp) /* fopenext will print detailed error message */
       exit(1);
   }
   fprintf(fp,"DROP   TABLE %s.%s;  \n",schema,name);

   /* If you want more sophisticated SQL, then you could change this */
   fprintf(fp,"CREATE TABLE %s.%s (\n",schema,name);
   comma = " ";                               /* Start with it blank */
}

void closeDDL(char *name)
{
  int i;
  if (fp)
  {
    /* Could add "... IN <tablespace>" to this if needed */
    fprintf(fp,");\n");
    for (i=0;i<INDEXSETSIZE;i++)
    {
      char *c = indexSet[i];
      if (c) {
        fprintf(fp,"CREATE INDEX %s ON %s.%s(%s);\n",c,schema,name,c);
      }
    }
    fprintf(fp,"\n");
    fflush(fp);
  }
  return;
}

void addIndex(char *key)
{
  if (nextIndex <INDEXSETSIZE) {
    indexSet[nextIndex] = key;
    nextIndex++;
  }
  else
  {
    /* Extremely unlikely that we'll have this number of index columns for a table */
    fprintf(stderr,"Warning: Cannot add %s to table index. Need to increase INDEXSETSIZE\n",key);
  }
  return;
}


void printDDL(char *name, int type, int len)
{
  char *p;
  char *p2;
  char nameCopy[COL_HEAD_LEN] = {0};  /* Cannot modify fixed strings, have to copy input*/

  if (!fp)
    return;

  strncpy(nameCopy,name,sizeof(nameCopy)-1);

  switch (type)
  {
  case DDL_I:
    /* SQL Integer is a 'signed 32-bit' when sometimes we need unsigned */
    /* So make it a big int always                                      */
    fprintf(fp,"%s %s \t BIGINT \n",comma,formatDDL(nameCopy));
    break;
  case DDL_I64:
    fprintf(fp,"%s %s \t BIGINT\n",comma,formatDDL(nameCopy));
    break;
  case DDL_C:
    if (!strcmp(name,"Date"))        /* special case for CommonHeader field*/
    {
      fprintf(fp,"%s %s \t DATE    \n",comma,formatDDL(nameCopy));
    }
    else
    {
      fprintf(fp,"%s %s \t CHAR(%d)\n",comma,formatDDL(nameCopy),len);
    }
    break;
  case DDL_SUS: /* a seconds/microseconds field */
    p = strstr(nameCopy,"(US)");
    if (p) *p = 0;
    fprintf(fp,"%s %s_us \t BIGINT\n",comma,formatDDL(nameCopy));
    break;

  case DDL_DATE:
    p = strstr(nameCopy," (DATE)");
    if (p) *p = 0; /* remove the "(DATE)" bit */
    fprintf(fp,"%s %s_Date \t DATE\n",comma,formatDDL(nameCopy));
    break;
  case DDL_TIME:
    p = strstr(nameCopy," (TIME)");
    if (p)
      *p = 0;
    fprintf(fp,"%s %s_Time \t CHAR(19)\n",comma,formatDDL(nameCopy));
    break;
  }

  comma = ",";
}


/*
 * The column names used in the CSV files may look OK in a
 * spreadsheet but not be suitable for use in a SQL table.
 * So this function renames them in a simple way.
 */
static char nameNoDup[COL_HEAD_LEN];
char *formatDDL(char *name)
{
  /* Leave room for _DATE etc as prefix */
  return formatDDLMaxLength(name,27);
}

char *formatDDLMaxLength(char *name,int maxLength)
{ char *i = name;
  char *o = nameNoDup;
  char prev = '.';
  char c;

  /* Copy the string, replacing problematic chars as we go */
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
      c = '_';
      break;

    default:
      c = *i;
      break;
    }

    /* Get rid of repeated '_' characters */
    if (c != prev || prev != '_')
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
      (o >= nameNoDup) && (*o == '_');
      o--)
    *o = 0;

  /*************************************************************************/
  /* Check for column name length - I've seen this as a limit in some DBs. */
  /* Will sometimes come into this function neeeding to account for adding */
  /* the (DATE)/(TIME) component so modify the maxLength.                  */
  /*************************************************************************/
  if (!strstr(name,"(DATE)") || !strstr(name,"(TIME)"))
    maxLength += 7;
  if (strlen(nameNoDup) > maxLength)
    fprintf(stderr,"Warning: Column %s may be too long\n",nameNoDup);

  return nameNoDup;
}

