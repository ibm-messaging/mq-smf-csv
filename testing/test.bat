set INDIR=y:\metaylor\mf\github\ibm-messaging\mq-smf-csv\bin\win
set OUTDIR=c:\temp\smf\out

mkdir %OUTDIR%

del %OUTDIR%\SMF-*.*

%INDIR%\mqsmfcsv.exe -i data\TEST116.dat -m 200   -o %outdir%
%INDIR%\mqsmfcsv.exe -i data\TEST115.dat          -o %outdir% -a

y:
cd %INDIR%
