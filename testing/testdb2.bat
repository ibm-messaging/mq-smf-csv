@echo off
rem -----------------------------------------------------
rem Note that this script tries to load tables that
rem may not have not been created if that class of data was
rem not collected. There may also be additional files created
rem that are NOT in this list, again depending on classes 
rem collected. So review this list and compare it
rem to the files actually created.
rem  
rem A failure to load a non-existent file is not a real error.
rem -----------------------------------------------------

db2 connect to mydb

rem Where are the files located? Change this for your loads
cd c:\temp\smf\out

db2 -tf smf-mqtables.ddl

db2 load client from SMF-Q5ST.csv of del insert into mqsmf.Q5ST
db2 load client from SMF-QEST.csv of del insert into mqsmf.QEST
db2 load client from SMF-QIST.csv of del insert into mqsmf.QIST

db2 load client from SMF-QJST.csv of del insert into mqsmf.QJST
db2 load client from SMF-QLST.csv of del insert into mqsmf.QLST

db2 load client from SMF-QMAC.csv of del insert into mqsmf.QMAC
db2 load client from SMF-QMST.csv of del insert into mqsmf.QMST
db2 load client from SMF-QPST.csv of del insert into mqsmf.QPST
db2 load client from SMF-QSST.csv of del insert into mqsmf.QSST
db2 load client from SMF-QTST.csv of del insert into mqsmf.QTST
db2 load client from SMF-WQ.csv   of del insert into mqsmf.WQ
db2 load client from SMF-WTAS.csv of del insert into mqsmf.WTAS
db2 load client from SMF-WTID.csv of del insert into mqsmf.WTID
db2 load client from SMF-QESD.csv    of del insert into mqsmf.QESD
db2 load client from SMF-QCTDSP.csv  of del insert into mqsmf.QCTDSP
db2 load client from SMF-QCTDNS.csv  of del insert into mqsmf.QCTDNS
db2 load client from SMF-QCTADP.csv  of del insert into mqsmf.QCTADP
db2 load client from SMF-QCST.csv    of del insert into mqsmf.QCST
db2 load client from SMF-QCCT.csv    of del insert into mqsmf.QCCT
