#!/bin/sh
## - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
## 
## wdb - weather and water data storage
##
## Copyright (C) 2009 met.no
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

UPGRADE_DIR=upgradecheck
INSTALL_DIR=$(UPGRADE_DIR)/_inst
OLDWDB_DIR=$(UPGRADE_DIR)/__OLD_VERSION__
NEWWDB_DIR=$(UPGRADE_DIR)/__WDB_VERSION__
WDBSVN_DIR=https://svn.met.no/wdbSystem/
TEST_DB=wdb_upgradecheck

# Create directories
mkdir $(UPGRADE_DIR)
mkdir $(INSTALL_DIR)

# Create package
make dist

# Check out WDB
svn co $(WDBSVN_DIR)/tags/__OLD_VERSION__/wdb $(OLDWDB_DIR)

# Make and install
cd $(OLDWDB_DIR)
./autogen.sh
./configure --srcdir=.. --prefix=$(INSTALL_DIR) --with-database-name=$(TEST_DB)
make all install
# -- Add some data to the database to port
cd ../..

# Upgrade database
make dist
mv __WDB_DISTDIR__.tar.gz $(UPGRADE_DIR)
cd $(UPGRADE_DIR)
gunzip -c __WDB_DISTDIR__.tar.gz | tar xvf
cd __WDB_DISTDIR__
./autogen.sh
./configure --srcdir=.. --prefix=$(INSTALL_DIR) --with-database-name=$(TEST_DB)
make upgrade
# -- Check the integrity of the database
cd ../..

echo "Upgrade worked correctly"
