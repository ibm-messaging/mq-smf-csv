/*
 * Copyright (c) 2016,2018 IBM Corporation and other Contributors.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Douglas Burns
 */

/***************************************************************************/
/* This program is to be run on z/OS if the file transfer mechanism to get */
/* the SMF data to the workstation platform does not support VBS datasets. */
/*                                                                         */
/* For example sftp works from USS files and the SMF dataset needs to be   */
/* converted before sftp can process the z/OS file.                        */
/***************************************************************************/
#include <stdio.h>
#include <unistd.h>

#define    BUFFERLENGTH    10485760   /* Max length of record accepted  */

/**********************************************************************/
/* Function name:    main                                             */
/*                                                                    */
/* Description:      Reads the specified dataset and writes the       */
/*                   contents                                         */
/*                                                                    */
/* Receives:         Two parameters  - SMF VBS dataset to read        */
/*                                     USS file to write              */
/*                                                                    */
/*                                                                    */
/**********************************************************************/
int  main(int argc, char *argv[] )
{
    char inputDatasetName[4096];
    char escapedInputDatasetName[4096];
    char outputFileName[4096];

    FILE *inputFile;
    FILE *outputFile;

    char databuffer[BUFFERLENGTH];

    int dataLength = 0;
    int recordCount = 0;

    /*                                       */
    /* Handle the arguments passed           */
    /*                                       */
    if (argc < 3) {           /* need prog name + two parm      */
        printf("\n%s called with incorrect arguments\n",argv[0] );
        printf("  expected parms are DATASETNAME FILENAME\n ");
        printf("  where\n");
        printf("    DATASETNAME = name of input dataset\n");
        printf("    FILENAME    = name of output file\n");
        return(4);
    }
    strcpy(inputDatasetName, argv[1]);
    strcpy(outputFileName, argv[2]);

    strcpy(escapedInputDatasetName, "//'");
    strcat(escapedInputDatasetName, inputDatasetName);
    strcat(escapedInputDatasetName, "'");

    /*  Open input file */
    if ((inputFile = fopen(escapedInputDatasetName, "rb, type=record, recfm=*")) == NULL) {
        printf("Could not open dataset: %S\n", inputDatasetName);
        return(4);
    }

    /*  Open output file */
    if ((outputFile = fopen(outputFileName, "wb")) == NULL) {
        printf("Could not open file: %S\n", outputFileName);
        return(4);
    }

    /*  Read in the data */
    while (dataLength = fread(&databuffer, 1, BUFFERLENGTH, inputFile)) {
        recordCount++;

        /*  Write the data */
        fwrite(&databuffer, 1, dataLength, outputFile);
    }

    /*  Close files */
    fclose(inputFile);
    fclose(outputFile);

    printf("\n *** %d records written. ***\n", recordCount);

    printf("\n");
}
