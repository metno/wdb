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

-- Write GRID Value
-- Limited Specification
CREATE OR REPLACE FUNCTION 
wci.write(
	data 			bytea,
	placename_ 		text,
	referencetime_ 	timestamp with time zone,
	validFrom_ 		timestamp with time zone,
	validTo_ 		timestamp with time zone,
	valueparameter_ text,
	levelparameter_ text,
	levelFrom_ 		real,
	levelTo_ 		real
)
RETURNS void AS
$BODY$
DECLARE
	myDataProviderId_ bigint := __WCI_SCHEMA__.wcidataproviderid();
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueparameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelparameter_ );
	currentVersion_ integer;
	placeid_ bigint := __WCI_SCHEMA__.getplaceid( placename_ );
	valueParameterId_ integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	levelParameterId_ integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ ); 
BEGIN
	
	--PERFORM verifyBlobSize(data, placeid_);

	-- Determine dataversion
	SELECT 
		max(dataversion) INTO currentVersion_ 
	FROM 
		__WCI_SCHEMA__.gridvalue v
	WHERE
		myDataProviderId_ = dataproviderid AND
		v.referencetime = referencetime_ AND
		v.placeid = placeid_ AND
		v.valueparameterid = valueParameterId_ AND
		v.levelparameterid = levelParameterId_ AND
		v.levelFrom = levelFrom_ AND
		v.levelTo = levelTo_ AND
		v.validtimeFrom = validFrom_ AND
		v.validtimeTo = validTo_;
	RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
	IF currentVersion_ IS NULL THEN
		currentVersion_ := 0;
	ELSE
		currentVersion_ := currentVersion_ + 1;
	END IF;

	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		__WCI_SCHEMA__.write_file(data),
		mydataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0, -- Exact
		valueparameterid_,
		NULL, 
		NULL,
		levelparameterid_,
		NULL, 
		NULL, 
		levelfrom_,
		levelto_,
		0, -- Exact
		currentversion_,
		NULL,
		0, -- Default
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';



-- Write GRID Value
-- All parameter specified
CREATE OR REPLACE FUNCTION 
wci.write(
	data bytea,
	dataproviderName_ text,
	placeName_ text,
	referencetime_ timestamp with time zone,
	validfrom_ timestamp with time zone,
	validto_ timestamp with time zone,
	valueParameterName_ text,
	levelParameterName_ text,
	levelfrom_ real,
	levelto_ real,
	dataVersion_ integer,
	setConfidenceCode_ integer
)
RETURNS void AS
$BODY$
DECLARE
	dataProviderId_ 		  bigint := __WCI_SCHEMA__.idfromdataprovider( dataProviderName_ );
	placeid_ 				  bigint := __WCI_SCHEMA__.getplaceid( placeName_ );
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueParameterName_ );
	valueParameterId_ 	   	  integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelParameterName_ );
	levelParameterId_      	  integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	currentVersion_ 		  integer := dataVersion_;
	confidenceCode_			  integer := setConfidenceCode_;
BEGIN
	--PERFORM verifyBlobSize(data, placeid_);
	-- Determine dataversion
	IF currentVersion_ IS NULL THEN
		SELECT 
			max(dataversion) INTO currentVersion_ 
		FROM 
			__WCI_SCHEMA__.gridvalue v
		WHERE
			v.dataproviderid = dataProviderId_ AND
			v.referencetime = referencetime_ AND
			v.placeid = placeid_ AND
			v.valueparameterid = valueParameterId_ AND
			v.levelparameterid = levelParameterId_ AND
			v.levelFrom = levelFrom_ AND
			v.levelTo = levelTo_ AND
			v.validtimeFrom = validFrom_ AND
			v.validtimeTo = validTo_;
		RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
		IF currentVersion_ IS NULL THEN
			currentVersion_ := 0;
		ELSE
			currentVersion_ := currentVersion_ + 1;
		END IF;
	END IF;
	IF confidenceCode_ IS NULL THEN
		confidenceCode_ := 0;
	END IF;

	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		__WCI_SCHEMA__.write_file(data),
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0,   -- Exact
		valueparameterId_,
		NULL, NULL,
		levelparameterId_,
		NULL, NULL, 
		levelfrom_,
		levelto_,
		0, --Exact
		currentVersion_,
		NULL,
		confidenceCode_,
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';



-- Write GRID Value
-- All parameter specified
CREATE OR REPLACE FUNCTION 
wci.write(
	gridId_ bigint,
	dataproviderName_ text,
	placeName_ text,
	referencetime_ timestamp with time zone,
	validfrom_ timestamp with time zone,
	validto_ timestamp with time zone,
	valueParameterName_ text,
	levelParameterName_ text,
	levelfrom_ real,
	levelto_ real,
	dataVersion_ integer,
	setConfidenceCode_ integer
)
RETURNS void AS
$BODY$
DECLARE
	dataProviderId_ 		  bigint := __WCI_SCHEMA__.idfromdataprovider( dataProviderName_ );
	placeid_ 				  bigint := __WCI_SCHEMA__.getplaceid( placeName_ );
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueParameterName_ );
	valueParameterId_ 	   	  integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelParameterName_ );
	levelParameterId_      	  integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	currentVersion_ 		  integer := dataVersion_;
	confidenceCode_			  integer := setConfidenceCode_;
