/*
 * Copyright (c) 2016, 2019 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Mark Taylor - Initial Contribution
 */

/*
 * This is a filter to take the product-provided
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
 * We change the implicit datatype sizes (eg unsigned long)
 * to an explicitly-sized standard datatype (eg uint64). This
 * is needed if the program is compiled in 64-bit mode. We also
 * rewrite bitfield elements in structures.
 *
 * NOTE: If the contents of the header file change, this
 * filter may need to be updated to match.
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>


/**********************************************************************/
/* Some platforms are starting to drop 32-bit application support. So */
/* it is possible that at some point we won't be able to compile this */
/* program with the original SMF header file while maintaining type   */
/* sizes. So we can't rely on (or enforce) "long" being 32-bit. Which */
/* means needing to be explicit about datatype sizes to match z/OS.   */
/*                                                                    */
/* These are the formats used in the z/OS header file that we need to */
/* replace with explicit sizes. Note the 2 variants of 'long int'     */
/* spacing. Also note the way we deal with the pointer type that is   */
/* not actually needed in this program.                               */
/*                                                                    */
/* Changes to the original z/OS header file may require changes to    */
/* this reformatter. But they hopefully will get caught by either a   */
/* breaking compilation or the sizeTest build rule.                   */
/**********************************************************************/
#define ZLONGLONG         "long long "
#define ZSHORTINT         "short int "
#define ZEXTRAINT         " int "
#define ZLONGINT1         "long int "
#define ZLONGINT2         "long  int "
#define ZLONGINTPTR       "long  int *" /* The ptrs are all spaced like this */
#define ZUNSIGNEDLONG     "unsigned long "

/* The signed/unsigned indicator is included in the replacement datatype.*/
/* The _BLANK value must be at least as long as as the original modifier */
#define UNSIGNED         "unsigned "
#define UNSIGNED_BLANK   "         "
#define SIGNED           "signed "
#define SIGNED_BLANK     "       "

/* Must be at least as long as the string it's replacing */
#define REPLACE_INT64    "int64_t       "
#define REPLACE_UINT64   "uint64_t      "
#define REPLACE_INT32    "int32_t       "
#define REPLACE_UINT32   "uint32_t      "
#define REPLACE_INT16    "int16_t       "
#define REPLACE_UINT16   "uint16_t      "
#define REPLACE_EXTRAINT "        "

/**********************************************************************/
/* Newer compiler versions on some platforms have also                */
/* affected how bitfields are laid out. This program now              */
/* rewrites bitfield structures that have been found to be a problem. */
/* The changes have to be explicitly for each issue, but the build    */
/* does now check if the structure sizes are unexpectedly different.  */
/* The AIX compiler seems to give the same sizes as z/OS so that's the*/
/* master we will drive the comparison from.                          */
/**********************************************************************/
#if defined(PLATFORM_WINDOWS)
#define BITFIELDREPRINTNEEDED (1)
#else
#define BITFIELDREPRINTNEEDED (0)
#endif

int inBitfieldReplace = 0;
void bitfieldReprint(int c);
void datatypeReplace(char *);

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
            datatypeReplace(line);

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

