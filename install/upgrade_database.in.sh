#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2008 met.no
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
#
# upgrade - upgrade the wdb system
#

DEFAULT_DATABASE=`__WDB_BINDIR__/wdbConfiguration --database`@`__WDB_BINDIR__/wdbConfiguration --host`
DEFAULT_USER=`__WDB_BINDIR__/wdbConfiguration --user`
DEFAULT_PORT=`__WDB_BINDIR__/wdbConfiguration --port`

SCRIPT_VERSION=__WDB_VERSION__

SCRIPT_USAGE="Usage: $0 [OPTION]

This script upgrades the WDB system in the database specified by the 
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
--pre=SQLFILE
				   specify an SQL file to be run prior to data migration.
				   The upgrade will abort if the file returns an error.
				   Path to SQL file must be an absolute path
--post=SQLFILE				
				   specify an SQL file to be run after the data migration.
				   The upgrade will abort if the file returns an error
				   Path to SQL file must be an absolute path
--dry-run		   runs an SQL file prior to the migration of the data,
				   that verifies whether the data values can be migrated
				   without errors. If this tests succeeds, the data transfer
				   should be ok
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
	--pre=*)
		WDB_UPGRADE_PRE=`echo $1 | sed 's/--pre=//'`
        shift
        continue;;
	--post=*)
		WDB_UPGRADE_POST=`echo $1 | sed 's/--post=//'`
        shift
        continue;;
    --dry-run)
    	WDB_UPGRADE_TEST="yes"
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

WDB_LIBDIR=__WDB_LIBDIR__
OLD_VERSION=__OLD_VERSION__
WDB_CONFIG=__WDB_BINDIR__/wdbConfiguration

# Directory for logging
export LOGDIR=/tmp/$USER/wdb/upgrade/logs/
mkdir -p $LOGDIR

# Code to rollback installation
upgrade_rollback() {
	echo -n "rolling back upgraded datamodel... "
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
--\set ON_ERROR_STOP
\o $LOGDIR/wdb_rollback_datamodel.log
--DROP SCHEMA __WCI_SCHEMA__ CASCADE;
--DROP SCHEMA __WDB_SCHEMA__ CASCADE;
EOF
	cd $WDB_LIBDIR
	rm wdb.so
	ln -s wdb-$OLD_VERSION.so wdb.so
	echo "done"
 }
 

# Start Upgrade
echo "---- wdb database upgrade ----"

# checking if we can find the sql source files
WDB_DATAMODEL_PATH=__WDB_DATADIR__/sql
WDB_METADATA_PATH=$WDB_DATAMODEL_PATH
WDB_CLEANUP_PATH=$WDB_DATAMODEL_PATH


if test ! -f $WDB_DATAMODEL_PATH/wdbSchemaDefinitions.sql; then
    echo "ERROR: Could not locate database installation files."
	echo "Checking: $WDB_DATAMODEL_PATH"
	echo "Unable to install wdb."
    exit 1
fi

# Check for presence of database
WDB_NAME=$WDB_INSTALL_DATABASE
export $WDB_NAME
echo -n "checking connection to $WDB_NAME... "
# DB_CHECK= list database | isolate pattern WDB_NAME | split record |  
# grab first line (name) | trim whitespace
DB_CHECK=`psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -l | sed -n /$WDB_NAME/p | sed -e 's/|/\n/' | sed q | sed -e 's/^[ \t]*//;s/[ \t]*$//'`
# Test whether database exists
# Note: as the list above only grabs the first tablename matching 
# the WDB_NAME pattern this may fail if there is a database with a similar 
# name present in the DB
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
PSQLARGS=`$WDB_CONFIG --psqlArgs`
current_version=`psql $PSQLARGS -l -c "select wci.version();" -q | sed -e '1,2d' | sed -e '2,$d' | sed 's/^[ ]//g'`
major_version=`echo $current_version | sed 's/[A-Za-z]*\s*//' |sed 's/\..*$//'`
minor_version=`echo $current_version | sed 's/[A-Za-z]*\s[0-9]*\.//' | sed 's/\..*$//'`
patch_version=`echo $current_version | sed 's/.*\.//'`
version_number=`echo $major_version.$minor_version.$patch_version`
if [ "$version_number" = "$OLD_VERSION" ]; then
	echo $current_version
else
	echo $current_version
	if [ "$version_number" = "__WDB_VERSION__" ]; then
		echo "EXIT: No upgrade of the database schema is required." 
		echo "---- wdb database upgrade completed ----"
		exit 0			
	else
	    echo "ERROR: Cannot upgrade WDB version $version_number to __WDB_VERSION__. This upgrade is only valid for WDB version $OLD_VERSION."
	    exit 1
	fi
