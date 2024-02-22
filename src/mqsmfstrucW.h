/*
 * This file was generated from cqsdsmfc.h version 935
 * by the convH program (convH64.c) in the src directory
 * Do not try to edit this header manually.
 * 
 * (C) Copyright IBM Corp. 1993, 2023 All Rights Reserved.
 * 
 */
#pragma pack(1)
#if !defined(CSQDSMFC_INCLUDED)
#define CSQDSMFC_INCLUDED
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   q5stid;
int16_t   q5stll;
char     q5steyec[4];
uint32_t  numtask;
uint32_t  acttask;
uint32_t  conncnt;
uint32_t  disccnt;
uint32_t  dhigmax;
uint32_t  abndcnt;
uint32_t  requcnt;
uint32_t  deadcnt;
uint32_t  delecnt;
uint32_t  listcnt;
uint32_t  readcnt;
uint32_t  updtcnt;
uint32_t  writcnt;
uint32_t  scssel;
uint32_t  scsins;
uint32_t  scsupd;
uint32_t  scsdel;
uint32_t  ssksel;
uint32_t  sskins;
uint32_t  sskdel;
uint32_t  scsbfts;
uint32_t  scsmaxr;
uint32_t  filler__001[2];
uint64_t      deletcuw;
uint64_t      deletmxw;
uint64_t      delescuw;
uint64_t      delesmxw;
uint64_t      listtcuw;
uint64_t      listtmxw;
uint64_t      listscuw;
uint64_t      listsmxw;
uint64_t      readtcuw;
uint64_t      readtmxw;
uint64_t      readscuw;
uint64_t      readsmxw;
uint64_t      updttcuw;
uint64_t      updttmxw;
uint64_t      updtscuw;
uint64_t      updtsmxw;
uint64_t      writtcuw;
uint64_t      writtmxw;
uint64_t      writscuw;
uint64_t      writsmxw;
uint64_t      scsstcuw;
uint64_t      scsstmxw;
uint64_t      scssscuw;
uint64_t      scsssmxw;
uint64_t      scsitcuw;
uint64_t      scsitmxw;
uint64_t      scsiscuw;
uint64_t      scsismxw;
uint64_t      scsutcuw;
uint64_t      scsutmxw;
uint64_t      scsuscuw;
uint64_t      scsusmxw;
uint64_t      scsdtcuw;
uint64_t      scsdtmxw;
uint64_t      scsdscuw;
uint64_t      scsdsmxw;
uint64_t      sskstcuw;
uint64_t      sskstmxw;
uint64_t      ssksscuw;
uint64_t      sskssmxw;
uint64_t      sskitcuw;
uint64_t      sskitmxw;
uint64_t      sskiscuw;
uint64_t      sskismxw;
uint64_t      sskdtcuw;
uint64_t      sskdtmxw;
uint64_t      sskdscuw;
uint64_t      sskdsmxw;
uint32_t  lmssel;
uint32_t  lmsins;
uint32_t  lmsupd;
uint32_t  lmsdel;
uint32_t  lmslis;
uint32_t  filler_002;
uint64_t      lmsstcuw;
uint64_t      lmsstmxw;
uint64_t      lmssscuw;
uint64_t      lmsssmxw;
uint64_t      lmsitcuw;
uint64_t      lmsitmxw;
uint64_t      lmsiscuw;
uint64_t      lmsismxw;
uint64_t      lmsutcuw;
uint64_t      lmsutmxw;
uint64_t      lmsuscuw;
uint64_t      lmsusmxw;
uint64_t      lmsdtcuw;
uint64_t      lmsdtmxw;
uint64_t      lmsdscuw;
uint64_t      lmsdsmxw;
uint64_t      lmsltcuw;
uint64_t      lmsltmxw;
uint64_t      lmslscuw;
uint64_t      lmslsmxw;
} q5st;
#define Q5STIDV  0xF50F
#define Q5STEYEV "Q5ST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qcctid;
uint16_t  qcctll;
char     qccteyec[4];
char     qcctjobn[8];
char     qcctqsgn[4];
uint32_t qcctnocc;
uint32_t qcctmxcc;
uint32_t qcctnoac;
uint32_t qcctmxac;
uint32_t qcctmxtp;
uint32_t qcctmxlu;
uint32_t qcctstus;
uint64_t      qcctstab;
uint64_t      qcctslim;
} qcct;
#define QCCTIDV  0xE741
#define QCCTEYEV "QCCT"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
#define qct_common \
uint32_t qcttskn;     /* Task number (TCB addr)         */\
uint32_t qctreqn;     /* Num of requests for task       */\
uint64_t      qctcptm;/* Busy CPU time of task          */\
uint64_t      qcteltm;/* Sum of elapsed time of requests*/\
uint64_t      qctwttm;
typedef
struct {
qct_common
uint32_t qctchln;
} qct_dsp;
typedef
struct {
qct_common
} qct_adp;
typedef
struct {
qct_common
uint64_t      qctlstm;
uint64_t      qctlsdu;
} qct_ssl;
typedef
struct {
qct_common
uint64_t      qctlgtm;
uint64_t      qctlgdu;
} qct_dns;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
/*
* CHLTYPE:
* SDR Sender
* RCV Receiver
* SVR Server
* RQR Requester
* SCO Server Connection
* CCO Client Connection
* CLS Cluster Sender
* CLR Cluster Receiver
*/
typedef
struct {
uint16_t  qcstid;
uint16_t  qcstll;
char     qcsteyec[4];
uint64_t      qcstcltm;
char     qcstchnm[20];
char     qcstchdp;
char     qcstchty;
char     qcstchst;
char     qcststcl;
char     qcstcnnm[48];
uint64_t      qcststrt;
uint64_t      qcstludt;
uint64_t      qcstlmst;
uint32_t qcstcbsz;
uint32_t qcstnmsg;
uint32_t qcstnpmg;
uint32_t qcstbatc;
uint32_t qcstfuba;
uint32_t qcstbfst;
uint32_t qcstbfrc;
uint32_t qcstcscv;
uint64_t      qcstnbyt;
uint64_t      qcstnpby;
uint64_t      qcstbyst;
uint64_t      qcstbyrc;
uint32_t qcstcpra;
uint32_t qcstetav;
uint32_t qcstetmn;
uint32_t qcstetmx;
uint64_t      qcstetdt;
uint32_t qcstdnrt;
uint32_t qcstntav;
uint32_t qcstntmn;
uint32_t qcstntmx;
uint64_t      qcstntdt;
char     qcstrqmn[48];
char     qcstslsn[8] ;
char     qcstslcn[16];
uint32_t qcstslcs;
uint32_t qcstptrc;
uint32_t qcstqetc;
uint32_t qcstcbit;
uint32_t qcstcbdl;
uint16_t  qcstdspn;
char     qcstrsv1[6] ;
} qcst;
#define QCSTIDV  0xE74A
#define QCSTEYEV "QCST"
typedef struct
{
char * csCipherSpec;
uint32_t lCipherSpec;
} smfSSL_CIPHER;
#define NOCIPHSPECS   1
static smfSSL_CIPHER smfSSL_CIPHER_TABLE[NOCIPHSPECS] =
{
{"????                            ", 0x00000000}
};
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qestid;
uint16_t  qestll;
char     qesteyec[4];
struct {
char     qeststr[12];
uint32_t  qeststrn;
uint32_t  qestcsec;
uint32_t  qestcmec;
uint64_t      qestsstc;
uint64_t      qestmstc;
uint32_t qestrsec;
uint32_t qestrmec;
uint32_t qestsful;
uint32_t qestmnus;
uint32_t qestmlus;
char filler__001[4];
} qeststuc[64];
} qest;
#define QESTIDV  0xC513
#define QESTEYEV "QEST"
#define QESTSNUM 64
#define QESTSLEN 64
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qesdid;
uint16_t  qesdll;
char     qesdeyec[4];
char     qesdstr[12];
uint32_t qesdstrn;
uint32_t qesdsmbt;
uint32_t qesdsmbs;
uint32_t qesdsmbd;
uint32_t qesdsmbu;
uint32_t qesdsmbf;
uint32_t qesdsmmc;
uint32_t qesdsmar;
uint32_t qesdsmfr;
uint32_t qesdsmrr;
uint32_t qesdsmcr;
uint32_t qesdsmap;
uint32_t qesdsmfp;
uint32_t qesdsmrp;
uint32_t qesdsmcp;
uint32_t qesdsmfl;
uint32_t qesdsmmm;
uint32_t qesdsmmu;
uint32_t qesdsmmf;
uint32_t qesdbfsz;
uint32_t qesdbfto;
uint32_t qesdbfus;
uint32_t qesdbfup;
uint32_t qesdbffs;
uint32_t qesdbffe;
uint32_t qesdbfpw;
uint32_t qesdbfbw;
uint32_t qesdbfgb;
uint32_t qesdbfgv;
uint32_t qesdbfgm;
uint32_t qesdbfgf;
uint32_t qesdbfgl;
uint32_t qesdbfgn;
uint32_t qesdbfrr;
uint32_t qesdbfrs;
uint32_t qesdbfrp;
uint32_t qesdbfwr;
uint32_t qesdbffb;
uint32_t qesdbfdb;
uint32_t qesdbfwp;
uint32_t qesdbfwb;
uint64_t      qesdbfpt;
uint64_t      qesdbfbt;
uint32_t qesdbfmu;
uint32_t qesdbfmf;
uint32_t qesdbfmp;
uint32_t qesdbfmb;
uint32_t qesdioha;
uint32_t qesdiohu;
uint32_t qesdioci;
uint32_t qesdioca;
uint32_t qesdiofr;
uint32_t qesdiofp;
uint64_t      qesdioft;
uint64_t      qesdiofw;
uint32_t qesdiowr;
uint32_t qesdiowp;
uint64_t      qesdiowt;
uint64_t      qesdioww;
uint32_t qesdiorr;
uint32_t qesdiorp;
uint64_t      qesdiort;
uint64_t      qesdiorw;
uint32_t qesdioor;
uint32_t qesdioop;
uint64_t      qesdioot;
uint64_t      qesdioow;
struct {
uint32_t qesdencf : 1;
uint32_t spare    : 31;
} qesdflag;
uint32_t spare;
} qesd;
#define QESDIDV  0xC523
#define QESDEYEV "QESD"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qistid;
int16_t   qistll;
char     qisteyec[4];
uint32_t  qistmget;
uint32_t  qistmput;
uint32_t  qistmblr;
uint32_t  qistdcre;
uint32_t  qistdput;
uint32_t  qistddel;
uint32_t  qistdget;
uint32_t  qistdloc;
uint32_t  qistmcnt;
uint32_t  qistalst;
uint32_t  qistlomm;
uint32_t  qistdlmm;
uint32_t  qistenum;
uint32_t  qistraio;
uint32_t  qistrabp;
uint32_t  qistgetd;
uint32_t  qistgetb;
uint32_t  FILLER___001;
} qist;
#define QISTIDV  0xC90F
#define QISTEYEV "QIST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qis1id;
uint16_t  qis1ll;
char     qis1eyec[4];
uint32_t qis1psno;
uint32_t qis1bpno;
uint32_t qis1topg;
uint32_t qis1unpg;
uint32_t qis1ppg;
uint32_t qis1nppg;
uint32_t qis1full;
uint32_t qis1pchi;
uint32_t qis1dwpn;
uint32_t qis1dwpp;
uint64_t      qis1dwpt;
uint32_t qis1imwn;
uint32_t qis1imwp;
uint64_t      qis1imwt;
uint32_t qis1getn;
uint32_t qis1getp;
uint64_t      qis1gett;
uint32_t qis1ckpp;
uint16_t  qis1expn;
char     qis1strn;
char     qis1expm;
char     qis1stat;
char     reserved[3];
struct {
uint32_t qis1expf : 1;
uint32_t qis1encf : 1;
uint32_t spare    : 30;
} qis1flag;
} qis1;
#define QIS1IDV  0xD90F
#define QIS1EYEV "QIS1"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
char     qjstid[2];
int16_t   qjstll;
char     qjsteid[4];
int32_t   qjstwrw;
int32_t   qjstwrnw;
int32_t   qjstwrf;
int32_t   qjstwtb;
int32_t   qjstrbuf;
int32_t   qjstract;
int32_t   qjstrarh;
int32_t   qjsttvc;
int32_t   qjstbsds;
int32_t   qjstbffl;
int32_t   qjstbfwr;
int32_t   qjstalr;
int32_t   qjstalw;
int32_t   qjstciof;
int32_t   qjstllcp;
int32_t   qjstwur;
int32_t   qjstlama;
int32_t   qjstlams;
int32_t   qjstlsus;
int32_t   qjstlogw;
int32_t   qjstciwr;
int32_t   qjstserw;
int32_t   qjstthrw;
int32_t   qjstbpag;
int32_t   filler__001[4];
struct   {
int32_t   qjstcmpreq  ;
int32_t   qjstcmpfail ;
int64_t   qjstcmpuncmp;
int64_t   qjstcmpcomp ;
int32_t   qjstdecreq  ;
int32_t   qjstdecfail ;
int64_t   qjstdecuncmp;
int64_t   qjstdeccomp ;
} qjstcompr[1];
char  filler__002[96];
struct   {
struct tagqjstiotype {
uint32_t      qjstiocount ;
uint32_t      qjstioci ;
uint64_t  qjstiototio;
uint64_t  qjstiototsus;
uint64_t  qjstiomaxio;
uint64_t  qjstiomaxioT;
char    qjstiomaxiol[8] ;
uint64_t  qjstiomaxsus;
uint64_t  qjstiomaxsusT;
char   qjstiomaxsusl[8] ;
} qjstiotype[2] ;
} qjstio[2] ;
uint64_t  qjstslptu;
uint64_t  qjstiosqu[2];
int32_t   qjstcp1n;
int32_t   qjstcp2n;
int32_t   qjsthwc;
int32_t   qjsthwe;
int32_t   qjstencr;
} qjst;
#define QJSTIDV  0x0093
#define QJSTEYEV "QJST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qlstid;
int16_t   qlstll;
char     qlsteyec[4];
uint32_t  qlstgetl;
uint32_t  qlsthldl;
uint32_t  qlstrell;
uint32_t  filler__001[3];
} qlst;
#define QLSTIDV  0xD30F
#define QLSTEYEV "QLST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qmacid;
int16_t   qmacll;
char     qmaceyec[4];
uint64_t      qmaccput;
struct {
uint32_t  qmacputa;
uint32_t  qmacputb;
uint32_t  qmacputc;
uint32_t  qmacputd;
} qmacputl;
struct {
uint32_t  qmacgeta;
uint32_t  qmacgetb;
uint32_t  qmacgetc;
uint32_t  qmacgetd;
} qmacgetl;
} qmac;
#define QMACIDV  0xD414
#define QMACEYEV "QMAC"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qmstid;
uint16_t  qmstll;
char qmsteyec[4];
int32_t  qmstopen;
int32_t  qmstclos;
int32_t  qmstget;
int32_t  qmstput;
int32_t  qmstput1;
int32_t  qmstinq;
int32_t  qmstinql;
int32_t  qmstset;
int32_t  qmstendw;
int32_t  qmstcalh;
int32_t  qmstsub;
int32_t  qmstsubr;
int32_t  qmstcb;
int32_t  qmstctl;
int32_t  qmststus;
int32_t  qmstpubs;
int64_t       qmstspp;
int64_t       qmstsnp;
int64_t       qmstpbp;
int64_t       qmstnbp;
} qmst;
#define QMSTIDV  0xD40F
#define QMSTEYEV "QMST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qpstid;
uint16_t  qpstll;
char     qpsteyec[4];
int32_t   qpstpool;
int32_t   qpstnbuf;
int32_t   qpstcbsl;
int32_t   qpstcbs;
int32_t   qpstgetp;
int32_t   qpstgetn;
int32_t   qpstrio;
int32_t   qpststw;
int32_t   qpsttpw;
int32_t   qpstwio;
int32_t   qpstimw;
int32_t   qpstdwt;
int32_t   qpstdmc;
int32_t   qpststl;
int32_t   qpststla;
int32_t   qpstsos;
uint32_t  qpstloc : 1;
uint32_t  qpstf4kb : 1;
uint32_t         : 30;
int32_t   filler__001[7];
} qpst;
#define QPSTIDV  0xD30F
#define QPSTEYEV "QPST"
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qqstid;
uint16_t  qqstll;
char     qqsteyec[4];
char     qqstqnam[48];
uint32_t   qqstflag;
int16_t   qqstpsid;
int16_t   qqstbpid;
char     qqstqsgn[4];
char     qqstcfst[12];
int32_t  qqstdpth;
uint32_t qqstopct;
uint32_t qqstipct;
uint32_t qqstmage;
uint32_t qqstqtst;
uint32_t qqstqtlt;
uint64_t      qqstlput;
uint64_t      qqstlget;
uint32_t qqstdphi;
uint32_t qqstdplo;
uint64_t      qqstputs;
uint64_t      qqstput1;
uint64_t      qqstnppt;
uint64_t      qqstppt;
uint64_t      qqstnpp1;
uint64_t      qqstpp1;
uint64_t      qqstputb;
uint64_t      qqstpt1b;
uint64_t      qqstnppb;
uint64_t      qqstppb;
uint64_t      qqstnp1b;
uint64_t      qqstp1b;
uint64_t      qqstflpt;
uint64_t      qqstflp1;
uint64_t      qqstfptc;
uint64_t      qqstfptb;
uint64_t      qqststrm;
uint64_t      qqstmsmi;
uint64_t      qqstmsma;
uint64_t      qqstmsav;
uint64_t      qqstgets;
uint64_t      qqstnpdg;
uint64_t      qqstpdg;
uint64_t      qqstgetb;
uint64_t      qqstnpdb;
uint64_t      qqstpdb;
uint64_t      qqstbrws;
uint64_t      qqstnpbr;
uint64_t      qqstpbr;
uint64_t      qqstbrwb;
uint64_t      qqstnpbb;
uint64_t      qqstpbb;
uint64_t      qqstflgt;
uint64_t      qqstnmag;
uint64_t      qqsttmfg;
uint64_t      qqstflbr;
uint64_t      qqstnmab;
uint64_t      qqsttmfb;
uint64_t      qqstflgw;
uint64_t      qqstrdgw;
uint64_t      qqstflbw;
uint64_t      qqstrdbw;
uint64_t      qqstsagt;
uint64_t      qqstsabr;
uint32_t qqstiphi;
uint32_t qqstiplo;
uint32_t qqstophi;
uint32_t qqstoplo;
uint64_t      qqstopen;
uint64_t      qqstclos;
uint64_t      qqstinqr;
uint64_t      qqstset;
uint64_t      qqstexpr;
uint64_t      qqstrbpt;
uint64_t      qqstrbgt;
} qqst;
#define QQSTIDV  0xD80F
#define QQSTEYEV "QQST"
#define QQSTDISP 0x80000000
#define QQSTPART 0x40000000
#define QQSTUNCM 0x20000000
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qsgmid;
uint16_t  qsgmll;
char     qsgmeyec[4];
int32_t  qsgmlvtot;
int32_t  qsgmlvnew;
int32_t  qsgmlvmax;
int32_t  qsgmlvmin;
int32_t  qsgmcount;
char     qsgmscls;
char     filler__002[3];
uint64_t      qsgmcsect;
int32_t  qsgmoffset;
char     filler__003[4];
#define QSGMIDV  0xE20F
#define QSGMEYEV "QSGM"
} qsgm;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qsphid;
uint16_t  qsphll;
char     qspheyec[4];
int32_t  qsphstor;
int32_t  qsphostor;
int32_t  qsphxstor;
uint16_t  qsphbsize;
uint16_t  qsphbcnt;
uint32_t qsphcount;
uint64_t      qsphdspnm;
char     qsphname[48];
char     qsphflags;
#define  QSPHFIXED 0x80
#define  QSPHGLOB  0x40
#define  QSPHDSPC  0x20
#define  QSPHIVSA  0x10
#define  QSPH64BIT 0x08
char     filler__001[3];
#define QSPHIDV  0xE210
#define QSPHEYEV "QSPH"
} qsph;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint16_t  qsrsid;
uint16_t  qsrsll;
char     qsrseyec[4];
int32_t  qsrsphbgf;
int32_t  qsrsphbgv;
int32_t  qsrsphblf;
int32_t  qsrsphblv;
int32_t  qsrsgmbt;
int32_t  qsrsstkt;
int32_t  qsrsstrta;
int32_t  qsrssiza;
int32_t  qsrscrgtp;
int32_t  qsrslimit;
int32_t  qsrsloal;
int32_t  qsrsoloal;
int32_t  qsrshial;
int32_t  qsrsohial;
int32_t  qsrssmfl;
int32_t  qsrssmfr;
int32_t  qsrsestra;
int32_t  qsrsesiza;
int32_t  qsrsergtp;
int32_t  qsrselim;
int32_t  qsrseloal;
int32_t  qsrsoeloal;
int32_t  qsrsehial;
int32_t  qsrsoehial;
int32_t  qsrssmfel;
int32_t  qsrssmfer;
uint64_t      qsrsmemlim;
uint64_t      qsrsabytes;
uint64_t      qsrsgbytes;
uint32_t qsrsnmomb;
char     qsrsmemlims;
char     filler__001[3];
int32_t  qsrsfmct;
char     filler__002[4];
int32_t  qsrsaval;
int32_t  qsrsmvs;
int32_t  qsrscrit;
int32_t  qsrssos;
uint64_t      qsrsaval64;
uint64_t      qsrscush64;
uint64_t      qsrssmds64;
uint64_t      qsrsphblgf;
uint64_t      qsrsphblgv;
uint64_t      qsrsphbllf;
uint64_t      qsrsphbllv;
uint64_t      qsrsgmbl;
#define QSRSIDV  0xE20E
#define QSRSEYEV "QSRS"
} qsrs;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qsstid;
int16_t   qsstlen;
char     qsstdesc[4];
struct {
uint32_t  qsstgplf;
uint32_t  qsstfplf;
uint32_t  qsstfref;
uint32_t  qsstexpf;
uint32_t  qsstconf;
uint32_t  qsstgplv;
uint32_t  qsstfplv;
uint32_t  qsstfrev;
uint32_t  qsstexpv;
uint32_t  qsstconv;
uint32_t  qsstgetm;
uint32_t  qsstfrem;
uint32_t  qsstrcnz;
uint32_t  qsstcont;
uint32_t  qsstcrit;
uint32_t  qsstabnd;
} qsstdata;
#define QSSTIDV  0x003C
#define QSSTEYEV "QSST"
} qsst;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qtstid;
int16_t   qtstll;
char     qtstdesc[4];
struct {
uint32_t  qtststot;
uint32_t  qtstsdur;
uint32_t  qtstshig[3];
uint32_t  qtstslow[3];
uint32_t  qtstsexp;
uint32_t  qtsttmsg;
uint32_t  qtstsphw;
uint32_t  qtstptot[3];
uint32_t  qtstphig;
uint32_t  qtstplow;
uint32_t  qtstpnos;
uint32_t  filler_001;
uint64_t      qtstethw;
uint64_t      qtstetto;
} qtstzero;
#define QTSTIDV  0xA302
#define QTSTEYEV "QTST"
} qtst;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint64_t      qwacbsc;
uint64_t      qwacesc;
uint64_t      qwacbjst;
uint64_t      qwacejst;
uint64_t      qwacbsrb;
uint64_t      qwacesrb;
uint32_t qwacrinv;
char     qwacnid[16];
uint32_t qwaccomm;
uint32_t qwacback;
uint64_t      qwacasc;
uint64_t      qwacajst;
uint64_t      qwacasrb;
uint64_t      qwacawti;
uint64_t      qwacawtl;
uint32_t qwacarna;
uint32_t qwacarne;
uint64_t      qwacawtr;
uint64_t      qwacawtw;
uint64_t      qwacawte;
uint64_t      qwacalog;
uint32_t qwacarnl;
uint32_t qwacarnr;
uint32_t qwacarnw;
uint32_t qwacarns;
uint32_t qwacalct;
} qwac;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qwhclen;
char     qwhctyp[1];
char     filler__001[1];
char     qwhcaid[8];
char     qwhccv[12];
char     qwhccn[8];
char     filler__002[8];
char     qwhcopid[8];
uint32_t qwhcatyp;
char     qwhctokn[22];
char     qwhcnid[16];
char     filler__003[2];
} qwhc;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   qwhslen;
char     qwhstyp[1];
char     qwhsrmid[1];
int16_t   qwhsiid;
char     qwhsnsda[1];
char     qwhsrn[1];
uint32_t  qwhsace;
char     qwhsssid[4];
uint64_t      qwhsstck;
char     qwhsiseq[4];
char     qwhswseq[4];
struct {
unsigned char qwhssmfc : 1;
unsigned char qwhspad1 : 7;
} qwhsflag1;
char     qwhsflag2;
char     qwhsflag3;
char     qwhsflag4;
uint64_t      qwhstime;
uint64_t      qwhsdurn;
} qwhs;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
struct {
uint32_t  cfcount;
uint32_t  cfsyncn;
uint32_t  cfsyncet;
uint32_t  cfasyncn;
uint32_t  cfasyncet;
} type[20];
} cfstat;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   wqid;
int16_t   wqll;
char     wqeye[4];
int32_t   wqver;
uint32_t   wqnext;
char     correl[16];
char     objname[48];
char     basename[48];
uint64_t      opentime;
uint64_t      clostime;
int32_t   qtype;
int32_t   indxtype;
int32_t   qsgdisp;
char     openeye[4];
uint64_t      openet;
uint64_t      openct;
uint32_t  openn;
char     closeeye[4];
uint64_t      closeet;
uint64_t      closect;
uint32_t  closen;
char     geteye[4];
uint64_t      getet;
uint64_t      getct;
uint32_t  getn;
uint32_t  getbrwa;
uint32_t  getbrws;
uint32_t  geta;
uint32_t  gets;
uint32_t  geterr;
uint64_t      getjwet;
uint32_t  getjwn;
uint64_t      getpset;
uint32_t  getpsn;
uint64_t      getsuset;
uint32_t  getsusn;
uint32_t  getepage;
uint32_t  getsmsg;
uint32_t  getexmsg;
char     puteye[4];
uint64_t      putet;
uint64_t      putct;
uint32_t  putn;
uint64_t      putjwet;
uint32_t  putjwn;
uint64_t      putsuset;
uint32_t  putsusn;
uint64_t      putpset;
int32_t   putpsn;
char     put1eye[4];
uint64_t      put1et;
uint64_t      put1ct;
uint32_t  put1n;
uint64_t      put1jwet;
uint32_t  put1jwn;
uint64_t      put1suset;
uint32_t  put1susn;
uint64_t      put1pset;
int32_t   put1psn;
char     inqeye[4];
uint64_t      inqet;
uint64_t      inqct;
uint32_t  inqn;
char     seteye[4];
uint64_t      setet;
uint64_t      setct;
uint32_t  setn;
uint64_t      setjwet;
uint32_t  setjwn;
uint32_t  nps;
char     cfstrucname[12];
uint32_t  nbuffpool;
uint64_t      putbytes;
uint64_t      getbytes;
uint32_t  validput;
uint32_t  validget;
uint32_t  ngen;
int32_t   getmaxms;
int32_t   getminms;
int32_t   putmaxms;
int32_t   putminms;
uint64_t      maxlatnt;
uint64_t      minlatnt;
uint64_t      totlatnt;
uint32_t   wqback;
int32_t   use_count;
int32_t   total_use;
int32_t   getpmsg;
int32_t   putpmsg;
int32_t   put1pmsg;
int32_t   maxqdpth;
struct {
unsigned char fNoAcctg : 1;
unsigned char fTopic   : 1;
unsigned char fAcctqC  : 1;
unsigned char spare    : 5;
unsigned char spare2   : 8;
} flags;
int16_t   z__001;
uint32_t  getdval;
uint64_t      getjcet;
uint32_t  getjcn;
uint32_t  putpwg;
uint64_t      putjcet;
uint32_t  putjcn;
uint32_t  put1pwg;
uint64_t      put1jcet;
uint32_t  put1jcn;
uint64_t      setjcet;
uint32_t  setjcn;
uint32_t  putdsq;
uint32_t  putigq;
uint32_t  put1igq;
int32_t   z__002[1];
uint32_t  selcount;
uint32_t  selmaxln;
char     cbeye[4];
uint64_t      cbet;
uint64_t      cbct;
uint32_t  cbn;
uint64_t      opensuet;
uint32_t  opensun;
uint64_t      closesuet;
uint32_t  closesun;
uint32_t  opencf0;
uint32_t  closecf0;
cfstat mqcfreq[5];
uint32_t  publishedn;
uint32_t  z__003[3];
uint64_t      topicopensrb;
/* CPU time used ( in STCK format)
used during open of a topic
*/
uint64_t      topicputsrb;
/* CPU time used (in STCK format)
used during put to a topic
*/
uint64_t      topicput1srb;
/* CPU time used ( in STCK format)
used during put1 to a topic
*/
uint64_t      topicclosesrb;
/* CPU time used ( in STCK format)
used during close of a topic
*/
uint32_t  putddly;   /* Number of messages put with
delivery delay, and hence sent to
delivery delay staging queue    */
uint32_t  z__004[1];
uint32_t  streamedn;
uint32_t  z__005[1];
} wq;
#define WQ_HEXID 0xF702
#define WQ_EYE   "WQST"
#define WQ_VER   9
#define WQ_VER_1 1
#define WQ_VER_2 2
#define WQ_VER_3 3
#define WQ_VER_4 4
#define WQ_VER_5 5
#define WQ_VER_6 6
#define WQ_VER_8 8
#define WQ_VER_9 9
#define WQ_LEN_1 576
#define WQ_LEN_2 592
#define WQ_LEN_3 608
#define WQ_LEN_4 672
#define WQ_LEN_5 704
#define WQ_LEN_6 2752
#define WQMQOPEN  1
#define WQMQCLOSE 2
#define WQMQGET   3
#define WQMQPUT   4
#define WQMQPUT1  5
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   wtasshex;
int16_t   wtaslen;
char     wtaseyec[4];
uint64_t      wtasstrt;
int32_t   wtasmthr;
int32_t   wtaswtas;
uint64_t      wtaslatc;
int32_t   wtashshi;
uint32_t   wtasnext;
uint32_t  wtasflag;
uint32_t  wtaswqct;
uint64_t      wtaslwet[32];
int32_t   wtaslwn[32];
uint64_t      wtasotet;
uint64_t      wtasotct;
int32_t   wtasotn;
uint64_t      wtasmlw;
int32_t   wtasmlwn;
char     wtaslown[8];
uint64_t      wtascmet;
uint64_t      wtascmct;
int32_t   wtascmn;
uint64_t      wtasbaet;
uint64_t      wtasbact;
int32_t   wtasban;
char     filler__007[4];
uint64_t      wtasjwet;
uint32_t  wtasjwn;
uint32_t  wtasjwb;
uint64_t      wtasjcet;
uint32_t  wtasjcn;
uint32_t  wtassusn;
uint64_t      wtassuse;
uint64_t      wtaspse0;
uint32_t  wtaspsn0;
uint64_t      wtasdbet;
uint64_t      wtasdbes;
uint64_t      wtasdbmt;
uint64_t      wtasdbms;
uint32_t               wtasdbct;
uint32_t  wtascsec;
uint32_t  wtascmec;
uint32_t  wtasrsec;
uint32_t  wtasrmec;
uint64_t      wtassstc;
uint64_t      wtasmstc;
uint32_t  wtassmrs;
uint32_t  wtassmrb;
uint32_t  wtassmwb;
uint32_t  wtassmrp;
uint32_t  wtassmwp;
uint64_t      wtassmwt;
uint64_t      wtasints;
uint64_t      wtasinte;
int32_t   wtasgpo;
int32_t   wtasgpn;
uint32_t   wtascqf;
uint32_t   wtascqb;
uint32_t  wtasver;
uint32_t  filler__002;
uint64_t      wtasdbpt;
uint64_t      wtasdbgt;
uint64_t      wtaspbhw;
uint64_t      wtaspbtt;
uint64_t      wtastpet;
uint64_t      wtastpct;
uint32_t  wtastpn;
uint64_t      wtassuet;
uint64_t      wtassuct;
uint32_t  wtassun;
uint32_t  wtassusc;
uint32_t  wtassusl;
uint64_t      wtassqet;
uint64_t      wtassqct;
uint32_t  wtassqn;
uint64_t      wtasctet;
uint64_t      wtasctct;
uint32_t  wtasctn;
uint64_t      wtasstet;
uint64_t      wtasstct;
uint32_t  wtasstn;
uint32_t  filler__009;
uint64_t      wtasctsr;
char     filler__010[8];
cfstat commitcf;
cfstat prepcf;
char     filler__012[20][20];
uint64_t      wtaspret;
uint64_t      wtasprct;
uint32_t wtasprn;
uint32_t filler__013;
uint64_t      wtaslmax[32];
} wtas;
#define WTAS_VER   8
#define WTAS_VER_1 1
#define WTAS_VER_2 2
#define WTAS_VER_3 3
#define WTAS_VER_4 4
#define WTAS_VER_5 5
#define WTAS_VER_8 8
#define WTASAEOT   0x20000000
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   wtidshex;
int16_t   wtidlen;
char     wtideyec[4];
int32_t   wtidatyp;
char     wtidccn[8];
char     wtidopid[8];
char     wtidnid[16];
char     wtidcori[12];
char     wtiduowi[24];
char     wtidacct[22];
char     wtidchl[20];
char     wtidchlc[48];
char     wtidctxt[16];
char     wtidtran[8];
char     filler__001[2];
uint32_t   wtidcfwd;
uint32_t   wtidcbwd;
uint32_t   wtidwtas;
} wtid;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   sm115len;
int16_t   sm115sgd;
char     sm115flg[1];
char     sm115rty[1];
uint32_t  sm115tme;
uint32_t  sm115dte;
char     sm115sid[4];
char     sm115SSI[4];
uint16_t  sm115stf;
char     sm115rel[3];
char     filler_001[1];
} qwst;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws00pso;
uint16_t  qws00psl;
uint16_t  qws00psn;
uint32_t  qws00r1o;
uint16_t  qws00r1l;
uint16_t  qws00r1n;
uint32_t  qws00r2o;
uint16_t  qws00r2l;
uint16_t  qws00r2n;
uint32_t  qws00r3o;
uint16_t  qws00r3l;
uint16_t  qws00r3n;
uint32_t  qws00r4o;
uint16_t  qws00r4l;
uint16_t  qws00r4n;
uint32_t  qws00r5o;
uint16_t  qws00r5l;
uint16_t  qws00r5n;
uint32_t  qws00r6o;
uint16_t  qws00r6l;
uint16_t  qws00r6n;
uint32_t  qws00r7o;
uint16_t  qws00r7l;
uint16_t  qws00r7n;
uint32_t  qws00r8o;
uint16_t  qws00r8l;
uint16_t  qws00r8n;
uint32_t  qws00r9o;
uint16_t  qws00r9l;
uint16_t  qws00r9n;
uint32_t  qws00rao;
uint16_t  qws00ral;
uint16_t  qws00ran;
uint32_t  qws00rbo;
uint16_t  qws00rbl;
uint16_t  qws00rbn;
} qws0;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws10pso;
uint16_t  qws10psl;
uint16_t  qws10psn;
uint32_t  qws10r1o;
uint16_t  qws10r1l;
uint16_t  qws10r1n;
uint32_t  qws10r2o;
uint16_t  qws10r2l;
uint16_t  qws10r2n;
uint32_t  qws10r3o;
uint16_t  qws10r3l;
uint16_t  qws10r3n;
uint32_t  qws10r4o;
uint16_t  qws10r4l;
uint16_t  qws10r4n;
uint32_t  qws10r5o;
uint16_t  qws10r5l;
uint16_t  qws10r5n;
uint32_t  qws10r6o;
uint16_t  qws10r6l;
uint16_t  qws10r6n;
uint32_t  qws10r7o;
uint16_t  qws10r7l;
uint16_t  qws10r7n;
uint32_t  qws10r8o;
uint16_t  qws10r8l;
uint16_t  qws10r8n;
} qws1;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws30pso;
uint16_t  qws30psl;
uint16_t  qws30psn;
uint32_t  qws30r1o;
uint16_t  qws30r1l;
uint16_t  qws30r1n;
} qws3;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws50pso;
uint16_t  qws50psl;
uint16_t  qws50psn;
uint32_t  qws50r1o;
uint16_t  qws50r1l;
uint16_t  qws50r1n;
} qws5;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws80pso;
uint16_t  qws80psl;
uint16_t  qws80psn;
uint32_t  qws80r1o;
uint16_t  qws80r1l;
uint16_t  qws80r1n;
} qws8;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qws90pso;
uint16_t  qws90psl;
uint16_t  qws90psn;
uint32_t  qws90r1o;
uint16_t  qws90r1l;
uint16_t  qws90r1n;
} qws9;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qwsx0pso;
uint16_t  qwsx0psl;
uint16_t  qwsx0psn;
uint32_t  qwsx0r1o;
uint16_t  qwsx0r1l;
uint16_t  qwsx0r1n;
uint32_t  qwsx0r2o;
uint16_t  qwsx0r2l;
uint16_t  qwsx0r2n;
uint32_t  qwsx0r3o;
uint16_t  qwsx0r3l;
uint16_t  qwsx0r3n;
uint32_t  qwsx0r4o;
uint16_t  qwsx0r4l;
uint16_t  qwsx0r4n;
uint32_t  qwsx0r5o;
uint16_t  qwsx0r5l;
uint16_t  qwsx0r5n;
} qwsx;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
int16_t   sm116len;
int16_t   sm116sgd;
char     sm116flg[1];
char     sm116rty[1];
uint32_t  sm116tme;
uint32_t  sm116dte;
char     sm116sid[4];
char     sm116SSI[4];
uint16_t  sm116stf;
char     sm116rel[3];
char     filler_001[1];
} qwas;
#if defined(__cplusplus)
}
#endif
#if defined(__cplusplus)
extern "C" {
#endif
typedef
struct {
uint32_t  qwa01pso;
uint16_t  qwa01psl;
uint16_t  qwa01psn;
uint32_t  qwa01r1o;
uint16_t  qwa01r1l;
uint16_t  qwa01r1n;
uint32_t  qwa01r2o;
uint16_t  qwa01r2l;
uint16_t  qwa01r2n;
uint32_t  qwa01r3o;
uint16_t  qwa01r3l;
uint16_t  qwa01r3n;
uint32_t  qwa01r4o;
uint16_t  qwa01r4l;
uint16_t  qwa01r4n;
uint32_t  qwa01r5o;
uint16_t  qwa01r5l;
uint16_t  qwa01r5n;
uint32_t  qwa01r6o;
uint16_t  qwa01r6l;
uint16_t  qwa01r6n;
} qwa0;
#if defined(__cplusplus)
}
#endif
#endif
