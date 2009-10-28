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
-- Add Measure Value Parameter
-- Measure Value Parameters are parameters based on measures; 
-- usually physical phenomena such as temperature, pressure, etc.
CREATE OR REPLACE FUNCTION
wci.addMeasureValueParameter
(
	parameterUsage_ 		text,
	parameterUnit_			text,
	parameterQuantity_		text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get value parameter Id 
	parameterId_ := nextval('__WDB_SCHEMA__.valueparameter_valueparameterid_seq');
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.valueparameter
	VALUES ( parameterid_, 'Measure Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.valuemeasureparameter
	VALUES ( parameterid_,
			 parameterUsage_,
			 parameterUnit_,
			 parameterQuantity_ );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Add Function Value Parameter
-- Function Value Parameters are measure parameters that are modified
-- by some function; i.e., aggregated or accumulated values.
CREATE OR REPLACE FUNCTION
wci.addFunctionValueParameter
(
	parameterUsage_ 		text,
	parameterUnit_			text,
	parameterFunction_		text,
	parameterQuantity_		text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get value parameter Id 
	parameterId_ := nextval('__WDB_SCHEMA__.valueparameter_valueparameterid_seq');
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.valueparameter
	VALUES ( parameterid_, 'Function Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.valuefunctionparameter
	VALUES ( parameterid_,
			 parameterFunction_,
			 parameterUsage_,
			 parameterUnit_,
			 parameterQuantity_ );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Add Code Value Parameter
-- Code Value Parameters are parameters that are set according to
-- some table of codes.
CREATE OR REPLACE FUNCTION
wci.addCodeValueParameter
(
	parameterName_ 			text,
	parameterReference_		text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get value parameter Id 
	parameterId_ := nextval('__WDB_SCHEMA__.valueparameter_valueparameterid_seq');
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.valueparameter
	VALUES ( parameterid_, 'Code Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.valuecodeparameter
	VALUES ( parameterid_,
			 parameterName_,
			 parameterReference_ );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Add Dimensionless Value Parameter
-- Dimensionless Value Parameters are parameters that do not have a
-- dimension in terms of the measures described in the measure table.
-- Examples of queries are most ratios.
CREATE OR REPLACE FUNCTION
wci.addDimensionlessValueParameter
(
	parameterName_ 			text,
	parameterDescription_	text
)
RETURNS int AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get value parameter Id 
	parameterId_ := nextval('__WDB_SCHEMA__.valueparameter_valueparameterid_seq');
	-- Insert Base
	INSERT INTO __WDB_SCHEMA__.valueparameter
	VALUES ( parameterid_, 'Dimensionless Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.valuedimensionlessparameter
	VALUES ( parameterid_,
			 parameterName_,
			 parameterDescription_ );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get Value Parameter (All Types)
CREATE OR REPLACE FUNCTION
wci.getValueParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.valueparameter AS
$BODY$
	SELECT 
		v.valueparameterid,
		v.parameternamespaceid,
		v.valueparametername, 
		v.valueunitname
	FROM 
		__WCI_SCHEMA__.valueparameter_mv v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR valueparametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


-- Get Measure Value Parameter
CREATE OR REPLACE FUNCTION
wci.getMeasureValueParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.measurevalueparameter AS
$BODY$
	SELECT
		v.valueparameterid,
		v.valueparameterusage,
		v.parameterquantitytype,
		v.measure, 
		v.valueunitname,
		v.parameternamespaceid,
		v.valueparametername
	FROM 
		__WCI_SCHEMA__.measurevalueparameter v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR valueparametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


-- Get Function Value Parameter
CREATE OR REPLACE FUNCTION
wci.getFunctionValueParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.functionvalueparameter AS
$BODY$
	SELECT
		v.valueparameterid,
		v.parameterfunctiontype,
		v.valueparameterusage,
		v.parameterquantitytype,
		v.measure, 
		v.valueunitname,
		v.parameternamespaceid,
		v.valueparametername
	FROM 
		__WCI_SCHEMA__.functionvalueparameter v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR valueparametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


-- Get Code Parameters
CREATE OR REPLACE FUNCTION
wci.getCodeValueParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.codevalueparameter AS
$BODY$
	SELECT
		v.valueparameterid,
		v.valuecodeparametername,
		v.codeparameterreference,
		v.parameternamespaceid,
		v.valueparametername
	FROM 
		__WCI_SCHEMA__.codevalueparameter v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR valueparametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


-- Get Dimensionless Parameter
CREATE OR REPLACE FUNCTION
wci.getDimensionlessValueParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.dimensionlessvalueparameter AS
$BODY$
	SELECT
		v.valueparameterid,
		v.valuedimensionlessparametername,
		v.valueparameterdescription,
		v.parameternamespaceid,
		v.valueparametername,
		v.valueunitname
	FROM 
		__WCI_SCHEMA__.dimensionlessvalueparameter v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR valueparametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


--
-- set Value Parameter Name
--
CREATE OR REPLACE FUNCTION
wci.setValueParameterName
(
	canonicalName_					text,
	valueParameterName_ 			text
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
BEGIN
	-- Get namespace
	SELECT parameternamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	IF ( namespace_ = 0 ) THEN
		RAISE EXCEPTION 'Cannot set the WDB Canonical ValueParameterName';
	END IF;	
	-- Get parameterid
	SELECT valueparameterid INTO parameterid_
	FROM __WCI_SCHEMA__.valueparameter_mv
	WHERE valueparametername = lower(canonicalName_) AND
		  parameternamespaceid = 0;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify any value parameter with the specified canonical ValueParameterName';		
	END IF;
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
LANGUAGE plpgsql VOLATILE;


-- Add Measure Level Parameter
CREATE OR REPLACE FUNCTION
wci.addMeasureLevelParameter
(
	parameterUsage_ 	text,
	parameterUnit_		text
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
	VALUES ( parameterid_, 'Measure Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.levelmeasureparameter
	VALUES ( parameterid_,
			 parameterUnit_,
			 lower(parameterUsage_ ) );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- add Code Level Parameter
CREATE OR REPLACE FUNCTION
wci.addCodeLevelParameter
(
	parameterName_ 			text,
	parameterReference_		text
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
	VALUES ( parameterid_, 'Code Parameter' );
	-- Insert Value
	INSERT INTO __WDB_SCHEMA__.levelcodeparameter
	VALUES ( parameterid_,
			 lower(parameterName_),
			 parameterReference_ );
	RETURN parameterid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get Level Parameter
CREATE OR REPLACE FUNCTION 
wci.getlevelparameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.levelparameter AS
$BODY$
	SELECT 
		l.levelparameterid,
		l.parameternamespaceid,
		l.levelparametername,
		l.levelunitname
	FROM 
		__WCI_SCHEMA__.levelparameter_mv l,
		__WCI_SCHEMA__.getSessionData() s
	WHERE 
		l.parameternamespaceid = s.parameternamespaceid AND
		($1 IS NULL OR levelparametername LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;


-- Get Measure Level Parameter
CREATE OR REPLACE FUNCTION 
wci.getmeasurelevelparameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.measurelevelparameter AS
$BODY$
	SELECT
		lp.levelparameterid,
		lp.levelparameterusage,
		lp.measure, 
		lp.levelunitname,
		lp.parameternamespaceid,
		lp.levelparametername
	FROM 
		__WCI_SCHEMA__.measurelevelparameter lp,
		__WCI_SCHEMA__.getSessionData() s
	WHERE 
		lp.parameternamespaceid = s.parameternamespaceid AND
		($1 IS NULL OR levelparametername LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;


-- Get Code Level Parameter
CREATE OR REPLACE FUNCTION 
wci.getcodelevelparameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.codelevelparameter AS
$BODY$
	SELECT
		lc.levelparameterid,
		lc.levelcodeparametername,
		lc.codeparameterreference,
		lc.parameternamespaceid,
		lc.levelparametername
	FROM 
		__WCI_SCHEMA__.codelevelparameter lc,
		__WCI_SCHEMA__.getSessionData() s
	WHERE 
		lc.parameternamespaceid = s.parameternamespaceid AND
		($1 IS NULL OR levelparametername LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;


--
-- add Value Parameter
--
CREATE OR REPLACE FUNCTION
wci.setLevelParameterName
(
	canonicalName_					text,
	levelParameterName_ 			text
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
BEGIN
	-- Get namespace
	SELECT parameternamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	IF ( namespace_ = 0 ) THEN
		RAISE EXCEPTION 'Cannot set the WDB Canonical LevelParameterName';
	END IF;	
	-- Get parameterid
	SELECT levelparameterid INTO parameterid_
	FROM __WCI_SCHEMA__.levelparameter_mv
	WHERE levelparametername = lower(canonicalName_) AND
		  parameternamespaceid = 0;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify any level parameter with the specified canonical LevelParameterName';		
	END IF;
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
LANGUAGE plpgsql VOLATILE;


-- Add Measure
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
LANGUAGE sql VOLATILE;



-- Get Measure
CREATE OR REPLACE FUNCTION 
wci.getMeasure(
	measure_	text
)
RETURNS SETOF __WCI_SCHEMA__.measure AS
$BODY$
	SELECT 	*
	FROM 	__WCI_SCHEMA__.measure
	WHERE 	measure LIKE $1 OR $1 IS NULL;
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


-- Add Unit
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
LANGUAGE plpgsql VOLATILE;


--- Get Unit
CREATE OR REPLACE FUNCTION 
wci.getUnit(
	unit		text
)
RETURNS SETOF __WCI_SCHEMA__.unitwithconversion AS
$BODY$
	SELECT *
	FROM __WCI_SCHEMA__.unitwithconversion
	WHERE unitname LIKE $1 OR $1 IS NULL; 
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


-- Add Function Type
CREATE OR REPLACE FUNCTION
wci.addParameterFunctionType(
    parameterfunctiontype 			varchar(80),
    parameterfunctiondescription 	varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.parameterfunctiontype
	VALUES ( lower($1), $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;


-- Get Function Type
CREATE OR REPLACE FUNCTION 
wci.getParameterFunctionType(
	type_	text
)
RETURNS SETOF __WCI_SCHEMA__.parameterfunctiontype AS
$BODY$
	SELECT 	*
	FROM 	__WCI_SCHEMA__.parameterfunctiontype
	WHERE 	parameterfunctiontype ILIKE $1 OR $1 IS NULL;
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


-- Add Value Usage
CREATE OR REPLACE FUNCTION
wci.addValueParameterUsage(
    valueparameterusage 	varchar(80),
    description 			varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.valueparameterusage
	VALUES ( lower($1), $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;


-- Get Value Usage
CREATE OR REPLACE FUNCTION 
wci.getValueParameterUsage(
	usage_	text
)
RETURNS SETOF __WCI_SCHEMA__.valueparameterusage AS
$BODY$
	SELECT 	*
	FROM 	__WCI_SCHEMA__.valueparameterusage
	WHERE 	valueparameterusage LIKE lower($1) OR $1 IS NULL;
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


-- Add Level Usage
CREATE OR REPLACE FUNCTION
wci.addLevelParameterUsage(
    levelparameterusage 	varchar(80),
    description 			varchar(255)
)
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.levelparameterusage
	VALUES ( lower($1), $2 );
$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;


-- Get Level Usage
CREATE OR REPLACE FUNCTION 
wci.getLevelParameterUsage(
	usage_	text
)
RETURNS SETOF __WCI_SCHEMA__.levelparameterusage AS
$BODY$
	SELECT 	*
	FROM 	__WCI_SCHEMA__.levelparameterusage
	WHERE 	levelparameterusage LIKE lower($1) OR $1 IS NULL;
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;
