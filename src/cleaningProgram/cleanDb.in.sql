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

/**
 * Cleans up the database. Deletes all entries older than their dataprovider
 * dataproviderlifecycle, in days.
 */
CREATE OR REPLACE FUNCTION cleanupdb() RETURNS void AS 
$$
BEGIN
	PERFORM clean_referencetime.clean();
	
	RAISE DEBUG 'Starting delete of unused field files.';
	PERFORM __WDB_SCHEMA__.vacuum_file_blob();
	PERFORM __WDB_SCHEMA__.remove_unreferenced_files();
	RAISE DEBUG 'Done';
END;
$$ 
SECURITY DEFINER
LANGUAGE plpgsql;

REVOKE ALL ON FUNCTION cleanupdb() FROM PUBLIC;
GRANT EXECUTE ON FUNCTION cleanupdb() TO wdb_clean;

-- Tester mangler tilgang til wdb_clean-rollen
