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
__WCI_SCHEMA__.addDataProvider
(
	dataProviderName_		text,
	dataProviderType_ 		text,
	domainDelivery_			text,
	dataProviderComment_	text
)
RETURNS bigint AS
$BODY$
DECLARE
	dpid_	bigint;
	namespace_	integer;
BEGIN
	-- Add new dataprovider
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	SELECT dataproviderid INTO dpid_ 
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = lower(dataProviderName_) AND
		  dataprovidernamespaceid = namespace_;
	-- Add dataprovider
	IF NOT FOUND THEN
		dpid_ := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);

		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		( dpid_, dataProviderType_, domainDelivery_, '1 day', 'now' );
	
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		( dpid_, dataProviderComment_, 'now' );
	
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
		( dpid_, namespace_, lower(dataProviderName_), 'today'::TIMESTAMP WITH TIME ZONE, 'infinity'::TIMESTAMP WITH TIME ZONE, dpid_, dpid_ );
	ELSE
		RAISE EXCEPTION 'Dataprovider already exists in WDB';
	END IF;
	-- Return dataproviderid
	RETURN dpid_;
END;
$BODY$
LANGUAGE plpgsql STRICT VOLATILE;



--
-- Add Place Definition
--
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.addPlacePoint(
	placeName_ 		text,
	placeGeometry_ 	geometry
)
RETURNS bigint AS
$BODY$
DECLARE
	placeId_ 	bigint;
	namespace_	int;
	newname_	text;
	indCode_ 	int;
BEGIN
	-- Get namespace
	SELECT placenamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Indeterminate Code 0 = Exact
	indCode_ := 0;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POINT' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB point';
	END IF;	
	-- Get placedef
	SELECT placeid INTO placeId_ 
	FROM  __WCI_SCHEMA__.placedefinition
	WHERE st_equals( placegeometry, placeGeometry_);
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
		( placeId_, indCode_, 'Point', 'now', placeGeometry_ );
	END IF;
	IF namespace_ <> 0 THEN
		INSERT INTO __WDB_SCHEMA__.placename VALUES
		( placeId_, namespace_, lower(placeName_), 'today', 'infinity' );
	END IF;
	RETURN placeId_;
	-- Default Name
	namespace_ := 0;
	SELECT astext( placeGeometry_ ) INTO newName_;
	INSERT INTO __WDB_SCHEMA__.placename
	VALUES ( placeId_,
			 namespace_,
			 lower(newName_),
			 'today'::TIMESTAMP WITH TIME ZONE,
			 'infinity'::TIMESTAMP WITH TIME ZONE );
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;



-- 
-- Add Place Defintion
--
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.addPlaceRegularGrid(
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
DECLARE
	placeId_ 	bigint;
	namespace_ 	int;
	newname_ 	text;
	srid_ 		int;
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
	-- Get placedef
	SELECT placeid INTO placeId_ 
	FROM  __WCI_SCHEMA__.placespec
	WHERE numberx = numX_ AND
		  numbery = numY_ AND
		  incrementx = incX_ AND
		  incrementy = incY_ AND
		  startx = startX_ AND
		  starty = startY_ AND
		  projdefinition = projection_;
	-- Add dataprovider
	IF NOT FOUND THEN
		-- Get PlaceId
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq') ;
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
	END IF;
	-- Insert new placename
	IF namespace_ <> 0 THEN
		INSERT INTO __WDB_SCHEMA__.placename
		VALUES ( placeId_,
				 nameSpace_,
				 lower(placeName_),
				 'today'::TIMESTAMP WITH TIME ZONE,
				 'infinity'::TIMESTAMP WITH TIME ZONE );
	END IF;
	-- Default Name
	namespace_ := 0;
	newname_ := 'grid( ' ||
				 numX_ || ' ' || numY_ || ' ' ||
				 incX_ || ' ' || incY_ || ' ' ||
				 startX_ || ' ' || startY_ || ' ' ||
				 srid_ || ')';
	INSERT INTO __WDB_SCHEMA__.placename
	VALUES ( placeId_,
			 namespace_,
			 newName_,
			 'today'::TIMESTAMP WITH TIME ZONE,
			 'infinity'::TIMESTAMP WITH TIME ZONE );
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;