BEGIN
	--PERFORM verifyBlobSize(data, placeid_);
	-- Verify GridId
	-- TODO: Not implemented yet.	
	-- Determine dataversion
	IF currentVersion_ IS NULL THEN
		SELECT 
			max(dataversion) INTO currentVersion_ 
		FROM 
			__WCI_SCHEMA__.gridvalue v
		WHERE
			v.dataproviderid = dataProviderId_ AND
			v.referencetime = referencetime_ AND
			v.placeid = placeid_ AND
			v.valueparameterid = valueParameterId_ AND
			v.levelparameterid = levelParameterId_ AND
			v.levelFrom = levelFrom_ AND
			v.levelTo = levelTo_ AND
			v.validtimeFrom = validFrom_ AND
			v.validtimeTo = validTo_;
		RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
		IF currentVersion_ IS NULL THEN
			currentVersion_ := 0;
		ELSE
			currentVersion_ := currentVersion_ + 1;
		END IF;
	END IF;
	IF confidenceCode_ IS NULL THEN
		confidenceCode_ := 0;
	END IF;

	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		gridId_,
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0,   -- Exact
		valueparameterId_,
		NULL, NULL,
		levelparameterId_,
		NULL, NULL, 
		levelfrom_,
		levelto_,
		0, --Exact
		currentVersion_,
		NULL,
		confidenceCode_,
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';



-- Write GRID Value
-- All parameter specified
CREATE OR REPLACE FUNCTION 
wci.write(
	data bytea,
	dataproviderid_ bigint,
	placeid_ bigint,
	referencetime_ timestamp with time zone,
	validfrom_ timestamp with time zone,
	validto_ timestamp with time zone,
	validic_ integer,
	valueparameter_ integer,
	levelparameter_ integer,
	levelfrom_ real,
	levelto_ real,
	levelic_ integer,
	dataversion_ integer,
	confidencecode_ integer
)
RETURNS void AS
$BODY$
BEGIN
	--PERFORM verifyBlobSize(data, placeid_);

	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		__WCI_SCHEMA__.write_file(data),
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		validIC_,
		valueparameter_,
		NULL, NULL,
		levelparameter_,
		NULL, NULL, 
		levelfrom_,
		levelto_,
		levelIC_,
		dataversion_,
		NULL,
		confidencecode_,
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';



-- Write GRID Value
CREATE OR REPLACE FUNCTION 
wci.write(
	what wci.returnGid
)
RETURNS void AS
$BODY$
DECLARE
	dataProviderId_ 		  bigint := __WCI_SCHEMA__.idfromdataprovider( what.dataProviderName );
	placeid_ 				  bigint := __WCI_SCHEMA__.getplaceid( what.placeName );
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( what.valueParameterName );
	valueParameterId_ 	   	  integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( what.levelParameterName );
	levelParameterId_      	  integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	currentVersion_ 		  integer := what.dataVersion;
BEGIN
	--PERFORM verifyBlobSize(data, placeid_);

	-- Determine dataversion
	IF currentVersion_ IS NULL THEN
		SELECT 
			max(dataversion) INTO currentVersion_ 
		FROM 
			__WCI_SCHEMA__.gridvalue v
		WHERE
			v.dataproviderid = dataProviderId_ AND
			v.referencetime = what.referencetime AND
			v.placeid = placeid_ AND
			v.valueparameterid = valueParameterId_ AND
			v.levelparameterid = levelParameterId_ AND
			v.levelFrom = what.levelFrom AND
			v.levelTo = what.levelTo AND
			v.validtimeFrom = what.validtimeFrom AND
			v.validtimeTo = what.validtimeTo;
		RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
		IF currentVersion_ IS NULL THEN
			currentVersion_ := 0;
		ELSE
			currentVersion_ := currentVersion_ + 1;
		END IF;
	END IF;
		
	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		what.value,
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		what.referencetime,
		what.validtimefrom,
		what.validtimeto,
		0, -- Exact
		valueparameterid_,
		NULL, 
		NULL,
		levelparameterid_,
		NULL, 
		NULL, 
		what.levelfrom,
		what.levelto,
		0, -- Exact
		currentversion_,
		NULL,
		0, -- Default
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';






--
-- wci.write with floats below here
--

