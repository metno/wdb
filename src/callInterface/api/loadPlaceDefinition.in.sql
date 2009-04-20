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

CREATE OR REPLACE FUNCTION
wci.loadPlaceDefinition(
	name text,
	location geometry -- must be a point
)
RETURNS bigint AS
$BODY$
DECLARE
	val bigint;
BEGIN
	-- todo: check that given geometry type is a point
	
	val := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
	
	INSERT INTO __WDB_SCHEMA__.placedefinition VALUES (val, 0, 'Point', 'now', location);
	INSERT INTO __WDB_SCHEMA__.placename VALUES (val, 0, name, 'today', '2100-01-01');
	
	RETURN val;
END;
$BODY$
LANGUAGE plpgsql STRICT VOLATILE;
