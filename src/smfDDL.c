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
#include <mqsmf.h>

static FILE *fp = NULL;
char *comma = "";

static char *format(char *);

void openDDL(char *name)
{
   BOOL newFile;
   if (!fp)
   {
     fp = fopenext("MQTABLES","ddl", &newFile);
     if (!fp)
       exit(1);
   }
   fprintf(fp,"DROP   TABLE MQSMF.%s;  \n",name);
   fprintf(fp,"CREATE TABLE MQSMF.%s (\n",name);
   comma = " ";                                      /* Start with it blank*/
}

void closeDDL(void)
{
  fprintf(fp,");\n\n");
  fflush(fp);
  return;
}

void printDDL(char *name, int type, int len)
{
  char *p;
  char *p2;
  char nameCopy[64];     /* Cannot modify fixed strings, have to copy input*/

  strcpy(nameCopy,name);

  switch (type)
  {
  case DDL_I:
    fprintf(fp,"%s \"%s\" \t INTEGER\n",comma,format(nameCopy));
    break;
  case DDL_I64:
    fprintf(fp,"%s \"%s\" \t BIGINT\n",comma,format(nameCopy));
    break;
  case DDL_C:
    if (!strcmp(name,"Date"))
      fprintf(fp,"%s \"%s\" \t CHAR(16)\n",comma,format(nameCopy));
    else if (!strcmp(name,"Time"))
      fprintf(fp,"%s \"%s\" \t CHAR(16)\n",comma,format(nameCopy));
    else
      fprintf(fp,"%s \"%s\" \t CHAR(%d)\n",comma,format(nameCopy),len+3);
    break;
  case DDL_SUS:
    p = strchr(nameCopy,',');
    *p = 0;

    p2 = strstr(nameCopy,"(S)");
    if (p2) *p2 = 0;
    fprintf(fp,"%s \"%s_s\" \t INTEGER\n",comma,format(nameCopy));

    p = p+1;
    p2 = strstr(p,"(US)");
    if (p2) *p2 = 0;
    fprintf(fp,"%s \"%s_us\" \t INTEGER\n",comma,format(p));
    break;

  case DDL_DATETIME:
    p = strchr(nameCopy,',');
    *p = 0;

    p2 = strstr(nameCopy," (DATE)");
    if (p2) *p2 = 0;
    fprintf(fp,"%s \"%s_Date\" \t CHAR(16)\n",comma,format(nameCopy));

    p = p+1;
    if (p2) p2 = strstr(p," (TIME)");
    *p2 = 0;
    fprintf(fp,"%s \"%s_Time\" \t CHAR(16)\n",comma,format(p));
    break;
  }

  comma = ",";
}


/*
 * The column names used in the CSV files may look OK in a
 * spreadsheet but not be suitable for use in a SQL table.
 * So this function renames them in a simple way.
 */
static char nameNoDup[64];
static char *format(char *name)
{ char *i = name;
  char *o = nameNoDup;
  char prev = '.';
  char c;

  /* Start by stripping trailing spaces */
  for (i = name + strlen(name) -1;
      (i >= name) && (*i == ' ');
      i--)
    *i = 0;


  /* Then copy the string, replacing problematic chars as we go */
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

  if(strlen(nameNoDup) > 31)
    printf("Warning: Column %s may be too long\n",nameNoDup);

  return nameNoDup;
}

