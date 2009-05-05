#!/bin/sh
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2007 met.no
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
# install - install the wdb system

# TODO: Add support for reading additional configuration files

DEFAULT_DATABASE=`__WDB_BINDIR__/wdbConfiguration --database`@`__WDB_BINDIR__/wdbConfiguration --host`
DEFAULT_USER=`__WDB_BINDIR__/wdbConfiguration --user`
DEFAULT_PORT=`__WDB_BINDIR__/wdbConfiguration --port`

SCRIPT_VERSION=__WDB_VERSION__

SCRIPT_USAGE="Usage: $0 [OPTION]

The script installs the WDB system in the database specified by
the PG environment variables, unless overriden on the command line or
in the configuration files.

PGDATABASE        (default: $DEFAULT_DATABASE)
PGUSER            (default: $DEFAULT_USER)
PGPORT            (default: $DEFAULT_PORT)

If the named database does not exist, the install script creates the
database.

Options:

-d NAME, --database=NAME    
                   install to database <NAME>

-u USER, --user=USER
                   install as user <NAME>

-p PORT, --port=PORT
                   install on port <PORT>

-c FILE, --config=FILE
                   Read additional configuration from <FILE>

-f, --force-install   
                   force installation over existing database

--with-postgis=PATH
                   Specify the contrib directory of postgres if this
                   is not <postgres>/share/contrib. This directory
                   needs to contain the postgis installation files
                   lwpostgis.sql and spatial_ref_sys.sql

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
	--force-install)
	    WDB_OVERWRITE_DATABASE="yes"
	    shift
	    continue;;
	-f)
	    WDB_OVERWRITE_DATABASE="yes"
	    shift
	    continue;;
	--with-postgis=*)
	    WDB_POSTGIS_CONTRIB=`echo $1 | sed 's/--with-postgis=//'`
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

# Start Installation
echo "---- wdb database installation ----"

# Directory for logging
export LOGDIR=/tmp/$USER/wdb/var/logs/
mkdir -p $LOGDIR

# checking if we can find the sql source files
WDB_DATAMODEL_PATH=__WDB_DATADIR__/sql
WDB_METADATA_PATH=$WDB_DATAMODEL_PATH
WDB_CLEANUP_PATH=$WDB_DATAMODEL_PATH

if test ! -f $WDB_DATAMODEL_PATH/wdbSchemaDefinitions.sql; then
    echo "Error: Could not locate database installation files."
	echo "Checking: $WDB_DATAMODEL_PATH"
	echo "Unable to install wdb."
    echo "Error: Could not locate database installation files. Unable to install wdb."
    exit 1
fi

# If WDB_POSTGIS_CONTRIB set...
if test -n "$WDB_POSTGIS_CONTRIB"; then
    POSTGIS_DIR=$WDB_POSTGIS_CONTRIB
else
    POSTGIS_DIR=__POSTGIS_DIR__
fi

# Check for Postgis file lw_postgis.sql
echo -n "checking for the presence of lwpostgis.sql... "
if test -f $POSTGIS_DIR/lwpostgis.sql; then
    echo "yes"
elif test -f $POSTGIS_DIR/../lwpostgis.sql; then
    echo "yes"
    POSTGIS_DIR=$POSTGIS_DIR/..
elif test -f $POSTGIS_DIR/contrib/lwpostgis.sql; then
    echo "yes"
    POSTGIS_DIR=$POSTGIS_DIR/contrib
else 
    echo "no"
    echo "Error: could not find lwpostgis.sql. Postgis must be installed together with postgres. Installation assumes that lwpostgis.sql is installed in $POSTGIS_DIR/contrib."
    exit 1
fi


# Check for Postgis file spatial_ref_sys.sql
echo -n "checking for the presence of spatial_ref_sys.sql... "
if test -f $POSTGIS_DIR/spatial_ref_sys.sql; then
    echo "yes"
else 
    echo "no"
    echo "Error: could not find spatial_ref_sys.sql. Postgis must be installed together with postgres. Installation assumes that spatial_ref_sys.sql is installed in <postgres>/share/contrib."
    exit 1
fi

# Verify that Postmaster is running
echo -n "checking that postgres is running... "
PID=`ps aux | grep postgres | grep -v grep`
if test -n "$PID"; then
    echo "yes"
else
    echo "no"
    echo "Error: could not find postgres. Check that your postgres installation is set up correctly and that postgres is running"
    exit 1
fi


# Get database name
WDB_NAME=`echo $WDB_INSTALL_DATABASE | sed 's/@/\n/' | sed q`
export $WDB_NAME

# Check that the database exists
echo -n "checking that database $WDB_NAME exists... "
# DB_CHECK= list database | isolate pattern WDB_NAME | split record |  
# grab first line (name) | trim whitesoace
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
fi

