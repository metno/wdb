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

mkdir -p __WDB_LOGDIR__
# Remove Test Data
psql `./wdbConfiguration --psqlArgs` -q <<EOF
\o __WDB_LOGDIR__/wdb_test_wci.log
DELETE FROM __WDB_SCHEMA__.gridvalue WHERE dataproviderid >= 0 AND dataproviderid < 100;
DELETE FROM __WDB_SCHEMA__.floatvalue WHERE dataproviderid >= 0 AND dataproviderid < 100;
EOF
