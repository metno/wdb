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


-- Caches the fields associated with the given query, making subsequent reads
-- (especially for points) more efficient.
CREATE OR REPLACE FUNCTION
wci.cacheQuery( 
	dataprovider 	text[],
	location 		text,
	referencetime 	text,
	validtime 		text,
	parameter 		text[],
	level 			text,
	dataversion 	integer[]
)
RETURNS void AS
$BODY$
DECLARE
	gridToCache bigint;
BEGIN
	FOR gridToCache IN 
		SELECT value FROM wci.read(
			dataprovider, location, 
			referencetime, validtime, 
			parameter, level, 
			dataversion, NULL::wci.returnGid) LOOP
			
		PERFORM __WCI_SCHEMA__.read_file(gridToCache);
	END LOOP;
END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION
wci.cacheQuery( 
	dataprovider 	text[],
	location 		text,
	referencetime 	text,
	validtime 		text,
	parameter 		text[],
	level 			text,
	dataversion 	integer[]
)
RETURNS void AS
$BODY$
BEGIN
	PERFORM __WCI_SCHEMA__.cache_file(value) 
	FROM wci.read(
		dataprovider, location, 
		referencetime, validtime, 
		parameter, level, 
		dataversion, NULL::wci.returnGid
	);
END;
$BODY$
LANGUAGE plpgsql VOLATILE;


