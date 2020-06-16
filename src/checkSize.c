#include "stdio.h"
#include "stddef.h"
#include "stdint.h"

#if defined(PLATFORM_WINDOWS)
#include "mqsmfstrucW.h"
#else
#include "mqsmfstrucU.h"
#endif

void checkStructureSizes(FILE *fp) {

fprintf(fp,"%s : %d\n","q5st",(int)sizeof(q5st));
fprintf(fp,"%s : %d\n","qcct",(int)sizeof(qcct));
fprintf(fp,"%s : %d\n","qct_dsp",(int)sizeof(qct_dsp));
fprintf(fp,"%s : %d\n","qct_adp",(int)sizeof(qct_adp));
fprintf(fp,"%s : %d\n","qct_ssl",(int)sizeof(qct_ssl));
fprintf(fp,"%s : %d\n","qct_dns",(int)sizeof(qct_dns));
fprintf(fp,"%s : %d\n","qcst",(int)sizeof(qcst));
fprintf(fp,"%s : %d\n","qest",(int)sizeof(qest));
fprintf(fp,"%s : %d\n","qesd",(int)sizeof(qesd));
fprintf(fp,"%s : %d\n","qist",(int)sizeof(qist));
fprintf(fp,"%s : %d\n","qis1",(int)sizeof(qis1));
fprintf(fp,"%s : %d\n","qjst",(int)sizeof(qjst));
fprintf(fp,"%s : %d\n","qlst",(int)sizeof(qlst));
fprintf(fp,"%s : %d\n","qmac",(int)sizeof(qmac));
fprintf(fp,"%s : %d\n","qmst",(int)sizeof(qmst));
fprintf(fp,"%s : %d\n","qpst",(int)sizeof(qpst));
fprintf(fp,"%s : %d\n","qsgm",(int)sizeof(qsgm));
fprintf(fp,"%s : %d\n","qsph",(int)sizeof(qsph));
fprintf(fp,"%s : %d\n","qsrs",(int)sizeof(qsrs));
fprintf(fp,"%s : %d\n","qsst",(int)sizeof(qsst));
fprintf(fp,"%s : %d\n","qtst",(int)sizeof(qtst));
fprintf(fp,"%s : %d\n","qwac",(int)sizeof(qwac));
fprintf(fp,"%s : %d\n","qwhc",(int)sizeof(qwhc));
fprintf(fp,"%s : %d\n","qwhs",(int)sizeof(qwhs));
fprintf(fp,"%s : %d\n","wtas",(int)sizeof(wtas));
fprintf(fp,"%s : %d\n","wtid",(int)sizeof(wtid));
fprintf(fp,"%s : %d\n","qwst",(int)sizeof(qwst));
fprintf(fp,"%s : %d\n","qws0",(int)sizeof(qws0));
fprintf(fp,"%s : %d\n","qws1",(int)sizeof(qws1));
fprintf(fp,"%s : %d\n","qws5",(int)sizeof(qws5));
fprintf(fp,"%s : %d\n","qws8",(int)sizeof(qws8));
fprintf(fp,"%s : %d\n","qws9",(int)sizeof(qws9));
fprintf(fp,"%s : %d\n","qwsx",(int)sizeof(qwsx));
fprintf(fp,"%s : %d\n","qwas",(int)sizeof(qwas));
fprintf(fp,"%s : %d\n","qwa0",(int)sizeof(qwa0));
fprintf(fp,"%s : %d\n","wq  ",(int)sizeof(wq));
fprintf(fp,"wq->maxqdpth: %d\n",(int)offsetof(wq,maxqdpth));
fprintf(fp,"wq->z__001  : %d\n",(int)offsetof(wq,z__001  ));
}