fi

# For minor upgrades, only update metadata and then exit 
if [ "__WDB_SCHEMA__" = "__OLD_SCHEMA__" ]; then
	echo -n "installing upgraded metadata... "
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP off
\o $LOGDIR/wdb_install_metadata.log
\i $WDB_METADATA_PATH/wdbMetadata.sql
EOF
	if [ 0 != $? ]; then
	    echo "failed"
	    echo "ERROR: installing new metadata failed."
	    upgrade_rollback
	    exit 1
	else
	    echo "done"
	fi
	exit 0
fi


# Install Datamodel
echo -n "installing upgraded datamodel (1/2)... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
\i $WDB_DATAMODEL_PATH/wdbSchemaDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbBaseTables.sql
\i $WDB_DATAMODEL_PATH/wdbDataProviderTables.sql
\i $WDB_DATAMODEL_PATH/wdbPlaceDefinitionTables.sql
\i $WDB_DATAMODEL_PATH/wdbParameterTables.sql
\i $WDB_DATAMODEL_PATH/wdbValueTables.sql
\i $WDB_DATAMODEL_PATH/wdbConstraintDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbMaterializedView.sql
\i $WDB_DATAMODEL_PATH/wdbTriggerDefinitions.sql
\i $WDB_DATAMODEL_PATH/wciViewDefinitions.sql
\i $WDB_DATAMODEL_PATH/fileblob.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: The upgrade of the datamodel failed. See log for details."
    upgrade_rollback
    exit 1
else
    echo "done"
fi

# Install Metadata
echo -n "installing upgraded metadata (1/3)... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP off
\o $LOGDIR/wdb_install_metadata.log
\i $WDB_METADATA_PATH/wdbMetadata.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: installing new metadata failed."
    upgrade_rollback
    exit 1
else
    echo "done"
fi

# Upgrade Metadata
echo -n "installing upgraded metadata (2/3)... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_migrate_data.log
\i $WDB_DATAMODEL_PATH/upgrade_metadata.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: Migrating data WDB version __WDB_VERSION__ failed."
    echo "ERROR: See wdb_migrate_data.log for details."
    upgrade_rollback
	exit 1
else
    echo "done"
fi

# Install Materialized Views
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
    echo "failed"
    echo "ERROR: installing new materialized views failed."
    upgrade_rollback
	exit 1
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
    echo "failed"
    echo "ERROR: installing new indexes failed."
    upgrade_rollback
	exit 1
else
    echo "done"
fi

# Install wci
echo -n "installing upgraded wci types... "
WCI_DIR="__WDB_DATADIR__/sql/wci"
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
-- WCI Types
\i $WCI_DIR/types/extractGridDataType.sql
\i $WCI_DIR/types/levelParameter.sql
\i $WCI_DIR/types/location.sql
\i $WCI_DIR/types/valueParameter.sql
\i $WCI_DIR/types/verifyGeometry.sql
\i $WCI_DIR/types/wciInterpolationSpec.sql
\i $WCI_DIR/types/wciLevelSpec.sql
--\i $WCI_DIR/types/wciReadReturnType.sql
\i $WCI_DIR/types/wciTimeSpec.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
	echo "ERROR: installing $FILE failed. See log for details."
    upgrade_rollback
	exit 1
fi
echo "done"

echo -n "installing upgraded wci core... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
-- WCI Core
\i $WCI_DIR/core/getDataProvider.sql
\i $WCI_DIR/core/wciBrowseInternals.sql
\i $WCI_DIR/core/wciSession.sql
\i $WCI_DIR/core/wciWriteInternals.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
	echo "ERROR: installing $FILE failed. See log for details."
    upgrade_rollback
	exit 1
fi
echo "done"

# Install cleanup script
echo -n "installing cleaner... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_cleanDb.log
\i $WDB_CLEANUP_PATH/cleanDb.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: The installation of the cleanup script failed. See log for details."
    upgrade_rollback
	exit 1
else
    echo "done"
fi

