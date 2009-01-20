#!/bin/sh
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2008 met.no
##
##  Contact information:
##  Norwegian Meteorological Institute
##  Box 43 Blindern
##  0313 OSLO
##  NORWAY
##  E-mail: wdb@met.no
##
##  This is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Public License as published by
##  the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
# upgrade - upgrade the wdb system
#

UPGRADE_LIMIT_MAJOR=0
UPGRADE_LIMIT_MINOR=7
UPGRADE_LIMIT_MICRO=4

DEFAULT_DATABASE=`wdbConfiguration --database`@`wdbConfiguration --host`
DEFAULT_USER=`wdbConfiguration --user`
DEFAULT_PORT=`wdbConfiguration --port`

SCRIPT_VERSION=__WDB_VERSION__

SCRIPT_USAGE="Usage: $0 [OPTION]

The script upgrades the WDB system in the database specified by the 
wdbConfiguration program (unless overriden on the command line) to the 
latest version (version __WDB_VERSION__).

PGDATABASE        (default: $DEFAULT_DATABASE)
PGUSER            (default: $DEFAULT_USER)
PGPORT            (default: $DEFAULT_PORT)

If the named database does not exist, the upgrade script terminates.

Options:
-d NAME, --database=NAME    
                   upgrade database <NAME>
-u USER, --user=USER
                   upgrade as user <USER>
-p PORT, --port=PORT
                   upgrade on port <PORT>
--help             display this help and exit
--version          output version information and exit
"

# Parse command line
while test -n "$1"; do
    case "$1" in
	--database=*)
	WDB_INSTALL_DATABASE=`echo $1 | sed 's/--database=//'`
        shift
        continue;;
	-d)
	shift
	WDB_INSTALL_DATABASE=$1
	shift
	continue;;
	--user=*)
	WDB_INSTALL_USER=`echo $1 | sed 's/--user=//'`
        shift
        continue;;
	-u)
	shift
	WDB_INSTALL_USER=$1
	shift
	continue;;
	--port=*)
	WDB_INSTALL_PORT=`echo $1 | sed 's/--port=//'`
        shift
        continue;;
	-p)
	shift
	WDB_INSTALL_PORT=$1
	shift
	continue;;
	--help) 
	echo "$SCRIPT_USAGE"; exit 0;;
	--version) 
	echo "$0 $SCRIPT_VERSION"; exit 0;;
	*)
	shift
	continue;;
    esac
done

# Set defaults variables
# DATABASE_NAME
if test -z "$WDB_INSTALL_DATABASE"; then
    WDB_INSTALL_DATABASE=$DEFAULT_DATABASE
fi

# DATABASE_USER
if test -z "$WDB_INSTALL_USER"; then
	WDB_INSTALL_USER=$DEFAULT_USER
fi

# DATABASE_PORT
if test -z "$WDB_INSTALL_PORT"; then
	WDB_INSTALL_PORT=$DEFAULT_PORT
fi

# Start Upgrade
echo "---- wdb database upgrade ----"

# Directory for logging
export LOGDIR=/tmp/$USER/wdb/upgrade/logs/
mkdir -p $LOGDIR

# checking if we can find the sql source files
WDB_DATAMODEL_PATH=__WDB_DATADIR__/sql
WDB_METADATA_PATH=$WDB_DATAMODEL_PATH
WDB_CLEANUP_PATH=$WDB_DATAMODEL_PATH

if test ! -f $WDB_DATAMODEL_PATH/wdbTableDefinitions.sql; then
    echo "Error: Could not locate database installation files. Unable to install wdb."
    exit 1
fi

# Check for presence of database
WDB_NAME=`wdbConfiguration --database`
export $WDB_NAME
echo -n "checking connection to $WDB_NAME... "
# DB_CHECK= list database | isolate pattern WDB_NAME | split record |  
# grab first line (name) | trim whitesoace
DB_CHECK=`psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -l | sed -n /$WDB_NAME/p | sed -e 's/|/\n/' | sed q | sed -e 's/^[ \t]*//;s/[ \t]*$//'`
# Test whether database exists
if test "$DB_CHECK" = "$WDB_NAME"; then 
    echo "yes"
    DATABASE_EXISTS="yes"
else
    echo "no"
    DATABASE_EXISTS="no"
    echo "ERROR: Could not locate the database. Unable to upgrade ${WDB_NAME}."
    exit 1
fi

# Check if user has upgrade rights
# Set role to WDB_ADMIN

# Check version of database
echo -n "current version of WDB... "
PSQLARGS=`wdbConfiguration --psqlArgs`
current_version=`psql $PSQLARGS -l -c "select wci.version();" -q | sed -e '1,2d' | sed -e '2,$d' | sed 's/^[ ]//g'`
echo $current_version
major_version=`echo $current_version | sed 's/\..*$//'`
minor_version=`echo $current_version | sed 's/[0-9]*\.//' | sed 's/\..*$//'`
patch_version=`echo $current_version | sed 's/.*\.//'`

