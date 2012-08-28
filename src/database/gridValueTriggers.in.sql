-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 - 2012 met.no
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

-- Deletion control
CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.deleteObsoleteGrids() RETURNS trigger
	AS 
$BODY$
DECLARE
	noOfRows integer;
	status integer;
BEGIN
	SELECT count(*) INTO noOfRows FROM __WDB_SCHEMA__.gridvalue WHERE value=OLD.value;
	IF noOfRows = 0 THEN
		BEGIN
			PERFORM __WDB_SCHEMA__.drop_file(OLD.value);
			-- No exception handling
		END;
		IF status = -1 THEN
			RAISE WARNING 'Error when attempting to delete large object <%>', OLD.value;
		END IF;
	ELSE
		RAISE DEBUG 'Still % rows left which refers to Grid <%>', noOfRows, OLD.value;
	END IF;
	RETURN NULL;
END;
$BODY$
LANGUAGE plpgsql;



CREATE TRIGGER trigger___WDB_SCHEMA___deleteObsoleteGrids
    AFTER DELETE ON __WDB_SCHEMA__.gridvalue
    FOR EACH ROW
    EXECUTE PROCEDURE __WDB_SCHEMA__.deleteObsoleteGrids();