/*************************************************************/
/* Convert datatypes like long long to int64_t etc           */
/*                                                           */
/* We do this simplistically without really parsing the      */
/* line but knowing how the original header file is written. */
/*                                                           */
/* There are no guaranteed rules; this function was coded    */
/* and tested against the existing header file.              */
/*                                                           */
/* Lines in the original cmqsmfdc.h file look like:          */
/*   unsigned long  int readcnt;      /. comments ./         */
/*   unsigned long long int deletcuw; /. comments ./         */
/*   char     qcctjobn[8];            /. comments ./         */
/*   unsigned long  int *wtascqf;     /. comments ./         */
/*   unsigned short int qwsx0r5n;                            */
/*************************************************************/
void datatypeReplace(char *line) {
  char *p,*p2;

  /* Deal first with "long long" datatypes */
  p = strstr(line,ZLONGLONG);
  if (p) {
    p2 = strstr(line,UNSIGNED);
    if (p2) {
      memcpy(p2,UNSIGNED_BLANK,strlen(UNSIGNED));
      memcpy(p,REPLACE_UINT64,strlen(ZLONGLONG));
    } else {
      memcpy(p,REPLACE_INT64,strlen(ZLONGLONG));
      p2 = strstr(line,SIGNED);
      if (p2) {
        memcpy(p2,SIGNED_BLANK,strlen(SIGNED));
      }
    }

    /*************************************************************/
    /* We may now have a line "uint64_t int deletcuw" so get rid */
    /* of the extraneous "int"                                   */
    /*************************************************************/
    p = strstr(line,ZEXTRAINT);
    if (p) {
      memcpy(p,REPLACE_EXTRAINT,strlen(ZEXTRAINT));
    }
  }

  /* And then the "short" ones */
  p = strstr(line,ZSHORTINT);
  if (p) {
    p2 = strstr(line,UNSIGNED);
    if (p2) {
      memcpy(p2,UNSIGNED_BLANK,strlen(UNSIGNED));
      memcpy(p,REPLACE_UINT16,strlen(ZSHORTINT));
    } else {
      memcpy(p,REPLACE_INT16,strlen(ZSHORTINT));
      p2 = strstr(line,SIGNED);
      if (p2) {
        memcpy(p2,SIGNED_BLANK,strlen(SIGNED));
      }
    }

    /*************************************************************/
    /* We may now have a line "uint16_t int qwsx0r5n" so get rid */
    /* of the extraneous "int"                                   */
    /*************************************************************/
    p = strstr(line,ZEXTRAINT);
    if (p) {
      memcpy(p,REPLACE_EXTRAINT,strlen(ZEXTRAINT));
    }
  }

  /*************************************************************************/
  /* Now look for lines that have "unsigned long int *". Ptrs are replaced */
  /* with a 32-bit datatype as we don't need to use the actual pointers    */
  /*************************************************************************/
  p = strstr(line,ZLONGINTPTR);
  if (p) {
    p2 = strstr(line,UNSIGNED);
    if (p2) {
      memcpy(p2,UNSIGNED_BLANK,strlen(UNSIGNED));
      memcpy(p,REPLACE_UINT32,strlen(ZLONGINTPTR));
    } else {
      memcpy(p,REPLACE_INT32,strlen(ZLONGINTPTR));
      p2 = strstr(line,SIGNED);
      if (p2) {
        memcpy(p2,SIGNED_BLANK,strlen(SIGNED));
      }
    }
  }

  /*************************************************************************/
  /* We should now be able to deal with remaining "long int" lines which   */
  /* seem to have 2 slightly differently-spaced formats                    */
  /*************************************************************************/
  p = strstr(line,ZLONGINT1);
  if (p) {
    p2 = strstr(line,UNSIGNED);
    if (p2) {
      memcpy(p2,UNSIGNED_BLANK,strlen(UNSIGNED));
      memcpy(p,REPLACE_UINT32,strlen(ZLONGINT1));
    } else {
      memcpy(p,REPLACE_INT32,strlen(ZLONGINT1));
      p2 = strstr(line,SIGNED);
      if (p2) {
        memcpy(p2,SIGNED_BLANK,strlen(SIGNED));
      }
    }
  }
  p = strstr(line,ZLONGINT2);
  if (p) {
    p2 = strstr(line,UNSIGNED);
    if (p2) {
      memcpy(p2,UNSIGNED_BLANK,strlen(UNSIGNED));
      memcpy(p,REPLACE_UINT32,strlen(ZLONGINT2));
    } else {
      memcpy(p,REPLACE_INT32,strlen(ZLONGINT2));
      p2 = strstr(line,SIGNED);
      if (p2) {
        memcpy(p2,SIGNED_BLANK,strlen(SIGNED));
      }
    }
  }

  p = strstr(line,ZUNSIGNEDLONG);
  if (p) {
    memcpy(p,REPLACE_UINT32,strlen(ZUNSIGNEDLONG));
    p2 = strstr(line,ZEXTRAINT);
    if (p2) {
      memcpy(p2,REPLACE_EXTRAINT,strlen(ZEXTRAINT));
    }
  }

  /* 'char' datatypes can be left alone */

  return;
}
