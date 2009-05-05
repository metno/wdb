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


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBilinearInterpolationData
(
	i float8,
	j float8,
	iNumber integer,
	field bigint
)
RETURNS float4 AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getBilinearInterpolationData'
LANGUAGE C IMMUTABLE
SECURITY DEFINER;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.bilinearInterpolation
(
	placeid		bigint,
	location	GEOMETRY, 
	valueoid	bigint
)
RETURNS __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
DECLARE
	i double precision;
	j double precision;
	iNumber integer;
	jNumber integer;
	val float4;
	ret __WCI_SCHEMA__.extractGridDataReturnType;
BEGIN
	SELECT * INTO i, j, iNumber, jNumber FROM __WCI_SCHEMA__.getExactIJ( location, placeid );
	
	IF i < 0 OR j < 0 OR i >= iNumber-1 OR j >= jNumber-1 THEN
		RETURN NULL;
	END IF;
	
	val := __WCI_SCHEMA__.getBilinearInterpolationData(i, j, iNumber, valueoid );
	ret := (location, val, -1, -1);
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql STABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getSinglePointData
(
	i integer,
	j integer,
	iNumber integer,
	field bigint
)
RETURNS float4 AS
$BODY$
DECLARE
	idx int := i + (j * iNumber);
BEGIN
	RETURN read_float_from_file(field, idx);
END;
$BODY$
LANGUAGE plpgsql STABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.nearestInterpolation
(
	placeid		bigint,
	location	GEOMETRY, 
	valueoid	bigint
)
RETURNS __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
DECLARE
	i double precision;
	j double precision;
	i_i integer;
	i_j integer;
	iNumber integer;
	jNumber integer;
	val float4;
	ret __WCI_SCHEMA__.extractGridDataReturnType;
BEGIN
	SELECT * INTO i, j, iNumber, jNumber FROM __WCI_SCHEMA__.getExactIJ( location, placeid );
	i_i := round(i); -- using round before cursor query improves 
	i_j := round(j); -- performance by a factor of 50-300, for some reason
	RAISE DEBUG 'NEAREST: %, %, %, %', i_i, i_j, iNumber, valueoid;
	--SELECT * INTO ret FROM __WCI_SCHEMA__.placepoint WHERE __WCI_SCHEMA__.placepoint.placeid = placeid AND __WCI_SCHEMA__.placepoint.i = i_i AND __WCI_SCHEMA__.placepoint.j = i_j;
	val := __WCI_SCHEMA__.getNearestInterpolationData( i_i, i_j, iNumber, valueoid );
	ret := (location, val, i_i, i_j);
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql STABLE;
