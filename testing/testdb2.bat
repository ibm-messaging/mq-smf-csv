db2 connect to mydb
db2 -tf c:\temp\smf\out\smf-mqtables.ddl

db2 load client from c:\temp\smf\out\SMF-Q5ST.csv of del insert into mqsmf.Q5ST
db2 load client from c:\temp\smf\out\SMF-QEST.csv of del insert into mqsmf.QEST
db2 load client from c:\temp\smf\out\SMF-QIST.csv of del insert into mqsmf.QIST

db2 load client from c:\temp\smf\out\SMF-QJST.csv of del insert into mqsmf.QJST
db2 load client from c:\temp\smf\out\SMF-QLST.csv of del insert into mqsmf.QLST

db2 load client from c:\temp\smf\out\SMF-QMAC.csv of del insert into mqsmf.QMAC
db2 load client from c:\temp\smf\out\SMF-QMST.csv of del insert into mqsmf.QMST
db2 load client from c:\temp\smf\out\SMF-QPST.csv of del insert into mqsmf.QPST
db2 load client from c:\temp\smf\out\SMF-QSST.csv of del insert into mqsmf.QSST
db2 load client from c:\temp\smf\out\SMF-QTST.csv of del insert into mqsmf.QTST
db2 load client from c:\temp\smf\out\SMF-WQ.csv   of del insert into mqsmf.WQ
db2 load client from c:\temp\smf\out\SMF-WTAS.csv of del insert into mqsmf.WTAS
db2 load client from c:\temp\smf\out\SMF-WTID.csv of del insert into mqsmf.WTID
db2 load client from c:\temp\smf\out\SMF-QESD.csv    of del insert into mqsmf.QESD
db2 load client from c:\temp\smf\out\SMF-QCTDSP.csv  of del insert into mqsmf.QCTDSP
db2 load client from c:\temp\smf\out\SMF-QCTDNS.csv  of del insert into mqsmf.QCTDNS
db2 load client from c:\temp\smf\out\SMF-QCTADP.csv  of del insert into mqsmf.QCTADP
db2 load client from c:\temp\smf\out\SMF-QCST.csv    of del insert into mqsmf.QCST
db2 load client from c:\temp\smf\out\SMF-QCCT.csv    of del insert into mqsmf.QCCT
