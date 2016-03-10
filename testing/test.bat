set INDIR=y:\metaylor\mf\L\smf
set OUTDIR=c:\temp\smf\out

mkdir %OUTDIR%

del %OUTDIR%\SMF-*.*

%INDIR%\mqsmfcsv.exe -i c:\temp\smf\data\TEST116.dat -m 200   -o %outdir%
%INDIR%\mqsmfcsv.exe -i c:\temp\smf\data\TEST115.dat          -o %outdir% -a

y:
cd %INDIR%
