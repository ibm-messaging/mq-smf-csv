
History (newest at top)
=======================

Oct 2024 (v5.5.1)
* Update for MQ 9.4.1
* Some QESD fields renamed to match product changes
* New sample data testing/data/SMF_MQ940_NORDW.dat 
  * Use "-f nordw" to format
  * Thanks to Lyn for providing the file 

Jun 2024 (v5.5)
* Update for MQ 9.4.0
* New stats available for zHyperlink
* Remove trailing spaces for ApplType (#1)
* Initial build option added for MacOS

Feb 2024 (v5.4.5)
* Update for MQ V9.3.5 
* QCCT now has info about 64-bit CHIN storage use
  * Previous "Storage_Usage" renamed to "Storage_Usage_31" 
  * New fields added

Oct 2023 (v5.4.4)
* Try to recognise some input file corruption and cleanly exit
* Update for MQ V9.3.4 (no changes to SMF records)

Jun 2023 (v5.4.3)
* Update for MQ V9.3.3
  * Many more QQST fields
* New "-f jsoncompact" option for single-line JSON records
* Add Task_Index field to channel structures, to easier tie up with MP1B output
* Correct a couple of column names in WQ structure

Feb 2023 (v5.4.2)
* Update for MQ V9.3.2
* WQ structure extended with streamed message count
* Fix problem when final record in input data was truncated/corrupt
* Removed QIST RAIO/RABP fields as they are never filled in by qmgr so misleading

Oct 2022 (v5.4.1)
* Update for MQ V9.3.1
* Extended QQST structure in the 115 records
* Add config option (-g) to print timestamps in GMT.
  * Default prints as localtime

Jul 2022 (v5.4)
* Update for MQ V9.3.0
* New QQST structure in the 115 records

Nov 2021 (v5.3.3)
* Update for MQ V9.2.4 (no functional change)
* Visual Studio 2022 setup script

Jul 2021 (v5.3.2)
* Recognise and ignore 115 subtype 240 (an internal undocumented stucture)
* Fix an error in QESD interpretation
* Update for MQ V9.2.3 (no real change)

Mar  2021 (v5.3.1)
* Update for MQ V9.2.2 (no real change)

July 2020 (v5.3)
* Update for MQ V9.2
* Support for AMS SMF type 180 (thanks to Andrew Mattingly)
* Experimental support for z/OS Connect EE type 123 (subtype version 1)

Apr 2020 (v5.2.1)
* Update for MQ V9.1.5
* Include mqsmfstruc.h so you can build without copying a header from z/OS

Dec 2019 (v5.2.0)
* Add support for MQ 9.1.4 encrypted datasets and logs
* Update the header filter to force C99 standardised types internally (eg uint32_t)

Sep 2019 (v5.1.5)
* Percentage processed was not showing correctly for very large files on Windows

Aug 2019 (v5.1.4)
* Newer compilers have changed bitfield structure padding layouts

Aug 2019 (v5.1.3)
* Print QJST SLPTU as number not timestamp

Jul 2019 (v5.1.2)
* Make sure Windows program pointer sizes are correct

Apr 2019 (v5.1.1)
* The 912 changes broke QJST printing

Mar 2019 (v5.1.0)
* Add support for MQ 9.1.2 zHyperWrite

Sep 2018 (v5.0.0)
* Add support for MySQL imports

Jul 2018 (v4.3.0)
* Add command for converting VBS dataset to USS on z/OS

Jun 2018 (v4.2.0)
* Visual Studio 2017 compiler warnings fixed
* -f sql was not setting correct internal flags

May 2018 (v4.1.0)
* Ability to add SQL/DDL commands to generated DDL file from templates.

Apr 2018 (v4.0)
* Add -f option to generate JSON output format
* Allow -o to specify stdout for output. Modify printf to select stdout/stderr
as appropriate
* Add data processing rate (MB/sec) to output

Mar 2018 (v3.2.0)
* Add code to help add INDEX to columns in the SQL DDL output
* Add optimisation to Makefiles
* Even 64-bit values may be too large when they are not really counters

Feb 2018 (v3.1.0)
* Added a simple "checkpoint" manager to recover after failures of the
formatter. This may help when you run out of disk space while formatting
very large output - the program may be able to restart from a more recent
checkpoint.
* Discovered a few fields in the WTAS structure are not used in the way
the documentation and product header files claim.
* Some SMF integers are too large for the INTEGER datatype. The unsigned
32-bits compared to the SQL which is signed 32-bit. So switching
to always use BIGINT.

Jan 2018 (v3.0.0)
* QCTDNS structure is not initialised when there has been no activity
* Can now process files that have not been downloaded with the RDW option,
as some sites do not have suitable ftp package available. New option
is "-f NORDW".

Oct 2017 (v2.0.1)
* Found that some 115 subtypes do not have a QWHS structure correctly
created; cannot rely on it being there

May 2017 (v2.0.0)
* Added support for the 903 SMF extensions
* Using a VERS flag during the build to select correct base header file
and features

February 2017
* STCK durations printed as just microseconds value when in SQL mode. Still
split as seconds/microseconds otherwise.

October 2016
* Support reading from large (>2GB) files
* SMF 115 subtype 231 (CHIN) records do not use same scale for qwhsdurn
(though this seems to be undocumented)

August 2016 (v1.2.1)
* Add correlator field to better link WQ/WTID/WTAS
* Add user-written content in docs directory. How to use SQL.

July 2016
* Add progress indicator

June 2016 (v1.2)
* Add SQL mode to generate DDL corresponding to column headings
* printWQ now has more descriptive column names

June 2016 (v1.1.1)
* Add the "hidden" WTASCORR field
* Print the buildtime to help know what version is in use
* Clearer printing of which records were unrecognised

April 2016 (v1.1)
* Update to support MQ V9 pageset statistics
* Improved formatting for import to Access
* Fixed some mis-labelled columns

March 2016 (v1.0)
* Initial release
