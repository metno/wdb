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
-- Verify that the size of a blob is equal to the corresponding 
-- placeregulargrid entry.
--
CREATE OR REPLACE FUNCTION 
verifyBlobSize(
	value bytea,
	placename_ text
)
RETURNS void AS
$BODY$
DECLARE
	blobSize real := length(value) / 4.0;
	specSize int;
BEGIN
	SELECT 
		g.numberx*g.numbery INTO specSize
	FROM 
		__WCI_SCHEMA__.placeregulargrid_v g, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		g.placename = placename_ AND
		g.placenamespaceid = s.placenamespaceid;

	IF blobSize != specSize THEN
		RAISE EXCEPTION 'verifyBlobSize: Wrong size of data field (size=%, should be %)', blobSize, specSize;
	END IF;
END;
$BODY$
LANGUAGE plpgsql STABLE;



--
-- __WCI_SCHEMA__.write
-- This is the main work function of the wci.write call (for OID)
-- 
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.write(
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
	confidencecode_ integer,
	dataId 			bigint
)
RETURNS void AS
$BODY$
DECLARE
	session __WCI_SCHEMA__.sessiondata_v;
BEGIN
	-- Get session data (codespaces) 
	SELECT * INTO session FROM __WCI_SCHEMA__.getSessionData(); 
	-- dataproviderid	
	-- placeid
	-- referencetime
	-- validtime
	-- Todo: Set TimeIndeterminateCode
	-- valueparameter
	-- levelparameter	
	-- Todo: Set Levels by levelIndeterminateCode
	-- dataversion
	-- Todo: what if dataversion is not max?
	UPDATE 	__WDB_SCHEMA__.gridvalue 
	SET 	maxdataversion = dataversion_ 
	WHERE
		dataproviderid = dataproviderid_ AND
		placeid = placeid_ AND
		referencetime = referencetime_ AND
		validtimefrom = validfrom_ AND
		validtimeto = validto_ AND
		validtimeindeterminatecode = validic_ AND
		valueparameterid = valueparameter_ AND
		levelparameterid = levelparameter_ AND
		levelfrom = levelfrom_ AND
		levelto = levelto_ AND
		levelindeterminatecode = levelic_;

	-- Insert value row
	INSERT INTO __WDB_SCHEMA__.gridvalue (
		dataproviderid,
		placeid,
		referencetime,
		validtimefrom,
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		levelparameterid,
		levelfrom, 
		levelto, 
		levelindeterminatecode, 
		dataversion, 
		maxdataversion,
		confidencecode,
		value, 
		valuestoretime
	)
	VALUES (
		dataproviderid_, 
		placeid_, 
		referencetime_,
		validfrom_,
		validto_,
		validic_,
 		valueparameter_, 
		levelparameter_,
		levelfrom_,
		levelto_,
		levelic_,
		dataversion_,
		dataversion_,
		confidencecode_,
		dataId,
		'now'
	);

END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql';



--
-- This rule permits INSERT statements on __WCI_SCHEMA__.gridvalue
-- The insert is then redirected to the appropriate write function
--
CREATE OR REPLACE RULE 
wci_internal_gridvalue_insert
AS ON INSERT TO __WCI_SCHEMA__.gridvalue_v
DO INSTEAD 
SELECT
__WCI_SCHEMA__.write(
	NEW.dataproviderid, 
	NEW.placeid, 
	NEW.referencetime, 
	NEW.validtimefrom, 
	NEW.validtimeto,
	NEW.validtimeindeterminatecode,
	NEW.valueparameterid, 
	NEW.levelparameterid, 
	NEW.levelfrom, 
	NEW.levelto,
	NEW.levelindeterminatecode,
	NEW.dataversion,
	NEW.confidencecode,
	NEW.value );


	
--
-- __WCI_SCHEMA__.write
-- This is the main work function of the wci.write call (for FLOAT)
-- 
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.write(
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
	confidencecode_ integer,
	value_ 			float
)
RETURNS void AS
$BODY$
DECLARE
	groupid_ 	bigint;
	max_		int;
