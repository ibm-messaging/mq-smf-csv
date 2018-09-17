# This script is an example of how to populate a Db2 database
#
# You will need to modify variables such as where the formatted csv files
# have been created, and which database/schema to work with.

# Where are the output files
cd /tmp/smf/out
chmod a+rx .

# Set up the environment for the db2 command
 . /home/db2inst3/sqllib/db2profile
export LIBPATH=/opt/freeware/lib64:/opt/freeware/lib:$LIBPATH
export PATH=$PATH:.

dbName="metdb"
schema="MQSMF"

# Connect
db2 -x connect to $dbName >/dev/null 2>&1
if [ $? -ne 0 ]
then
  echo "Connection to DB2 $dbName failed"
  exit 1
else
 echo "Connected to $dbName"
fi

# Run the DDL to create the tables
db2 -tf SMF-MQTABLES.ddl


# And import all the data files
for x in *csv
do
  b=`basename $x .csv`
  f=`echo $b | sed "s/SMF-//g"`

  echo Loading from $b into $f
  chmod a+r $x
  db2 load client from $x of del insert into $schema.$f
done
