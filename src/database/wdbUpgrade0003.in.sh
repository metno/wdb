#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2007-2010 met.no
#
#  Contact information:
#  Norwegian Meteorological Institute
#  Box 43 Blindern
#  0313 OSLO
#  NORWAY
#  E-mail: wdb@met.no
#
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
# This is the update file for WDB from schema version 1 to schema version 2
#
# Set Variables
export WDB_NAME=$1
export WDB_USER=$2
export WDB_PORT=$3
export WDB_PATH=$4
export WCI_PATH=$5
export WDB_LOGS=$6

# Run PSQL Query
psql -U $WDB_USER -p $WDB_PORT -d $WDB_NAME -q <<EOF

SET CLIENT_MIN_MESSAGES TO "WARNING";

\set ON_ERROR_STOP

\o $WDB_LOGS/wdb_upgrade_datamodel.log

-- Place Name Table
ALTER TABLE __WDB_SCHEMA__.placename ADD COLUMN 
    placenameupdatetime	timestamp with time zone NOT NULL default 'now';

ALTER TABLE ONLY __WDB_SCHEMA__.placename
    DROP CONSTRAINT placename_pkey;
ALTER TABLE ONLY __WDB_SCHEMA__.placename
    ADD CONSTRAINT placename_pkey PRIMARY KEY (placeid, placenamespaceid, placenamevalidfrom);

ALTER TABLE ONLY __WDB_SCHEMA__.placename
    DROP CONSTRAINT placename_unique;
ALTER TABLE ONLY __WDB_SCHEMA__.placename
	ADD CONSTRAINT placename_unique UNIQUE (placenamespaceid, placename, placenamevalidfrom);


CREATE VIEW __WCI_SCHEMA__.placename_valid_v AS
SELECT
    p.placeid,
    p.placename,
    p.placenamevalidfrom,
    p.placenamevalidto
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename_valid_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_valid_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_valid_V TO wdb_read, wdb_write;




--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePoint(
	placeName_ 			text,
	placeGeometry_ 		geometry
)
RETURNS bigint AS
\$BODY$
DECLARE
	placeId_ 	bigint;
	namespace_	int;
	newname_	text;
	indCode_ 	int;
BEGIN
	SELECT wci.addPlacePoint(placeName_, placeGeometry_, 'today'::timestamp with time zone, 'infinity'::timestamp with time zone ) INTO placeId_;
	RETURN placeId_;
END;
\$BODY$
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
\$BODY$
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
\$BODY$
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
\$BODY$
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
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'point', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placevalidfrom_ < placenamevalidto;
			IF FOUND THEN 
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidto = placevalidfrom_
				WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placevalidfrom_ < placenamevalidto;
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_,
				  namespace_,
				  lower(placeName_),
				  placevalidfrom_,
				  placevalidto_, 
				  'now' );
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placeid <> placeId_ AND
				placevalidfrom_ < placenamevalidto;
			IF FOUND THEN
				-- Other placeIds with same name 
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidto = placevalidfrom_,
					placenameupdatetime = 'now'
				WHERE	placeid <> placeId_ AND
					placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placevalidfrom_ < placenamevalidto;
			END IF;
			-- Update for same placeid, if exists
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placeId = placeId_ AND
				placevalidfrom_ < placenamevalidto;
			IF NOT FOUND THEN
				INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_,
					  'now' );
			ELSE
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidfrom = placevalidfrom_,
					placenamevalidto = placevalidto_,
					placenameupdatetime = 'now'
				WHERE	placeid = placeId_ AND
					placename = lower(placeName_) AND
					placenamespaceid = namespace_;
			END IF;
		END IF;
	END IF;
	RETURN placeId_;
END;
\$BODY$
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
\$BODY$
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
\$BODY$
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
\$BODY$
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
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
			( placeId_, indCode_, 'polygon', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placevalidfrom_ < placenamevalidto;
			IF FOUND THEN 
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidto = placevalidfrom_,
					placenameupdatetime = 'now'
				WHERE	placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placevalidfrom_ < placenamevalidto;
			END IF;
			INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_,
				  namespace_,
				  lower(placeName_),
				  placevalidfrom_,
				  placevalidto_,
				  'now' );
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placeid <> placeId_ AND
				placevalidfrom_ < placenamevalidto;
			IF FOUND THEN
				-- Other placeIds with same name 
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidto = placevalidfrom_,
					placenameupdatetime = 'now'
				WHERE	placeid <> placeId_ AND
					placename = lower(placeName_) AND
					placenamespaceid = namespace_ AND
					placevalidfrom_ < placenamevalidto;
			END IF;
			-- Update for same placeid, if exists
			PERFORM	*
			FROM  	__WDB_SCHEMA__.placename
			WHERE	placename = lower(placeName_) AND
				placenamespaceid = namespace_ AND
				placeId = placeId_ AND
				placevalidfrom_ < placenamevalidto;
			IF NOT FOUND THEN
				INSERT INTO __WDB_SCHEMA__.placename VALUES
					( placeId_,
					  namespace_,
					  lower(placeName_),
					  placevalidfrom_,
					  placevalidto_,
					  'now' );
			ELSE
				UPDATE	__WDB_SCHEMA__.placename 
				SET	placenamevalidfrom = placevalidfrom_,
					placenamevalidto = placevalidto_,
					placenameupdatetime = 'now'
				WHERE	placeid = placeId_ AND
					placename = lower(placeName_) AND
					placenamespaceid = namespace_;
			END IF;
		END IF;
	END IF;
	RETURN placeId_;
END;
\$BODY$
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
\$BODY$
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
\$BODY$
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
\$BODY$
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
\$BODY$
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
\$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.placename_v
	WHERE placeid = SOME
		  ( SELECT placeid 
		    FROM   __WCI_SCHEMA__.placename p
		    WHERE  ( p.placename LIKE lower(\$1) OR \$1 IS NULL ) );
\$BODY$
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
\$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.placename_valid_v
	WHERE
		( placename LIKE lower(\$1) OR \$1 IS NULL ) AND
		( \$2 IS NULL OR 
		 ( \$2 >= placenamevalidfrom AND \$2 <= placenamevalidto ) );
\$BODY$
SECURITY DEFINER
LANGUAGE sql VOLATILE;

SELECT wci.setConfiguration( 'WDB', '__WDB_VERSION__', 3 );

EOF
