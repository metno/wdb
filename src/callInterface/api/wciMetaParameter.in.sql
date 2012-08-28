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

-- Get CF-Metadata Surface Parameter
CREATE OR REPLACE FUNCTION 
wci.getcfsurface( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfsurface_v AS
$BODY$
	SELECT
		cfsurface,
		cfsurfacecomment
	FROM 
		__WCI_SCHEMA__.cfsurface_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfsurface LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for Surface  
CREATE OR REPLACE FUNCTION
wci.addcfSurface
(
	surface_ 			text,
	comment_			text
)
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM *
	FROM __WCI_SCHEMA__.cfsurface_v
	WHERE cfsurface = surface_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfsurface
		VALUES ( surface_, comment_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfsurface
		SET cfsurface=surface_, cfsurfacecomment=comment_
		WHERE cfsurface = surface_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get CF-Metadata component Parameter
CREATE OR REPLACE FUNCTION 
wci.getcfcomponent( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfcomponent_v AS
$BODY$
	SELECT
		cfcomponent,
		cfcomponentcomment
	FROM 
		__WCI_SCHEMA__.cfcomponent_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfcomponent LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for component  
CREATE OR REPLACE FUNCTION
wci.addcfcomponent
(
	component_ 			text,
	comment_			text
)
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM * 
	FROM __WCI_SCHEMA__.cfcomponent_v
	WHERE cfcomponent = component_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfcomponent
		VALUES ( component_, comment_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfcomponent
		SET cfcomponent=component_, cfcomponentcomment=comment_
		WHERE cfcomponent = component_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get CF-Metadata medium Parameter
CREATE OR REPLACE FUNCTION 
wci.getcfmedium( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfmedium_v AS
$BODY$
	SELECT
		cfmedium,
		cfmediumcomment
	FROM 
		__WCI_SCHEMA__.cfmedium_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfmedium LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for medium  
CREATE OR REPLACE FUNCTION
wci.addCfmedium
(
	medium_ 			text,	
	comment_			text
)
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM * 
	FROM __WCI_SCHEMA__.cfmedium_v
	WHERE cfmedium = medium_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfmedium
		VALUES ( medium_, comment_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfmedium
		SET cfmedium=medium_, cfmediumcomment=comment_
		WHERE cfmedium = medium_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get CF-Metadata process Parameter
CREATE OR REPLACE FUNCTION 
wci.getcfprocess( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfprocess_v AS
$BODY$
	SELECT
		cfprocess,
		cfprocesscomment
	FROM 
		__WCI_SCHEMA__.cfprocess_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfprocess LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for process  
CREATE OR REPLACE FUNCTION
wci.addcfprocess
(
	process_ 			text,
	comment_			text
)
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM * 
	FROM __WCI_SCHEMA__.cfprocess_v
	WHERE cfprocess = process_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfprocess
		VALUES ( process_, comment_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfprocess
		SET cfprocess=process_, cfprocesscomment=comment_
		WHERE cfprocess = process_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get CF-Metadata condition Parameter
CREATE OR REPLACE FUNCTION 
wci.getcfcondition( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfcondition_v AS
$BODY$
	SELECT
		cfcondition,
		cfconditioncomment
	FROM 
		__WCI_SCHEMA__.cfcondition_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfcondition LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for condition  
CREATE OR REPLACE FUNCTION
wci.addcfcondition
(
	condition_ 			text,
	comment_			text
)
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ int;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM * 
	FROM __WCI_SCHEMA__.cfcondition_v
	WHERE cfcondition = condition_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfcondition
		VALUES ( condition_, comment_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfcondition
		SET cfcondition=condition_, cfconditioncomment=comment_
		WHERE cfcondition = condition_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Get CF-Metadata Parameter Function
CREATE OR REPLACE FUNCTION 
wci.getcfmethods( searchterm 		text )	
RETURNS SETOF __WCI_SCHEMA__.cfmethods_v AS
$BODY$
	SELECT
		cfmethods,
		cfmethodscomment,
		cfmethodsname
	FROM 
		__WCI_SCHEMA__.cfmethods_v v,
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		($1 IS NULL OR cfmethods LIKE lower($1));
$BODY$
LANGUAGE sql STABLE;

-- Add CF-metadata for Surface  
CREATE OR REPLACE FUNCTION
wci.addcfmethods
(
	cfmethods_ 			text,
	comment_ 			text,
	cfmethodsname_		text
)
RETURNS void AS
$BODY$
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for Duplicate
	PERFORM * 
	FROM __WCI_SCHEMA__.cfmethods_v
	WHERE cfmethods = cfmethods_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.cfmethods
		VALUES ( cfmethods_, comment_, cfmethodsname_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.cfmethods
		SET cfmethods=cfmethods_, cfmethodscomment = comment_, cfmethodsname=cfmethodsname_
		WHERE cfmethods = cfmethods_;
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION
wci.getParameter( parameter 		text )	
RETURNS SETOF __WCI_SCHEMA__.parameter_v AS
$BODY$
	SELECT 
		parameterid,
		cfstandardname,
 		cfsurface,
    	cfcomponent,
    	cfmedium,
    	cfprocess,
    	cfcondition,
    	cfmethods,
    	unitname,
    	v.parameternamespaceid,
    	parametername
	FROM 
		__WCI_SCHEMA__.parameter_mv v, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE v.parameternamespaceid = s.parameternamespaceid 
	  AND ( $1 IS NULL OR v.parametername LIKE lower($1) );
$BODY$
LANGUAGE sql STABLE;


CREATE OR REPLACE FUNCTION
wci.addparameter( cfstandardname_		text,
				  cfsurface_ 			text,
				  cfcomponent_			text,
				  cfmedium_				text,
				  cfprocess_			text,
				  cfcondition_			text,
				  cfmethods_			text,
				  unitname_				text )	
RETURNS void AS
$BODY$
DECLARE	
	parameterid_ 	int;
	namespace_   	int;
	canonicname_    text;
	parametername_  text;
BEGIN
	-- Get namespace
	SELECT parameternamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- No cfstandardname
	IF cfstandardname_ IS NULL THEN
		RAISE EXCEPTION 'The CF standard name must be non-null';
	END IF;		
	-- Check Dimensions
	IF ( cfsurface_ IS NOT NULL ) THEN
		PERFORM * 
		FROM wci.getcfsurface( cfsurface_ );
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Could not identify the CF surface identified %', cfsurface_;
		END IF;		
	END IF;
	-- NAME
	SELECT getcanonicalparametername INTO canonicname_
	FROM __WCI_SCHEMA__.getCanonicalParameterName( cfstandardname_,
												   cfsurface_,
												   cfcomponent_,
												   cfmedium_,
												   cfprocess_,
												   cfcondition_,
												   cfmethods_ );
	SELECT getparametername INTO parametername_
	FROM __WCI_SCHEMA__.getParameterName( cfstandardname_,
										  cfsurface_,
										  cfcomponent_,
										  cfmedium_,
										  cfprocess_,
										  cfcondition_,
										  cfmethods_ );
	-- Check for Duplicate
	SELECT parameterid INTO parameterid_
	FROM   __WCI_SCHEMA__.parameter_v
	WHERE  parametername::text = canonicname_;
	-- Insert
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.parameter
			   ( cfstandardname,
			   	 cfsurface,
			   	 cfcomponent,
			   	 cfmedium,
			   	 cfprocess,
			   	 cfcondition,
			   	 cfmethods,
			   	 unitname )
		VALUES ( cfstandardname_,
				 cfsurface_,
				 cfcomponent_,
				 cfmedium_,
				 cfprocess_,
				 cfcondition_,
				 cfmethods_,
				 unitname_ );
	ELSE
	-- Update
		UPDATE __WDB_SCHEMA__.parameter
		SET  cfstandardname = cfstandardname_,
			 cfsurface = cfsurface_,
			 cfcomponent = cfcomponent_,
			 cfmedium = cfmedium_,
			 cfprocess = cfprocess_,
			 cfcondition = cfcondition_,
			 cfmethods = cfmethods_,
			 unitname = unitname_ 
		WHERE parameterid = parameterid_;
	END IF;
	-- Namespace
	IF ( namespace_ <> 0 ) THEN
		PERFORM wci.setParameterName( canonicname_, parametername_ );
	END IF;
	RETURN;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


--
-- set Value Parameter Name
--
CREATE OR REPLACE FUNCTION
wci.setParameterName
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
	SELECT parameterid INTO parameterid_
	FROM __WCI_SCHEMA__.parameter_mv
	WHERE parametername = lower(canonicalName_) AND
		  parameternamespaceid = 0;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify any parameter with the specified canonical parametername';		
	END IF;
	-- Delete old name if it exists
	DELETE FROM __WDB_SCHEMA__.parametername
	WHERE parameternamespaceid = namespace_ AND
		  parameterid = parameterid_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.parametername
	VALUES ( parameterid_,
			 namespace_,
			 lower(btrim(valueParameterName_)) );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


-- Add Unit
CREATE OR REPLACE FUNCTION
wci.addUnit(
    unitname_							varchar(80),
	description_						varchar(255),
    baseunitname_						varchar(80),
    baseunitconversioncoefficient_		float,
    baseunitconversionterm_				float
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	parameterid_ 	int;
	unittype_ 		varchar(80);
BEGIN
	-- Unit
	IF ( unitname_ = baseunitname_ ) THEN
		unittype_ := 'base unit';
	ELSE
		unittype_ := 'derived unit';	
	END IF;	
	PERFORM *
	FROM __WDB_SCHEMA__.unit
	WHERE unitname = unitname_;
	IF NOT FOUND THEN
		-- Insert into units
		INSERT INTO __WDB_SCHEMA__.unit 
		VALUES ( unitname_, unittype_, description_ );
		-- If not a base unit
		IF ( unittype_::text = 'derived unit'::text ) THEN
			INSERT INTO __WDB_SCHEMA__.baseunitconversion
			VALUES( unitname_, baseunitname_, baseunitconversioncoefficient_, baseunitconversionterm_ );	
		END IF;
	ELSE
		UPDATE __WDB_SCHEMA__.unit SET
			unittype = unittype_,
			description = description_;
		IF ( unittype_::text = 'derived unit'::text ) THEN
			PERFORM * 
			FROM __WDB_SCHEMA__.baseunitconversion;
			IF NOT FOUND THEN
				INSERT INTO __WDB_SCHEMA__.baseunitconversion
				VALUES( unitname_, baseunitname_, baseunitconversioncoefficient_, baseunitconversionterm_ );	
			ELSE
				UPDATE __WDB_SCHEMA__.baseunitconversion SET
					baseunitname = baseunitname_,
					baseunitconversioncoefficient = baseunitconversioncoefficient_,
					baseunitconversionterm = baseunitconversionterm_;
			END IF;
		END IF;			
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
RETURNS SETOF __WCI_SCHEMA__.unitwithconversion_v AS
$BODY$
	SELECT *
	FROM __WCI_SCHEMA__.unitwithconversion_v
	WHERE unitname LIKE $1 OR $1 IS NULL; 
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;
