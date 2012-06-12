#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2007 met.no
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
# install - install the wdb system

# TODO: Add support for reading additiona configuration files


DEFAULT_DATABASE=`__WDB_LIBEXECDIR__/wdbConfiguration --database`@`__WDB_LIBEXECDIR__/wdbConfiguration --host`
DEFAULT_USER=`__WDB_LIBEXECDIR__/wdbConfiguration --user`
DEFAULT_PORT=`__WDB_LIBEXECDIR__/wdbConfiguration --port`

SCRIPT_VERSION=__WDB_VERSION__

SCRIPT_USAGE="Usage: $0 [OPTION]

The script uninstalls the WDB system from the database using the PG environment variables:

PGDATABASE        (default: $DEFAULT_PGDATABASE)
PGUSER            (default: $DEFAULT_USER)
PGPORT            (default: $DEFAULT_PORT)

Options:
-d NAME, --database=NAME    
                   uninstall database <NAME>
-u USER, --user=USER
                   uninstall as user <USER>
-p PORT, --port=PORT
                   uninstall on port <PORT>
--help             display this help and exit
--version          output version information and exit
"

# Parse command line
while test -n "$1"; do
    case $1 in
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
echo "---- wdb database uninstall ----"

# Set PATHS
# echo -n "checking for the presence of wdb source files... "
# if test -z "$top_builddir"; then
#     if test -f ./src/database/wdbTableDefinitions.sql; then
# 	WDB_DATAMODEL_PATH="./src/database"
# 	WDB_METADATA_PATH="./etc/metadata"
# 	WDB_CLEANUP_PATH="./src/common/cleaner"
#     else 
# 	if test -f ../src/database/wdbTableDefinitions.sql; then
# 	    WDB_DATAMODEL_PATH="../src/database"
# 	    WDB_METADATA_PATH="../etc/metadata"
# 	    WDB_CLEANUP_PATH="../src/common/cleaner"
# 	else
# 	    echo "no"
# 	    echo "Error: Could not locate database installation files."
# 	    exit 1
# 	fi
#     fi
# else
#     WDB_DATAMODEL_PATH=$top_srcdir"/src/database"
#     WDB_METADATA_PATH=$top_srcdir"/etc/metadata"
#     WDB_CLEANUP_PATH=$top_srcdir"/src/common/cleaner"
# fi
# echo "yes"

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
echo -n "checking whether database $WDB_NAME exists... "
# DB_CHECK= list database | isolate pattern WDB_NAME | split record |  
# grab first line (name) | trim whitesoace
DB_CHECK=`psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT -l | sed -n /$WDB_NAME/p | sed -e 's/|/\n/' | sed q | sed -e 's/^[ \t]*//;s/[ \t]*$//'`
# Test whether database exists
# Note: as the list above only grabs the first tablename matching 
# the WDB_NAME pattern this may fail if there is a database with a
# similar name present in the DB
if test "$DB_CHECK" = "$WDB_NAME"; then 
    echo "yes"
    DATABASE_EXISTS="yes"
else
    echo "no"
    DATABASE_EXISTS="no"
fi

# Delete the field data, so it won't remain in the postgresql database folder
echo -n "deleting data fields... "
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT $WDB_NAME -c "DELETE FROM __WDB_SCHEMA__.gridvalue" >> /dev/null
psql -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT $WDB_NAME -c "SELECT __WDB_SCHEMA__.vacuum_file_blob()" >> /dev/null
echo "done"
# Drop the wdb database
echo -n "dropping database... "
if ! dropdb -p $WDB_INSTALL_PORT $WDB_NAME; then
    echo "No database found - skipping database uninstall"
	exit 1
fi
echo "done"

# echo -n "dropping users and roles... "
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT cleantest -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wcitest -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT gribtest -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT gribload -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_test -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_write -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_clean -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_read -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_grib -q
# dropuser -U $WDB_INSTALL_USER -p $WDB_INSTALL_PORT wdb_admin -q
# echo "done"

echo "---- wdb database uninstall completed ----"
exit 0
