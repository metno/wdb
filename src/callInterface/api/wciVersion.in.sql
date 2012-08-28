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

CREATE OR REPLACE FUNCTION 
wci.version( )
RETURNS text AS
$BODY$
DECLARE
	version text;
	ret  text;
BEGIN
	SELECT 	c.softwareversion INTO version 
	FROM 	__WCI_SCHEMA__.configuration_v c
	WHERE 	c.name = 'WDB';
	ret := 'WDB ' || version;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;

CREATE OR REPLACE FUNCTION
wci.configuration( )
RETURNS SETOF __WCI_SCHEMA__.configuration_v AS
$BODY$
	SELECT * FROM __WCI_SCHEMA__.configuration_v;
$BODY$
LANGUAGE 'SQL' STRICT STABLE;