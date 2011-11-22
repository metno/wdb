#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2007-2010 met.no
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
#
# This is the update file for WDB from schema version 1 to schema version 2
#
# Set Variables
export WDB_NAME=$1
export WDB_USER=$2
export WDB_PORT=$3
export WDB_PATH=$4
export WCI_PATH=$5
export WDB_LOGS=$6

# Run PSQL Query
psql -U $WDB_USER -p $WDB_PORT -d $WDB_NAME -q <<EOF
SET CLIENT_MIN_MESSAGES TO "WARNING";
\set ON_ERROR_STOP
\o $WDB_LOGS/wdb_upgrade_datamodel.log

-- Fix dataprovidername to correctly handle similar names in different namespaces
ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    DROP CONSTRAINT dataprovidername_skey;
ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_skey UNIQUE (dataprovidernamespaceid, dataprovidername);

-- Input data
\i $WCI_PATH/api/wciAdmin.sql
\i $WCI_PATH/api/wciMetaParty.sql
\i $WCI_PATH/api/wciMetaDataProvider.sql

SELECT wci.setConfiguration( 'WDB', '__WDB_VERSION__', 1 );
EOF