-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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
-- add New Data Provider
-- 
CREATE OR REPLACE FUNCTION
wci.addDataProvider
(
	dataProviderName_		text,
	dataProviderType_ 		text,
	domainDelivery_			text,
	dataProviderComment_	text
)
RETURNS bigint AS
$BODY$
	SELECT __WCI_SCHEMA__.addDataProvider( $1, $2, $3, $4 ); 
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


--
-- add new data provider name in namespace
-- 
CREATE OR REPLACE FUNCTION
wci.addDataProviderName
(
	canonicalName_		text,
	dataProviderName_ 	text
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	dataProviderId_ int;
BEGIN
	-- Get namespace
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	IF ( namespace_ = 0 ) THEN
		RETURN;
	END IF;	
	-- Get parameterid
	SELECT dataproviderid INTO dataProviderId_
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = lower(canonicalName_) AND
		  dataprovidernamespaceid = 0;
	-- Delete old name if any exist
	DELETE FROM __WDB_SCHEMA__.dataprovidername
	WHERE dataprovidernamespaceid = namespace_ AND
		  dataproviderid = dataProviderId_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.dataprovidername
	VALUES ( dataProviderId_,
			 namespace_,
			 lower(dataProviderName_),
			  'today'::TIMESTAMP WITH TIME ZONE, 
			  'infinity'::TIMESTAMP WITH TIME ZONE, 
			  dataproviderid_, 
			  dataproviderid_ );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;



CREATE OR REPLACE FUNCTION
wci.addwciuser
(
	newDataProviderName text
)
RETURNS bigint AS
$BODY$
DECLARE
	namespace_	int;
	id 			bigint;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for existing name
	SELECT dataproviderid INTO id 
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = newDataProviderName AND
		dataprovidertype = 'WCI User';
	IF NOT FOUND THEN
		id := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);
		namespace_ = 0;
		-- Insert into tables
		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		(id, 'WCI User', 'Any', '1 day', 'now');
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		(id, 'wci user', 'now');
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
		(id, namespace_, newDataProviderName, 'today', '2999-12-31', id, id);
	ELSE
		RAISE INFO 'User was already defined as a dataprovider';
	END IF;

	RETURN id;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePoint
(
	placeName_ 		text,
	placeGeometry_ 	geometry
)
RETURNS bigint AS
$BODY$
	SELECT __WCI_SCHEMA__.addPlacePoint( $1, $2 ); 
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlaceRegularGrid(
	placeName_ 	text,
	numX_ 		int,
	numY_ 		int,
	incX_ 		float,
	incY_ 		float,
	startX_ 	float,
	startY_ 	float,
	projection_ text
)
RETURNS bigint AS
$BODY$
	SELECT __WCI_SCHEMA__.addPlaceRegularGrid( $1, $2, $3, $4, $5, $6, $7, $8 );
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;

--
-- Add SRID
--
CREATE OR REPLACE FUNCTION
wci.addSrid(
	name_		text,
	projection_	text
)
RETURNS int AS
$BODY$
DECLARE
	srid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get SRID
	SELECT max(srid) + 1 INTO srid_
	FROM spatial_ref_sys;	
	-- Insert Data
	INSERT INTO spatial_ref_sys
	VALUES ( srid_,
			 name_,
			 NULL,
			 NULL,
			 projection_ );			 
	-- The two nulls represent the EPSG codes, which we do not attempt to 
	-- derive at this point.
	-- Return	 
	RETURN srid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;

