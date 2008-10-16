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
-- Get the details of a regular grid given its place id
--
-- @param placeids[] An array of placeids that the user wishes to retrieve 
--		     information about
--
-- @return A set of grid information for regular grids.
--CREATE OR REPLACE FUNCTION
--wci.regularGridFromId
--(
          --placeids int[]
--)
--RETURNS SETOF __WCI_SCHEMA__.regulargrid AS
--$BODY$
--	SELECT 
		--* 
	--FROM 
--		__WCI_SCHEMA__.regulargrid
--	WHERE 
		--$1 IS NULL OR
		--placeid = ANY ( $1 );
--$BODY$
--LANGUAGE 'sql';


-- Get the full list of place specifications, for which there exists a regular 
-- grid.
--
-- @return All data on all registered regular grids  
CREATE OR REPLACE FUNCTION wci.placespecification()
  RETURNS SETOF __WCI_SCHEMA__.placespec AS
$BODY$
	SELECT 
		p.placeid,
		p.placename,
		p.placenamespaceid,
		p.placegeometry,
		p.placeindeterminatetype,
		p.inumber, 
		p.jnumber, 
		p.iincrement, 
		p.jincrement, 
		p.startlongitude, 
		p.startlatitude, 
		p.originalsrid,
		p.projdefinition 
	FROM __WCI_SCHEMA__.placespec p, __WCI_SCHEMA__.getSessionData() s
	WHERE p.placenamespaceid = s.placenamespaceid;
$BODY$
  LANGUAGE 'sql' VOLATILE;