-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2012 met.no
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
--
-- Update Schema to use Place Definition with Valid Times
-- 

SET SESSION client_min_messages TO 'warning';

DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseDataProviderQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowsePlaceQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseReferenceTimeQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseValidTimeQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseValueParameterQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseLevelParameterQuery(text[],text,text,text,text[],text,integer[]);
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getBrowseDataVersionQuery(text[],text,text,text,text[],text,integer[]);

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataProviderQueryGrid(	dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataProviderQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowsePlaceQueryGrid( dataprovider 		text[],
								    location 			text,
								    referencetime 		text,
								    validtime 			text,
								    parameter 			text[],
								    level 				text,
								    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowsePlaceQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseReferenceTimeQueryGrid( dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseReferenceTimeQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValidTimeQueryGrid( dataprovider 		text[],
									    location 			text,
									    referencetime 		text,
									    validtime 			text,
									    parameter 			text[],
									    level 				text,
									    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValidTimeQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValueParameterQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValueParameterQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseLevelParameterQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseLevelParameterQueryGrid'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQueryGrid'
LANGUAGE 'c' IMMUTABLE;





CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataProviderQueryFloat(	dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataProviderQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowsePlaceQueryFloat( dataprovider 		text[],
								    location 			text,
								    referencetime 		text,
								    validtime 			text,
								    parameter 			text[],
								    level 				text,
								    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowsePlaceQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseReferenceTimeQueryFloat( dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseReferenceTimeQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValidTimeQueryFloat( dataprovider 		text[],
									    location 			text,
									    referencetime 		text,
									    validtime 			text,
									    parameter 			text[],
									    level 				text,
									    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValidTimeQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValueParameterQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValueParameterQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseLevelParameterQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseLevelParameterQueryFloat'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQueryFloat'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsedataprovider
)
RETURNS SETOF wci.browsedataprovider
AS 
$BODY$
DECLARE
	query text;
	ret wci.browsedataprovider;
BEGIN
	query := __WCI_SCHEMA__.getBrowseDataProviderQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) || 
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseDataProviderQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browsedataprovider )	
RETURNS SETOF wci.browsedataprovider AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsedataprovider );
$BODY$
LANGUAGE 'sql' STABLE;


-- browse place info
-- returns wci.browseplace
--   PlaceName
--   min ReferenceTime
--   max ReferenceTime
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browseplace
)
RETURNS SETOF wci.browseplace
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browseplace;
BEGIN
	query := __WCI_SCHEMA__.getBrowsePlaceQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowsePlaceQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browseplace )	
RETURNS SETOF wci.browseplace AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browseplace );
$BODY$
LANGUAGE 'sql' STABLE;



-- browse referencetime
-- returns wci.referenceTime
--   ReferenceTime
--   min ReferenceTime
--   max ReferenceTime
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsereferencetime
)
RETURNS SETOF wci.browsereferencetime
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browsereferencetime;
BEGIN
	query := __WCI_SCHEMA__.getBrowseReferenceTimeQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseReferenceTimeQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browsereferencetime )	
RETURNS SETOF wci.browsereferencetime AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsereferencetime );
$BODY$
LANGUAGE 'sql' STABLE;



-- validtimes
-- returns wci.browsevalidTime
--   ReferenceTime
--   min ReferenceTime
--   max ReferenceTime
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsevalidtime
)
RETURNS SETOF wci.browsevalidtime
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browsevalidtime;
BEGIN
	query := __WCI_SCHEMA__.getBrowseValidTimeQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion)||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseValidTimeQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browsevalidtime )	
RETURNS SETOF wci.browsevalidtime AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsevalidtime );
$BODY$
LANGUAGE 'sql' STABLE;



-- value parameter
-- returns wci.browsevalueparameter
--   valueparametername
--   valueunitname
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsevalueparameter
)
RETURNS SETOF wci.browsevalueparameter
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browsevalueparameter;
BEGIN
	query := __WCI_SCHEMA__.getBrowseValueParameterQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseValueParameterQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browsevalueparameter )	
RETURNS SETOF wci.browsevalueparameter AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsevalueparameter );
$BODY$
LANGUAGE 'sql' STABLE;


-- place info by name
-- returns wci.browselevelparameter
--   levelparametername
--   levelunitname
--   levelfrom
--   levelto
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browselevelparameter
)
RETURNS SETOF wci.browselevelparameter
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browselevelparameter;
BEGIN
	query := __WCI_SCHEMA__.getBrowseLevelParameterQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseLevelParameterQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ;
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


-- Overloaded function
CREATE OR REPLACE FUNCTION 
wci.browse( returntype 			wci.browselevelparameter )	
RETURNS SETOF wci.browselevelparameter AS
$BODY$
	SELECT * 
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browselevelparameter );
$BODY$
LANGUAGE 'sql' STABLE;


--CREATE TYPE wci.browsedataversion AS
--(
--	dataversion integer,
--	numberoftuples integer
--);

-- listing available data versions
-- returns wci.browsedataversion
--   version number
--   count 
CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsedataversion
)
RETURNS SETOF wci.browsedataversion
AS 
$BODY$
DECLARE
	query 	text;
	ret 	wci.browsedataversion;
BEGIN
	query := __WCI_SCHEMA__.getBrowseDataVersionQueryGrid(dataprovider, location, referencetime, validtime, parameter, level, dataversion) ||
	' UNION ALL ' ||
	__WCI_SCHEMA__.getBrowseDataVersionQueryFloat(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



CREATE OR REPLACE FUNCTION
wci.browse(returntype wci.browsedataversion)
RETURNS SETOF wci.browsedataversion AS
$BODY$
	SELECT *
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsedataversion);
$BODY$
LANGUAGE sql STABLE;