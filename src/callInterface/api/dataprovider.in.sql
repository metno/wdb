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
-- 
-- Get dataproviderids from the given name and type.
--
-- If any of the provided function arguments are NULL, all dataproviderids
-- mathcing the other function arguments are returned.
--
-- @param providerName The name of the data provider
-- @param providerType The type of the data provider ('Named Observation 
--                     Site', 'Person', 'Ship', 'Aeroplane' or 'Computer 
--                     System').
--
-- @return An array of dataproviderids matching the given characteristics.
CREATE OR REPLACE FUNCTION
wci.idFromDataProvider
(
          providerName    character varying(80),
          providerType    character varying(80)
)
RETURNS bigint[] AS
$BODY$
	SELECT ARRAY(
		SELECT 
			DISTINCT dataproviderid
		FROM 
			__WCI_SCHEMA__.dataprovider
		WHERE 
			( $1 IS NULL OR dataprovidername = $1 ) AND
			( $2 IS NULL OR dataprovidertype = $2 ) 
	);
$BODY$
LANGUAGE 'sql' STABLE;



-- Get parameter characteristics from the given parameterids.
--
-- @param dataproviders[] An array of dataproviderids which we are interested 
--                        in information about.
--
-- @return A set of parameter characteristics.
CREATE OR REPLACE FUNCTION
wci.dataProviderFromId
(
          dataproviders int[]
)
RETURNS SETOF __WCI_SCHEMA__.dataProvider AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.dataprovider
	WHERE 
		$1 IS NULL OR
		dataproviderid = ANY ( $1 );
$BODY$
LANGUAGE 'sql' STABLE;

CREATE OR REPLACE FUNCTION
wci.createWciDataProvider
(
	newDataProviderName text
)
RETURNS integer AS
$BODY$
DECLARE
	id bigint;
BEGIN

	SELECT dataproviderid INTO id 
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = newDataProviderName AND
		dataprovidertype = 'WCI User';

	IF NOT FOUND THEN
		id := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);

		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		(id, 'WCI User', 'Any', 'now');
	
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		(id, 'wci user', 'now');
	
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
		(id, 0, newDataProviderName, 'today', '2100-01-01', id, id);
	ELSE
		RAISE INFO 'User was already defined as a dataprovider';
	END IF;

	RETURN id;
END;
$BODY$
LANGUAGE plpgsql VOLATILE;

