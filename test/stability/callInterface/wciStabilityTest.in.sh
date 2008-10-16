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
#!/bin/sh
# Help Info
export SCRIPT_USAGE="Usage: $0 [OPTION]

Description: Script for running the stability tests for the WDB
Call Interface.

(See wdb_call-interface_system-test-specification for details)

Options:
--help             display this help and exit
--version          output version information and exit
"
export SCRIPT_VERSION=__WDB_VERSION__

# Parse command line
case $1 in
    --help) 
    echo "$SCRIPT_USAGE"; exit 0;;
    --version) 
    echo "$0 $SCRIPT_VERSION"; exit 0;;
esac

export TESTER=./wciStabilityTester
export MAX_PROCESSES=4

echo "WCI Stability Tests"
# Run Tester
if test $(( `ps -ef | grep $TESTER | grep -vc grep` > $MAX_PROCESSES )) == 0 ; then
	echo "Test Running"
	$TESTER	
else
	echo "Test Not Run"
	exit 0;
fi
