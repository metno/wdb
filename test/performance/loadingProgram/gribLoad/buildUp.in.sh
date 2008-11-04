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

echo -n "# Generating test data (please wait)... "

# Remove previous test data
rm -f ${TEST_FILE}01
rm -f ${TEST_FILE}02
rm -f ${TEST_FILE}03
rm -f ${TEST_FILE}04
rm -f ${TEST_FILE}05
rm -f ${TEST_FILE}06
rm -f ${TEST_FILE}07
rm -f ${TEST_FILE}08

# Generate Test Data
days="01 02 03 04 05 06 07 08"
hours="00 06 12 18"
params="1 3 10 11 15 16"
timesteps="0 1 2 3 4 5 6 7 8 9 10 11"

for dd in $days
  do
  rm -f ${TEST_FILE}$dd
  for hh in $hours 
    do
    for parameter in $params 
      do
      for timestep in $timesteps
		do	
		${GRIBWRITE_PATH}/gribWrite --parameter $parameter --proff --timeValue 1980-02-"$dd"T"$hh":00:00 --timeUnit 1 --P1 $timestep --P2 0 --timeRangeIndicator 0 --levelValue 0 --levelUnit 102 --generatingCentre 1 --generatingProcess 252 -P 100,100=1 ${TEST_FILE}$dd
      done
   	done
  done
done

#25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48

# Exit
echo -e "done"
