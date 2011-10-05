-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2011 met.no
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

BEGIN;

CREATE SCHEMA clean_referencetime;
REVOKE ALL ON SCHEMA clean_referencetime FROM PUBLIC;
GRANT ALL ON SCHEMA clean_referencetime TO wdb_admin;
GRANT ALL ON SCHEMA clean_referencetime TO wdb_clean;

--
-- Lifetime table. A single NULL dataprovider can (and probably should) exist 
-- in this table. That entry marks the default lifetime for data. All data 
-- providers in the database that does not have an entry in this table will 
-- get the NULL dataprovider's lifetime. 
--
CREATE TABLE clean_referencetime.lifetime (
	dataprovidername text UNIQUE,
	oldest_lifetime interval NOT NULL
);

INSERT INTO clean_referencetime.lifetime VALUES (NULL, '1 day');
INSERT INTO clean_referencetime.lifetime VALUES ('statkart.no', '10000 years');


CREATE OR REPLACE FUNCTION clean_referencetime.clean()
RETURNS void AS
$BODY$
DECLARE
	dataprovider text;
	lifetime interval;
	dataprovider_array text[1];
	referencetime text;
BEGIN
	FOR dataprovider IN SELECT dataprovidername FROM wci.browse(NULL::wci.browsedataprovider) LOOP
		SELECT 
			oldest_lifetime INTO lifetime 
		FROM 
			clean_referencetime.lifetime 
		WHERE 
			dataprovidername = dataprovider OR 
			dataprovidername IS NULL
		ORDER BY 
			dataprovidername 
		LIMIT 1;
		
		IF NOT FOUND THEN 
			RAISE EXCEPTION 'Unable to find lifetime (specific or otherwise) for dataprovider %', dataprovider;
		END IF;

		dataprovider_array[1] = dataprovider;
		referencetime = 'before ' || (now() - lifetime)::text;

		RAISE DEBUG 'Removing all data for dataprovider <%> with reference time <%>', dataprovider_array[1], referencetime;
		PERFORM wci.remove(dataprovider_array,NULL, referencetime,NULL, NULL,NULL, NULL);
	END LOOP;
END;
$BODY$
LANGUAGE plpgsql;

END;