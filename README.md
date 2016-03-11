# mq-smf-csv
Basic formatter for IBM MQ's z/OS SMF records that can run on workstations
to simplify import of statistics to spreadsheets and databases.

Description
===========
On z/OS, IBM MQ generates SMF accounting and statistics records to track
the usage of the product. These are often used for capacity planning, 
for performance tuning, or for chargeback.

This package has been created to simplify the task of doing your 
own analysis of MQ SMF records from a z/OS system, by making it 
easy to import formatted records into a database or spreadsheet.

The program takes the data from a z/OS dataset, downloaded to a 
local file, and then runs on a Windows or Unix system, creating 
standard CSV (comma-separated value) output files. 

Unlike packages such as SupportPac MP1B, there is no interpretation
at all of the SMF data. That can be done by your own spreadsheet or
database programs and macros.

This makes the code for the program very easy to understand; all the 
structures in the SMF records are printed directly.

See mqsmfcsv.doc for more information on using or building this code, and
thoughts on possible enhancements to the code or this repository.


History
=======
March 2016 - Initial release


Pull requests
=============
Contributions to this package can be accepted under the terms of the 
IBM Contributor License Agreement, found in the file CLA.md of this repository.

When submitting a pull request, you must include a statement stating you accept the terms in CLA.md.
