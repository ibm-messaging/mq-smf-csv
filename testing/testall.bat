set INDIR=y:\metaylor\mf\L\mq-smf-csv\src
set OUTDIR=c:\temp\smf\out

mkdir %OUTDIR%

del /q %OUTDIR%\*.*

%INDIR%\mqsmfcsv.exe -i c:\temp\smf\data\TEST116.dat  -o %outdir%
%INDIR%\mqsmfcsv.exe -i c:\temp\smf\data\TEST115.dat  -o %outdir% -a


y:
cd %INDIR%
