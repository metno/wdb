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
CREATE FUNCTION __WCI_SCHEMA__.timeIndeterminateType_in( cstring )
    RETURNS __WCI_SCHEMA__.timeIndeterminateType
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'timeIndeterminateType_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION __WCI_SCHEMA__.timeIndeterminateType_out( __WCI_SCHEMA__.timeIndeterminateType )
    RETURNS cstring
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'timeIndeterminateType_out'
    LANGUAGE C IMMUTABLE STRICT;

-- Describes the "fuzziness" of a time range specification.
--
-- There are currently two valid values for indeterminate type: 'exact' and 
-- 'any'.
-- 'exact' means a time range of exactly timefrom to timeto, or a single point 
-- of time between timefrom and timeto.
-- 'any' will match anything, regardless of the values of timefrom and timeto.
CREATE TYPE __WCI_SCHEMA__.timeIndeterminateType (
   internallength = 4,
   input = __WCI_SCHEMA__.timeIndeterminateType_in,
   output = __WCI_SCHEMA__.timeIndeterminateType_out,
   -- receive = int4recv,
   -- send = int4send, --does not work
   alignment = int,
   PASSEDBYVALUE
);

CREATE CAST ( __WCI_SCHEMA__.timeIndeterminateType AS int ) WITHOUT FUNCTION AS IMPLICIT;
CREATE CAST ( int AS __WCI_SCHEMA__.timeIndeterminateType ) WITHOUT FUNCTION;


-- A specification of a time range, along with a specification of the time 
-- range's "fuzziness".
--CREATE TYPE __WCI_SCHEMA__.timeSpec AS (
--	timefrom timestamp with time zone,
--	timeto timestamp with time zone,
--	indeterminate __WCI_SCHEMA__.timeIndeterminateType
--);

CREATE TYPE __WCI_SCHEMA__.timeSpec AS (
	indeterminate __WCI_SCHEMA__.timeIndeterminateType,
	timefrom timestamp with time zone,
	timeto timestamp with time zone,
	timeInterval interval
);


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.refineTimeSpec(spec __WCI_SCHEMA__.timeSpec)
RETURNS __WCI_SCHEMA__.timeSpec AS
$BODY$
DECLARE
	ret __WCI_SCHEMA__.timeSpec;
BEGIN
	ret.indeterminate := spec.indeterminate;
	ret.timefrom := spec.timefrom;
	IF spec.timeto IS NULL THEN
		ret.timeto := ret.timefrom + spec.timeInterval;
	ELSE
		ret.timeto := spec.timeto;
	END IF;
	
	IF ret.timefrom > ret.timeto THEN
	DECLARE
		tmp timestamp with time zone;
	BEGIN
		tmp := ret.timefrom;
		ret.timefrom := ret.timeto;
		ret.timeto := tmp;
	END;
	END IF;
	
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.timespecification(spec text)
RETURNS __WCI_SCHEMA__.timeSpec
AS '__WDB_LIBDIR__/__WCI_LIB__', 'timeSpec'
LANGUAGE C IMMUTABLE STRICT;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getTimeSpec(spec text)
RETURNS __WCI_SCHEMA__.timeSpec AS
$BODY$
DECLARE
	tmpStore __WCI_SCHEMA__.timeSpec;
BEGIN
	tmpStore := __WCI_SCHEMA__.refineTimeSpec(__WCI_SCHEMA__.timespecification(spec));
	RETURN tmpStore;
END;
$BODY$
LANGUAGE plpgsql IMMUTABLE STRICT;

-- Does the given time spec match timeFrom to timeTo?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.matches( timeFrom timestamp with time zone, timeTo timestamp with time zone, timeIndCode integer, what __WCI_SCHEMA__.timeSpec ) RETURNS boolean AS
$BODY$
DECLARE
	timeF timestamp with time zone;
	timeT timestamp with time zone;
