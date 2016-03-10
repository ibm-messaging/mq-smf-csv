#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;

static char *verb[] = {
  "Open",
  "Close",
  "Get",
  "Put",
  "Put1"
};

static char *type[] = {
  "Lock"            ,
  "Unlock"          ,
  "WrLCControls"    ,
  "SigUsingXCF"     ,
  "SigUsingCF"      ,
  "Read"            ,
  "Write"           ,
  "StartMon"        ,
  "StopMon"         ,
  "Unused"          ,
  "New"             ,
  "Move"            ,
  "MoveEntry"       ,
  "Delete"          ,
  "Unused"          ,
  "Unused"          ,
  "Unused"          ,
  "Unused"          ,
  "Unused"          ,
  "Unused"
};

/**********************************************************/
/* Print out the contents of the WQ block                 */
/**********************************************************/
void printWQ(wq *p)
{
  char index[64];
  int i,j;

  SMFPRINTSTART("WQ", p, conv16(p->wqll));

  ADDSTR  ("CORREL " ,convBin(p->correl, 16));
  ADDSTREN("OBJNAME" ,p->objname,48);
  ADDSTREN("BASENAME",p->basename,48);
  ADDTIME ("OPENTIME",p->opentime);
  ADDTIME ("CLOSETIME",p->clostime);

  /*--------------------------------------------------------*/
  /* Object information                                     */
  /*--------------------------------------------------------*/
  ADDSTR("QTYPE   ",strMQQT(conv32(p->qtype)));
  ADDSTR("INDXTYPE",strMQIT(conv32(p->indxtype)));
  ADDSTR("QSGDISP ",strMQQSGD(conv32(p->qsgdisp)));

  /*--------------------------------------------------------*/
  /* Open   - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("OPENET", p->openet);
  ADDSTCK("OPENCT", p->openct);
  ADDU32 ("OPENN ", p->openn);
  /*--------------------------------------------------------*/
  /* Close  - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("CLOSEET", p->closeet);
  ADDSTCK("CLOSECT", p->closect);
  ADDU32 ("CLOSEN " ,p->closen);

  /*--------------------------------------------------------*/
  /* Get    - Information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("GETET", p->getet);
  ADDSTCK("GETCT", p->getct);
  ADDU32 ("GETN   ",p->getn);
  ADDU32 ("GETBRWA",p->getbrwa);
  ADDU32 ("GETBRWS",p->getbrws);
  ADDU32 ("GETA   ",p->geta);
  ADDU32 ("GETS   ",p->gets);
  ADDU32 ("GETERR ",p->geterr);
  ADDU32 ("GETDVAL",p->getdval);
  ADDSTCK("GETJWET",p->getjwet);
  ADDSTCK("GETJCET",p->getjcet);
  ADDU32 ("GETJWN ",p->getjwn);
  ADDU32 ("GETJCN ",p->getjcn);
  ADDSTCK("GETPSET",p->getpset);
  ADDU32 ("GETPSN ",p->getpsn);
  ADDSTCK("GETSUSET",p->getsuset);
  ADDU32 ("GETSUSN ",p->getsusn);
  ADDU32 ("GETEPAGE",p->getepage);
  ADDU32 ("GETSMSG ",p->getsmsg);
  ADDU32 ("GETEXMSG",p->getexmsg);

  /*--------------------------------------------------------*/
  /* Put    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("PUTET", p->putet);
  ADDSTCK("PUTCT", p->putct);
  ADDU32 ("PUTN   ",p->putn);
  ADDU32 ("PUTPWG ",p->putpwg);
  ADDU32 ("PUTIGQ ",p->putigq);
  ADDSTCK("PUTJWET",p->putjwet);
  ADDSTCK("PUTJCET",p->putjcet);
  ADDU32 ("PUTJWN ",p->putjwn);
  ADDU32 ("PUTJCN ",p->putjcn);
  ADDSTCK("PUTSUSET",p->putsuset);
  ADDU32 ("PUTSUSN",p->putsusn);
  ADDSTCK("PUTPSET",p->putpset);
  ADDU32 ("PUTPSN ",p->putpsn);
  ADDU32 ("PUTDDLY",p->putddly);

  /*--------------------------------------------------------*/
  /* Put1   - information                                   */
  /*-------------------------------------------------------*/
  ADDSTCK("PUT1ET",   p->put1et);
  ADDSTCK("PUT1CT",   p->put1ct);
  ADDU32 ("PUT1N  "  ,p->put1n);
  ADDU32 ("PUT1PWG"  ,p->put1pwg);
  ADDU32 ("PUT1IGQ"  ,p->put1igq);
  ADDSTCK("PUT1JWET" ,p->put1jwet);
  ADDSTCK("PUT1JCET" ,p->put1jcet);
  ADDU32 ("PUT1JWN"  ,p->put1jwn);
  ADDU32 ("PUT1JCN"  ,p->put1jcn);
  ADDSTCK("PUT1SUSET",p->put1suset);
  ADDU32 ("PUT1SUSN" ,p->put1susn);
  ADDSTCK("PUT1PSET" ,p->put1pset);
  ADDU32 ("PUT1PSN " ,p->put1psn);

  /*--------------------------------------------------------*/
  /* Inq    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("INQET",p->inqet);
  ADDSTCK("INQCT",p->inqct);
  ADDU32 ("INQN ",p->inqn);

  /*--------------------------------------------------------*/
  /* set    - information                                   */
  /*--------------------------------------------------------*/
  ADDSTCK("SETET", p->setet);
  ADDSTCK("SETCT", p->setct);
  ADDU32 ("SETN  ",p->setn);
  ADDSTCK("SETJWET", p->setjwet);
  ADDSTCK("SETJCET", p->setjcet);
  ADDU32 ("SETJWN ",p->setjwn);
  ADDU32 ("SETJCN ",p->setjcn);

  /*--------------------------------------------------------*/
  /* other  - information                                   */
  /*--------------------------------------------------------*/
  ADDU32  ("NPS",p->nps);
  ADDSTREN("CFSTRUCNAME",p->cfstrucname,12);
  ADDU32  ("NBUFFPOOL",p->nbuffpool);

  /*--------------------------------------------------------*/
  /* Byte counts and persistent data information            */
  /* The MINMS values for put/get are initially set to      */
  /* 0x0FFFFFFF, and appear that way in the SMF report if   */
  /* noputs/gets have touched that queue.  But that looks   */
  /* ugly, so I'm turning them to -1.                       */
  /*--------------------------------------------------------*/
  if (conv32(p->getminms) == 0x0FFFFFFF)
    p->getminms = conv32(-1);
  if (conv32(p->putminms) == 0x0FFFFFFF)
    p->putminms = conv32(-1);

  ADDS64 ("PUTBYTES",p->putbytes);
  ADDS64 ("GETBYTES",p->getbytes);
  ADDS32 ("VALIDPUT",p->validput);
  ADDS32 ("VALIDGET",p->validget);
  ADDS32 ("NGEN    ",p->ngen);
  ADDS32 ("GETMAXMS",p->getmaxms);
  ADDS32 ("GETMINMS",p->getminms);
  ADDS32 ("PUTMAXMS",p->putmaxms);

  ADDS32 ("PUTMINMS",p->putminms);
  ADDSTCK("MAXLATNT",p->maxlatnt);
  ADDSTCK("MINLATNT",p->minlatnt);
  ADDSTCK("TOTLATNT",p->totlatnt);
  ADDS32("USE_COUNT",p->use_count);
  ADDS32("TOTAL_USE",p->total_use);

  /*********************************************************/
  /* This is one structure that has been extended with     */
  /* every version of MQ. So we test the version and only  */
  /* add columns where they might apply. Note that the     */
  /* structure version is not the same number as the MQ    */
  /* product version.                                      */
  /*********************************************************/

  if (conv32(p->wqver) > 1)
  {
    ADDS32("GETPMSG ",p->getpmsg);
    ADDS32("PUTPMSG ",p->putpmsg);
    ADDS32("PUT1PMSG",p->put1pmsg);
    ADDS32("MAXQDPTH",p->maxqdpth);
  }

  if (conv32(p->wqver) > 3)
  {
    ADDU32 ("GETDVAL",p->getdval);
    ADDSTCK("GETJCET",p->getjcet);
    ADDU32 ("GETJCN" ,p->getjcn);
    ADDU32 ("PUTPWG ",p->putpwg);
    ADDSTCK("PUTJCET",p->putjcet);
    ADDU32 ("PUTJCN ",p->putjcn);
    ADDU32 ("PUT1PWG",p->put1pwg);
    ADDSTCK("PUT1JCET",p->put1jcet);
    ADDU32 ("PUT1JCN",p->put1jcn);
    ADDSTCK("SETJCET", p->setjcet);
    ADDU32 ("SETJCN ",p->setjcn);
  }

  if (conv32(p->wqver) > 4)
  {
    ADDU32("SELCOUNT",p->selcount);
    ADDU32("SELMAXLN",p->selmaxln);
    /*----------------------------------------------------*/
    /* CB     - Information                               */
    /*----------------------------------------------------*/
    ADDSTCK("CBET",p->cbet);
    ADDSTCK("CBCT",p->cbct);
    ADDU32 ("CBN ",p->cbn);
  }

  if (conv32(p->wqver) > 5)
  {
    ADDSTCK("OPENSUET" ,p->opensuet);
    ADDU32 ("OPENSUN " ,p->opensun);
    ADDSTCK("CLOSESUET",p->closesuet);
    ADDU32 ("CLOSESUN ",p->closesun);
    ADDU32 ("OPENCF0",p->opencf0);
    ADDU32 ("CLOSECF0",p->closecf0);

    for (i=0;i<5;i++)
    {
      for (j=0;j<14;j++) /* Last few of the array are unused, so don't print them */
      {
        if (first)
          sprintf(index,"%s.%s",verb[i],type[j]);
        ADDU32IDX("CFCOUNT",  index,p->mqcfreq[i].type[j].cfcount);
        ADDU32IDX("CFSYNCN",  index,p->mqcfreq[i].type[j].cfsyncn);
        ADDU32IDX("CFSYNCET", index,p->mqcfreq[i].type[j].cfsyncet);
        ADDU32IDX("CFASYNCN", index,p->mqcfreq[i].type[j].cfasyncn);
        ADDU32IDX("CFASYNCET",index,p->mqcfreq[i].type[j].cfasyncet);
      }
    }

    ADDU32("PUBLISHEDN"     , p->publishedn);
    ADDSTCK("TOPICOPENSRB"  , p->topicopensrb);
    ADDSTCK("TOPICPUTSRB "  , p->topicputsrb);
    ADDSTCK("TOPICPUT1SRB " , p->topicput1srb);
    ADDSTCK("TOPICCLOSESRB ", p->topicclosesrb);
  }

  if (conv32(p->wqver) > 8)
  {
     ADDU32("PUTDDLY",p->putddly);
  }

  SMFPRINTSTOP;

  return;
}

