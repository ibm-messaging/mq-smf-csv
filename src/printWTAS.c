
#include <stdio.h>
#include "mqsmf.h"

SMFPRINTGLOB;

void printWTAS(wtas *p)
{
  int i,j;
  char index[64];

  SMFPRINTSTART("WTAS",p,conv16(p->wtaslen));

  ADDS32 ("VER" , p->wtasver);
  ADDTIME("STRT", p->wtasstrt);
  ADDS32 ("WQCT", p->wtaswqct);

  /*-----------------------------------------------------------*/
  /* STCK elapse waiting for a each latch class and the number */
  /* of times that a wait occurred.                            */
  /*-----------------------------------------------------------*/
  for (i = 0; i < 32; i++)
  {
    /* Latches are known by index from 1, not from zero  */
    sprintf(index,"%d",i+1);
    ADDSTCKIDX("LWET",index,p->wtaslwet[i]);
    ADDS32IDX ("LWN",index,p->wtaslwn[i]);
  }

  ADDSTR("LOWN", convBin(p->wtaslown,8));

  /*-----------------------------------------------------------*/
  /* other statistics                                          */
  /*-----------------------------------------------------------*/
  ADDSTCK("OTET", p->wtasotet);
  ADDSTCK("OTCT", p->wtasotct);
  ADDS32 ("OTN ", p->wtasotn);
  ADDSTCK("MLW ", p->wtasmlw);
  ADDS32 ("MWLN", p->wtasmlwn);

  /*-----------------------------------------------------------*/
  /* Commit statistics                                         */
  /*-----------------------------------------------------------*/
  ADDSTCK("CMET", p->wtascmet);
  ADDSTCK("CMT ", p->wtascmct);
  ADDS32 ("CMN ", p->wtascmn);

  /*-----------------------------------------------------------*/
  /* Backout statistics                                        */
  /*-----------------------------------------------------------*/
  ADDSTCK("BAET", p->wtasbaet);
  ADDSTCK("BACT", p->wtasbact);
  ADDS32 ("BAN ", p->wtasban);

  /*-----------------------------------------------------------*/
  /* Journal and logging information                           */
  /*-----------------------------------------------------------*/
  ADDSTCK("JWET", p->wtasjwet);
  ADDU32 ("JWN ", p->wtasjwn);
  ADDU32 ("JWB ", p->wtasjwb);
  ADDSTCK("JCET", p->wtasjcet);
  ADDU32 ("JCN ", p->wtasjcn);
  ADDU32 ("SUSN", p->wtassusn);
  ADDSTCK("SUSE", p->wtassuse);

  /*-----------------------------------------------------------*/
  /* Pageset 0 logging activity                                */
  /*-----------------------------------------------------------*/
  ADDSTCK("PSE0", p->wtaspse0);
  ADDU32 ("PSN0", p->wtaspsn0);

  /*-----------------------------------------------------------*/
  /* DB2 activity                                              */
  /*-----------------------------------------------------------*/
  ADDSTCK("DBET", p->wtasdbet);
  ADDSTCK("DBES", p->wtasdbes);
  ADDSTCK("DBMT", p->wtasdbmt);
  ADDSTCK("DBMS", p->wtasdbms);
  ADDU32 ("DBCT", p->wtasdbct);

  ADDU64("DBPT", p->wtasdbpt);
  ADDU64("DBGT", p->wtasdbgt);

  /*-----------------------------------------------------------*/
  /* CF activity                                               */
  /*-----------------------------------------------------------*/
  ADDU32 ("CSEC",p->wtascsec);
  ADDU32 ("CMEC",p->wtascmec);
  ADDU32 ("RSEC",p->wtasrsec);
  ADDU32 ("RMEC",p->wtasrmec);
  ADDSTCK("SSTC",p->wtassstc);
  ADDSTCK("MSTC",p->wtasmstc);

  /*-----------------------------------------------------------*/
  /* Interval data and page counts                             */
  /*-----------------------------------------------------------*/
  ADDSTCK("INTS", p->wtasints);
  ADDSTCK("INTE", p->wtasinte);
  ADDS32 ("GPO ", p->wtasgpo);
  ADDS32 ("GPN ", p->wtasgpn);

  /*-----------------------------------------------------------*/
  /* Version 7 (WTAS_VER_4) inclusions                         */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_4)
  {
    ADDSTCK("PBHW", p->wtaspbhw);
    ADDSTCK("PBTT", p->wtaspbtt);

    ADDSTCK("TPET", p->wtastpet);
    ADDSTCK("TPCT", p->wtastpct);
    ADDU32 ("TPN ", p->wtastpn);

    ADDSTCK("SUET", p->wtassuet);
    ADDSTCK("SUET", p->wtassuct);

    ADDU32 ("SUN ", p->wtassun );
    ADDU32 ("SUSC", p->wtassusc);
    ADDU32 ("SUSL", p->wtassusl);

    ADDSTCK("SQET", p->wtassqet);
    ADDSTCK("SQCT", p->wtassqct);
    ADDU32 ("SQN ", p->wtassqn);

    ADDSTCK("CTET", p->wtasctet);
    ADDSTCK("CTCT", p->wtasctct);
    ADDU32 ("CTN ", p->wtasctn);

    ADDSTCK("STET", p->wtasstet);
    ADDSTCK("STCT", p->wtasstct);
    ADDU32 ("STN ", p->wtasstn);

    ADDSTCK("CTSR", p->wtasctsr);

  }                                                  /* End if (WTAS_VER_4)*/

  /*-----------------------------------------------------------*/
  /* Version 7.1 (WTAS_VER_5) inclusions                       */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_5)
  {
    ADDU32 ("SMWB", p -> wtassmwb);
    ADDU32 ("SMRB", p -> wtassmrb);
    ADDU32 ("SMWP", p -> wtassmwp);
    ADDU32 ("SMRP", p -> wtassmrp);
    ADDU32 ("SMRS", p -> wtassmrs);

    ADDSTCK("SMWT", p -> wtassmwt);
  }

  /*-----------------------------------------------------------*/
  /* Version 8   (WTAS_VER_8) inclusions                       */
  /*-----------------------------------------------------------*/
  if (conv32(p->wtasver) >= WTAS_VER_8)
  {
    ADDSTCK("PRET", p -> wtaspret);
    ADDSTCK("PRCT", p -> wtasprct);
    ADDU32 ("PRN ", p -> wtasprn);
    for (i = 0; i < 32; i++)
    {
      sprintf(index,"%d",i+1);
      ADDSTCKIDX("LMAX", index, p -> wtaslmax[i]);
    }
  }

  SMFPRINTSTOP;
  return;
}
