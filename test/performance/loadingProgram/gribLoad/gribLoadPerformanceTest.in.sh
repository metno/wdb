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
#!/bin/sh
# Help Info
export SCRIPT_USAGE="Usage: $0 [OPTION]

Description: Script for running the performance tests for the WDB
GRIB Loading system.

(See wdb_call-interface_system-test-specification for details)

Options:
--help             display this help and exit
--version          output version information and exit
"

export SCRIPT_VERSION=__WDB_VERSION__
export TEST_PATH=__WDB_BUILDDIR__/test/performance/loadingProgram/gribLoad
export TMP_PATH=__WDB_BUILDDIR__/var/tmp
export TEST_FILE=$TMP_PATH/gribLoadPerformance.grib
export GRIBWRITE_PATH=__WDB_BUILDDIR__

# Parse command line
case $1 in
    --help) 
    echo "$SCRIPT_USAGE"; exit 0;;
    --version) 
    echo "$0 $SCRIPT_VERSION"; exit 0;;
esac

echo "GribLoad Performance Tests"

source ${TEST_PATH}/buildUp.sh

RUN_TESTS=0
SKIPPED_TESTS=0
FAILED_TESTS=0

list=`ls $TEST_PATH/*.test | grep -v .in.test`;

# Run Tests
if test -n "$list"
    then
    for tst in $list; do
      $tst
      if  test $? -eq 0; then \
	  RUN_TESTS=`expr $RUN_TESTS + 1`;
      elif test $? -ne 77; then \
	  RUN_TESTS=`expr $RUN_TESTS + 1`; \
	  FAILED_TESTS=`expr $FAILED_TESTS + 1`;
      else \
	  RUN_TESTS=`expr $RUN_TESTS + 1`; \
	  SKIPPED_TESTS=`expr $SKIPPED_TESTS + 1`;
      fi;
    done 
fi

# Print Test Result    
echo "GribLoad Performance Tests done"
echo "Run: $RUN_TESTS   Failed: $FAILED_TESTS   Skipped: $SKIPPED_TESTS"
if test "$FAILED_TESTS" -eq 0
    then 
    exit 0;
else
    exit 1;
fi

