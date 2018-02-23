
History
=======
March 2016 (v1.0)
* Initial release

April 2016 (v1.1)
* Update to support MQ V9 pageset statistics
* Improved formatting for import to Access
* Fixed some mis-labelled columns

June 2016 (v1.1.1)
* Add the "hidden" WTASCORR field
* Print the buildtime to help know what version is in use
* Clearer printing of which records were unrecognised

June 2016 (v1.1.2)
* Add SQL mode to generate DDL corresponding to column headings
* printWQ now has more descriptive column names

July 2016
* Add progress indicator

August 2016
* Add correlator field to better link WQ/WTID/WTAS
* Add user-written content in docs directory. How to use SQL.

October 2016
* Support reading from large (>2GB) files
* SMF 115 subtype 231 (CHIN) records do not use same scale for qwhsdurn
(though this seems to be undocumented)

February 2017
* STCK durations printed as just microseconds value when in SQL mode. Still
split as seconds/microseconds otherwise.

May 2017
* Added support for the 903 SMF extensions
* Using a VERS flag during the build to select correct base header file
and features

Oct 2017
* Found that some 115 subtypes do not have a QWHS structure correctly
created; cannot rely on it being there

Jan 2018
* QCTDNS structure is not initialised when there has been no activity
* Can now process files that have not been downloaded with the RDW option,
as some sites do not have suitable ftp package available. New option
is "-f NORDW".

Feb 2018
* Added a simple "checkpoint" manager to recover after failures of the
formatter. This may help when you run out of disk space while formatting
very large output - the program may be able to restart from a more recent
checkpoint.
* Discovered a few fields in the WTAS structure are not used in the way
the documentation and product header files claim.
