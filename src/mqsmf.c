/*
 * Copyright (c) 2016,2018 IBM Corporation and other Contributors.
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
/*                 When downloading the SMF dump, it is preferred to  */
/*                 have the RDW (record descriptor word) included in  */
/*                 the file.                                          */
/*                 This is because the I/O on Windows and Unix        */
/*                 platforms does not understand the record-oriented  */
/*                 z/OS file format; instead we have to know how long */
/*                 each record is - given by the RDW. This is not     */
/*                 the default option on many file transfer programs. */
/*                 Check the documentation for your file manager on   */
/*                 how to include the RDW when downloading. If you    */
/*                 cannot get the RDW included, then this program now */
/*                 supports a mechanism that does not require it, but */
/*                 is slightly slower when formatting the data.       */
/*                                                                    */
/*                 Structures are defined as "packed" which means that*/
/*                 fields within a structure may not start on a       */
/*                 natural (eg word) boundary. Some platforms may need*/
/*                 additional compiler options to deal with that      */
/*                 without doing something like SIGILL.               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/* Define flags so that this 32-bit program can access 64-bit files   */
/* These flags need to be set up before including any other headers   */
/**********************************************************************/
#ifndef _WIN32
#define __USE_LARGEFILE64    /* for Linux */
#define _FILE_OFFSET_BITS 64 /* for Linux */
#define _LARGE_FILES         /* for AIX   */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys/stat.h>

/**********************************************************************/
/* Different ways of getting to 64-bit file offsets, and different    */
/* datatypes. Now the system headers have been loaded, define the     */
/* real functions and datatypes for the large values.                 */
/**********************************************************************/
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>

#define fstat   _fstat64
#define ftello  _ftelli64
#define fseeko  _fseeki64
typedef signed long long myoff_t;
typedef struct __stat64 mystat_t;
#else
typedef off_t myoff_t;
typedef struct stat mystat_t;
#endif

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

#include "mqsmf.h"

static void Usage();
void takeCheckPoint(char *,myoff_t);
myoff_t readCheckPoint(FILE *);
static char *getFormatRate(myoff_t pos);
static char *getFormatPercent(myoff_t totalFileSize,myoff_t pos);
static BOOL inconsistentVersions(char *v1,char *v2,int l);

#define DEFAULT_TICKER   10000         /* Print out status every N records */

/********************************************************************/
/* Definition of the global variables                               */
/********************************************************************/
int   debugLevel = 0;
BOOL  addEquals = TRUE;
BOOL  printHeaders = TRUE;
BOOL  useRDW = TRUE;
BOOL  streamOutput = FALSE;
BOOL  streamInput = TRUE;
char *ddlTemplateOpen = NULL;
char *ddlTemplateClose = NULL;
char *ddlQuote = "\"";
commonFields_t commonF = {0};
enum outputFormat_e outputFormat = OF_CSV;
FILE *infoStream;

unsigned int   recordType;
unsigned short   recordSubType;

/********************************************************************/
/* Structure and vars to control checkpoint recording and playback  */
/********************************************************************/
typedef struct {
  char *name;
  FILE *fp;
  myoff_t offset;
} checkPoint_t;

#define MAXCP (50) /* Maximum number of output files */
checkPoint_t checkPoint[MAXCP] = {NULL,NULL,0};
char *checkPointFileBaseName = "SMF-CKPT.txt";
char  checkPointFileName[PATH_MAX] = {0};
BOOL  checkPointTaken = FALSE;


/********************************************************************/
/* Variables scoped to this module                                  */
/********************************************************************/
static BOOL  append = FALSE;
static BOOL  resumeCheckPoint = FALSE;
static char *directory = NULL;
static unsigned int totalRecords = 0;
static unsigned int startingRecords = 0;
static myoff_t pos;

static time_t startTime = 0;
static char *formatRate;

static unsigned int Count115[256] = {0};             /* Max subtype is 255 */
static unsigned int Count116[256] = {0};

/********************************************************************/
/* Not all platforms have getopt, so use our own version. Prefix    */
/* the standard names with "mq" to keep distinct.                   */
/********************************************************************/
static int   mqgetopt(int, char **, char *);
static int   mqoptind = 1;             /* getopt index                     */
static int   mqoptopt;                 /* getopt option                    */
static char* mqoptarg;                 /* getopt argument                  */

static BOOL debugBREAK = FALSE;

