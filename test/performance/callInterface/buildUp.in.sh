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
TEST_WRITE=./testWrite

echo -n "# Generating test data (please wait)... "

# Generate Test Data
days="01"
hours="00 06 12 18"
#params="'air pressure'"
# 'ozone dobson surface density' 'air temperature' 'max air temperature' 'min air temperature'
timesteps="0 1 2 3 4 5 6 7 8 9 10 11"

for dd in $days
  do
  for hh in $hours 
    do
    for timestep in $timesteps
	  do
	  vh=`expr $timestep + $hh`
	  if test $vh -ge 24; then
		vh=`expr $vh - 24`
	  fi		
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'air pressure' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'air pressure at sea level' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'air temperature' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'max air temperature' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'min air temperature' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'altitude' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'hirlam 10 grid' --reftime 1980-01-"$dd"T"$hh":00:00+00 --valueparameter 'snowfall amount' --validtimefrom 1980-01-"$dd"T"$vh":00:00+00 --validtimeto 1980-01-"$dd"T"$vh":00:00+00 
    done
  done
done

# Complex Test
days="01"
hours="00"
timesteps="0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29"
for dd in $days
  do
  for hh in $hours 
    do
    for timestep in $timesteps
	  do
	  vd="01"
	  vh=`expr $timestep + $hh`
	  if test $vh -ge 48; then
		vh=`expr $vh - 48`
		vd="03"
	  fi		
	  if test $vh -ge 24; then
		vh=`expr $vh - 24`
		vd="02"
	  fi		
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure at sea level' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'max air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'min air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'altitude' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00	  
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'precipitation amount' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00	  
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'x wind' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00	  
	  $TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'y wind' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00	  
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure at sea level' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'max air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'min air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 1' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'precipitation amount' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00	  
	  $TEST_WRITE --dataprovider 'test wci 2' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 2' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air pressure at sea level' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 2' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 2' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'max air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
	  $TEST_WRITE --dataprovider 'test wci 2' --placename 'proff grid' --reftime 1980-02-"$dd"T00:00:00+00 --valueparameter 'min air temperature' --validtimefrom 1980-02-"$vd"T"$vh":00:00+00 --validtimeto 1980-02-"$vd"T"$vh":00:00+00 
    done
  done
done
$TEST_WRITE --dataprovider 'test wci 0' --placename 'proff grid' --reftime 1980-02-01T00:00:00+00 --valueparameter 'thunderstorm probability' --validtimefrom 1980-01-01T00:00:00+00 --validtimeto 1981-01-01T00:00:00+00


# Tune database
psql `__WDB_BINDIR__/wdbConfiguration --psqlArgs` -q -c "ANALYZE;"

# Exit
echo -e "done"
