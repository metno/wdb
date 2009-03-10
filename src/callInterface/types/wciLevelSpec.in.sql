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
CREATE FUNCTION __WCI_SCHEMA__.levelIndeterminateType_in( cstring )
    RETURNS __WCI_SCHEMA__.levelIndeterminateType
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'levelIndeterminateType_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION __WCI_SCHEMA__.levelIndeterminateType_out( __WCI_SCHEMA__.levelIndeterminateType )
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
CREATE TYPE __WCI_SCHEMA__.levelIndeterminateType (
   internallength = 4,
   input = __WCI_SCHEMA__.levelIndeterminateType_in,
   output = __WCI_SCHEMA__.levelIndeterminateType_out,
   -- receive = int4recv,
   -- send = int4send, --does not work
   alignment = int,
   PASSEDBYVALUE
);

CREATE CAST ( __WCI_SCHEMA__.levelIndeterminateType AS int ) WITHOUT FUNCTION AS IMPLICIT;
CREATE CAST ( int AS __WCI_SCHEMA__.levelIndeterminateType ) WITHOUT FUNCTION;


-- Specifies a level range, including a single parameter. Also the level's 
-- "fuzziness" may be specified, through the indeterminate type.
CREATE TYPE __WCI_SCHEMA__.levelSpec AS (
	levelfrom real,
	levelto real,
	levelparametername text,
	indeterminate __WCI_SCHEMA__.levelIndeterminateType
);

CREATE FUNCTION 
__WCI_SCHEMA__.getLevelSpec(spec text)
RETURNS __WCI_SCHEMA__.levelSpec 
AS '__WDB_LIBDIR__/__WCI_LIB__', 'levelSpec'
LANGUAGE C IMMUTABLE STRICT;

-- Does the given level spec match level parameter, from and to?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.matches( levelFrom real, levelTo real, levelparametername text, what __WCI_SCHEMA__.levelSpec ) 
RETURNS boolean AS
$BODY$
BEGIN
	
	IF what.indeterminate IS NULL 
	THEN
		RETURN TRUE;
	END IF;

	IF levelparametername != what.levelparametername THEN
		RETURN FALSE;
		
	ELSIF what.indeterminate = 'exact'::__WCI_SCHEMA__.levelIndeterminateType THEN
		RETURN levelFrom >= what.levelFrom AND levelTo <= what.levelTo AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'below'::__WCI_SCHEMA__.levelIndeterminateType THEN
		RETURN levelTo <= what.levelTo AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'above'::__WCI_SCHEMA__.levelIndeterminateType THEN
		RETURN levelFrom >= what.levelFrom AND levelparametername = what.levelparametername;
		
	ELSIF what.indeterminate = 'any'::__WCI_SCHEMA__.levelIndeterminateType THEN
		RETURN TRUE;
		
	END IF;

	RAISE EXCEPTION 'Unknown value for __WCI_SCHEMA__.levelSpec.indeterminate: %', what;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;