/********************************************************************/
/* MAIN                                                             */
/*   See the Usage function for parameters and syntax               */
/********************************************************************/
int main( int argc, char *argv[] )
{

  SMFRecord_t *pSMFRecord;
  int ticker = DEFAULT_TICKER;
  int i,j;                             /* loop counters                    */
  char  dataBuf[MAX_SMF_DATA];         /* Contains the SMF data            */
  int   bytesRead;                     /* Number of bytes from fread()     */
  int   offset;                        /* total number of bytes in a record*/
  unsigned int d[3];                   /* used in date conversion          */
  unsigned int ddd,year;               /* day number and year number       */
  unsigned int smfTime;                   /* Copied from the SMF header       */
  char savedMqVer[3] = {0};

  char *inputFile = NULL;
  FILE *fp;
  char *b;
  char *basename;
  myoff_t currentOffset = 0;
  myoff_t startingOffset = 0;
  myoff_t totalFileSize = 0;
  int     seekRc = 0;
  mystat_t statbuf;
  unsigned int maxRecords = 0xFFFFFFFF;
  unsigned int unknownCount = 0;
  unsigned int ignoredCount = 0;
  unsigned short nextLength;
  int subTypesValid = 0;
  int qTriplet;
  char *p;
  qwhs *pqwhs;
  unsigned char correlid[16];
  int offsetCorrection;

  BOOL error = FALSE;
  BOOL knownSubType = TRUE;
  BOOL formatWarning = FALSE;
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
      (sizeof(int *) != 4) ||
      (sizeof(int) != 4))
  {
    fprintf(stderr,"Data type sizes do not match requirements.\n");
    fprintf(stderr,"Need to rebuild program with correct options.\n");
    fprintf(stderr,"Here: short=%d int=%d long=%d long long=%d ptr=%d bytes\n",
       (int)sizeof(short), (int)sizeof(int), (int)sizeof(long), (int)sizeof(long long),(int)sizeof(int *));
    fprintf(stderr,"Need: short=%d int=%d long=%d long long=%d ptr=%d bytes\n",2,4,4,8,4);
    exit(1);
  }


  infoStream=stdout;


  /******************************************************************/
  /* Parse command-line parameters                                  */
  /******************************************************************/
  while((c = mqgetopt(argc, argv, "ab:cd:e:f:h:i:m:o:p:rst:v")) != EOF)
  {
    switch(c)
    {
      case 'a':
        append = TRUE;
        break;
      case 'b': /* 'b' for dataBase */
        for (i=0;i<strlen(mqoptarg);i++)
          mqoptarg[i] = toupper(mqoptarg[i]);
        if (!strcmp(mqoptarg,"MYSQL")) {
          ddlQuote="`";
          ddlTemplateOpen = "-";
        } else if (!strcmp(mqoptarg,"DB2")) {
          ddlQuote="\"";
        } else {
          error = TRUE;
        }
        break;
      case 'c':
        resumeCheckPoint = TRUE;
        append = TRUE;
        break;
      case 'd':
        debugLevel = atoi(mqoptarg);
        break;
      case 'e':
         ddlTemplateClose = mqoptarg;
         break;
      case 'f':
        for (i=0;i<strlen(mqoptarg);i++)
          mqoptarg[i] = toupper(mqoptarg[i]);
        if (strstr(mqoptarg,"NORDW"))
          useRDW = FALSE;
        else if (strstr(mqoptarg,"RDW"))
          useRDW = TRUE;
        if (strstr(mqoptarg,"JSON"))
          outputFormat = OF_JSON;
        else if (strstr(mqoptarg,"SQL")) {
          outputFormat = OF_SQL;
          printHeaders = FALSE;
          addEquals=0;
        } else if (strstr(mqoptarg,"CSV"))
          outputFormat = OF_CSV;
        break;
      case 'h':
        for (i=0;i<strlen(mqoptarg);i++)
          mqoptarg[i] = toupper(mqoptarg[i]);
        if (!strcmp(mqoptarg,"NO"))
          printHeaders = FALSE;
        break;
      case 'i':
        inputFile = mqoptarg;
        streamInput = FALSE;
        break;
      case 'm':
        maxRecords = atoi(mqoptarg);
        break;
      case 'o':
        directory = mqoptarg;
        if (!strcmp(directory,"-"))
        {
          infoStream = stderr;
          streamOutput=TRUE;
        }
        break;
      case 'p': /* 'p' for Pool ... "create bufferpool" is default cmd */
        ddlTemplateOpen = mqoptarg;
        break;
      case 'r':
        addEquals = 0;
        break;
      case 's':
        outputFormat = OF_SQL;
        printHeaders = FALSE;
        addEquals = 0;
        fprintf(stderr,"The -s option is now deprecated. Please use '-f sql' in future.\n");
        break;
      case 't':
        ticker     = atoi(mqoptarg);
        break;
      case 'v':
         checkStructureSizes(infoStream);
         exit(0);
      default:
        error = TRUE;
        break;
    }
  }

  fprintf(infoStream,"MQ SMF CSV - Build %s %s\n",__DATE__,__TIME__);

  /******************************************************************/
  /* Were there any problems parsing the parameters.                */
  /* Any remaining parameters are taken as an error                 */
  /******************************************************************/
  if (error || mqoptind<argc)
  {
    Usage();
    exit(1);
  }

  /* Set up any special debug processing */
  if (getenv("MQSMFCSV_BREAK")) {
    debugBREAK=TRUE;
  }

  /*********************************************************************/
  /* Open the input file                                               */
  /* If this fails, then put out an error message and quit. If no      */
  /* filename was given on command line, use stdin.                    */
  /* Windows requires the "b" option for binary input. And force       */
  /* stdin to binary in case we need to read from that too.            */
  /*********************************************************************/
  if (!inputFile && !useRDW)
  {
    fprintf(stderr,"Cannot use NORDW format input via stdin piping\n");
    exit(1);
  }

  #ifdef _WIN32
  setmode(fileno(stdin), O_BINARY);
  #endif
  fp = inputFile?fopen(inputFile, "rb" ):stdin;
  if (!fp)
  {
    fprintf(stderr," Cannot open input file %s. Error \"%s\" (%d)\n",
      inputFile,strerror(errno),errno);
    goto mod_exit;
  }
  /* Make sure we've got a printable name */
  if (!inputFile)
  {
    inputFile = "<<stdin>>";
  }

  fstat(fileno(fp),&statbuf);
  totalFileSize = statbuf.st_size;
  if (debugLevel >=3 )
    fprintf(infoStream,"Total File Size = %lld\n",totalFileSize);

  convInit();      /* Decide whether this is a big or little endian machine*/

  b = strrchr(inputFile,'/');
  if (!b)
    b = strrchr(inputFile,'\\');
  if (!b)
    basename = inputFile;
  else
    basename = b+1;
  fprintf(infoStream,"Input file: %s. Format: %s.\n",basename, useRDW?"RDW":"Without RDW");

  /********************************************************************/
  /* Open the checkpoint file for reading if we have been asked to do */
  /* recovery. Create the filename first as we use it in the writer   */
  /* too.                                                             */
  /********************************************************************/
  snprintf(checkPointFileName,sizeof(checkPointFileName)-1,
      "%s/%s",directory?directory:".",checkPointFileBaseName);
  if (resumeCheckPoint) {
    FILE *checkPointFP = fopen(checkPointFileName,"r");
    if (!checkPointFP)
    {
      fprintf(stderr," Cannot open checkpoint file %s. Error \"%s\" (%d)\n",
        checkPointFileBaseName,strerror(errno),errno);
      goto mod_exit;
    }
    startingOffset = readCheckPoint(checkPointFP);
    fclose(checkPointFP);
  }

  /********************************************************************/
  /* Loop until we have no more data or enough records have been read */
  /********************************************************************/
  startTime = time(&startTime);
  currentOffset = startingOffset;
  totalRecords = startingRecords;
  seekRc = (currentOffset!=0)?fseeko(fp,currentOffset,SEEK_SET):0;
  if (seekRc == -1) {
    fprintf(stderr,"Cannot move to correct offset for input file - error %s (%d)\n",strerror(errno),errno);
    goto mod_exit;
  }

  pSMFRecord = (SMFRecord_t *)&dataBuf;
  do
  {
    /**********************************************************************/
    /* The mechanism for reading the file depends on whether the RDW      */
    /* field is available. If it is, we know how long to read for each    */
    /* record. If it is not, then we read the maximum length each time and*/
    /* reset the pointer for the next read once we have parsed the record.*/
    /**********************************************************************/
    memset(dataBuf,0,sizeof(dataBuf));
    if (useRDW)
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
      if (debugLevel >=3)
        fprintf(infoStream,"   NextLen = %d bytes \n",nextLength);

      bytesRead = fread(&pSMFRecord->Header.SMFRECFLG, 1, nextLength , fp);
      if (bytesRead != nextLength)
      {
        fprintf(stderr,"Error reading full record from input file\n");
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

          if (debugLevel >=3)
            fprintf(infoStream,"   NextLen = %d bytes \n",nextLength);

          if (offset+nextLength > sizeof(dataBuf))
          {
            fprintf(stderr,"SMF record appears to be too large for buffer\n");
            goto mod_exit;
          }

          bytesRead = fread(&dataBuf[offset], 1, nextLength-4 , fp );
          offset += bytesRead;
        } while (pSMFRecord->Header.SMFSEG[0] != 0x02);/* end of record indicator*/

      }
      offsetCorrection = 0;
    }
    else
    {
      seekRc = fseeko(fp,currentOffset,SEEK_SET);
      if (seekRc == -1) {
        fprintf(stderr,"Cannot move to correct offset for input file - error %s (%d)\n",strerror(errno),errno);
        goto mod_exit;
      }
      bytesRead = fread(&pSMFRecord->Header.SMFRECFLG,1,32768,fp);
      offset = bytesRead;
      offsetCorrection = 4;
      if (bytesRead <= 0)
        break;
    }

    totalRecords++;

    /*********************************************************************/
    /* We are only interested in SMF 115 and SMF 116 records. Both of    */
    /* these always have subtypes, but assign a value even for           */
    /* unexpected types for debug purposes.                              */
    /*********************************************************************/
    recordType    = pSMFRecord->Header.SMFRECRTY;

    /*********************************************************************/
    /* zOS refers to bits from the left - reversed from what you might   */
    /* expect. So "bit 1" indicating subtypes is '0100 0000' == 0x40     */
    /*********************************************************************/
    subTypesValid = ((pSMFRecord->Header.SMFRECFLG & 0x40) == 0x40);
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
    memcpy(&smfTime,&(pSMFRecord->Header.SMFRECTME),4);
    smfTime=conv32(smfTime);
    {
      int mon, day;
      int hour, min,sec,hund;
      char *sep;
      calcYMD(year, ddd, &mon, &day);

      hund = smfTime % 100;
      smfTime = smfTime / 100;

      hour = (smfTime / (60 * 60)) % 24;
      smfTime = smfTime - (hour * 60 * 60);
      min = (smfTime / 60) % 60;
      smfTime = smfTime - (min * 60);
      sec = smfTime % 60;

      if (outputFormat == OF_SQL)
        sep = "-";
      else
        sep = "/";

      sprintf(commonF.recordDate,"%4.4d%s%02.2d%s%02.2d",
        year,
        sep,
        mon,
        sep,
        day);

      /*******************************************************************/
      /* Spreadsheets seem to have a problem importing times             */
      /* when formatted just as hh:mm:ss.hh. Adding some trailing 0s     */
      /* sorts that out, as well as looking consistent with other places */
      /* where a full STCK is formatted.                                 */
      /*******************************************************************/
      sprintf(commonF.recordTime,"%02.2d:%02.2d:%02.2d%s%02.2d0000",
        hour,
        min,
        sec,
        (outputFormat==OF_JSON)?".":",",
        hund);
    }

    if (recordType == 116 || recordType == 115)
    {
      if (savedMqVer[0] == 0)
        memcpy(savedMqVer,commonF.mqVer,3);

      if (inconsistentVersions(commonF.mqVer,savedMqVer,3)) {
        fprintf(stderr,"Warning: Data contains records from multiple versions of MQ - %3.3s and %3.3s\n",commonF.mqVer,savedMqVer);
      }

      /*******************************************************************/
      /* The first triplet past the standard header usually points at a  */
      /* QWHS structure.                                                 */
      /*                                                                 */
      /* This has various pieces of information about the record.        */
      /* Most important here is to say how many data  sections are       */
      /* in it, which says how many further triplets should be           */
      /* used. If there is no valid QWHS, then we assume that there      */
      /* is only one further triplet so force that value.                */
      /*                                                                 */
      /* Also pick up the STCK value that may be useful                  */
      /* for sorting.                                                    */
      /*******************************************************************/
      p = &dataBuf[conv32(pSMFRecord->s[0].offset)];
      if (conv16(pSMFRecord->s[0].l) == 4)           /* There is no QWHS */
      {
        sectionCount = 2;    /* 1 extra triplet beyond the QWHS location */
        pqwhs = NULL;
        commonF.intstart = 0;
        commonF.intduration = 0;
      }
      else
      {
        char *dt[2];
        pqwhs = (qwhs *)p;
        sectionCount =  pqwhs->qwhsnsda[0];
        convDate(pqwhs->qwhsstck,dt);
        strcpy(commonF.stckFormatDate,dt[0]);
        strcpy(commonF.stckFormatDate,dt[1]);
        if (recordType == 115)
        {
          if (conv16(pqwhs->qwhslen) >= 52)
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
    }
    else
    {
      sectionCount = 0;
      pqwhs = NULL;
    }

    if (debugLevel >=3 && pqwhs != NULL)
       fprintf(infoStream,"Section count %d for %4.4s, qwhslen=%d\n",sectionCount,commonF.qMgr,conv16(pqwhs->qwhslen));

    /*********************************************************************/
    /* One we know how many sections there are, copy the triplet values  */
    /* into a local array, doing the endianness conversion on the way.   */
    /* That makes it look a  bit easier rather than than having convxxx  */
    /* function calls everywhere else.                                   */
    /* We do not need to correct for the offset in non-RDW files as that */
    /* space is still allocated at the front of the buffer.              */
    /*********************************************************************/
    {
      int highestOffset = 0;
      int h = -1;
      int recLength = 0;
      memset(triplet,0,sizeof(triplet));
      for (i=0;i<sectionCount;i++)
      {
        triplet[i].offset  = conv32(pSMFRecord->s[i].offset);
        triplet[i].l       = conv16(pSMFRecord->s[i].l);
        triplet[i].n       = conv16(pSMFRecord->s[i].n);
        if (triplet[i].offset > highestOffset &&
           triplet[i].offset > 0 &&
           triplet[i].n > 0)
        {
          highestOffset = triplet[i].offset;
          h = i;
        }
      }
      if (h < 0) {
        if (subTypesValid)
        {
          recLength = sizeof(SMFHeader_t) - offsetCorrection;
        }
        else
        {
          recLength = offsetof(SMFHeader_t,SMFRECSSID) - offsetCorrection;
        }
      } else {
        recLength = triplet[h].offset + triplet[h].l * triplet[h].n - offsetCorrection;
      }
      currentOffset += recLength;

      if (debugLevel >= 3)
      {
        fprintf(infoStream,"Highest triple = %d RecLength = %d New Offset = %lld\n",
          h, recLength,currentOffset);
      }

      if (debugLevel >= 2)
        printDEBUG("FULL RECORD",dataBuf + offsetCorrection,recLength);
    }


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
    knownSubType = TRUE;
    switch(recordType)
    {
    /*********************************************************************/
    /* Ignore these dump start/stop time records                         */
    /*********************************************************************/
    case 2:
    case 3:
      ignoredCount++;
      break;

    /*********************************************************************/
    /* Processing 115 records                                            */
    /*********************************************************************/
    case 115:
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

#ifdef QIS1IDV
     /* Pageset statistics introduced in MQ V9 - only compile this block if using */
     /* the V9 header file.                                                       */
     case 201:
        for(i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQIS1((qis1 *)p);
        }
        break;
#endif

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
              /* If nothing has been done with DNS in this interval, the
                 record seems to be present but contain garbage.
               */
              case 5: if (triplet[i].l == sizeof(qct_dns))
                          printQCTDNS((qct_dns *)p);
                      break;
              default: break;
              }
            }
          }
        }
        break;

      default:
        knownSubType = FALSE;
        sprintf(tmpHead,"Unknown SMF 115 subtype %d",recordSubType);
        printDEBUG(tmpHead, dataBuf,offset);
        fprintf(infoStream,"%s\n",tmpHead);
        break;
      }
      if (knownSubType)
        Count115[recordSubType]++;
      break;

    /*********************************************************************/
    /* Processing 116 records                                            */
    /*********************************************************************/
    case 116:
      switch(recordSubType)
      {
      case 0:
        /*****************************************************************/
        /* This is unlike the rest of the formatters in that we combine  */
        /* data from two elements (and the QWHC structure is overlaid on */
        /* the end of the QWHS, so we have to refer to it in an odd way).*/
        /*                                                               */
        /* There are also QWAC structures defined, but never seem to be  */
        /* used, so we do not have a formatter for them.                 */
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
        memset(correlid,0,16);

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
          /* The wtasstrt is 8 bytes but the following fields combine to */
          /* make a default correlation token.                           */
          memcpy(correlid,&((wtas *)p)->wtasstrt,16);

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
            memcpy(correlid,((wq *)p)->correl,16);
          }
        }
        p = &dataBuf[triplet[1].offset  ];
        printWTID((wtid *)p,correlid);

        break;


      case 10:
        for (i = 0; i < triplet[1].n; i++)
        {
          p = &dataBuf[triplet[1].offset + triplet[1].l * i];
          printQCST((qcst *)p);
        }
        break;

      default:
        knownSubType = FALSE;
        sprintf(tmpHead, "Unknown subtype %d for 116 records",recordSubType);
        printDEBUG(tmpHead, dataBuf,offset);
        fprintf(infoStream,"%s\n",tmpHead);
        break;
      }
      if (knownSubType)
        Count116[recordSubType]++;
      break;


    /*********************************************************************/
    /* Other types are unknown and unexpected, so dump them to a         */
    /* debug file for validation.                                        */
    /*********************************************************************/
    default:
      unknownCount++;
      sprintf(tmpHead,"Unknown SMF record type %d at record %u",recordType,totalRecords);
      printDEBUG(tmpHead, dataBuf,offset);
      fprintf(infoStream,"%s\n",tmpHead);
      if (offset > 32768 && !formatWarning) {
        fprintf(infoStream,"WARNING: Possible incorrect input format. Check use of RDW/NORDW flag.\n");
        formatWarning = TRUE;
      }
    }

    if (totalRecords % ticker == 0 && totalRecords > startingRecords)
    {
      pos = ftello(fp);
      fprintf(infoStream,"Processed %u records %s %s\n",totalRecords,
        getFormatPercent(totalFileSize,pos), getFormatRate(pos));
      if (!streamOutput && outputFormat != OF_JSON)
        takeCheckPoint(checkPointFileName,pos);
    }

    if (debugBREAK) /* for testing purposes */
    {
      /* Want to force an abend at some point after a checkpoint */
      if (totalRecords % ticker == 3 && checkPointTaken)
      {
        fprintf(infoStream,"Testing: Exiting after %d records processed\n",totalRecords);
        exit(0);
      }
    }
  } while (0 != bytesRead && totalRecords < maxRecords);

  /***********************************************************************/
  /* Cleanup and exit. If we get here normally, then the checkpoint      */
  /* file is not needed any more, so it is deleted.                      */
  /***********************************************************************/
  if (debugLevel >= 1) {
    fprintf(infoStream,"Removing checkpoint file\n");
  }
  remove(checkPointFileName);
  pos = ftello(fp);

  if (outputFormat == OF_SQL && ddlTemplateClose != NULL) {
       closeFinalDDL();
  }

