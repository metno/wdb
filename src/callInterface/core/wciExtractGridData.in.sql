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


-- Get the exact (i,j) index for a given geometry and placeid, as two doubles
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getExactIJ
(
	location GEOMETRY, 
	placeid bigint, 
	OUT i double precision, 
	OUT j double precision 
)
AS
$BODY$
DECLARE
	placeSpec __WCI_SCHEMA__.placespec;
BEGIN
	SELECT * INTO placeSpec FROM __WCI_SCHEMA__.placespec WHERE __WCI_SCHEMA__.placespec.placeid=placeid;
	SELECT * INTO i, j FROM __WCI_SCHEMA__.indexFromLonLat( placeSpec, X(location), Y(location) );
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Get a cursor to the nearest exact placepoint for the given geometry and placeid.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getNearestPlacePoint
(
	location GEOMETRY, 
	placeid bigint 
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
	i double precision;
	j double precision;
	i_i integer;
	i_j integer;
BEGIN
	SELECT * INTO i, j FROM __WCI_SCHEMA__.getExactIJ( location, placeid );

	i_i := round(i); -- using round before cursor query improves 
	i_j := round(j); -- performance by a factor of 50-300, for some reason

	OPEN ret FOR 
		SELECT * FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.placeid = placeid AND __WCI_SCHEMA__.placepoint.i = i_i AND __WCI_SCHEMA__.placepoint.j = i_j;
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
	placeid bigint 
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
	i double precision;
	j double precision;
	i_ceil integer;
	i_floor integer;
	j_ceil integer;
	j_floor integer;
BEGIN
	SELECT * INTO i, j FROM __WCI_SCHEMA__.getExactIJ( location, placeid );

	-- See __WCI_SCHEMA__.getNearestPlacePoint for performance note:
	i_ceil  := ceil( i );
	i_floor := floor( i );
	j_ceil  := ceil( j );
	j_floor := floor( j );

	OPEN ret FOR 
		SELECT * FROM __WCI_SCHEMA__.placepoint 
			WHERE __WCI_SCHEMA__.placepoint.placeid = placeid 
			AND __WCI_SCHEMA__.placepoint.i in ( i_ceil, i_floor ) 
			AND __WCI_SCHEMA__.placepoint.j in ( j_ceil, j_floor );

	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


-- Is the given interpolation a true derivate of data, 
-- or is it merely a way to find one or several points 
-- which we have data for?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.isRealInterpolation
(
	interpolation wci.interpolationtype
)
RETURNS boolean AS
$BODY$
BEGIN
	IF interpolation = 'bilinear'::wci.interpolationtype THEN
		return TRUE;
	END IF;
	return FALSE;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT IMMUTABLE;
		

-- Extract placepoints for the given placeid, according to geometry and 
-- interpolation type.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getPlacePoints
(
	location GEOMETRY, 
	interpolation wci.interpolationType, 
	placeid bigint 
)
RETURNS refcursor AS
$BODY$
DECLARE
	ret refcursor;
	geomType CONSTANT text := geometrytype( location );
BEGIN
	IF location IS NULL THEN
		RETURN __WCI_SCHEMA__.getAllPlacePoints( placeid );

	ELSIF geomType = 'POLYGON' THEN
		RETURN __WCI_SCHEMA__.getExactPlacePoint( location, placeid );

	ELSIF geomType = 'POINT' THEN
		IF interpolation = 'exact'::wci.interpolationType THEN
			RETURN __WCI_SCHEMA__.getExactPlacePoint( location, placeid );

		ELSIF interpolation = 'nearest'::wci.interpolationType THEN
			RETURN __WCI_SCHEMA__.getNearestPlacePoint( location, placeid );

		ELSIF interpolation = 'surround'::wci.interpolationType THEN 
			RETURN __WCI_SCHEMA__.getSurroundingPlacePoint( location, placeid );
		ELSE
			RAISE EXCEPTION 'Unrecognized interpolation type: %', interpolation;
		END IF;

	ELSIF geomType = 'LINE' THEN
		RAISE EXCEPTION 'Usupported geometry type: %', geomType;

	ELSE
		RAISE EXCEPTION 'Unrecognized geometry type: %', geomType;
	END IF;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



-- Extract values for all points in the provided floatgridvalue which matches 
-- the given geometry and interpolation type, along with their exact geometry.
-- The interpolation parameter is only relevant if the given geometry is a 
-- point.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.extractGridData
(
	location GEOMETRY, 
	interpolation wci.interpolationType, 
	data __WCI_SCHEMA__.oidValue 
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
	DECLARE
		fd integer;
		pos integer := 0;
		readSize CONSTANT int4 := 4;
		ret __WCI_SCHEMA__.extractGridDataReturnType;

		iNum integer;
		idx integer;
		p __WCI_SCHEMA__.placepoint;

		isInterpolation boolean;

		curs refcursor;
	BEGIN
		SELECT iNumber INTO iNum FROM __WCI_SCHEMA__.placespec WHERE placeid = data.placeid;
	
		isInterpolation := __WCI_SCHEMA__.isRealInterpolation(interpolation);
		IF isInterpolation = TRUE THEN
			IF interpolation = 'bilinear'::wci.interpolationType THEN
				SELECT * INTO ret FROM __WCI_SCHEMA__.bilinearInterpolation( location, data );
				--IF ret IS NOT NULL THEN -- the following line also works on postgresql 8.1:
				IF ret.location IS NOT NULL THEN 
					RETURN NEXT ret;
				END IF;
			ELSE
				RAISE EXCEPTION 'Unknown interpolation type: %', interpolation; 
			END IF;
		ELSE
			curs := __WCI_SCHEMA__.getPlacePoints( location, interpolation, data.placeid );
			FOR ret IN 
				SELECT * FROM __WCI_SCHEMA__.readField( data.value, iNum, curs ) 
			LOOP
				RETURN NEXT ret;
			END LOOP;
		END IF;
	END;
$BODY$
LANGUAGE 'plpgsql' STABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.readField
(
	whatFile oid,
	iNumber integer, -- number of rows in i-direction
	placepoints refcursor
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS 
$BODY$
DECLARE
	fd integer;
	idx integer;
	pos integer := 0;
	readSize CONSTANT int4 := 4;
	p __WCI_SCHEMA__.placepoint;
	ret __WCI_SCHEMA__.extractGridDataReturnType;
BEGIN
		fd := lo_open( whatFile, 262144 ); -- 262144 = "INV_READ"
		IF -1 = fd THEN
			RAISE EXCEPTION 'Cannot read grid: %', whatFile;
		END IF;
		LOOP
			FETCH placepoints INTO p;
			EXIT WHEN NOT FOUND;
			--RAISE DEBUG 'Point %,% ', p.i, p.j;
			idx := ((iNumber * p.j) + p.i) * readSize;
			IF pos != idx THEN
				pos := lo_lseek( fd, idx, 0 ); -- 0 = "SEEK_SET"
			END IF;
			ret := ( p.location, __WCI_SCHEMA__.binary_toReal( loread( fd, readSize ) ), p.i, p.j );
			pos := pos + readSize;
			RETURN NEXT ret;
		END LOOP;
		PERFORM lo_close( fd );	
		RETURN;
END;
$BODY$
LANGUAGE plpgsql STABLE;

-- Extract values for all points in the provided floatgridvalue, along with 
-- their exact geometry.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.extractGridData
(
	data __WCI_SCHEMA__.oidValue 
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
	DECLARE
		fd integer;
		readSize CONSTANT int4 := 4;
		ret __WCI_SCHEMA__.extractGridDataReturnType;

		placepoint __WCI_SCHEMA__.placepoint;

		curs refcursor;
	BEGIN
		fd := lo_open( data.value, 262144 ); -- 262144 = "INV_READ"
		IF -1 = fd THEN
			RAISE EXCEPTION 'Cannot read grid: %', data.value;
		END IF;

		FOR placepoint IN SELECT * FROM __WCI_SCHEMA__.placepoint WHERE placeid=data.placeid ORDER BY j, i LOOP
			ret := ( placepoint.location, __WCI_SCHEMA__.binary_toReal( loread( fd, readSize ) ), placepoint.i, placepoint.j );
			RETURN NEXT ret;
		END LOOP;
		PERFORM lo_close( fd );
	END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;
