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
WDB_TMP_DIR=__WDB_BUILDDIR__/var/tmp
GRIB_FILE=$WDB_TMP_DIR/wciPerformanceTest.grib
mkdir -p $WDB_TMP_DIR
CREATE_GRIB_FIELD=./gribWrite
LOAD_GRIB_FILE=./gribLoad

echo -n "# Generating test data (please wait)... "

# Remove previous test data
rm -f ${GRIB_FILE}

# Generate Test Data
days="01"
hours="00 06 12 18"
params="1 2 3 4 5 6"
timesteps="0 1 2 3 4 5 6 7 8 9 10 11"

for dd in $days
  do
  for hh in $hours 
    do
    for parameter in $params 
      do
      for timestep in $timesteps
	do	
     $CREATE_GRIB_FIELD --parameter $parameter --hirlam10 --timeValue 1980-01-"$dd"T"$hh":00:00 --timeUnit 1 --P1 $timestep --P2 0 --timeRangeIndicator 0 --levelValue 0 --levelUnit 102 --generatingCentre 1 --generatingProcess 254 -P 100,100=1  ${GRIB_FILE}
      done
    done
  done
done

$LOAD_GRIB_FILE --loadPlaceDefinition ${GRIB_FILE}

# Clean up Test File
rm -f $GRIB_FILE

# Exit
echo -e "done"
