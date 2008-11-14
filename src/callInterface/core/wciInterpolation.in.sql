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
	placeid bigint,
	field oid
)
RETURNS float4 AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getBilinearInterpolationData'
LANGUAGE C IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.bilinearInterpolation
(
	placeid		bigint,
	location	GEOMETRY, 
	valueoid	oid
)
RETURNS __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
DECLARE
	i double precision;
	j double precision;
	val float4;
	ret __WCI_SCHEMA__.extractGridDataReturnType;
BEGIN
	SELECT * INTO i, j FROM __WCI_SCHEMA__.getExactIJ( location, placeid );
	val := __WCI_SCHEMA__.getBilinearInterpolationData(i, j, placeid, valueoid );
	ret := (location, val, -1, -1);
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql STABLE;
