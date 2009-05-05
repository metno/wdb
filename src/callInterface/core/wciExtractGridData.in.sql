-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 met.no
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

-- Interpret a bytea (which must be of size 8) as a real
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.binary_toReal
(
	buffer bytea
) 
RETURNS real AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_core_binary_toReal'
LANGUAGE 'c' STRICT IMMUTABLE;


-- Interpret a bytea (which must be of size 4) as an int
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.binary_toInt4
(
	buffer bytea
)
RETURNS int4 AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_core_binary_toInt4'
LANGUAGE 'c' STRICT IMMUTABLE;


-- Get the (i,j) index, based on a placespec, and lon-lat coordinates.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.indexFromLonLat
( 
	gridDefinition __WCI_SCHEMA__.placespec, 
	lon double precision, 
	lat double precision, 
	OUT i double precision, 
	OUT j double precision 
)
AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciIndexFromLonLat'
LANGUAGE 'c' STRICT STABLE;


-- Get all placepoints for a placeid.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getAllPlacePoints
(
	placeid bigint 
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
BEGIN
	OPEN ret FOR
		SELECT * FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.placeid = placeid;
	return ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Get a cursor to the exact placepoint for the given geometry and placeid. If 
-- the given geometry does not exactly match a point in the given placeid, 
-- nothing is returned.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getExactPlacePoint
(
	location GEOMETRY, 
	placeid bigint 
)
RETURNS __WCI_SCHEMA__.placepoint AS
$BODY$
DECLARE
	ret __WCI_SCHEMA__.placepoint;
BEGIN
	SELECT * INTO ret FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.location @ location AND __WCI_SCHEMA__.placepoint.placeid = placeid;
	return ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


CREATE TYPE __WCI_SCHEMA__.gridPointSpec AS 
(
	i		double precision,
	j		double precision,
	iNum	integer,
	jNum	integer
);

-- Get the exact (i,j) index for a given geometry and placeid, as two doubles
-- Also extract iNum (number of points on i axis)
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getExactIJ
(
	IN location GEOMETRY, 
	IN placeid 	bigint 
)
RETURNS __WCI_SCHEMA__.gridPointSpec AS
$BODY$
DECLARE
	placeSpec __WCI_SCHEMA__.placespec;
	ret		  __WCI_SCHEMA__.gridPointSpec;
BEGIN
	SELECT * INTO placeSpec FROM __WCI_SCHEMA__.placespec WHERE __WCI_SCHEMA__.placespec.placeid=placeid;
	SELECT * INTO ret.i, ret.j FROM __WCI_SCHEMA__.indexFromLonLat( placeSpec, X(location), Y(location) );
	ret.iNum = placespec.inumber;
	ret.jNum = placespec.jnumber;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Get a cursor to the nearest exact placepoint for the given geometry and placeid.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getNearestPlacePoint
(
	location GEOMETRY, 
	placeid bigint,
	i double precision,
	j double precision
)
RETURNS __WCI_SCHEMA__.placepoint AS
$BODY$
DECLARE
	ret __WCI_SCHEMA__.placepoint;
	i_i integer;
	i_j integer;
BEGIN
	i_i := round(i); -- using round before cursor query improves 
	i_j := round(j); -- performance by a factor of 50-300, for some reason
	SELECT * INTO ret FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.placeid = placeid AND __WCI_SCHEMA__.placepoint.i = i_i AND __WCI_SCHEMA__.placepoint.j = i_j;
	-- the following is a very slow alternative: 
	--SELECT * from __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.placeid=placeid ORDER BY distance( __WCI_SCHEMA__.placepoint.location, location ) LIMIT 1;
	return ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Get a cursor to the four nearest placepoint for the given geometry and placeid.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getSurroundingPlacePoint
(
	location GEOMETRY, 
	placeid bigint,
	i double precision,
	j double precision	
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
	i_ceil integer;
	i_floor integer;
	j_ceil integer;
	j_floor integer;
BEGIN
	-- See __WCI_SCHEMA__.getNearestPlacePoint for performance note:
	i_ceil  := ceil( i );
	i_floor := floor( i );
	j_ceil  := ceil( j );
	j_floor := floor( j );
	-- Retrieve all 4 "corners"
	OPEN ret FOR 
		SELECT * FROM __WCI_SCHEMA__.placepoint 
			WHERE __WCI_SCHEMA__.placepoint.placeid = placeid 
			AND __WCI_SCHEMA__.placepoint.i in ( i_ceil, i_floor ) 
			AND __WCI_SCHEMA__.placepoint.j in ( j_ceil, j_floor );

	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Get a cursor to the exact placepoint for the given geometry and placeid. If 
-- the given geometry does not exactly match a point in the given placeid, 
-- nothing is returned.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getPolygonPlacePoints
(
	location GEOMETRY, 
	placeid bigint 
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
BEGIN
	OPEN ret FOR 
		SELECT * FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.location @ location AND __WCI_SCHEMA__.placepoint.placeid = placeid;
	return ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Is the given interpolation a true derivate of data, 
-- or is it merely a way to find one or several points 
-- which we have data for?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.isRealInterpolation
(
	interpolation __WCI_SCHEMA__.interpolationtype
)
RETURNS boolean AS
$BODY$
BEGIN
	IF interpolation = 'bilinear'::__WCI_SCHEMA__.interpolationtype THEN
		return TRUE;
	END IF;
	return FALSE;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT IMMUTABLE;
	

-- Extract values for all points in the provided floatgridvalue which matches 
-- the given geometry and interpolation type, along with their exact geometry.
-- The interpolation parameter is only relevant if the given geometry is a 
-- point.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.extractGridData
(
	placeid			bigint,
	location 		GEOMETRY,
	interpolation 	__WCI_SCHEMA__.interpolationType, 
	dataid		bigint 
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
DECLARE
	-- The return value of the function
	ret 			__WCI_SCHEMA__.extractGridDataReturnType;
	isInterpolation	boolean;
	geomType 		CONSTANT text := geometrytype( location );
	pSpec  			__WCI_SCHEMA__.gridPointSpec;
	-- Cursor to a placepoint query
	p 				__WCI_SCHEMA__.placepoint;
	curs 			refcursor;
	val				float4;
BEGIN
	isInterpolation := __WCI_SCHEMA__.isRealInterpolation(interpolation);
	IF isInterpolation = TRUE THEN
		-- If Interpolation is bilinear
		IF interpolation = 'bilinear'::__WCI_SCHEMA__.interpolationType THEN
			SELECT * INTO ret FROM __WCI_SCHEMA__.bilinearInterpolation( placeid, location, dataid );
			--IF ret IS NOT NULL THEN -- (the following line works in postgresql 8.1)
			IF ret.location IS NOT NULL THEN 
				RETURN NEXT ret;
			END IF;
		ELSE
			RAISE EXCEPTION 'Unknown interpolation type: %', interpolation; 
		END IF;
	ELSE
		-- Retrieve All Points in the Grid
		IF location IS NULL THEN
			SELECT iNumber INTO pSpec.iNum FROM __WCI_SCHEMA__.placespec WHERE __WCI_SCHEMA__.placespec.placeid = placeid;
			curs := __WCI_SCHEMA__.getAllPlacePoints( placeid );
			FOR ret IN 
				SELECT * FROM __WCI_SCHEMA__.readSetOfFieldPoints( dataid, pSpec.iNum, curs ) 
			LOOP
				RETURN NEXT ret;
			END LOOP;
			RETURN;
		-- Retrieve All Points in a Polygon
		ELSIF geomType = 'POLYGON' THEN
			SELECT iNumber INTO pSpec.iNum FROM __WCI_SCHEMA__.placespec WHERE __WCI_SCHEMA__.placespec.placeid = placeid;
			curs := __WCI_SCHEMA__.getPolygonPlacePoints( location, placeid );
			FOR ret IN 
				SELECT * FROM __WCI_SCHEMA__.readSetOfFieldPoints( dataid, pSpec.iNum, curs ) 
			LOOP
				RETURN NEXT ret;
			END LOOP;
			RETURN;
		ELSIF geomType = 'POINT' THEN
			SELECT * INTO pSpec FROM __WCI_SCHEMA__.getExactIJ( location, placeid );
			IF interpolation = 'exact'::__WCI_SCHEMA__.interpolationType THEN
				p := __WCI_SCHEMA__.getExactPlacePoint( location, placeid );
				-- PostgeSQL 8.1 does not handle NULL check on tuples, so have to check the attribute
				IF p.placeid IS NOT NULL THEN
					val := __WCI_SCHEMA__.getSinglePointData( p.i, p.j, pSpec.iNum, dataid );
					ret := (p.location, val, p.i, p.j);
					RETURN NEXT ret;
				END IF;
				RETURN;
			ELSIF interpolation = 'nearest'::__WCI_SCHEMA__.interpolationType THEN
				p := __WCI_SCHEMA__.getNearestPlacePoint( location, placeid, pSpec.i, pSpec.j );
				-- PostgeSQL 8.1 does not handle NULL check on tuples, so have to check the attribute
				IF p.placeid IS NOT NULL THEN
					val := __WCI_SCHEMA__.getSinglePointData( p.i, p.j, pSpec.iNum, dataid );
					ret := (p.location, val, p.i, p.j);
					RETURN NEXT ret;
				END IF;
				RETURN;
			ELSIF interpolation = 'surround'::__WCI_SCHEMA__.interpolationType THEN 
				curs := __WCI_SCHEMA__.getSurroundingPlacePoint( location, placeid, pSpec.i, pSpec.j );
				FOR ret IN 
					SELECT * FROM __WCI_SCHEMA__.readSetOfFieldPoints( dataid, pSpec.iNum, curs ) 
				LOOP
					RETURN NEXT ret;
				END LOOP;
				RETURN;
			ELSE
				RAISE EXCEPTION 'Unrecognized interpolation type: %', interpolation;
			END IF;
		ELSIF geomType = 'LINE' THEN
			RAISE EXCEPTION 'Unsupported geometry type: %', geomType;
		ELSE
			RAISE EXCEPTION 'Unrecognized geometry type: %', geomType;
		END IF;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


--CREATE OR REPLACE FUNCTION 
--__WCI_SCHEMA__.extractGridData
--(
--	placeid			bigint,
--	location 		GEOMETRY,
--	interpolation 	__WCI_SCHEMA__.interpolationType, 
--	dataid		bigint 
--)
--RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS
--'__WDB_LIBDIR__/__WCI_LIB__', 'extractGridData'
--LANGUAGE 'c' STABLE;



CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.readSetOfFieldPoints
(
	whatFile bigint,
	iNumber integer, -- number of rows in i-direction
	placepoints refcursor
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS 
$BODY$
DECLARE
	idx integer;
	p __WCI_SCHEMA__.placepoint;
	ret __WCI_SCHEMA__.extractGridDataReturnType;
BEGIN
		LOOP
			FETCH placepoints INTO p;
			EXIT WHEN NOT FOUND;
			idx := ((iNumber * p.j) + p.i);
			RAISE DEBUG 'Point %,% (idx %)', p.i, p.j, idx;
			ret := ( p.location, read_float_from_file(whatFile, idx), p.i, p.j );
			RETURN NEXT ret;
		END LOOP;
		RETURN;
END;
$BODY$
LANGUAGE plpgsql STABLE;
