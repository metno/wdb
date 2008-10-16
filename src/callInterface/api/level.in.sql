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
-- Get levelparameterids from the given parameter characteristics.
--
-- If any of the provided function arguments are NULL, all levelparameterids 
-- mathcing the other function arguments are returned.
--
-- @param statisticsType
-- @param physicalPhenomena
-- @param usageArea
--
-- @return An array of levelparameterids matching the given parameter 
--         characteristics.
CREATE OR REPLACE FUNCTION
wci.idFromLevelParameter
(
	levelName CHARACTER VARYING(80)
)
RETURNS integer[] AS
$BODY$
	SELECT ARRAY(
		SELECT 
			levelparameterid
		FROM 
			__WCI_SCHEMA__.levelparameter 
		WHERE 
			( $1 IS NULL OR levelparametername = $1 )
	);
$BODY$
LANGUAGE 'sql';



-- Get parameter characteristics from the given parameterids.
--
-- @param parameterid[] An array of levelparameterids which we are interested 
--                      in information about.
--
-- @return A set of parameter characteristics.
CREATE OR REPLACE FUNCTION
wci.levelParameterFromId
(
          parameterid int[]
)
RETURNS SETOF __WCI_SCHEMA__.levelParameter AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.levelParameter
	WHERE 
		$1 IS NULL OR
		levelparameterid = ANY ( $1 );
$BODY$
LANGUAGE 'sql';
