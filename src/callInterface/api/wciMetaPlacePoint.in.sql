-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 - 2012 met.no
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


-- add new placepoint
-- aborts if new definition overlaps existing entry 
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


-- add new placepoint
-- aborts if new definition overlaps existing entry 
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
	FROM	__WCI_SCHEMA__.placedefinition_v
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


-- Add PlacePoint
-- Or Update existing entries if they exist
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
	FROM	__WCI_SCHEMA__.placedefinition_v
	WHERE	st_equals( placegeometry, placeGeometry_ ) AND
		    placenamespaceid = 0;
	-- Add placedefinition
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'point', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			-- Same Name, but overlapping values
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ <> placeid AND
					(((placevalidfrom_ >= placenamevalidfrom) AND 
					  (placevalidfrom_ <  placenamevalidto)) OR  
					 ((placevalidto_   <= placenamevalidto) AND
					  (placevalidto_   >  placenamevalidfrom)) OR
					 ((placevalidfrom_ <= placenamevalidfrom ) AND
					  (placevalidto_   >= placenamevalidto))); 
			IF FOUND THEN
				RAISE EXCEPTION 'An identical placename for a different placeId overlaps with this validtime period. Adjust the  placename to avoid overlapping.';
			END IF;
			-- Not same PlaceName or No Overlaps
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
			-- Same PlaceName, extends bounds
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
			-- Same PlaceName, inside bounds
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
			-- Same PlaceName, underlaps
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ <= placenamevalidfrom) AND 
					(placevalidto_   <= placenamevalidto) AND
					(placevalidto_   >= placenamevalidfrom); 
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidfrom = placevalidfrom_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ <= placenamevalidfrom) AND 
						(placevalidto_   <= placenamevalidto) AND
						(placevalidto_   >= placenamevalidfrom); 
				RETURN placeId_;
			END IF;
			-- Same PlaceName, overlaps
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placeid_ = placeid AND
					(placevalidfrom_ >= placenamevalidfrom) AND 
					(placevalidto_   >= placenamevalidto) AND
					(placevalidfrom_ <= placenamevalidto);  
			IF FOUND THEN
				UPDATE	__WDB_SCHEMA__.placename
				SET		placenamevalidto = placevalidto_,
						placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
						placenamespaceid = namespace_ AND	
						placeid_ = placeid AND
						(placevalidfrom_ >= placenamevalidfrom) AND 
						(placevalidto_   >= placenamevalidto) AND
						(placevalidfrom_ <= placenamevalidto);  
				RETURN placeId_;
			END IF;
			-- Same PlaceId, Same Name, No Overlaps - Insert 
			-- Same PlaceId, Different Name - Insert
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


-- Get Place Points from database
CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text,
				   valid 				timestamp with time zone )
RETURNS SETOF wci_int.placedefinition_v AS
$BODY$
	SELECT 	p.placeid,
			p.placegeometrytype,
			p.placegeometry,
			p.placeindeterminatecode,
			p.placenamespaceid,
			p.placename,
			p.placenamevalidfrom,
			p.placenamevalidto,
			p.originalsrid,
			p.placestoretime
	FROM	wci_int.placeDefinition_mv p,
			wci_int.getSessionData() s
	WHERE	p.placenamespaceid = s.placenamespaceid
	  AND	( $1 IS NULL OR placename LIKE lower($1) )
	  AND	( $2 IS NULL OR ( placenamevalidfrom <= $2 AND placenamevalidto >= $2 ) )
	  AND	p.placegeometrytype = 'point';
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text )	
RETURNS SETOF wci_int.placedefinition_v AS
$BODY$
	SELECT 	*
	FROM	wci.getPlacePoint($1, 'now');
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;
