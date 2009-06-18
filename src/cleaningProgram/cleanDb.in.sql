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
DECLARE 
	-- The rows in __WDB_SCHEMA__.dataprovider - to be iterated over
	provider __WDB_SCHEMA__.dataprovider%ROWTYPE;
BEGIN
	RAISE DEBUG 'Deleting old data from the database';
	FOR provider IN SELECT * FROM __WDB_SCHEMA__.dataprovider LOOP
		DECLARE
			-- A dataprovider's Time To Live
			ttl INTERVAL := provider.dataproviderlife;
			-- Maximum allowed age for a given dataprovider
			oldest_allowed TIMESTAMP := TIMESTAMP 'today' - ttl;
		BEGIN
			RAISE LOG 'GridValue: Deleting all data for data provider % from before %', provider.dataproviderid, oldest_allowed::date;
			DELETE FROM __WDB_SCHEMA__.gridvalue 
			WHERE dataproviderid = provider.dataproviderid AND referencetime < oldest_allowed;
		END;
	END LOOP;

	RAISE DEBUG 'Removed old table entries Starting delete of unused field files.';
	PERFORM __WDB_SCHEMA__.vacuum_file_blob();
	PERFORM __WDB_SCHEMA__.remove_unreferenced_files();
	
	RAISE DEBUG 'Done';
END;
$$ 
LANGUAGE plpgsql;

/**
 * Cleans up the database of test data. 
 * Deletes all entries of dataproviders < 100.
 */
CREATE OR REPLACE FUNCTION testclean() RETURNS void AS 
$$
BEGIN
	DELETE FROM __WDB_SCHEMA__.gridvalue 
	WHERE dataproviderid < 100;
	DELETE FROM __WDB_SCHEMA__.floatvalue 
	WHERE dataproviderid < 100;
	--PERFORM __WDB_SCHEMA__.vacuum_file_blob();
	--PERFORM __WDB_SCHEMA__.remove_unreferenced_files();
END;
$$ 
LANGUAGE plpgsql;