--
-- add new data provider name in namespace
-- 
CREATE OR REPLACE FUNCTION
wci.addPlaceName
(
	canonicalName_	text,
	placeName_ 		text
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	placeId_ 		int;
BEGIN
	-- Get namespace
	SELECT placenamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	IF ( namespace_ = 0 ) THEN
		RAISE EXCEPTION 'Cannot set the WDB Canonical PlaceName';
	END IF;	
	-- Get placeid
	SELECT placeid INTO placeId_
	FROM __WCI_SCHEMA__.placedefinition_mv
	WHERE placename = lower(canonicalName_) AND
		  placenamespaceid = 0;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify any place definition with the specified canonical PlaceName';		
	END IF;
	-- Delete old name if any exist
	DELETE FROM __WDB_SCHEMA__.placename
	WHERE placenamespaceid = namespace_ AND
		  placeid = placeId_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.placename
	VALUES ( placeId_,
			 namespace_,
			 lower(placeName_),
			  'today'::TIMESTAMP WITH TIME ZONE, 
			  'infinity'::TIMESTAMP WITH TIME ZONE );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;



--
-- add Value Parameter
--
CREATE OR REPLACE FUNCTION
wci.addValueParameter
(
	parameterType_					text,
	parameterUsageOrName_ 			text,
	parameterUnitOrReference_		text,
	parameterFunctionOrDescription_	text,
	parameterQuantity_				text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	RAISE INFO 'Start AddValueParam';
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get ID
	SELECT nextval('__WDB_SCHEMA__.valueparameter_valueparameterid_seq'::regclass) INTO parameterid_;
	RAISE DEBUG 'ParameterID is: %', parameterid_;
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.valueparameter
	VALUES ( parameterid_, parameterType_ );
	-- Insert Value
	IF ( parameterType_ = 'Measure Parameter'::text) THEN
		-- Base Table
		INSERT INTO __WDB_SCHEMA__.valuemeasureparameter
		VALUES ( parameterid_,
				 parameterUsageOrName_,
				 parameterUnitOrReference_,
				 parameterQuantity_ );
		RETURN parameterid_;
	ELSIF ( parameterType_ = 'Function Parameter'::text) THEN
		-- Function Table
		INSERT INTO __WDB_SCHEMA__.valuefunctionparameter
		VALUES ( parameterid_,
				 parameterFunctionOrReference_,
				 parameterUsageOrName_,
				 parameterUnitOrReference_,
				 parameterQuantity_ );
		RETURN parameterid_;
	ELSIF ( parameterType_ = 'Code Parameter'::text) THEN
		-- Code Table
		INSERT INTO __WDB_SCHEMA__.valuecodeparameter
		VALUES ( parameterid_,
				 parameterUsageOrName_,
				 parameterUnitOrReference_ );
		RETURN parameterid_;
	ELSIF ( parameterType_ = 'Dimensionless Parameter'::text) THEN
		-- Dimensionless Table
		INSERT INTO __WDB_SCHEMA__.valuedimensionlessparameter
		VALUES ( parameterid_,
				 parameterUsageOrName_,
				 parameterFunctionOrDescription_ );
		RETURN parameterid_;
	ELSE
		RAISE EXCEPTION 'Undefined parameter type: %', parameterType_;
	END IF;
	RETURN -1;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


--
-- add Value Parameter
--
CREATE OR REPLACE FUNCTION
wci.addValueParameterName
(
	canonicalName_					text,
	valueParameterName_ 			text
)
RETURNS int AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
BEGIN
	-- Get namespace
	SELECT parameternamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Get parameterid
	SELECT valueparameterid INTO parameterid_
	FROM __WCI_SCHEMA__.valueparameter
	WHERE valueparametername = lower(canonicalName_) AND
		  parameternamespaceid = namespace_;
	-- Delete old name if it exists
	DELETE FROM __WDB_SCHEMA__.valueparametername
	WHERE parameternamespaceid = namespace_ AND
		  valueparameterid = parameterid_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.valueparametername
	VALUES ( parameterid_,
			 namespace_,
			 valueParameterName_ );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


--
-- add Level Parameter
--
CREATE OR REPLACE FUNCTION
wci.addLevelParameter
(
	parameterType_					text,
	parameterUsageOrName_ 			text,
	parameterUnitOrReference_		text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get value parameter Id 
	parameterId_ := nextval('__WDB_SCHEMA__.levelparameter_levelparameterid_seq');
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.levelparameter
	VALUES ( parameterid_, parameterType_ );
	-- Insert Value
	IF ( parameterType_ = 'Measure Parameter'::text) THEN
		-- Base Table
		INSERT INTO __WDB_SCHEMA__.levelmeasureparameter
		VALUES ( parameterid_,
				 parameterUnitOrReference_,
				 lower(parameterUsageOrName_) );
	ELSIF ( parameterType_ = 'Code Parameter'::text) THEN
		-- Code Table
		INSERT INTO __WDB_SCHEMA__.levelcodeparameter
		VALUES ( parameterid_,
				 lower(parameterUsageOrName_),
				 parameterUnitOrReference_ );
	END IF;
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


--
-- add Value Parameter
--
CREATE OR REPLACE FUNCTION
wci.addLevelParameterName
(
	canonicalName_					text,
	levelParameterName_ 			text
)
RETURNS int AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
BEGIN
	-- Get namespace
	SELECT parameternamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Get parameterid
	SELECT levelparameterid INTO parameterid_
	FROM __WCI_SCHEMA__.levelparameter
	WHERE levelparametername = lower(canonicalName_) AND
		  parameternamespaceid = namespace_;
	-- Delete old name if it exists
	DELETE FROM __WDB_SCHEMA__.levelparametername
	WHERE parameternamespaceid = namespace_ AND
		  levelparameterid = parameterid_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.levelparametername
	VALUES ( parameterid_,
			 namespace_,
			 levelParameterName_ );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


--
-- Measure
--
CREATE OR REPLACE FUNCTION
wci.addMeasure(
    measure						varchar(80),
    distancepower				int,
    masspower					int,
    timepower					int,
    luminositypower				int,
    electriccurrentpower		int,
    temperaturepower			int,
    substanceamountpower		int,
	anglepower					int
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.measure
	VALUES ( $1, $2, $3, $4, $5, $6, $7, $8, $9 );
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


--
-- Unit
--
CREATE OR REPLACE FUNCTION
wci.addUnit(
    unitname_						varchar(80),
    unittype_						varchar(80),
    measure_						varchar(80),
	description_					varchar(255),
    siunitconversioncoefficient_	float,
    siunitconversionterm_			float
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
BEGIN
	-- Insert into units
	INSERT INTO __WDB_SCHEMA__.unit 
	VALUES ( unitname_, unittype_, measure_, description_ );
	-- If not SI unit
	IF ( unittype_::text <> 'SI Unit'::text ) THEN
		INSERT INTO __WDB_SCHEMA__.siunitconversion
		VALUES( unitname_, siunitconversioncoefficient_, siunitconversionterm_ );	
	END IF;	
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;


CREATE OR REPLACE FUNCTION
wci.addParameterFunctionType(
    parameterfunctiontype 			varchar(80),
    parameterfunctiondescription 	varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.parameterfunctiontype
	VALUES ( $1, $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


CREATE OR REPLACE FUNCTION
wci.addValueParameterUsage(
    valueparameterusage 	varchar(80),
    description 			varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.valueparameterusage
	VALUES ( $1, $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


CREATE OR REPLACE FUNCTION
wci.addLevelParameterUsage(
    levelparameterusage 	varchar(80),
    description 			varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.levelparameterusage
	VALUES ( $1, $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;