# Data Migration - Dry Run
if test -n "$WDB_UPGRADE_TEST"; then
	echo -n "dry run of data migration... "
	cd __WDB_DATADIR__/sql
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_migrate_data.log
\i upgrade_test.sql
EOF
	if [ 0 != $? ]; then
	    echo "failed"
	    echo "ERROR: Dry run of data migration threw errors. This implies that"
	    echo "ERROR: it would not be possible to migrate all of the data in"
	    echo "ERROR: the database correctly."
	    echo "ERROR: See wdb_migrate_data.log for details."
	    upgrade_rollback
		exit 1
	fi
	echo "done"
    upgrade_rollback
    echo "Dry run of data migration worked as expected. This suggests that"
    echo "an upgrade of the database should work correctly. Nevertheless,"
    echo "make sure your system is backed up before attempting an upgrade."
	echo "---- wdb database upgrade test successful ----"
	exit 0
fi

# Data Migration
echo -n "migrating data... "
cd __WDB_DATADIR__/sql

# Pre-operation
if test -n "$WDB_UPGRADE_PRE"; then
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_migrate_data.log
\i $WDB_UPGRADE_PRE
EOF
	if [ 0 != $? ]; then
	    echo "failed"
	    echo "ERROR: Migrating data WDB version __WDB_VERSION__ failed."
	    echo "ERROR: Pre-operation of data migration failed."
	    echo "ERROR: See wdb_migrate_data.log for details."
	    upgrade_rollback
		exit 1
	fi
fi

# Actual Migration
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\i upgrade_database.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: Migrating data WDB version __WDB_VERSION__ failed."
    echo "ERROR: See wdb_migrate_data.log for details."
    upgrade_rollback
	exit 1
fi

# Post-operation
if test -n "$WDB_UPGRADE_POST"; then
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q  <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\o $LOGDIR/wdb_migrate_data.log
\set ON_ERROR_STOP
\i $WDB_UPGRADE_POST
EOF
	if [ 0 != $? ]; then
	    echo "failed"
	    echo "ERROR: Migrating data WDB version __WDB_VERSION__ failed."
	    echo "ERROR: Post-operation of data migration failed."
	    echo "ERROR: See wdb_migrate_data.log for details."
	    upgrade_rollback
		exit 1
	fi
fi
# Migrate Completed
echo "done"

# Dropping Stuff
echo -n "dropping views and functions... "
#psql $PSQLARGS -c "DELETE FROM __WDB_SCHEMA__.materializedView"
for SCHEMA in test admin wci; do
	psql $PSQLARGS -c "SET CLIENT_MIN_MESSAGES TO "WARNING"; DROP SCHEMA $SCHEMA CASCADE" -o $LOGDIR/wdb_install_datamodel.log
done
echo "done" 

# Install additional schemas
echo -n "installing upgraded datamodel (3/3)... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
\i $WDB_DATAMODEL_PATH/wdbAdminDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbTestDefinitions.sql
\i $WDB_DATAMODEL_PATH/wciSchemaDefinitions.sql
EOF
if [ 0 != $? ]; then
    echo "failed"
    echo "ERROR: The completion of the datamodel upgrade failed. See log for details."
	echo "ERROR: No roll back possible. Restore database from backup."
    exit 1
fi
echo "done"


# Install wci
echo -n "installing upgraded wci api... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
-- WCI API
\i $WCI_DIR/types/wciReadReturnType.sql
\i $WCI_DIR/api/wciBegin.sql
\i $WCI_DIR/api/wciBrowse.sql
\i $WCI_DIR/api/wciCacheQuery.sql
\i $WCI_DIR/api/wciEnd.sql
\i $WCI_DIR/api/wciFetch.sql
\i $WCI_DIR/api/wciMetaDataProvider.sql
\i $WCI_DIR/api/wciMetaParameter.sql
\i $WCI_DIR/api/wciMetaPlace.sql
\i $WCI_DIR/api/wciRead.sql
\i $WCI_DIR/api/wciVersion.sql
\i $WCI_DIR/api/wciWrite.sql
EOF
if [ 0 != $? ]; then
	echo "failed"
	echo "ERROR: installing WCI API failed. See log for details."
	echo "ERROR: The completion of the datamodel upgrade failed. See log for details."
	echo "ERROR: No roll back possible. Restore database from backup."
	exit 1
fi
echo "done"

# Clean out old schemas
echo -n "dropping old schemas... "
#for SCHEMA in __OLD_WCI_SCHEMA__ __OLD_SCHEMA__; do
#	psql $PSQLARGS -c "SET CLIENT_MIN_MESSAGES TO "WARNING"; DROP SCHEMA $SCHEMA CASCADE" -o $LOGDIR/wdb_install_datamodel.log
#done
echo "done" 

echo "---- wdb database upgrade completed ----"
exit 0

# Comments for the DOCS
# Default namespace is not preserved - this has to be reset by the user.
