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
/*   Module Name : mqsmf.c                                            */
/*                                                                    */
/*   Description : Example of printing MQ SMF records in a format     */
/*                 suitable for use in other tools such as a          */
/*                 spreadsheet. Run this on Windows or a Unix system. */
/*                                                                    */
/*   Function    : Reads SMF records from an SMF dump data set and    */
/*                 prints them in a CSV format. Each element of the   */
/*                 SMF format is written to a separate file.          */
/*                                                                    */
/*   Notes       : This program is NOT run on z/OS, unlike the        */
/*                 the product-provided sample (CSQ4SMFD) or MP1B.    */
/*                 Instead, the SMF dump is downloaded to another     */
/*                 platform and formatted there, which is likely to   */
/*                 be more convenient for where spreadsheet and DB    */
/*                 programs can access it.                            */
/*                                                                    */
/*                 This program handles ASCII/EBCDIC conversions and  */
/*                 necessary byte-swapping from big to little-endian  */
/*                 values.                                            */
/*                                                                    */
/*                 When downloading the SMF dump, make sure the RDW   */
/*                 (record descriptor word) is included in the file.  */
/*                 This is because the I/O on Windows and Unix        */
/*                 platforms does not understand the record-oriented  */
/*                 z/OS file format; instead we have to know how long */
/*                 each record is - given by the RDW. This is not     */
/*                 the default option on many file transfer programs. */
/*                 Check the documentation for your file manager on   */
/*                 how to include the RDW when downloading.           */
/*                                                                    */
/*                 Structures are defined as "packed" which means that*/
/*                 fields within a structure may not start on a       */
/*                 natural (eg word) boundary. Some platforms may need*/
/*                 additional compiler options to deal with that      */
/*                 without doing something like SIGILL.               */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>

#include "mqsmf.h"

static void Usage();

#define DEFAULT_TICKER   10000         /* Print out status every N records */

/********************************************************************/
/* Definition of the global variables                               */
/********************************************************************/
int   debugLevel = 0;
BOOL  addEquals = TRUE;
BOOL  printHeaders = TRUE;
commonFields_t commonF = {0};

char headings[HEADINGS_LEN];  /* Ensure these are big enough for any line */
char dataline[DATA_LEN];
char tmpHead[64];      /* Working space for a column heading       */
unsigned int   recordType;

/********************************************************************/
/* Variables scoped to this module                                  */
/********************************************************************/
static BOOL  append = FALSE;
static char *directory = NULL;

static unsigned int Count115[256] = {0};  /* Max subtype is 255 */
static unsigned int Count116[256] = {0};

/********************************************************************/
/* Not all platforms have getopt, so use our own version. Prefix    */
/* the standard names with "mq" to keep distinct.                   */
/********************************************************************/
static int   mqgetopt(int, char **, char *);
static int   mqoptind = 1;             /* getopt index                     */
static int   mqoptopt;                 /* getopt option                    */
static char* mqoptarg;                 /* getopt argument                  */


