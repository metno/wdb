-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2008 met.no
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

-- migratedata
-- This function migrates data from an old database version (using wci.read)
-- to a new database. It relies on the wci.read of the old databases being
-- stable, and being able to retrieve ALL required data through the WCI 
-- interface. It only utilizes the 0 namespace.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.migratedata( )
RETURNS void AS
$BODY$
DECLARE
	value_ 					  oid;
	dataProviderName_ 		  text;
	dataProviderId_ 		  bigint;
	placename_ 				  text;
	placeId_ 				  bigint;
	referencetime_ 			  timestamp with time zone;
	validFrom_ 				  timestamp with time zone;
	validTo_ 				  timestamp with time zone;
	valueparameter_ 		  text;
	levelparameter_ 		  text;
	levelFrom_ 				  real;
	levelTo_ 				  real;
	currentVersion_ 		  integer;
	normalizedValueParameter_ text;
	normalizedLevelParameter_ text;
	valueParameterId_ 		  integer;
	levelParameterId_ 		  integer;
	valRef					  wci.returnoidvalue;
BEGIN
	-- TODO: Check database version. If the database version of the old database
	-- is too ancient, abort with an error message

	-- Transfer the OID Raw Data Values
	-- Read all raw data using wci.read
	FOR valRef IN
		SELECT * FROM wci.read( NULL, NULL, NULL, NULL,
								NULL, NULL, NULL, NULL::wci.returnOid )
	LOOP
		dataProviderId_ 		  := __WCI_SCHEMA__.getDataProviderId( valRef.dataProviderName );
		placeId_ 				  := __WCI_SCHEMA__.getPlaceId( valRef.placename_ );
		normalizedValueParameter_ := __WCI_SCHEMA__.normalizeParameter( valueparameter_ );
		normalizedLevelParameter_ := __WCI_SCHEMA__.normalizeLevelParameter( levelparameter_ );
		valueParameterId_ 		  := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
		levelParameterId_ 		  := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ ); 
		
		-- Write all raw data using __WCI_SCHEMA__.write
		SELECT __WCI_SCHEMA__.write(
					dataProviderId_,
					placeId_,
					valRef.referenceTime,
					valRef.validTimeFrom,
					valRef.validTimeTo,
					valRef.validTimeIndeterminateCode,
					valueParameterId_,
					levelParameterId_,
					valRef.levelFrom,
					valRef.levelTo,
					valRef.levelIndeterminateCode,
					valRef.dataVersion,
					valRef.confidenceCode,
					valRef.value
		)
	END LOOP;
	-- TODO: Error handling. If a value can not be inserted into the database
	-- (e.g., because of missing metadata), the migrate operation should either
	-- abort (if so, we need to implement the possibility of a dry-run migration),
	-- or we should be able to save the data ported.
	
	-- After reading all raw data, we will need to pick up any "detailed" info
	-- for the data values (e.g., data import information, quality information, etc)
	-- Also, there should be the possibility to run both pre- and post- operations
	-- to the data migration.
	


	-- Transfer the FLOAT Raw Data Values
	-- Read all raw data using wci.read
	FOR valRef IN
		SELECT * FROM wci.read( NULL, NULL, NULL, NULL,
								NULL, NULL, NULL, NULL::wci.returnFloat )
	LOOP
		dataProviderId_ 		  := __WCI_SCHEMA__.getDataProviderId( valRef.dataProviderName );
		placeId_ 				  := __WCI_SCHEMA__.getPlaceId( valRef.placename_ );
		normalizedValueParameter_ := __WCI_SCHEMA__.normalizeParameter( valueparameter_ );
		normalizedLevelParameter_ := __WCI_SCHEMA__.normalizeLevelParameter( levelparameter_ );
		valueParameterId_ 		  := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
		levelParameterId_ 		  := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ ); 
		
		-- Write all raw data using __WCI_SCHEMA__.write
		SELECT __WCI_SCHEMA__.write(
					dataProviderId_,
					placeId_,
					valRef.referenceTime,
					valRef.validTimeFrom,
					valRef.validTimeTo,
					valRef.validTimeIndeterminateCode,
					valueParameterId_,
					levelParameterId_,
					valRef.levelFrom,
					valRef.levelTo,
					valRef.levelIndeterminateCode,
					valRef.dataVersion,
					valRef.confidenceCode,
					valRef.value
		)
	END LOOP;
	-- TODO: Error handling. If a value can not be inserted into the database
	-- (e.g., because of missing metadata), the migrate operation should either
	-- abort (if so, we need to implement the possibility of a dry-run migration),
	-- or we should be able to save the data ported.

END 
$BODY$
LANGUAGE 'plpgsql';

SELECT __WCI_SCHEMA__.migratedata( );

DROP FUNCTION __WCI_SCHEMA__.migratedata( );