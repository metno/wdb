#!/bin/sh

set -e

WDB_CONFIG=__WDB_BINDIR__/wdbConfiguration
PSQL="psql `$WDB_CONFIG --psqlArgs`"

DATAVERSION_TEST_FILE=__WDB_SRCDIR__/test/install/cleaningProgram/remove_low_dataversions/test.sql

if $PSQL wdb < $DATAVERSION_TEST_FILE 2>&1 | grep ERROR
then 
	echo Failed
	exit 1
else 
	echo Success
fi
