DIR=/tmp/smf/out
mkdir -p $DIR
rm -f $DIR/*
mqsmfcsv   -i ../testing/data/TESTCHL.dat  -o $DIR         -a
mqsmfcsv   -i ../testing/data/TEST115.dat  -o $DIR   -m 10 -h No -a
mqsmfcsv   -i ../testing/data/TEST115.dat  -o $DIR   -m 10 -a -d 1