-- Write Float Value
-- Limited Specification
CREATE OR REPLACE FUNCTION 
wci.write(
	value_ 			float,
	placename_ 		text,
	referencetime_ 	timestamp with time zone,
	validFrom_ 		timestamp with time zone,
	validTo_ 		timestamp with time zone,
	valueparameter_ text,
	levelparameter_ text,
	levelFrom_ 		real,
	levelTo_ 		real
)
RETURNS void AS
$BODY$
DECLARE
	myDataProviderId_ bigint := __WCI_SCHEMA__.wcidataproviderid();
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueparameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelparameter_ );
	currentVersion_ integer;
	placeid_ bigint := __WCI_SCHEMA__.getplaceid( placename_ );
	valueParameterId_ integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	levelParameterId_ integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ ); 
BEGIN
	
	--PERFORM verifyBlobSize(value_, placeid_);

	-- Determine dataversion
	SELECT 
		max(dataversion) INTO currentVersion_ 
	FROM 
		__WCI_SCHEMA__.floatvalue v
	WHERE
		myDataProviderId_ = dataproviderid AND
		v.referencetime = referencetime_ AND
		v.placeid = placeid_ AND
		v.valueparameterid = valueParameterId_ AND
		v.levelparameterid = levelParameterId_ AND
		v.levelFrom = levelFrom_ AND
		v.levelTo = levelTo_ AND
		v.validtimeFrom = validFrom_ AND
		v.validtimeTo = validTo_;
	RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
	IF currentVersion_ IS NULL THEN
		currentVersion_ := 0;
	ELSE
		currentVersion_ := currentVersion_ + 1;
	END IF;

	INSERT INTO __WCI_SCHEMA__.floatvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		value_,
		mydataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0, -- Exact
		valueparameterid_,
		NULL, 
		NULL,
		levelparameterid_,
		NULL, 
		NULL, 
		levelfrom_,
		levelto_,
		0, -- Exact
		currentversion_,
		NULL,
		0, -- Default
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';


-- Write OID Value
CREATE OR REPLACE FUNCTION 
wci.write(
	what wci.returnFloat
)
RETURNS void AS
$BODY$
DECLARE
	dataProviderId_ 		bigint := __WCI_SCHEMA__.idfromdataprovider( what.dataProviderName );
	placeid_ 				bigint := __WCI_SCHEMA__.getplaceid( what.placeName );
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( what.valueParameterName );
	valueParameterId_ 	   	integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( what.levelParameterName );
	levelParameterId_      	integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	currentVersion_ 		integer := what.dataVersion;
BEGIN
	--PERFORM verifyBlobSize(value_, placeid_);

	-- Determine dataversion
	IF currentVersion_ IS NULL THEN
		SELECT 
			max(dataversion) INTO currentVersion_ 
		FROM 
			__WCI_SCHEMA__.floatvalue v
		WHERE
			v.dataproviderid = dataProviderId_ AND
			v.referencetime = what.referencetime AND
			v.placeid = placeid_ AND
			v.valueparameterid = valueParameterId_ AND
			v.levelparameterid = levelParameterId_ AND
			v.levelFrom = what.levelFrom AND
			v.levelTo = what.levelTo AND
			v.validtimeFrom = what.validtimeFrom AND
			v.validtimeTo = what.validtimeTo;
		RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
		IF currentVersion_ IS NULL THEN
			currentVersion_ := 0;
		ELSE
			currentVersion_ := currentVersion_ + 1;
		END IF;
	END IF;
		
	INSERT INTO __WCI_SCHEMA__.floatvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		what.value,
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		what.referencetime,
		what.validtimefrom,
		what.validtimeto,
		0, -- Exact
		valueparameterid_,
		NULL, 
		NULL,
		levelparameterid_,
		NULL, 
		NULL, 
		what.levelfrom,
		what.levelto,
		0, -- Exact
		currentversion_,
		NULL,
		0, -- Default
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';


--
-- Write Float Value
-- Explicit data specification
--
CREATE OR REPLACE FUNCTION 
wci.write(
	value_ 			float,
	dataproviderid_ bigint,
	placeid_ 		bigint,
	referencetime_ 	timestamp with time zone,
	validfrom_ 		timestamp with time zone,
	validto_ 		timestamp with time zone,
	validic_ 		integer,
	valueparameter_ integer,
	levelparameter_ integer,
	levelfrom_ 		real,
	levelto_ 		real,
	levelic_ 		integer,
	dataversion_ 	integer,
	confidencecode_ integer
)
RETURNS void AS
$BODY$
BEGIN
	INSERT INTO __WCI_SCHEMA__.floatvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		value_,
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		validIC_,
		valueparameter_,
		NULL, 
		NULL,
		levelparameter_,
		NULL, 
		NULL, 
		levelfrom_,
		levelto_,
		levelIC_,
		dataversion_,
		NULL,
		confidencecode_,
		NULL,
		NULL,
		NULL
	);
END 
$BODY$
LANGUAGE 'plpgsql';
