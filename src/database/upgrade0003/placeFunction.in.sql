-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2012 met.no
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
-- Update Schema to use Place Definition with Valid Times
-- 

SET SESSION client_min_messages TO 'warning';

--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePoint(
	placeName_ 			text,
	placeGeometry_ 		geometry
)
RETURNS bigint AS
$BODY$
DECLARE
	placeId_ 	bigint;
	namespace_	int;
	newname_	text;
	indCode_ 	int;
BEGIN
	SELECT wci.addPlacePoint(placeName_, placeGeometry_, 'today'::timestamp with time zone, 'infinity'::timestamp with time zone ) INTO placeId_;
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePoint(
	placeName_ 			text,
	placeGeometry_ 		geometry,
	placevalidfrom_		timestamp with time zone,
	placevalidto_		timestamp with time zone
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
	-- Indeterminate Code 1 = Exact
	indCode_ := 1;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POINT' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB point';
	END IF;	
	-- Get placedef
	SELECT	placeid INTO placeId_ 
	FROM	__WCI_SCHEMA__.placedefinition
	WHERE	st_equals( placegeometry, placeGeometry_) AND
		placenamespaceid = 0;
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'point', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_,
				  namespace_,
				  lower(placeName_),
				  placevalidfrom_,
				  placevalidto_, 
				  'now' );
		END IF;
	ELSE
		RAISE EXCEPTION 'Place point already exists in database. Use addOrUpdatePlacePoint to update existing place point.';
	END IF;
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addOrUpdatePlacePoint(
	placeName_ 		text,
	placeGeometry_ 		geometry,
	placevalidfrom_		timestamp with time zone,
	placevalidto_		timestamp with time zone
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
	-- Indeterminate Code 1 = Exact
	indCode_ := 1;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POINT' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB point';
	END IF;	
	-- Get placedef
	SELECT	placeid INTO placeId_
	FROM	__WCI_SCHEMA__.placedefinition
	WHERE	st_equals( placegeometry, placeGeometry_ ) AND
		    placenamespaceid = 0;
	-- Add placedefinition
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'point', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			-- Different PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
			END IF;
			-- Different PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_, 
					  'now' );
			RETURN placeId_;
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			-- Same PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Different PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Same PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
			END IF;
			-- Different PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_, 
					  'now' );
			RETURN placeId_;
		END IF;
	END IF;
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;

--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePolygon(
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
	-- Indeterminate Code 1 = Exact
	indCode_ := 1;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POLYGON' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB polygon';
	END IF;	
	-- Get placedef
	SELECT	placeid INTO placeId_ 
	FROM	__WCI_SCHEMA__.placedefinition
	WHERE	st_equals( placegeometry, placeGeometry_) AND
		placenamespaceid = 0;
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'polygon', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_,
				  namespace_,
				  lower(placeName_),
				  placevalidfrom_,
				  placevalidto_,
				  'now' );
		END IF;
	ELSE
		RAISE EXCEPTION 'Place polygon already exists in database. Use addOrUpdatePlacePolygon to update existing place polygon.';
	END IF;
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addOrUpdatePlacePolygon(
	placeName_ 		text,
	placeGeometry_ 		geometry,
	placevalidfrom_		timestamp with time zone,
	placevalidto_		timestamp with time zone
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
	-- Indeterminate Code 1 = Exact
	indCode_ := 1;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POLYGON' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB polygon';
	END IF;	
	-- Get placedef
	SELECT	placeid INTO placeId_
	FROM	__WCI_SCHEMA__.placedefinition
	WHERE	st_equals( placegeometry, placeGeometry_ ) AND
		placenamespaceid = 0;
	-- Add placedefinition
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'polygon', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			-- Different PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
			END IF;
			-- Different PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_, 
					  'now' );
			RETURN placeId_;
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			-- Same PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Same PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
				RETURN placeId_;
			END IF;
			-- Different PlaceId, extends bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Different PlaceId, inside bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId is inside the validtime period. Split your placename.';
			END IF;
			-- Same PlaceId, early bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto);
			END IF;
			-- Different PlaceId, later bounds
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ <> placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto);
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_, 
					  'now' );
			RETURN placeId_;
		END IF;
	END IF;
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


--
-- add New Place Definition (Regular Grid)
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
	WHERE btrim(proj4text) = btrim(projection_);
	-- If SRID not found...
	IF srid_ IS NULL THEN
		RAISE EXCEPTION 'Could not identify the PROJ.4 projection in the database. Check that the projection is valid and, if needed, insert it into the database using wci.addSrid( ... )';		
	END IF;
	-- Get placedef
	SELECT placeid INTO placeId_ 
	FROM  __WCI_SCHEMA__.placeregulargrid_mv
	WHERE numberx = numX_ AND
		  numbery = numY_ AND
		  round(incrementx::numeric, 3) = round(incX_::numeric, 3) AND
		  round(incrementy::numeric, 3) = round(incY_::numeric, 3) AND
		  round(startx::numeric, 3) = round(startX_::numeric, 3) AND
		  round(starty::numeric, 3) = round(startY_::numeric, 3) AND
		  projdefinition = btrim(projection_) AND
		  placenamespaceid = 0;
	-- Add dataprovider
	IF NOT FOUND THEN
		-- Get PlaceId
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
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
		IF namespace_ <> 0 THEN
			INSERT INTO __WDB_SCHEMA__.placename
			VALUES ( placeId_,
					 nameSpace_,
					 lower(placeName_),
					 'today'::TIMESTAMP WITH TIME ZONE,
					 'infinity'::TIMESTAMP WITH TIME ZONE,
					 'now' );
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			PERFORM * 
			FROM  __WDB_SCHEMA__.placename
			WHERE placeid = placeId_ AND 
				  placenamespaceid = namespace_;
			IF NOT FOUND THEN
				INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_, namespace_, lower(placeName_), 'today', 'infinity', 'now' );
			ELSE
				UPDATE __WDB_SCHEMA__.placename 
				SET placename = lower(placeName_), 
					placenameupdatetime = 'now'
				WHERE placeid = placeId_ AND
					  placenamespaceid = namespace_;
			END IF;
		END IF;
	END IF;
	-- Insert new placename
	RETURN placeId_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;

--
-- add new data provider name in namespace
-- 
CREATE OR REPLACE FUNCTION
wci.setPlaceName
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
			  'infinity'::TIMESTAMP WITH TIME ZONE,
			   'now' );
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


--
-- Get PlaceName
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
		    WHERE  ( p.placename LIKE lower($1) OR $1 IS NULL ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;


--
-- Get DataProviderName
-- Using valid times
CREATE OR REPLACE FUNCTION 
wci.getPlaceName(
	name_	text,
	valid_	timestamp with time zone
		
)
RETURNS SETOF __WCI_SCHEMA__.placename_valid_v AS
$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.placename_valid_v
	WHERE
		( placename LIKE lower($1) OR $1 IS NULL ) AND
		( $2 IS NULL OR 
		 ( $2 >= placenamevalidfrom AND $2 <= placenamevalidto ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;
