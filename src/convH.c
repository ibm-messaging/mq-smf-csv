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

/*
 * This is a very simple filter to take the product-provided
 * header file from z/OS and strip a few things that are
 * problematic on other platform compilers.
 *
 * It means we don't need to keep a variant copy
 * of the header in sync when new versions come out.
 *
 * In particular, get rid of the z/OS format for
 * the pragmas, and also the trigraph at the top
 * of the file.
 *
 * If the contents of the header file change, this
 * filter may need to be updated to match.
 */
int main(int argc, char **argv)
{
  char line[256] = {0};
  char *c;

  /******************************************************/
  /* Need a portable pragma that works on all platforms */
  /******************************************************/
  printf("#pragma pack(1)\n");

  /******************************************************/
  /* Then copy stdin to stdout, stripping just a few    */
  /* lines.                                             */
  /******************************************************/
  do
  {
     c = fgets(line,sizeof(line)-1,stdin);
     if (c)
     {
       if (strstr(line,"pragma"))
         continue;
       /****************************************************/
       /* Can't just put "??=" in here as it's converted   */
       /* into a trigraph and no longer matches.           */
       /****************************************************/
       else if (strstr(line,"\?\?="))
         continue;
       else
       {
          printf(line);
          if (line[strlen(line)-1] != '\n')
            printf("\n");
       }
     }
  } while (c);

  fflush(stdout);
  return 0;
}