# Todo: Check that current version is upgradeable.

# Install Datamodel
echo -n "installing upgraded datamodel... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
\i $WDB_DATAMODEL_PATH/wdbTableDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbConstraintDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbMaterializedView.sql
\i $WDB_DATAMODEL_PATH/wdbViewDefinitions.sql
--\i $WDB_DATAMODEL_PATH/wdbTriggerDefinitions.sql
\i $WDB_DATAMODEL_PATH/wciViewDefinitions.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

# Install Metadata
echo -n "installing upgraded metadata... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_metadata.log
\set ON_ERROR_STOP off	
\i $WDB_METADATA_PATH/wdbMetadataLoad.sql 
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

exit

# Install Metadata
echo -n "installing new materialized views... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_matview.log
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.dataprovider_mv'); 
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.valueparameter_mv');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.levelparameter_mv'); 
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placedefinition_mv'); 
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

# Install Indexes
echo -n "installing upgraded indexes... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
\i $WDB_DATAMODEL_PATH/wdbIndexDefinitions.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

# Install wci
echo -n "installing upgraded wci types... "
cd __WDB_DATADIR__/sql/wci
for FILE in `ls -1f types/*.sql | grep -v [.]in[.]sql | grep -v types/wci`; do
    psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\i $FILE
EOF
    if [ 0 != $? ]; then
	echo "ERROR when installing $FILE"; exit 1
    fi
done
echo done

echo -n "installing upgraded wci core... "
cd __WDB_DATADIR__/sql/wci
#psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
#SET CLIENT_MIN_MESSAGES TO "WARNING";
#\set ON_ERROR_STOP
#\i types/wciInterpolationSpec.sql
#\i core/wciSession.sql
#\i core/wciWriteInternals.sql
#\i core/wciExtractGridData.sql
#EOF

#\i core/readWhereClause.sql
#\i core/readOidQuery.sql
#\i core/readFloatQuery.sql
#\i core/wciInterpolation.sql


for FILE in `ls -1f core/*.sql | grep -v [.]in[.]sql | grep -v wciSession.sql | grep -v wciInterpolation.sql` ; do
	echo $FILE
    psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\i $FILE
EOF
    if [ 0 != $? ]; then
	echo "ERROR when installing $FILE"; exit 1
    fi
done
echo done

# Data Migration
echo -n "migrating data... "
cd __WDB_DATADIR__/sql
#psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
#SET CLIENT_MIN_MESSAGES TO "WARNING";
#\set ON_ERROR_STOP
#\i $FILE
#EOF
echo done

#echo -n "Dropping views and functions... "
#psql $PSQLARGS -c "DELETE FROM __WDB_SCHEMA__.materializedView"
#for SCHEMA in test gribload feltload kvalobs wci wci_internal ; do
#for SCHEMA in test wci wci_internal ; do
#	psql $PSQLARGS -c "DROP SCHEMA $SCHEMA CASCADE"
#done
#echo "done" 

# Install Later
#--\i $WDB_DATAMODEL_PATH/wdbSchemaDefinitions.sql
#--\i $WDB_DATAMODEL_PATH/wdbGribDefinitions.sql
#--\i $WDB_DATAMODEL_PATH/wdbFeltDefinitions.sql
# \i $WDB_DATAMODEL_PATH/wdbTestDefinitions.sql

# Install cleanup script
#echo -n "installing cleaner... "
#psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
#SET CLIENT_MIN_MESSAGES TO "WARNING";
#\set ON_ERROR_STOP
#\o $LOGDIR/wdb_install_cleanDb.log
#\i $WDB_CLEANUP_PATH/cleanDb.sql
#EOF
#if [ 0 != $? ]; then
#    echo "ERROR"; exit 1
#else
#    echo "done"
#fi

# Install wci
#echo -n "installing upgraded wci core... "
#cd __WDB_DATADIR__/sql/wci
#for FILE in `ls -1f types/*.sql core/*.sql api/*.sql | grep -v [.]in[.]sql`; do
#    psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
#SET CLIENT_MIN_MESSAGES TO "WARNING";
#\set ON_ERROR_STOP
#\i $FILE
#EOF
#    if [ 0 != $? ]; then
#	echo "ERROR when installing $FILE"; exit 1
#    fi
#done
#echo done

#psql $PSQLARGS -c "UPDATE __WDB_SCHEMA__.softwareversion SET softwareversioncode='__WDB_VERSION__' WHERE partyid=10000"
