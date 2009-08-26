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

CREATE TYPE __WCI_SCHEMA__.extractGridDataIntermediateReturnType AS 
(
	location bytea,
	value real,
	i integer,
	j integer
);


CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.extractGridData
(
	placeid_		bigint,
	location 		GEOMETRY,
	interpolation 	__WCI_SCHEMA__.interpolationType, 
	dataid		bigint 
)
RETURNS SETOF __WCI_SCHEMA__.extractGridDataReturnType AS
$BODY$
DECLARE
	ps  __WCI_SCHEMA__.placespec;
	loc bytea := asBinary(location);
	intermediate __WCI_SCHEMA__.extractGridDataIntermediateReturnType;
	ret __WCI_SCHEMA__.extractgriddatareturntype;
BEGIN
	select * into ps FROM __WCI_SCHEMA__.placespec WHERE placeid=placeid_;
	FOR intermediate IN SELECT * FROM extractGridData(ps, loc, interpolation, dataid) LOOP
		ret.location = GeomFromWKB(intermediate.location);
		ret.value = intermediate.value;
		ret.i = intermediate.i;
		ret.j = intermediate.j;
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE plpgsql STABLE;

CREATE OR REPLACE FUNCTION extractGridData(
	ps  __WCI_SCHEMA__.placespec, 
	loc bytea, 
	interpolation __WCI_SCHEMA__.interpolationtype, 
	dataid bigint)
RETURNS 
	SETOF __WCI_SCHEMA__.extractGridDataIntermediateReturnType AS
'__WDB_LIBDIR__/__WCI_LIB__', 'extractGridData'
SECURITY DEFINER
LANGUAGE 'c' STABLE;
