-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2008 met.no
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

-- Note: ~* is used instead of = in the query over the array.
-- This is to ensure that the function is case-insensitive

-- Determine whether a set of dataproviders contains data extracted from
-- database grids
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.hasGrid
(
	 dataprov	 		text[]
)
RETURNS boolean AS
$BODY$
DECLARE
	grid	integer;
BEGIN
	IF dataprov	IS NULL THEN
		RETURN TRUE;
	END IF;
	IF array_upper( dataprov, 1 ) IS NULL THEN
		RAISE EXCEPTION 'Invalid number of dimensions in dataprovider array';
	END IF;
	SELECT count(*) INTO grid 
	FROM __WCI_SCHEMA__.dataprovider_mv
	WHERE dataprovidername ~* ANY (dataprov)
	  AND ( spatialdomaindelivery='Grid'
	   OR spatialdomaindelivery='Any' );
	RETURN ( grid > 0 );
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;

-- Determine whether a set of dataproviders contains data extracted 
-- from individual data points.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.hasPoint
(
	 dataprov	 		text[]
)
RETURNS boolean AS
$BODY$
DECLARE
	point	integer;
BEGIN
	IF dataprov IS NULL THEN 
		RETURN TRUE;
	END IF;
	IF array_upper( dataprov, 1 ) IS NULL THEN
		RAISE EXCEPTION 'Invalid number of dimensions in dataprovider array';
	END IF;
	SELECT count(*) INTO point
	FROM __WCI_SCHEMA__.dataprovider_mv
	WHERE dataprovidername ~* ANY (dataprov)
	  AND ( spatialdomaindelivery='Point'
	   OR spatialdomaindelivery='Any' );
	RETURN ( point > 0 );
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;
