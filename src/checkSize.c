#include "stdio.h"
#include "stddef.h"
#include "../src/mqsmfstruc.h"

void checkStructureSizes(FILE *fp) {

fprintf(fp,"%s : %d\n","q5st",sizeof(q5st));
fprintf(fp,"%s : %d\n","qcct",sizeof(qcct));
fprintf(fp,"%s : %d\n","qct_dsp",sizeof(qct_dsp));
fprintf(fp,"%s : %d\n","qct_adp",sizeof(qct_adp));
fprintf(fp,"%s : %d\n","qct_ssl",sizeof(qct_ssl));
fprintf(fp,"%s : %d\n","qct_dns",sizeof(qct_dns));
fprintf(fp,"%s : %d\n","qcst",sizeof(qcst));
fprintf(fp,"%s : %d\n","qest",sizeof(qest));
fprintf(fp,"%s : %d\n","qesd",sizeof(qesd));
fprintf(fp,"%s : %d\n","qist",sizeof(qist));
fprintf(fp,"%s : %d\n","qis1",sizeof(qis1));
fprintf(fp,"%s : %d\n","qjst",sizeof(qjst));
fprintf(fp,"%s : %d\n","qlst",sizeof(qlst));
fprintf(fp,"%s : %d\n","qmac",sizeof(qmac));
fprintf(fp,"%s : %d\n","qmst",sizeof(qmst));
fprintf(fp,"%s : %d\n","qpst",sizeof(qpst));
fprintf(fp,"%s : %d\n","qsgm",sizeof(qsgm));
fprintf(fp,"%s : %d\n","qsph",sizeof(qsph));
fprintf(fp,"%s : %d\n","qsrs",sizeof(qsrs));
fprintf(fp,"%s : %d\n","qsst",sizeof(qsst));
fprintf(fp,"%s : %d\n","qtst",sizeof(qtst));
fprintf(fp,"%s : %d\n","qwac",sizeof(qwac));
fprintf(fp,"%s : %d\n","qwhc",sizeof(qwhc));
fprintf(fp,"%s : %d\n","qwhs",sizeof(qwhs));
fprintf(fp,"%s : %d\n","wtas",sizeof(wtas));
fprintf(fp,"%s : %d\n","wtid",sizeof(wtid));
fprintf(fp,"%s : %d\n","qwst",sizeof(qwst));
fprintf(fp,"%s : %d\n","qws0",sizeof(qws0));
fprintf(fp,"%s : %d\n","qws1",sizeof(qws1));
fprintf(fp,"%s : %d\n","qws5",sizeof(qws5));
fprintf(fp,"%s : %d\n","qws8",sizeof(qws8));
fprintf(fp,"%s : %d\n","qws9",sizeof(qws9));
fprintf(fp,"%s : %d\n","qwsx",sizeof(qwsx));
fprintf(fp,"%s : %d\n","qwas",sizeof(qwas));
fprintf(fp,"%s : %d\n","qwa0",sizeof(qwa0));
fprintf(fp,"%s : %d\n","wq  ",sizeof(wq));
fprintf(fp,"wq->maxqdpth: %d\n",offsetof(wq,maxqdpth));
fprintf(fp,"wq->z__001  : %d\n",offsetof(wq,z__001  ));
}
