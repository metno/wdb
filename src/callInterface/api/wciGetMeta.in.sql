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


-- This function is deprecated, but is maintained still for the sake 
-- of the wdb2ts functions. 
-- Get the full list of place specifications, for which there exists a regular 
-- grid.
--
-- @return All data on all registered regular grids  
CREATE OR REPLACE FUNCTION wci.placespecification()
  RETURNS SETOF __WCI_SCHEMA__.placespec AS
$BODY$
	SELECT 
		p.placeid,
		p.placename,
		p.placenamespaceid,
		p.placegeometry,
		p.placegeometrytype,
		p.placeindeterminatetype,
		p.numberx, 
		p.numbery, 
		p.incrementx, 
		p.incrementy, 
		p.startx, 
		p.starty, 
		p.originalsrid,
		p.projdefinition,
		p.placestoretime
	FROM __WCI_SCHEMA__.placespec p, __WCI_SCHEMA__.getSessionData() s
	WHERE p.placenamespaceid = s.placenamespaceid;
$BODY$
LANGUAGE sql VOLATILE;



--
-- Get DataProviders
--
-- Use wci.info( NULL::wci.infodataprovider ); 

--
-- Get DataProviderName
--
CREATE OR REPLACE FUNCTION 
wci.getDataProviderName(
	name_	text
)
RETURNS SETOF __WCI_SCHEMA__.dataprovidername_v AS
$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.dataprovidername_v
	WHERE dataproviderid = SOME 
		  ( SELECT dataproviderid 
		    FROM   __WCI_SCHEMA__.dataprovidername_v d,
		    	   __WCI_SCHEMA__.getSessionData() s
		    WHERE  d.dataprovidernamespaceid = s.dataprovidernamespaceid
		      AND  ( d.dataprovidername = $1 OR $1 IS NULL ) );
$BODY$
LANGUAGE sql VOLATILE;


--
-- Get PlaceDefinition
--
-- Use wci.info( NULL::wci.infoplace ); 
-- Use wci.info( NULL::wci.inforegulargrid ); 


--
-- Get DataProviderName
--
CREATE OR REPLACE FUNCTION 
wci.getPlaceName(
	name_	text
)
RETURNS SETOF __WCI_SCHEMA__.placename_V AS
$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.placename_v
	WHERE placeid = SOME
		  ( SELECT placeid 
		    FROM   __WCI_SCHEMA__.placename p
		    WHERE  ( p.placename LIKE $1 OR $1 IS NULL ) );
$BODY$
LANGUAGE sql VOLATILE;


--
-- Get PlaceName using regular grid info
--  
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
	SELECT placename
	FROM __WCI_SCHEMA__.placeregulargrid_mv grd, __WCI_SCHEMA__.getSessionData() s
	WHERE numberX = $1
	AND   numberY = $2
	AND   round(incrementX::numeric, 3) = round($3::numeric, 3)
	AND   round(incrementY::numeric, 3) = round($4::numeric, 3)
	AND	  round(startX::numeric, 3) = round($5::numeric, 3)
	AND   round(startY::numeric, 3) = round($6::numeric, 3)
	AND   projdefinition = $7
	AND   grd.placenamespaceid = s.placenamespaceid;
$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT VOLATILE;


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
	WHERE 	proj4text LIKE $1
$BODY$
SECURITY DEFINER
LANGUAGE sql;


--
-- Get Measure
--
CREATE OR REPLACE FUNCTION 
wci.getMeasure(
	measure_	text
)
RETURNS __WCI_SCHEMA__.measure AS
$BODY$
	SELECT 	*
	FROM 	__WCI_SCHEMA__.measure
	WHERE 	measure LIKE $1 OR $1 IS NULL;
$BODY$
SECURITY DEFINER
LANGUAGE sql;
