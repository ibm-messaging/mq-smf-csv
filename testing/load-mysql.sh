# This script is an example of how to populate a MySQL database
#
# You will need to modify variables such as where the formatted csv files
# have been created, and which database/schema to work with.

# Set credentials
user="root"
pw="Passw0rd!"

# Where are the output files
cd /tmp/smf/out

# Which database/schema to work with
schema="MQSMF"

# Use the "force" option to make the command continue even if
# there are errors. Will be common to have the DROP TABLE fail for
# a new database.

# Run the DDL to create the tables
mysql -u $user --password=$pw --force < SMF-MQTABLES.ddl

# And import all the data files
for x in *csv
do
  b=`basename $x .csv`
  f=`echo $b | sed "s/SMF-//g"`

  echo Loading from $b into $f

  mysql -u $user --password=$pw --local-infile=1 --force << EOF
USE $schema;

load data local infile '$x' into table $f
 fields terminated by ','
 enclosed by '"'
 lines terminated by '\n';

EOF
done