/********************************************************************/
/* MAIN                                                             */
/*   See the Usage function for parameters and syntax               */
/********************************************************************/
int main( int argc, char *argv[] )
{

  SMFRecord_t *pSMFRecord;
  int ticker = DEFAULT_TICKER;
  int i,j;                           /* loop counters                    */
  char  dataBuf[MAX_SMF_DATA];       /* Contains the SMF data            */
  int   bytesRead;                   /* Number of bytes from fread()     */
  int   offset;                      /* total number of bytes in a record*/
  unsigned int d[3];                 /* used in date conversion          */
  unsigned int ddd,year;             /* day number and year number       */
  unsigned int time;                 /* Copied from the SMF header       */

  char *inputFile = NULL;
  FILE *fp;
  unsigned int totalRecords = 0;
  unsigned int maxRecords = 0xFFFFFFFF;
  unsigned int unknownCount = 0;
  unsigned short recordSubType;
  unsigned short nextLength;
  int subTypesValid = 0;
  int qTriplet;
  char *p;
  qwhs *pqwhs;

  BOOL error = FALSE;
  int c;

  int sectionCount;
  triplet_t triplet[12];

  /******************************************************************/
  /* Make sure program was correctly compiled. These datatypes must */
  /* match the standard z/OS sizes. Generally this will mean        */
  /* compiling this code as a 32-bit program. We don't want to have */
  /* to change the product-provided header file excessively to      */
  /* convert it to use types like uint_32, so are stuck with        */
  /* dealing with it like this.                                     */
  /******************************************************************/
  if ((sizeof(short) != 2)     ||
      (sizeof(long) != 4)      ||
      (sizeof(long long) != 8) ||
      (sizeof(int) != 4))
  {
    printf("Data type sizes do not match requirements.\n");
    printf("Need to rebuild program with correct options.\n");
    printf("Here: short=%d int=%d long=%d long long=%d bytes\n",
       sizeof(short), sizeof(int), sizeof(long), sizeof(long long));
    printf("Need: short=%d int=%d long=%d long long=%d bytes\n",2,4,4,8);
    exit(1);
  }

  /******************************************************************/
  /* Parse command-line parameters                                  */
  /******************************************************************/
  while((c = mqgetopt(argc, argv, "ad:h:i:m:o:rt:")) != EOF)
  {
    switch(c)
    {
      case 'a':
        append = TRUE;
        break;
      case 'd':
        debugLevel = atoi(mqoptarg);
        break;
      case 'h':
        for (i=0;i<strlen(mqoptarg);i++)
          mqoptarg[i] = toupper(mqoptarg[i]);
        if (!strcmp(mqoptarg,"NO"))
          printHeaders = FALSE;
        break;
      case 'i':
        inputFile = mqoptarg;
        break;
      case 'm':
        maxRecords = atoi(mqoptarg);
        break;
      case 'o':
        directory = mqoptarg;
        break;
      case 'r':
        addEquals = 0;
        break;
      case 't':
        ticker     = atoi(mqoptarg);
        break;
      default:
        error = TRUE;
        break;
    }
  }

  /******************************************************************/
  /* Were there any problems parsing the parameters.                */
  /* Any remaining parameters are taken as an error                 */
  /******************************************************************/
  if (error || mqoptind<argc)
  {
    Usage();
    exit(1);
  }


  /*********************************************************************/
  /* Open the input file                                               */
  /* If this fails, then put out an error message and quit. If no      */
  /* filename was given on command line, use stdin.                    */
  /* Windows requires the "b" option for binary input.                 */
  /*********************************************************************/
  fp = inputFile?fopen(inputFile, "rb" ):stdin;
  if (!fp)
  {
    printf(" Cannot open input file %s. Error \"%s\" (%d)\n",
      inputFile,strerror(errno),errno);
    goto mod_exit;
  }

  convInit();      /* Decide whether this is a big or little endian machine*/

  /********************************************************************/
  /* Loop until we have no more data or enough records have been read */
  /********************************************************************/
  pSMFRecord = (SMFRecord_t *)&dataBuf;
  do
  {
    /********************************************************************/
    /* Start reading data and processing the MQ records                 */
    /* First, read the length of this file record (not necessarily the  */
    /* same as the SMF record length). It comes from the first two      */
    /* bytes of the Record Descriptor Word (RDW).                       */
    /********************************************************************/

    bytesRead = fread(&pSMFRecord->Header.SMFLEN,1,2,fp);
    if (bytesRead < 2)
      continue;

    /********************************************************************/
    /* The second half-word is the segment indicator.                   */
    /********************************************************************/
    fread(&pSMFRecord->Header.SMFSEG ,1,2,fp);

    /********************************************************************/
    /* And then read the actual data, starting at the RECFLG field in   */
    /* the structure. The amount of data to read is given by the length */
    /* minus the 4 bytes of the RDW. Check that we have read the right  */
    /* amount.                                                          */
    /********************************************************************/
    nextLength = conv16(pSMFRecord->Header.SMFLEN) - 4;
    bytesRead = fread(&pSMFRecord->Header.SMFRECFLG, 1, nextLength , fp);
    if (bytesRead != nextLength)
    {
      printf("Error reading full record from input file\n");
      goto mod_exit;
    }
    offset = bytesRead + 4;

    /********************************************************************/
    /* If the segment indicator is non-zero, then we need to continue   */
    /* reading the next record into the same buffer.  There is no       */
    /* SMF header on the subsequent segments, it's just raw data.  So   */
    /* we read the length of the next partial record, and its segment   */
    /* value.  Then the data itself.  And loop until we get to          */
    /* end-of-record indicator in the segment field.  Regardless of     */
    /* segmentation, no SMF record is meant to be >32768 bytes, so we   */
    /* need to check that.  Once again, ignore the RDW when working     */
    /* out how much real data there is.                                 */
    /********************************************************************/
    if (pSMFRecord->Header.SMFSEG[0] != 0)
    {
      do
      {

        fread(&nextLength ,1,2,fp);
        nextLength = conv16(nextLength);
        fread(&pSMFRecord->Header.SMFSEG ,1,2,fp);

        if (offset+nextLength > sizeof(dataBuf))
        {
          printf("SMF record appears to be too large for buffer\n");
          goto mod_exit;
        }

        bytesRead = fread(&dataBuf[offset], 1, nextLength-4 , fp );
        offset += bytesRead;
      } while (pSMFRecord->Header.SMFSEG[0] != 0x02);/* end of record indicator*/

    }

    totalRecords++;

    /*********************************************************************/
    /* We are only interested in SMF 115 and SMF 116 records. Both of    */
    /* these always have subtypes, but assign a value even for           */
    /* unexpected types for debug purposes.                              */
    /*********************************************************************/
    recordType    = pSMFRecord->Header.SMFRECRTY;

    subTypesValid = (pSMFRecord->Header.SMFRECFLG & 0x02);
    if (subTypesValid)
      recordSubType = conv16(pSMFRecord->Header.SMFRECSTY);
    else
      recordSubType = 0xFF;

    /*********************************************************************/
    /* Some items from the record need to be shown for all types         */
    /* of structure. Save them into a shared block available for all.    */
    /*********************************************************************/
    memcpy(commonF.qMgr,convStr(pSMFRecord->Header.SMFRECSSID,4),4);
    memcpy(commonF.systemId,convStr(pSMFRecord->Header.SMFRECSID,4),4);
    memcpy(commonF.mqVer,convStr(pSMFRecord->Header.SMFRECREL,3),3);

    /*********************************************************************/
    /* The SMF header contains a date formatted as 0x0cyydddF The "ddd"  */
    /* is the day-of-the-year (1-365).  Extract the digits and convert   */
    /* into a decimal.                                                   */
    /*********************************************************************/
    d[0] = (pSMFRecord->Header.SMFRECDTE[2] & 0xF0) /16;
    d[1] = (pSMFRecord->Header.SMFRECDTE[2] & 0x0F);
    d[2] = (pSMFRecord->Header.SMFRECDTE[3] & 0xF0) /16;

    ddd= d[0]*100 + d[1]*10 + d[2];

    /*********************************************************************/
    /* Similarly extract the 3 digits that indicate year and century to  */
    /* get the actual year, counting from 1900.                          */
    /*********************************************************************/
    d[0] = (pSMFRecord->Header.SMFRECDTE[0] & 0x0F);
    d[1] = (pSMFRecord->Header.SMFRECDTE[1] & 0xF0) /16;
    d[2] = (pSMFRecord->Header.SMFRECDTE[1] & 0x0F);

    year = 1900 + 10*d[1] + d[2] + (d[0]*100);  /* d1 is 1 if > 2000*/

    /*********************************************************************/
    /* Extract the timestamp from the SMF header This is in number of    */
    /* hundredths of a second past midnight.  Need to convert to correct */
    /* endianness first.                                                 */
    /*********************************************************************/
    memcpy(&time,&(pSMFRecord->Header.SMFRECTME),4);
    time=conv32(time);
    {
      int mon, day;
      int hour, min,sec,hund;

      calcYMD(year, ddd, &mon, &day);

      hund = time % 100;
      time = time / 100;

      hour = (time / (60 * 60)) % 24;
      time = time - (hour * 60 * 60);
      min = (time / 60) % 60;
      time = time - (min * 60);
      sec = time % 60;
      sprintf(commonF.recordDate,"%4.4d/%02.2d/%02.2d",
        year,
        mon,
        day);

      /*******************************************************************/
      /* Spreadsheets seem to have a problem importing times             */
      /* when formatted just as hh:mm:ss.hh. Adding some trailing 0s     */
      /* sorts that out, as well as looking consistent with other places */
      /* where a full STCK is formatted.                                 */
      /*******************************************************************/
      sprintf(commonF.recordTime,"%02.2d:%02.2d:%02.2d,%02.2d0000",
        hour,
        min,
        sec,
        hund);
    }

    if (recordType == 116 || recordType == 115)
    {
      /*******************************************************************/
      /* The first triplet past the standard header points at a          */
      /* QWHS structure.                                                 */
      /*                                                                 */
      /* This has various pieces of information about the record.        */
      /* Most important here is to say how many data  sections are       */
      /* in it, which says how many further triplets should be           */
      /* used.                                                           */
      /*                                                                 */
      /* Also pick up the STCK value that may be useful                  */
      /* for sorting.                                                    */
      /*******************************************************************/
      p = &dataBuf[conv32(pSMFRecord->s[0].offset)];
      pqwhs = (qwhs *)p;
      sectionCount =  pqwhs->qwhsnsda[0];
      strcpy(commonF.stckFormat,convDate(pqwhs->qwhsstck));
      if (recordType == 115)
      {
        if (strncmp(commonF.mqVer,"701",3) > 0)
        {
          /* This structure changes size on different platforms because */
          /* of how the compiler deals with bitfields. So have to do it */
          /* explicitly with known offsets after the structure changes. */
          char *t = (char *)&(pqwhs->qwhsflag1);
          t+=4;
          memcpy(&commonF.intstart,t,8);
          t+=8;
          memcpy(&commonF.intduration,t,8);
        }
        else
        {
           /* These fields were added after V701 so ignore for old SMF */
           commonF.intstart = 0;
           commonF.intduration = 0;
        }
      }
    }
    else
      sectionCount = 0;

    /*********************************************************************/
    /* One we know how many sections there are, copy the triplet values  */
    /* into a local array, doing the endianness conversion on the way.   */
    /* That makes it look at bit easier rather than than having convxxx  */
    /* function calls everywhere else.                                   */
    /*********************************************************************/
    for (i=0;i<sectionCount;i++)
    {
      triplet[i].offset  = conv32(pSMFRecord->s[i].offset);
      triplet[i].l       = conv16(pSMFRecord->s[i].l);
      triplet[i].n       = conv16(pSMFRecord->s[i].n);
    }

    if (debugLevel >= 2)
      printDEBUG("FULL RECORD",dataBuf,offset);

    /*********************************************************************/
    /* And now we can start formatting the actual data.                  */
    /* For each subtype, we have to know (from the documentation) which  */
    /* triplet refers to which data structure. Not all structures have   */
    /* eyecatchers (strings or bytes) to define them; structures are     */
    /* not referenced from contiguous triplets. Some triplets on some    */
    /* subtypes are unused and others may refer to internal data. So     */
    /* despite the claims, this is not totally self-describing data.     */
    /*                                                                   */
    /* In theory all of these formatters should be using a loop through  */
    /* the triplet[].n values, but we know that some of the entries      */
    /* consist always of a single entry.                                 */
    /*********************************************************************/
    switch(recordType)
    {
    /*********************************************************************/
    /* Ignore these dump start/stop time records                         */
    /*********************************************************************/
    case 2:
    case 3:
      unknownCount++;
      break;

    /*********************************************************************/
    /* Processing 115 records                                            */
    /*********************************************************************/
    case 115:
      Count115[recordSubType]++;
      switch(recordSubType)
      {
      case 1:
        /*****************************************************************/
        /* Some of the triplets seem to refer to internal/undocumented   */
        /* structures. Only the QSST and QJST are known elements.        */
        /*****************************************************************/
        for (i=1;i<=11;i++)
        {
          if (sectionCount >=i && triplet[i].n != 0)
          {
            for(j = 0; j < triplet[i].n; j++)
            {
              p = &dataBuf[triplet[i].offset + triplet[i].l * j];
              switch(i)
              {
              case  9: printQSST((qsst *)p); break;
              case 11: printQJST((qjst *)p); break;
              default: break;
              }
            }
          }
        }
        break;

      case 2:
        /*****************************************************************/
        /* All of the triplets may be used, so we can just iterate       */
        /* through them and pick the right formatter for each.           */
        /* Note that we don't use the 0th element of the triplet array   */
        /* - that's already been used for the QWHS info.                 */
        /*****************************************************************/
        for (i=1;i<=8;i++)
        {
          if (sectionCount >=i && triplet[i].n != 0)
          {
            for(j = 0; j < triplet[i].n; j++)
            {
              p = &dataBuf[triplet[i].offset + triplet[i].l * j];
              switch(i)
              {
              case 1: printQMST((qmst *)p);break;
              case 2: printQIST((qist *)p);break;
              case 3: printQPST((qpst *)p);break;
              case 4: printQLST((qlst *)p);break;
              case 5: printQ5ST((q5st *)p);break;
              case 6: printQEST((qest *)p);break;
              case 7: printQTST((qtst *)p);break;
              case 8: printQESD((qesd *)p);break;
              default:break;
              }
            }
          }
        }
        break;

      case 5:
        for (i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQSPH((qsph *)p);
        }
        break;

      case 6:
        for (i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQSGM((qsgm *)p);
        }
        break;

      case 7:
        for (i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQSRS((qsrs *)p);
        }
        break;

      case 215:
        for(i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQPST((qpst *)p);
        }
        break;

      case 231:
        /*****************************************************************/
        /* All of the triplets may be used, so we can just iterate       */
        /* through them and pick the right formatter for each.           */
        /*****************************************************************/
        for (i=1;i<=5;i++)
        {
          if (sectionCount >=i && triplet[i].n != 0)
          {
            for(j = 0; j < triplet[i].n; j++)
            {
              p = &dataBuf[triplet[i].offset + triplet[i].l * j];
              switch(i)
              {
              case 1: printQCCT((qcct *)p);break;
              case 2: printQCTDSP((qct_dsp *)p);break;
              case 3: printQCTADP((qct_adp *)p);break;
              case 4: printQCTSSL((qct_ssl *)p);break;
              case 5: printQCTDNS((qct_dns *)p);break;
              default: break;
              }
            }
          }
        }
        break;

      default:
        sprintf(tmpHead,"Unknown SMF 115 subtype %d",recordSubType);
        printDEBUG(tmpHead, dataBuf,offset);
        printf("%s\n",tmpHead);
        break;
      }
      break;

    /*********************************************************************/
    /* Processing 116 records                                            */
    /*********************************************************************/
    case 116:
      Count116[recordSubType]++;
      switch(recordSubType)
      {
      case 0:
        /*****************************************************************/
        /* This is unlike the rest of the formatters in that we combine  */
        /* data from two elements (and the QWHC structure is overlaid on */
        /* the end of the QWHS, so we have to refer to it in an odd way).*/
        /*****************************************************************/
        p = &dataBuf[triplet[2].offset ];
        {
          char *t = (char *)&(pqwhs->qwhsflag1);
          t+=4;
          printQMAC((qmac *)p, (qwhc *)t);
        }
        break;

      case 1:
      case 2:
        p = &dataBuf[triplet[1].offset  ];
        printWTID((wtid *)p);

        /*****************************************************************/
        /* The queue records may or may not exist in subtype 1 records.  */
        /* The WTAS structure shows if they do.  They will always be     */
        /* there in subtype 2 records.  So we assign which triplet (if   */
        /* any) contains the offsets to the start of these queue         */
        /* records.                                                      */
        /*****************************************************************/
        qTriplet = 0;
        if (recordSubType == 1)
        {
          p = &dataBuf[triplet[2].offset  ];
          printWTAS((wtas *)p);

          if (conv32(((wtas *)p)->wtaswqct) > 0)
            qTriplet = 3;
        }
        else
        {
          qTriplet = 2;
        }

        /************************************************************/
        /* For each queue record (if there are any)                 */
        /************************************************************/
        if (qTriplet != 0)
        {
          for (i = 0;i < triplet[qTriplet].n; i++)
          {
            p = &dataBuf[triplet[qTriplet].offset + triplet[qTriplet].l * i];
            printWQ((wq *)p);
          }
        }
        break;


      case 10:
        for (i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQCST((qcst *)p);
        }
        break;

      default:
        sprintf(tmpHead, "Unknown subtype %d for 116 records");
        printDEBUG(tmpHead, dataBuf,offset);
        printf("%s\n",tmpHead);
        break;
      }
      break;


    /*********************************************************************/
    /* Other types are unknown and unexpected, so dump them to a         */
    /* debug file for validation.                                        */
    /*********************************************************************/
    default:
      unknownCount++;
      sprintf(tmpHead,"Unknown SMF record type %d at record %u",recordType,totalRecords);
      printDEBUG(tmpHead, dataBuf,offset);
      printf("%s\n",tmpHead);
    }

    if (totalRecords % ticker == 0)
      printf("Processed %u records\n",totalRecords);

  } while (0 != bytesRead && totalRecords < maxRecords);

  /***********************************************************************/
  /* Cleanup and exit.                                                   */
  /***********************************************************************/
mod_exit:

  fflush(NULL); /* Ensure all streams flushed if possible */


  printf("Processed %u records total\n",totalRecords);
  printf("  Unknown and ignored       record count: %u\n",unknownCount);
  for (i=0;i<256;i++)
  {
    if (Count115[i] > 0)
      printf("  Formatted 115 subtype %3d record count: %u\n",i,Count115[i]);
  }
  for (i=0;i<256;i++)
  {
    if (Count116[i] > 0)
      printf("  Formatted 116 subtype %3d record count: %u\n",i,Count116[i]);
  }

  exit(0);
}

