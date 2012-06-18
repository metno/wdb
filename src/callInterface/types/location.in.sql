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


DROP TYPE IF EXISTS __WCI_SCHEMA__.location CASCADE;
CREATE TYPE  __WCI_SCHEMA__.location AS (
	interpolation 	text,
	location 		text,
	isGeometry 		boolean
);

CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getLocation( spec TEXT )
RETURNS __WCI_SCHEMA__.location AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciLocation'
LANGUAGE 'c' IMMUTABLE STRICT;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getPlaceId(
	name_ text
)
RETURNS bigint AS
$BODY$
DECLARE
	session __WCI_SCHEMA__.sessionData;
	ret bigint;
BEGIN
	SELECT placeid INTO ret
	FROM
		__WCI_SCHEMA__.placedefinition_mv p,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		p.placenamespaceid = s.placenamespaceid AND
		p.placename = lower(name_);

	IF NOT FOUND THEN
		RAISE EXCEPTION 'Failed to identify placename in namespace: %', name_;
	END IF;

	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql';



CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getGeometry(
	location __WCI_SCHEMA__.location
)
RETURNS geometry AS
$BODY$
DECLARE
	ret geometry;
BEGIN
	IF location.isGeometry THEN
		RETURN st_geomfromtext(location.location, 4030);
	END IF;

	SELECT 
		placegeometry
	INTO 
		ret 
	FROM 
		__WCI_SCHEMA__.placedefinition_mv p,
		__WCI_SCHEMA__.getSessionData() s
	WHERE 
		p.placenamespaceid = s.placenamespaceid AND
		p.placename = lower(location.location);

	-- This should not be required. 
	--IF ret IS NULL THEN
	--	RAISE EXCEPTION '%: No such place', location.location;
	--END IF;

	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getGeometry (
	locationSpec TEXT
)
RETURNS  geometry AS
$BODY$
DECLARE
	location __WCI_SCHEMA__.location;
	ret geometry;
BEGIN
	SELECT * INTO location FROM __WCI_SCHEMA__.getLocation(locationSpec);
	SELECT * INTO ret FROM __WCI_SCHEMA__.getGeometry(location);
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;
