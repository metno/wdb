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
	FROM __WCI_SCHEMA__.regulargrid
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
	-- Get SRID
	
	-- Get PlaceId
	
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
			 placeName_,
			 -infinity,
			 +infinity );			 
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;