/*************************************************************************/
/* FUNCTION: fopencsv/fopenext                                           */
/* PURPOSE:                                                              */
/*   Open the output files, printing error info if file cannot be        */
/*   opened/created. fopenext allows the caller to specify the filetype; */
/*   fopencsv enforces a .csv filetype.                                  */
/*   The files are given the name "SMF-<basename>.<filetype>             */
/*   If the file has just been created, regardless of the append mode,   */
/*   then we will want to know that.                                     */
/*************************************************************************/
FILE * fopencsv(const char * basename, BOOL *newFile)
{
  return fopenext(basename,"csv",newFile);
}

FILE * fopenext(const char * basename, const char *ext, BOOL *newFile)
{
  FILE  * fp = NULL;
  char filename[256] = {0};
  char *filetype;
  char *mode = (append)?"a":"w";

  snprintf(filename,sizeof(filename)-1,
      "%s/SMF-%s.%s",directory?directory:".",basename,ext);

  fp = fopen(filename, mode);
  if (fp)
  {
    long pos = ftell(fp);
    if (pos == 0) /* Have we just created the file, even for "append" mode */
      *newFile = TRUE;
    else
      *newFile = FALSE;
  }
  else
  {
    printf(" Cannot open file %s. Error \"%s\" (%d) \n",
      filename, strerror(errno),errno);
  }
  return (fp);
}


