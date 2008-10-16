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
export SCRIPT_USAGE="multiLoad (wdb) __WDB_VERSION__

multiLoad is a simple program to load multiple data files, of the same
type, in parallel. It takes as its input arguements the name of the
loading program to utilize and the file name(s) of the data to be 
loaded.

Usage: $0 [OPTIONS] LOADER DATAFILE1 [DATAFILE2... DATAFILEN]

LOADER:
  gribLoad					For GRIB files

Options:

General:
  --help                	Produce help message
  --version             	Produce version information, then exit

Multiple Setting
  -r arg					Number of processes to run in parallel
  							(Default = 2) 

Database configuration:
  -d [ --database ] arg		Database name (ex. wdb)
  -h [ --host ] arg 		Database host (ex. somehost.met.no)
  -u [ --user ] arg 		Database user name
  -p [ --port ] arg 		Database port number to connect to

Script configuration:
  -w [ --working-area ] arg	Set a storage area for any temporary files
  							created by the loading process. Default
  							location is /tmp/multi 

Logging:
  --logfile arg         	Name of logfile. If not set stdout will be 
  							used

Output:
  -l [ --list ]         	List content of file(s) instead of inserting data 
  							into the database
"
export SCRIPT_VERSION=__WDB_VERSION__
TMP_STORE=/tmp/multi
LOAD_OPTIONS=
LOADER=
MAX_PROCESSES=2
DATA_FILES=

# Parse command line
if test $# == 0; then  
	echo "Usage: $0 [OPTIONS] LOADER DATAFILE1 [DATAFILE2... DATAFILEN]"
	echo "Use --help for detailed usage information"
fi
while test $# != 0; do
	case $1 in
    	--help) 
	    	echo "$SCRIPT_USAGE"; exit 0;;
    	--version) 
    		echo "$0 $SCRIPT_VERSION"; exit 0;;
		-r)
			MAX_PROCESSES=$2
			shift
			shift
			continue;;
		-d | --database | -h | --host |	-u | --user | -p | --port |	--logfile)
			LOAD_OPTIONS="${LOAD_OPTIONS} $1 $2"
			shift
			shift
			continue;;
		-l | --list)
			LOAD_OPTIONS="${LOAD_OPTIONS} $1"
			shift
			continue;;
		-w | --working-area)
			TMP_STORE=$2
			shift
			shift
			continue;; 
		*)
			if test -z LOADER; then
				LOADER=$1
  			else
				DATA_FILES="${DATA_FILES} $1"
			fi
			shift
			continue;;			
	esac
done
	
# Check settings
if test -z LOADER; then
	echo "No valid loading program specified"; exit 1;
fi
if test -z DATA_FILES; then
	echo "No valid data files specified for loading"; exit 1;
fi
	
# Create temp dir if it does not exist
mkdir -p ${TMP_STORE}
LOAD_OPTIONS="${LOAD_OPTIONS} -w $TMP_STORE"
	
for DATAFILE in ${DATA_FILES}; do
	COMMAND=${LOADER} ${LOAD_OPTIONS} ${DATAFILE}
    # Wait for the number of running processes to go down to $MAX_PROCESSES
    while test $(( `ps -ef | grep $LOADER | grep -vc grep` < $MAX_PROCESSES )) == 0 ; do
		sleep 1
    done
    # Run Command
    $COMMAND &	
done
