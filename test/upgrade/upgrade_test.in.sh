#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2009 met.no
#
#  Contact information:
#  Norwegian Meteorological Institute
#  Box 43 Blindern
#  0313 OSLO
#  NORWAY
#  E-mail: wdb@met.no
#
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# Preamble

version() {
	echo $0 __WDB_VERSION__
}

help() {
	version
	echo
	echo "Usage: $0"
	echo
	echo "This program tests that upgrade of a clean WDB database is"
	echo "possible. It checks out the old version of WDB, installs it,"
	echo "runs the upgrade algorithms, and performs some basic checks of"
	echo "the upgraded database."
	echo
	echo "Note that the upgrade check has to be run from the build"
	echo "directory of WDB"
	echo
	echo "OPTIONS:"
	echo 
	echo "--help       Show this help message"
	echo "--version    Show version information, and then exit"
}

while [ x`echo $1 | cut -b-2` = "x--" ]; do
	case "$1" in
	--help)
		help
		exit 0
		;;
	--version)
		version
		exit 0
		;;
	*)
		error Unknown argument: $1
		;;
	esac
	shift
done

# Variables
WORK_DIR=`pwd | sed -e 's,^[^:\\/]:[\\/],/,'`
UPGRADE_DIR=__WDB_BUILDDIR__/upgradecheck
OLDWDB_DIR=$UPGRADE_DIR/__OLD_VERSION__
NEWWDB_DIR=$UPGRADE_DIR/__WDB_VERSION__
INSTALL_DIR0=$UPGRADE_DIR/_inst0
INSTALL_DIR1=$UPGRADE_DIR/_inst1
TEST_DB0=wdb_upgradecheck0
TEST_DB1=wdb_upgradecheck1
WDBSVN_DIR=https://wdb.svn.sourceforge.net/svnroot/wdb
TEST_WRITE="./testWrite"

test_cleanup() {
	echo "Cleaning up database"
	cd $WORK_DIR
	rm -rf $UPGRADE_DIR
	dropdb $TEST_DB0
	dropdb $TEST_DB1
}

test_datawrite() {
	# Insert some test data
	echo -n "Inserting test data into the database... "
	days="01"
	hours="00 06 12 18"
	timesteps="0 6 12 18"
	for dd in $days
	  do
	  for hh in $hours 
	    do
	    for timestep in $timesteps
		  do
		  vh=`expr $timestep + $hh`
		  if test $vh -ge 24; then
			vh=`expr $vh - 24`
		  fi		
		  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'air pressure' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
		  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'air temperature' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
		  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'snowfall amount' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	    done
	  done
	done
	echo "done"
}

# Create directories
mkdir -p $OLDWDB_DIR
mkdir -p $NEWWDB_DIR

# Create Baseline
export PGDATABASE=$TEST_DB0
export PSQL="psql -d $TEST_DB0"

cd $WORK_DIR

make dist
mv __WDB_DISTDIR__.tar.gz $UPGRADE_DIR
cd $UPGRADE_DIR
tar xvf __WDB_DISTDIR__.tar.gz
cd __WDB_DISTDIR__
./configure --srcdir=. --prefix=$INSTALL_DIR0 --with-database-name=$TEST_DB0
if [ 0 != $? ]; then
    echo "ERROR: Failed to run configure on clean database"
	test_cleanup
    exit 1
fi
make all install installcheck
if [ 0 != $? ]; then
    echo "ERROR: Install anc check of clean database failed"
	test_cleanup
    exit 1
fi

# Write test data
test_datawrite

# -- Check for presence of test data 
echo -n "Checking number of rows in gridvalue... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi
echo -n "Checking number of rows in fileblob... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi

echo -n "Writing data check files... "
$PSQL -Atc "SELECT dataprovidername, st_astext(placegeometry), placeindeterminatecode, referencetime, validtimefrom, validtimeto, validtimeindeterminatecode, valueparametername, valueunitname, levelparametername, levelunitname, levelFrom, levelTo, levelindeterminatecode, dataversion, maxdataversion, confidencecode FROM test.gridvalue_v  order by 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17" -t -o $UPGRADE_DIR/data_check0.txt 
echo "done"

# Ensure we are in build directory
cd $WORK_DIR

# Create Upgrade
export PGDATABASE=$TEST_DB1
export PSQL="psql -d $TEST_DB1"

# Check out WDB
svn co $WDBSVN_DIR/tags/wdb___OLD_VERSION__ $OLDWDB_DIR
if [ 0 != $? ]; then
    echo "ERROR: Failed to check out the old version of WDB."
	test_cleanup
    exit 1
fi

# Make and install
cd $OLDWDB_DIR
if [ 0 != $? ]; then
    echo "ERROR: Could not locate the old version of WDB."
	test_cleanup
    exit 1
fi
./autogen.sh
./configure --srcdir=. --prefix=$INSTALL_DIR1 --with-database-name=$TEST_DB1
make all install
if [ 0 != $? ]; then
    echo "ERROR: Install of old database failed"
	test_cleanup
    exit 1
fi

# Write test data
test_datawrite

# -- Check for presence of test data 
echo -n "Checking number of rows in gridvalue... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi
echo -n "Checking number of rows in fileblob... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi

# Return to build directory
cd $WORK_DIR

# Upgrade database
make dist
mv __WDB_DISTDIR__.tar.gz $UPGRADE_DIR
cd $UPGRADE_DIR
tar xvf __WDB_DISTDIR__.tar.gz
cd __WDB_DISTDIR__
./configure --srcdir=. --prefix=$INSTALL_DIR1 --with-database-name=$TEST_DB1
if [ 0 != $? ]; then
    echo "ERROR: Failed to run configure on the new database version"
	test_cleanup
    exit 1
fi
make all install
if [ 0 != $? ]; then
    echo "ERROR: Upgrading of the database failed"
	test_cleanup
    exit 1
fi

# -- Check for presence of test data 
echo -n "Checking number of rows in gridvalue... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi
echo -n "Checking number of rows in fileblob... "
if ! $PSQL -Atc "SELECT count(*) FROM test.gridvalue_v" | grep -qE "48"
then
   	echo "failed"
	test_cleanup
    exit 1
else
	echo "48"
fi

echo -n "Writing data check files... "
$PSQL -Atc "SELECT dataprovidername, st_astext(placegeometry), placeindeterminatecode, referencetime, validtimefrom, validtimeto, validtimeindeterminatecode, valueparametername, valueunitname, levelparametername, levelunitname, levelFrom, levelTo, levelindeterminatecode, dataversion, maxdataversion, confidencecode FROM test.gridvalue_v order by 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17" -t -o $UPGRADE_DIR/data_check1.txt 
echo "done"

echo -n "Diff between data check files... "
if diff $UPGRADE_DIR/data_check0.txt $UPGRADE_DIR/data_check1.txt
then
   	echo "done"
else
	echo "failed"
	# Save diff files
	cp $UPGRADE_DIR/data_check0.txt $UPGRADE_DIR/data_check1.txt $WORK_DIR
	test_cleanup
    exit 1
fi

# Test for point-based data (grid and point values)

# -- Check the integrity of the database
make installcheck
if [ 0 != $? ]; then
    echo "ERROR: Installcheck of the upgraded database failed"
	test_cleanup
    exit 1
fi

# -- Exit
cd $WORK_DIR

test_cleanup

echo "Upgrade worked correctly"
