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
wci.read( dataprovider 		text[],
		  location 			text,
		  referencetime 	text,
		  validtime 		text,
		  parameter 		text[],
		  level 			text,
		  dataversion 		integer[],
		  returntype 		wci.returnGid
)	
RETURNS SETOF wci.returnGid AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciReadGid'
LANGUAGE 'c' STABLE;


CREATE OR REPLACE FUNCTION 
wci.read( dataprovider 		text[],
		  location 			text,
		  referencetime 	text,
		  validtime 		text,
		  parameter 		text[],
		  level 			text,
		  dataversion 		integer[],
		  returntype 		wci.returnFloat
)
RETURNS SETOF wci.returnFloat AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciReadFloat'
SECURITY DEFINER
LANGUAGE 'c' STABLE;
