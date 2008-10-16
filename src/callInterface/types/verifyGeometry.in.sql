-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2008 met.no
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


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.verifyGeometry(
	geo geometry
)
RETURNS void AS
$BODY$
DECLARE
		geomType CONSTANT text := GeometryType( geo );
BEGIN
	IF NOT ( geomType IS NULL OR geomType = 'POINT' OR geomType = 'POLYGON' )
	THEN
		RAISE EXCEPTION 'Cannot process %', geomType;
	END IF;
END;
$BODY$
LANGUAGE plpgsql STRICT IMMUTABLE;
