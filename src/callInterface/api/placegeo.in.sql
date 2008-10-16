-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 met.no
--
--  Contact information:
--  Norwegian Meteorological Institute
--  Box 43 Blindern
--  0313 OSLO
--  NORWAY
--  E-mail: wdb@met.no
--
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- Get dataproviderids from the given .
--
-- If any of the provided function arguments are NULL, all dataproviderids
-- mathcing the other function arguments are returned.
--
-- @param providerName The name of the data provider
-- @param providerType The type of the data provider ('Named Observation 
--                     Site', 'Person', 'Ship', 'Aeroplane' or 'Computer 
--                     System').
--
-- @return An array of dataproviderids matching the given characteristics.
CREATE OR REPLACE FUNCTION
wci.idFromPlaceGeo
(
          geoString	character varying (254)
)
RETURNS bigint[] AS
$BODY$
	SELECT ARRAY(
		SELECT 
			distinct placeid
		FROM 
			__WCI_SCHEMA__.placegeo
		WHERE 
			( $1 IS NULL OR 
			  EQUALS(GeomFromText($1), GeomFromText(placegeometry) ) )
	);
$BODY$
LANGUAGE 'sql';



-- Get parameter characteristics from the given parameterids.
--
-- @param dataproviders[] An array of dataproviderids which we are interested 
--                        in information about.
--
-- @return A set of parameter characteristics.
CREATE OR REPLACE FUNCTION
wci.placeGeoFromId
(
          placeids int[]
)
RETURNS SETOF __WCI_SCHEMA__.placegeo AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.placegeo
	WHERE 
		$1 IS NULL OR
		placeid = ANY ( $1 );
$BODY$
LANGUAGE 'sql';
