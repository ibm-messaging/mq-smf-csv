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

#if defined(PLATFORM_WINDOWS)
#define BITFIELDREPRINTNEEDED (1)
#else
#define BITFIELDREPRINTNEEDED (0)
#endif

int inBitfieldReplace = 0;
void bitfieldReprint(int c);

/*
 * This is a very simple filter to take the product-provided
 * header file from z/OS and strip a few things that are
 * problematic on other platform compilers.
 *
 * It means we don't need to keep a variant copy
 * of the header in sync [Awhen new versions come out.
 *
 * In particular, get rid of the z/OS format for
 * the pragmas, and also the trigraph at the top
 * of the file.
 *
 * If the contents of the header file change, this
 * filter may need to be updated to match.
 *
 * Newer compiler versions on some platforms have also
 * affected how bitfields are laid out. This program now
 * rewrites bitfield structures that have been found to be a problem.
 * The changes have to be explicitly for each issue, but the build process
 * does now check if the structure sizes are unexpectedly different.
 * The AIX compiler seems to give the same sizes as z/OS so that's the master
 * we will drive the comparison from.
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
          if (BITFIELDREPRINTNEEDED) {
            if (strstr(line,"fNoAcctg")) {
              bitfieldReprint(1);
            }
            if (strstr(line,"qwhssmfc")) {
              bitfieldReprint(2);
            }
          }

          if (inBitfieldReplace) {
            /* Have we reached the end of the bitfield structure */
            if (strstr(line,"}"))
              inBitfieldReplace = 0;
          }

          if (!inBitfieldReplace) {
            printf("%s",line);
            if (line[strlen(line)-1] != '\n')
              printf("\n");
          }

       }
     }
  } while (c);

  fflush(stdout);
  return 0;
}

void bitfieldReprint(int c) {
  inBitfieldReplace = 1;
  switch (c) {
  case 1:
    printf("  unsigned char fNoAcctg : 1;\n");
    printf("  unsigned char fTopic   : 1;\n");
    printf("  unsigned char fAcctqC  : 1;\n");
    printf("  unsigned char spare    : 5;\n");
    printf("  unsigned char spare2   : 8;\n");
    break;
  case 2:
    printf("  unsigned char qwhssmfc : 1;\n");
    printf("  unsigned char qwhspad1 : 7;\n");
    break;
  default:
    break;
  }
}

