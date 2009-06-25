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

CREATE OR REPLACE FUNCTION
wci.getPlaceName(
	numX_ int,
	numY_ int,
	incX_ float,
	incY_ float,
	startX_ float,
	startY_ float,
	projection_ text
)
RETURNS text AS
$BODY$
DECLARE
	ret text;
BEGIN
	SELECT placename INTO ret
	FROM __WCI_SCHEMA__.regulargrid, __WCI_SCHEMA__.getSessionData()
	WHERE inumber = numX_
	AND   jnumber = numY_
	AND   round(iIncrement::numeric, 3) = round(incX_::numeric, 3)
	AND   round(jIncrement::numeric, 3) = round(incY_::numeric, 3)
	AND	  round(startLongitude::numeric, 3) = round(startX_::numeric, 3)
	AND   round(startLatitude::numeric, 3) = round(startY_::numeric, 3)
	AND   projdefinition = projection_;
	RETURN ret;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;



CREATE OR REPLACE FUNCTION
wci.addPlaceDefinition(
	placeName_ text,
	numX_ int,
	numY_ int,
	incX_ float,
	incY_ float,
	startX_ float,
	startY_ float,
	projection_ text
)
RETURNS void AS
$BODY$
DECLARE
	namespace_ int;
	placeId_ bigint;
	srid_ int;
BEGIN
	-- Get Namespace
	SELECT placenamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();	
	-- Get SRID
	SELECT srid INTO srid_
	FROM spatial_ref_sys
	WHERE proj4text = projection_;
	-- If SRID not found...
	IF srid_ IS NULL THEN
		RAISE EXCEPTION 'Could not identify the PROJ.4 projection in the database. Check that the projection is valid and, if needed, insert it into the database using wci.addSrid( ... )';		
	END IF;
	-- Get PlaceId
	SELECT max(placeid) + 1 INTO placeid_
	FROM __WCI_SCHEMA__.placedefinition;	
	-- Insert Data
	INSERT INTO __WDB_SCHEMA__.placeregulargrid
	VALUES ( placeId_,
			 numX_,
			 numY_,
			 incX_,
			 incY_,
			 startX_,
			 startY_,
			 srid_ );
	-- Insert new placename
	INSERT INTO __WDB_SCHEMA__.placename
	VALUES ( placeId_,
			 nameSpace_,
			 lower(placeName_),
			 '-infinity'::timestamp,
			 'infinity'::timestamp );			 
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;




--
-- Add SRID
--
CREATE OR REPLACE FUNCTION
wci.addSrid(
	name_ text,
	projection_ text
)
RETURNS void AS
$BODY$
DECLARE
	srid_ int;
BEGIN
	-- WCI User Check
	SELECT __WCI_SCHEMA__.getSessionData();
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
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;

--
-- Get SRID
--
CREATE OR REPLACE FUNCTION 
wci.getSrid(
	srid_	text
)
RETURNS integer AS
$BODY$
	SELECT 	srid 
	FROM 	public.spatial_ref_sys
	WHERE 	proj4text = $1
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';
