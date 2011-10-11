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


CREATE SCHEMA clean;
REVOKE ALL ON SCHEMA clean FROM PUBLIC;
GRANT ALL ON SCHEMA clean TO wdb_admin;
GRANT USAGE ON SCHEMA clean TO wdb_clean;

CREATE OR REPLACE FUNCTION cleanupdb() RETURNS void AS 
'__WDB_LIBDIR__/__WCI_LIB__', 'cleanupdb'
SECURITY DEFINER
LANGUAGE c;
REVOKE ALL ON FUNCTION cleanupdb() FROM PUBLIC;
GRANT EXECUTE ON FUNCTION cleanupdb() TO wdb_clean;

CREATE TABLE clean.strategies (
	call_order INTEGER NOT NULL UNIQUE,  
	function text NOT NULL UNIQUE
);