BEGIN
	timeF := timeFrom;
	timeT := timeTo;
	IF what.indeterminate IS NULL 
	THEN
		RETURN TRUE;
	END IF;
	IF what.timeFrom > what.timeTo
	THEN
		RAISE EXCEPTION 'Timespec FROM cannot be after TO.';
	END IF;
	IF what.indeterminate = 'any'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN TRUE;
	END IF;

	-- Handle Time Indeterminate Code of Data  
	-- Note: timeIndCode = 2 is treated like 1 ('exact').
	IF timeIndCode < 1 OR timeIndCode > 2
	THEN
		IF timeIndCode = 0  
		THEN
			timeF := -infinity; 
			timeT := infinity;
		ELSIF timeIndCode = 4
		THEN
			timeF := -infinity;
		ELSIF timeIndCode = 5
		THEN
			timeT := infinity;
		ELSIF timeIndCode >= 6
		THEN
			RETURN FALSE;
		ELSIF timeIndCode = 3
		THEN
			-- Outside Time: Not implemented yet.
			RAISE EXCEPTION 'Time Indeterminate code not implemented: %', timeIndCode;			
		END IF;
	END IF;
	
	-- Missing: after, before, overlaps, overlapsbegin, overlapsend
	IF what.indeterminate = 'exact'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN timeF = what.timeFrom AND timeT = what.timeTo;
	ELSIF what.indeterminate = 'inside'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN timeFrom >= what.timeFrom AND timeTo <= what.timeTo;
	END IF;

	RAISE EXCEPTION 'Unknown value for __WCI_SCHEMA__.timeSpec.indeterminate: %', what;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;




-- Does the given time spec match timeFrom to timeTo?
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.matches( 
	timeFrom timestamp with time zone, 
	timeTo timestamp with time zone, 
	what __WCI_SCHEMA__.timeSpec 
)
RETURNS boolean AS
$BODY$
BEGIN
	IF what.indeterminate IS NULL 
	THEN
		RETURN TRUE;
	END IF;
	
	IF what.timeFrom > what.timeTo
	THEN
		RAISE EXCEPTION 'Timespec FROM cannot be after TO.';
	END IF;

	-- Missing: after, before, overlaps, overlapsbegin, overlapsend
	IF what.indeterminate = 'exact'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN timeFrom = what.timeFrom AND timeTo = what.timeTo;
	ELSIF what.indeterminate = 'inside'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN timeFrom >= what.timeFrom AND timeTo <= what.timeTo;
	ELSIF what.indeterminate = 'contains'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN TRUE;
	ELSIF what.indeterminate = 'any'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN TRUE;
	END IF;

	RAISE EXCEPTION 'Unknown value for __WCI_SCHEMA__.timeSpec.indeterminate: %', what;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;

-- Test matches
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.matches( 
	timeFrom timestamp with time zone, 
	timeTo timestamp with time zone, 
	what __WCI_SCHEMA__.timeSpec 
)
RETURNS boolean AS
$BODY$
BEGIN
	IF (what.indeterminate IS NULL) 
       OR 
	   (what.indeterminate = 'exact'::__WCI_SCHEMA__.timeIndeterminateType AND
	   timeFrom = what.timeFrom AND timeTo = what.timeTo)
       OR
	   (what.indeterminate = 'inside'::__WCI_SCHEMA__.timeIndeterminateType AND
		timeFrom >= what.timeFrom AND timeTo <= what.timeTo) 
       OR
	   (what.indeterminate = 'contains'::__WCI_SCHEMA__.timeIndeterminateType)
	   OR
		(what.indeterminate = 'any'::__WCI_SCHEMA__.timeIndeterminateType)
	THEN
		RETURN TRUE;
	ELSE
		RETURN FALSE;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;
-- Test matches

-- Does the given time spec match whatTime?
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.matches(
	whatTime timestamp with time zone,
	what __WCI_SCHEMA__.timeSpec ) 
RETURNS boolean AS
$BODY$
BEGIN
	IF what.indeterminate IS NULL 
	THEN
		RETURN TRUE;
	END IF;

	IF what.timeFrom > what.timeTo
	THEN
		RAISE EXCEPTION 'Timespec FROM cannot be after TO.';
	END IF;

	IF what.indeterminate = 'any'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN TRUE;
	ELSIF what.indeterminate = 'exact'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN what.timeFrom = whatTime AND whatTime = what.timeTo;
	ELSIF what.indeterminate = 'inside'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN what.timeFrom <= whatTime AND whatTime <= what.timeTo;
	ELSIF what.indeterminate = 'contains'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN what.timeFrom <= whatTime AND whatTime <= what.timeTo;
	ELSIF what.indeterminate = 'after'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN what.timeFrom <= whatTime;
	ELSIF what.indeterminate = 'before'::__WCI_SCHEMA__.timeIndeterminateType THEN
		RETURN whatTime <= what.timeTo;
	END IF;

	RAISE EXCEPTION 'Unknown value for __WCI_SCHEMA__.timeSpec.indeterminate: %', what;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;