BEGIN
	-- Todo: Set Time by timeIndeterminateCode
	-- Todo: Set Levels by levelIndeterminateCode
	-- Insert value group row
	BEGIN
		INSERT INTO __WDB_SCHEMA__.floatvaluegroup (
    		dataproviderid,
    		placeid,
    		validtimefrom,
    		validtimeto,
    		validtimeindeterminatecode,
			valueparameterid,
			levelparameterid,
			levelfrom, 
			levelto, 
			levelindeterminatecode, 
			dataversion 
		)
		VALUES (
			dataproviderid_, 
			placeid_, 
			(validfrom_ - referencetime_),
			(validto_ - referencetime_),
			validic_,
	 		valueparameter_, 
			levelparameter_,
			levelfrom_,
			levelto_,
			levelic_,
			dataversion_
		)
		RETURNING valuegroupid INTO groupid_;
	EXCEPTION
		WHEN unique_violation THEN
			-- Do not abort on unique violations
			SELECT valuegroupid INTO groupid_
			FROM __WDB_SCHEMA__.floatvaluegroup
			WHERE dataproviderid = dataproviderid_
			AND placeid = placeid_
			AND validtimefrom = (validfrom_ - referencetime_)
			AND validtimeto = (validto_ - referencetime_)
			AND validtimeindeterminatecode = validic_
	 		AND valueparameterid = valueparameter_ 
			AND levelparameterid = levelparameter_
			AND levelfrom = levelfrom_
			AND levelto = levelto_
			AND levelindeterminatecode = levelic_
			AND dataversion = dataversion_;
	END;
	-- Insert value item row
	INSERT INTO __WDB_SCHEMA__.floatvalueitem (
    	valuegroupid,
    	referencetime,
    	maxdataversion,
    	confidencecode,
    	value,
    	valuestoretime
	)
	VALUES (
		groupid_, 
		referencetime_,
		dataversion_,
		confidencecode_,
		value_,
		'now'
	);
	-- Get Max Data Version
	IF (dataversion_ > 0) THEN
		SELECT max(dataversion) INTO max_
		FROM wci_int.floatvalue_v
		WHERE dataproviderid = dataproviderid_
		AND placeid = placeid_
		AND referencetime = referencetime_	
		AND validtimefrom = validfrom_
		AND validtimeto = validto_
		AND validtimeindeterminatecode = validic_
		AND valueparameterid = valueparameter_ 
		AND levelparameterid = levelparameter_
		AND levelfrom = levelfrom_
		AND levelto = levelto_
		AND levelindeterminatecode = levelic_;
		-- Set Max Data Version
		UPDATE 	__WDB_SCHEMA__.floatvalueitem 
		SET 	maxdataversion = max_
		WHERE   referencetime = referencetime_	
		AND		valuegroupid IN ( SELECT valuegroupid
								  FROM	wdb_int.floatvaluegroup vg		
								  WHERE	vg.dataproviderid = dataproviderid_
								  AND 	vg.placeid = placeid_
								  AND 	vg.validtimefrom = (validfrom_ - referencetime_)
								  AND 	vg.validtimeto = (validto_ - referencetime_)
								  AND 	vg.validtimeindeterminatecode = validic_
								  AND 	vg.valueparameterid = valueparameter_ 
								  AND 	vg.levelparameterid = levelparameter_
								  AND 	vg.levelfrom = levelfrom_
								  AND 	vg.levelto = levelto_
								  AND 	vg.levelindeterminatecode = levelic_ );
	END IF;	
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql';


--
-- This rule permits INSERT statements on __WCI_SCHEMA__.floatvalue
-- The insert is then redirected to the appropriate write function
--
CREATE OR REPLACE RULE 
wci_internal_floatvalue_insert
AS ON INSERT TO __WCI_SCHEMA__.floatvalue_v
DO INSTEAD 
SELECT
__WCI_SCHEMA__.write(
	NEW.dataproviderid, 
	NEW.placeid, 
	NEW.referencetime, 
	NEW.validtimefrom, 
	NEW.validtimeto,
	NEW.validtimeindeterminatecode,
	NEW.valueparameterid, 
	NEW.levelparameterid, 
	NEW.levelfrom, 
	NEW.levelto,
	NEW.levelindeterminatecode,
	NEW.dataversion,
	NEW.confidencecode,
	NEW.value );


--
-- Get the data provider id associated with the current user, if it is 
-- registered as a wci writer. 
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.wcidataproviderid()
RETURNS bigint AS
$BODY$
DECLARE 
	myDataProviderId bigint;
BEGIN
	-- Get data provider
	SELECT 
		dataproviderid INTO myDataProviderId 
	FROM 
		__WCI_SCHEMA__.dataprovider_mv 
	WHERE 
		dataprovidername=(SELECT user) AND
		dataprovidertype = 'wci user';
	IF myDataProviderId IS NULL THEN
		RAISE EXCEPTION 'Cannot recognize % as a valid wci writer - will not proceed', (SELECT user);
	END IF;

	RETURN  myDataProviderId;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



-- 
-- Get dataproviderids from the given dataprovidername 
--
-- @param providerName The name of the data provider
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.idFromDataProvider
(
          providerName    character varying(80)
)
RETURNS bigint AS
$BODY$
DECLARE 
	myDataProviderId bigint;
BEGIN
	-- Get data provider
	SELECT 
		dataproviderid INTO myDataProviderId 
	FROM 
		__WCI_SCHEMA__.dataprovider_mv dp, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE 
		dp.dataprovidername = providerName AND
		dp.dataprovidernamespaceid = s.dataprovidernamespaceid;		

	IF myDataProviderId IS NULL THEN
		RAISE EXCEPTION 'Cannot recognize % as a valid data provider - will not proceed', providerName;
	END IF;

	RETURN myDataProviderId;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;
