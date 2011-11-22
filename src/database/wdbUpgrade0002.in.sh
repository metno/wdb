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

-- PlaceDefinition Tables
ALTER TABLE __WDB_SCHEMA__.placedefinition
DROP CONSTRAINT placedefinition_placegeometrytype_check;
ALTER TABLE __WDB_SCHEMA__.placedefinition
ADD CONSTRAINT placedefinition_placegeometrytype_check 
CHECK (	((placegeometrytype)::text = 'grid'::text) OR
		((placegeometrytype)::text = 'line'::text) OR
		((placegeometrytype)::text = 'point'::text) OR
		((placegeometrytype)::text = 'polygon'::text) );
-- UserDefinitions
--ALTER ROLE wdb_test INHERIT wdb_read;
--WCI View
CREATE OR REPLACE VIEW __WCI_SCHEMA__.placedefinition AS
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pg.originalsrid,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn,
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
	AND pd.placeid = pn.placeid
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn
WHERE
	pd.placeid = pn.placeid
	AND pd.placegeometrytype <> 'grid' 
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	pg.originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	lower(st_astext( pd.placegeometry )) as placename,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd
WHERE
	pd.placegeometrytype != 'grid';
		
REVOKE ALL ON __WCI_SCHEMA__.placedefinition FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.placedefinition TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placedefinition TO wdb_read, wdb_write;
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placedefinition_mv');


-- WCI Types
\i $WCI_PATH/types/location.sql
-- WCI Core
\i $WCI_PATH/core/nearestNeighbor.sql
\i $WCI_PATH/core/wciBrowseInternals.sql
\i $WCI_PATH/core/wciWriteInternals.sql
-- WCI Api
\i $WCI_PATH/api/wciAdmin.sql
\i $WCI_PATH/api/wciBrowse.sql
\i $WCI_PATH/api/wciMetaDataProvider.sql
\i $WCI_PATH/api/wciMetaPlace.sql
\i $WCI_PATH/api/wciWrite.sql
\i $WCI_PATH/api/wciAggregate.sql
\i $WCI_PATH/api/wciRemove.sql

-- Clean out a redundant function (removed in WDB 1.0.2)
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getPlaceQuery( spec TEXT, returnType INTEGER );

EOF
