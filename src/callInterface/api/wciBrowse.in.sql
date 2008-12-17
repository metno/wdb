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

-- browse dataprovider info
-- returns wci.browsedataprovider
--   DataProviderName
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
		    returntype 			wci.browsedataprovider
)	
RETURNS SETOF wci.browsedataprovider AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browsedataprovider;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT dataprovidername, min(referencetime), max(referencetime), count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0, 
										    dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by dataprovidername';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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
RETURNS SETOF wci.browseplace AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browseplace;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT placename, min(referencetime), max(referencetime), count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0,
										    dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by placename';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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
RETURNS SETOF wci.browsereferencetime AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browsereferencetime;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT referencetime, count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0, 
											dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by referencetime';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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
RETURNS SETOF wci.browsevalidtime AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browsevalidtime;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT validtimefrom, validtimeto, count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0,
											dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by validtimefrom, validtimeto';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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
RETURNS SETOF wci.browsevalueparameter AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browsevalueparameter;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT valueparametername, valueunitname, count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0,
											dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by valueparametername, valueunitname';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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
RETURNS SETOF wci.browselevelparameter AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.browselevelparameter;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT levelparametername, levelunitname, min(levelfrom), max(levelto), count(*) ' ||
				  __WCI_SCHEMA__.readQuery( 0,
											dataprovider,
											__WCI_SCHEMA__.getPlaceQuery(location, 0),
											__WCI_SCHEMA__.getTimeSpec(referencetime),
											__WCI_SCHEMA__.getTimeSpec(validtime),
											parameter,
											__WCI_SCHEMA__.getLevelSpec(level),
											dataversion ) ||
                  ' group by levelparametername, levelunitname';
	RAISE DEBUG 'WCI.browse.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
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