mod_exit:

  fflush(NULL); /* Ensure all streams flushed if possible */

  for (i=0;i<MAXCP;i++) {
    if (checkPoint[i].fp) {
      fclose(checkPoint[i].fp);
    }
  }

  formatRate = getFormatRate(pos);
  fprintf(infoStream,"Processed %u records total %s\n",totalRecords,formatRate);


  /**********************************************************************/
  /* If the program has been restarted via checkpoint, these counts are */
  /* not accurate, so don't bother printing them                        */
  /**********************************************************************/
  if (!resumeCheckPoint) {
    if (unknownCount >0)
      fprintf(infoStream,"  Unknown                   record count: %u\n",unknownCount);
    if (ignoredCount >0)
      fprintf(infoStream,"  Ignored                   record count: %u\n",ignoredCount);
    for (i=0;i<256;i++)
    {
      if (Count115[i] > 0)
        fprintf(infoStream,"  Formatted 115 subtype %3d record count: %u\n",i,Count115[i]);
    }
    for (i=0;i<256;i++)
    {
      if (Count116[i] > 0)
        fprintf(infoStream,"  Formatted 116 subtype %3d record count: %u\n",i,Count116[i]);
    }
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
/*                                                                       */
/*   Use any recovered checkpoint information to set the file pointer    */
/*   to the correct place so we start writing where we left off before.  */
/*   If no checkpoint data is found for this particular file, then make  */
/*   sure we've got a structure to save it on this run.                  */
/*************************************************************************/
FILE * fopencsv(const char * basename, BOOL *newFile)
{
  int i;
  BOOL foundGap = FALSE;
  int seekRc;

  FILE * fp = fopenext(basename,"csv",newFile);

  if (!streamOutput) {
    for (i=0; i<MAXCP;i++) {
      if (checkPoint[i].name != NULL && strcmp(checkPoint[i].name,basename)==0) {
         checkPoint[i].fp = fp;
         seekRc = fseeko(fp,checkPoint[i].offset,SEEK_SET);
         if (seekRc == -1) {
           fprintf(stderr,"Cannot set recovery location for file %s",basename);
           exit(1);
         }
         return fp;
      }
    }

    /* Get here if we've not read an offset from the checkpoint recovery file */
    for (i=0;i<MAXCP;i++) {
      if (checkPoint[i].name == NULL) {
        checkPoint[i].name = strdup(basename);
        checkPoint[i].fp = fp;
        checkPoint[i].offset = 0;
        foundGap = TRUE;
        break;
      }
    }
    if (!foundGap) {
      fprintf(stderr,"Need to increase MAXCP and rebuild.\n");
      exit(1);
    }
  }
  return fp;
}

FILE * fopenext(const char * basename, const char *ext, BOOL *newFile)
{
  FILE  * fp = NULL;
  char filename[PATH_MAX] = {0};
  char *mode = (append)?"a":"w";

  if (streamOutput)
    return stdout;

  if (resumeCheckPoint && append && !strcmp(ext,"csv"))
    mode = "r+";

  snprintf(filename,sizeof(filename)-1,
      "%s/SMF-%s.%s",directory?directory:".",basename,ext);

  fp = fopen(filename, mode);
  if (fp)
  {
    myoff_t pos;

    fseek(fp,0,SEEK_END);
    pos = ftello(fp);
    if (debugLevel > 0) {
      setbuf(fp,0);  /* useful to have this line when debugging */
    }

    if (pos == 0) /* Have we just created the file, even for "append" mode */
      *newFile = TRUE;
    else
      *newFile = FALSE;
  }
  else
  {
    fprintf(stderr," Cannot open file %s. Error \"%s\" (%d) \n",
      filename, strerror(errno),errno);
  }
  return (fp);
}

/*************************************************************************/
/* FUNCTION: takeCheckPoint                                              */
/* PURPOSE: A very simple recovery mechanism in case the formatter       */
/*   needs to be stopped during the processing of large input.           */
/*   The current location in the input file is recorded, followed by     */
/*   the offsets we are writing to in each of the output files.          */
/*************************************************************************/
void takeCheckPoint(char *f, myoff_t pos) {
  int i,b;
  FILE *fp;

  if (debugLevel >=1) {
    fprintf(infoStream,"Taking checkpoint\n");
  }

  /********************************************************************/
  /* Reopen the file (with truncation) always.                        */
  /********************************************************************/
  fp = fopen(f,"w+");
  if (!fp)
  {
    fprintf(stderr," Cannot open checkpoint file %s. Error \"%s\" (%d)\n",
       f,strerror(errno),errno);
    exit(1);
  }

  /********************************************************************/
  /* Record offset and current status for the input file              */
  /********************************************************************/
  b = fprintf(fp, "%d %lld\n",totalRecords,pos);
  for (i=0;i<MAXCP;i++) {
    /* Record name and offset for all the output files */
    if (checkPoint[i].name) {
      b = fprintf(fp,"%s %lld\n",checkPoint[i].name,ftello(checkPoint[i].fp));
    }
  }
  fflush(fp);
  fclose(fp);
  checkPointTaken = TRUE; /* This is used during testing */
  return;
}

/*************************************************************************/
/* FUNCTION: readCheckPoint                                              */
/* PURPOSE: Work out where we were on the previous execution of the      */
/*   formatter. It causes us to reset the input file and all of the      */
/*   output streams to wherever we had got to.                           */
/*   There is not a lot of validation on the input format of the file so */
/*   if it goes wrong, you have to restart the formatting from the       */
/*   beginning.                                                          */
/*************************************************************************/
myoff_t readCheckPoint(FILE *fp) {
  myoff_t inputOffset = 0;
  char *c = NULL;
  char line[128];
  int i = 0;

  memset(line,0,sizeof(line));
  c = fgets(line,sizeof(line)-1,fp);
  if (!c) {
    fprintf(stderr,"Error reading from recovery checkpoint file\n");
    exit(1);
  }

  /************************************************************************/
  /* First line has two fields for the input file starting state          */
  /************************************************************************/
  sscanf(line,"%d %lld",&startingRecords,&inputOffset);
  fprintf(infoStream,"Reading from recovery checkpoint.\n");

  if (debugLevel >=1) {
    fprintf(infoStream,"InputOffset: %lld Processed records: %d\n",inputOffset, startingRecords);
  }

  /************************************************************************/
  /* Subsequent lines have two fields naming the output structure and how */
  /* far it's been written.                                               */
  /************************************************************************/
  do {
    memset(line,0,sizeof(line));
    c = fgets(line,sizeof(line)-1,fp);
    if (c) {
      char field[128];
      myoff_t offset;
      sscanf(line,"%s %lld",field,&offset);
      checkPoint[i].name = strdup(field);
      checkPoint[i].offset = offset;
      if (debugLevel >=2) {
        fprintf(infoStream,"Loaded status[%d] for %s as offset %lld\n",i,field,offset);
      }
      i++;
    }
  } while (c);
  return inputOffset;
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
  fprintf(infoStream,"Usage: mqsmfcsv [-o <output dir>] [-a] [ -d <level> ]\n");
  fprintf(infoStream,"         [-h yes|no] [ -i <input file> [-m <max records>]\n");
  fprintf(infoStream,"         [-f RDW | NORDW | JSON | SQL | CSV ] \n");
  fprintf(infoStream,"         [-b Db2 | MySQL ] \n");
  fprintf(infoStream,"         [-p <template DDL file prefix>  ] \n");
  fprintf(infoStream,"         [-e <template DDL file ending>  ] \n");
  fprintf(infoStream,"         [-r] [-c] [-t <ticker>]\n");
  fprintf(infoStream,"  -a               Append to files if they exist. Default is overwrite.\n");
  fprintf(infoStream,"  -b <Database>    Database DDL format can be Db2 or MySQL. Default is Db2.\n");
  fprintf(infoStream,"  -c               Recover after aborted run by using the checkpoint.\n");
  fprintf(infoStream,"  -d <Level>       Debug by dumping binary records (Level = 1 or 2).\n");
  fprintf(infoStream,"  -f               File formats. Default to RDW for input, CSV for output.\n");
  fprintf(infoStream,"  -h yes|no        Print column headers for new output files. Default is yes.\n");
  fprintf(infoStream,"  -i <Input file>  Default is to read from stdin.\n");
  fprintf(infoStream,"  -m <Max records> End after formatting M records. Default to process all.\n");
  fprintf(infoStream,"  -o <Directory>   Where to put output files. Default to current directory.\n");
  fprintf(infoStream,"                   Use '-' to send output to stdout.\n");
  fprintf(infoStream,"  -p <file>        Add contents of <file> to start of generated DDL.\n");
  fprintf(infoStream,"                   Set to '-' to bypass default commands.\n");
  fprintf(infoStream,"  -e <file>        Add contents of <file> to end of generated DDL.\n");
  fprintf(infoStream,"  -r               Do not print '=' on numeric-looking strings.\n");
  fprintf(infoStream,"  -s               (Deprecated) SQL mode - generate DDL for tables.\n");
  fprintf(infoStream,"  -t <Ticker>      Print progress message every T records.\n");
  return;
}

static char formatRateString[64];
static char *getFormatRate(myoff_t pos)
{
   time_t currentTime = time(NULL);
   time_t elapsedTime = currentTime - startTime;
   char *units;

   if (streamInput) {
     strcpy(formatRateString,"");
   } else if (elapsedTime == 0) {
     strcpy(formatRateString,"at too fast to count");
   } else {
     long long formatRate = pos / elapsedTime;
     if (formatRate > (1024 * 1024)) {
       units="MB";
       sprintf(formatRateString, "at %lld %s/sec",formatRate/(1024*1024),units);
     } else if (formatRate > 1024) {
       units="kB";
       sprintf(formatRateString, "at %lld %s/sec",formatRate/(1024),units);
     } else {
       units="bytes";
       sprintf(formatRateString, "at %lld %s/sec",formatRate,units);
     }
   }
   return formatRateString;
}
static char formatPercentString[10];
static char *getFormatPercent(myoff_t totalFileSize,myoff_t pos)
{
  if (streamInput) {
    strcpy(formatPercentString,"");
  } else {
    sprintf(formatPercentString,"[%5.2f%%]", (float)(100.0*pos)/totalFileSize);
  }
  return formatPercentString;
}

/*****************************************************************/
/* FUNCTION: inconsistentVersions                                */
/* PURPOSE:                                                      */
/*   There may be times where the MQ versions in the data would  */
/*   cause problems with missing header or columns (eg fields    */
/*   put into a later version, when we have already printed the  */
/*   headers for an older version). So this test will be able    */
/*   to look for that.                                           */
/*                                                               */
/*   But for now, always return FALSE.                           */
/*****************************************************************/
static BOOL inconsistentVersions(char *v1,char *v2,int l)
{
  BOOL rc = FALSE;
  if (memcmp(v1,v2,l) != 0) /* Maybe need to do more about specific version comparisons */
    rc= TRUE;
  return FALSE;
}

