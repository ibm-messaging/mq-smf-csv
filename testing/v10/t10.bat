@echo off

setlocal

cd ../../src

rem Assumed you've already run something to setup a Visual Studio compile environment
nmake -f Makefile.win

del c:\temp\smf\v10\*.* /q /f

set d=0

mqsmfcsv -o c:\temp\smf\v10 -d %d% -i ../testing/data/v10smfa.nordw.bin -f nordw -a
rem mqsmfcsv -o c:\temp\smf\v10 -d %d% -i ../testing/data/v10smfa.bin -f sql 2>&1 | tee t10.out

rem  mqsmfcsv -o c:\temp\smf\v10 -d %d% -i ../testing/data/CUST2_115.dat -m 100 2>&1 | tee t10.out

rem mqsmfcsv -i ../testing/data/SMF116.dat  -o c:\temp\smf\v10 -m 100 -t 10 -a -d %d% -f csv
rem mqsmfcsv -i ../testing/data/TESTCHL.dat -o c:\temp\smf\v10              -a -d %d% -f csv
rem mqsmfcsv -i ../testing/data/TEST115.dat -o c:\temp\smf\v10 -m 100 -h No -a -d %d% -f csv

rem mqsmfcsv -i ../testing/data/TEST123.dat -o  c:\temp\smf\v10 -m 100 -h No -a -d %d% -f csv
rem mqsmfcsv -i ../testing/data/AMS180.dat -o  c:\temp\smf\v10 -m 100 -h No -a -d %d% -f csv


