-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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

CREATE TYPE wci.grid AS (
	grid bytea,
	numberX real,
	numberY real,
	incrementX real,
	incrementY real,
	startX real,
	startY real,
	projDefinition text
);


-- Output Large Objects
CREATE OR REPLACE FUNCTION 
wci.fetch( gridid 			bigint,
		   returntype 		wci.grid )	
RETURNS wci.grid AS
$BODY$
DECLARE
	placeid_		bigint;
	placeinfo_		__WCI_SCHEMA__.placeregulargrid;
	returnObject_ 	wci.grid;
BEGIN
	-- Fetch PlaceId
	SELECT 	placeid INTO placeid_ 
	FROM 	__WCI_SCHEMA__.gridvalue 
	WHERE 	value = gridid;

	-- Retrieve PlaceInfo
	SELECT 	* INTO STRICT placeinfo_
	FROM 	__WCI_SCHEMA__.placeregulargrid_mv p,
			__WCI_SCHEMA__.getSessionData() s
	WHERE 	placeid_ = placeid AND
			p.placenamespaceid = s.placenamespaceid;

	-- Pack result and return
	returnObject_ := (
					__WCI_SCHEMA__.read_file( gridid ),
					placeinfo_.numberX,
					placeinfo_.numberY,
					placeinfo_.incrementX,
					placeinfo_.incrementY,
					placeinfo_.startX,
					placeinfo_.startY,
					placeinfo_.projdefinition );
	RETURN returnObject_;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;