/*****************************************************************/
/* FUNCTION: mqgetopt                                            */
/* PURPOSE:                                                      */
/*   Not all platforms have a getopt function, so this is a      */
/*   simple reimplementation.                                    */
/*****************************************************************/
int mqgetopt(int argc, char *argv[], char *opts)
{
  static int sp = 1;
  int c;
  char *cp;

  if(sp == 1)  {
    if(mqoptind >= argc ||  argv[mqoptind][1] == '\0' ||
      (argv[mqoptind][0] != '-' && argv[mqoptind][0] != '/'))  {
      return(EOF);
    }
    else if(strcmp(argv[mqoptind], "--") == 0 ||
      strcmp(argv[mqoptind], "//") == 0)  {
      mqoptind++;
      return(EOF);
    }
  }

  mqoptopt = c = argv[mqoptind][sp];

  if(c == ':' || (cp=strchr(opts, c)) == NULL)  {
    if(argv[mqoptind][++sp] == '\0')  {
      mqoptind++;
      sp = 1;
    }
    return('?');
  }

  if(*++cp == ':')  {
    if(argv[mqoptind][sp+1] != '\0')  {
      mqoptarg = &argv[mqoptind++][sp+1];
    }
    else if(++mqoptind >= argc)  {
      sp = 1;
      return('?');
    }
    else  {
      mqoptarg = argv[mqoptind++];
    }

    sp = 1;
  }
  else  {
    if(argv[mqoptind][++sp] == '\0')  {
      sp = 1;
      mqoptind++;
    }
    mqoptarg = NULL;
  }
  return(c);
}

/*****************************************************************/
/* FUNCTION: Usage                                               */
/* PURPOSE:  Print command syntax and options                    */
/*****************************************************************/
static void Usage(void)
{
  printf("Usage: mqsmfcsv [-o <output dir>] [-a] [ -d <level> ]\n");
  printf("         [-h yes|no] [ -i <input file> [-m <max records>]\n");
  printf("         [-r ] [-t <ticker>]\n");
  printf("  -a               Append to files if they exist. Default is overwrite.\n");
  printf("  -d <Level>       Debug by dumping binary records (Level = 1 or 2).\n");
  printf("  -h yes|no        Print column headers for new output files. Default is yes.\n");
  printf("  -i <Input file>  Default is to read from stdin.\n");
  printf("  -m <Max records> End after formatting M records. Default to process all.\n");
  printf("  -o <Directory>   Where to put output files. Default to current directory.\n");
  printf("  -r               Do not print '=' on numeric-looking strings.\n");
  printf("  -t <Ticker>      Print progress message every T records.\n");
  return;
}
