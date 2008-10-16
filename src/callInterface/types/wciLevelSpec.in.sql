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
-- Input / Output
CREATE FUNCTION wci.levelIndeterminateType_in( cstring )
    RETURNS wci.levelIndeterminateType
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'levelIndeterminateType_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION wci.levelIndeterminateType_out( wci.levelIndeterminateType )
    RETURNS cstring
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'levelIndeterminateType_out'
    LANGUAGE C IMMUTABLE STRICT;

-- Describes the "fuzziness" of a level range specification.
--
-- There are currently four valid values for indeterminate type: 'exact', 
-- 'below', 'above' and 'any'.
-- 'exact' means a level range of exactly levelfrom to levelto, or a single 
-- level between levelfrom and levelto. Also level parameter must be given 
-- correctly.
-- 'above' means any level which is below or on levelto. levelparameter must 
-- also match.
-- 'below' means any level which is above or on levelfrom. levelparameter must 
-- also match.
-- 'any' will match anything, regardless of the values of levelfrom, levelto 
-- or levelparameter.
CREATE TYPE wci.levelIndeterminateType (
   internallength = 4,
   input = wci.levelIndeterminateType_in,
   output = wci.levelIndeterminateType_out,
   -- receive = int4recv,
   -- send = int4send, --does not work
   alignment = int,
   PASSEDBYVALUE
);

CREATE CAST ( wci.levelIndeterminateType AS int ) WITHOUT FUNCTION AS IMPLICIT;
CREATE CAST ( int AS wci.levelIndeterminateType ) WITHOUT FUNCTION;


-- Specifies a level range, including a single parameter. Also the level's 
-- "fuzziness" may be specified, through the indeterminate type.
CREATE TYPE wci.levelSpec AS (
	levelfrom real,
	levelto real,
	levelparametername text,
	indeterminate wci.levelIndeterminateType
);


-- Does the given level spec match level parameter, from and to?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.matches( levelFrom real, levelTo real, levelparametername text, what wci.levelSpec ) 
RETURNS boolean AS
$BODY$
BEGIN
	
	IF what.indeterminate IS NULL 
	THEN
		RETURN TRUE;
	END IF;

	IF levelparametername != what.levelparametername THEN
		RETURN FALSE;
		
	ELSIF what.indeterminate = 'exact'::wci.levelIndeterminateType THEN
		RETURN levelFrom >= what.levelFrom AND levelTo <= what.levelTo AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'below'::wci.levelIndeterminateType THEN
		RETURN levelTo <= what.levelTo AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'above'::wci.levelIndeterminateType THEN
		RETURN levelFrom >= what.levelFrom AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'any'::wci.levelIndeterminateType THEN
		RETURN TRUE;
		
	END IF;

	RAISE EXCEPTION 'Unknown value for wci.levelSpec.indeterminate: %', what;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;