# Check whether user exists
#echo -n "checking whether database user $WDB_INSTALL_USER exists... "
# USER_CHECK= list users | isolate pattern $WDB_INSTALL_USER | split record |  
# grab first line (name) | trim whitesoace
#USER_CHECK=`psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d postgres -c "\du" | sed -n /wdb_admin/p | sed -e 's/|/\n/' | sed q | sed -e 's/^[ \t]*//;s/[ \t]*$//'`
# Test whether user exists
# Note: same limitation as above
#if test "$USER_CHECK" = "wdb_admin"; then 
#    echo "yes"
#    ADMIN_EXISTS="yes"
#else
#    echo "no"
#    ADMIN_EXISTS="no"
#fi

# Create wdb_admin if role does not exist
#if test "$ADMIN_EXISTS" = "no"; then
#    echo -n "creating role wdb_admin... "
#    createuser -p $WDB_INSTALL_PORT wdb_admin -s -q
#    echo "done"
#fi

# Create database if it does not exist
if test "$DATABASE_EXISTS" = "no"; then
    echo -n "creating database $WDB_NAME... "
    createdb -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT $WDB_NAME -q
    echo "done"
# Clean out  database if it exists
else
    echo -n "checking that database is clean... "
    WDB_ENTITIES=`psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d postgres -c "select relname from pg_class" | sed -n '/wdb_/p'`
    if test -n "$WDB_ENTITIES"; then
	echo "no"
	# If database is not clean, don't install
	if test -z "$WDB_OVERWRITE_DATABASE"; then
	    echo "Error: database contains entities that may have been left behind by an earlier installation of WDB. Either clean up the database manually or run install using the option --force-install to attempt to drop the database."
	    exit 1
	# Unless --force-install was set 
	else
	    echo -n "dropping database $WDB_NAME... "
	    dropdb -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT $WDB_NAME -q
	    echo "done"
	    echo -n "creating database $WDB_NAME... "
	    createdb -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT $WDB_NAME -O wdb_admin -q
	    echo "done"
	fi
    else
	echo "yes"
    fi
fi

# Check if autovacuum is on
PSQL_VALUES_OK_REG="^(on|1|t|true)$"
TEST_VALUES="autovacuum"
for VAL in $TEST_VALUES; do
    echo -n "checking the database configuration of $VAL... "
    if ! psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -Pformat=unaligned -Pt -c "show $VAL" | grep -qiE $PSQL_VALUES_OK_REG; then
	echo "off"
	echo "Error: Value $VAL must be set to on in postgresql.conf"
	echo -n "$VAL is set to "
	psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -Pformat=unaligned -Pt -c "show $VAL"
	echo "Unable to proceed"
	exit 1
    else
	echo "on"
    fi
done

# Install PostGIS
echo -n "installing postgis... "
# Setup plpgsql
createlang -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT plpgsql $WDB_NAME
# Install Postgis objects
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\i $POSTGIS_DIR/lwpostgis.sql
\i $POSTGIS_DIR/spatial_ref_sys.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi


# Set up roles/users
echo -n "setting up roles... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "FATAL";
--\set ON_ERROR_STOP
\i $WDB_DATAMODEL_PATH/wdbUserDefinitions.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

# Install Datamodel
echo -n "installing datamodel... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_datamodel.log
\i __WDB_DATADIR__/sql/wci/core/pg_fileblob/fileblob.sql
\i $WDB_DATAMODEL_PATH/wdbSchemaDefinitions.sql
\i $WDB_DATAMODEL_PATH/wciSchemaDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbBaseTables.sql
\i $WDB_DATAMODEL_PATH/wdbDataProviderTables.sql
\i $WDB_DATAMODEL_PATH/wdbPlaceDefinitionTables.sql
\i $WDB_DATAMODEL_PATH/wdbParameterTables.sql
\i $WDB_DATAMODEL_PATH/wdbValueTables.sql
\i $WDB_DATAMODEL_PATH/wdbConstraintDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbMaterializedView.sql
\i $WDB_DATAMODEL_PATH/wdbViewDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbTriggerDefinitions.sql
\i $WDB_DATAMODEL_PATH/wciViewDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbLoaderBaseDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbGribDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbFeltDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbXmlDefinitions.sql
\i $WDB_DATAMODEL_PATH/wdbTestDefinitions.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi


# Install Metadata
echo -n "installing metadata... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF 
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_metadata.log
\i $WDB_METADATA_PATH/wdbMetadataLoad.sql 
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi

# Install Metadata
echo -n "updating materialized views... "
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
echo -n "installing indexes... "
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


# Install cleanup script
echo -n "installing cleaner... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $LOGDIR/wdb_install_cleanDb.log
\i $WDB_CLEANUP_PATH/cleanDb.sql
EOF
if [ 0 != $? ]; then
    echo "ERROR"; exit 1
else
    echo "done"
fi


# Install wci
echo -n "installing wci components... "
cd __WDB_DATADIR__/sql/wci
for FILE in `ls -1f types/*.sql core/*.sql api/*.sql | grep -v [.]in[.]sql`; do
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


echo "---- wdb database installation completed ----"
exit 0
