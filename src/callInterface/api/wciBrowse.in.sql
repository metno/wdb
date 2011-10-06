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

CREATE TYPE wci.browsedataprovider AS 
(
	dataprovidername	text,
	referencetimefrom 	timestamp with time zone,
	referencetimeto 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browseplace AS 
(
	placename			text,
	referencetimefrom 	timestamp with time zone,
	referencetimeto 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsereferencetime AS 
(
	referencetime	 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsevalidtime AS 
(
	validtimefrom 		timestamp with time zone,
	validtimeto	 		timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsevalueparameter AS 
(
	valueparametername	text,
	valueunitname		text,
	numberoftuples 		integer
);

CREATE TYPE wci.browselevelparameter AS 
(
	levelparametername	text,
	levelunitname		text,
	levelfrom		 	real,
	levelto			 	real,
	numberoftuples 		integer
);

CREATE TYPE wci.browsedataversion AS
(
	dataversion integer,
	numberoftuples integer
);



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
	query := __WCI_SCHEMA__.getBrowseDataProviderQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowsePlaceQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowseReferenceTimeQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowseValidTimeQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowseValueParameterQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowseLevelParameterQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
	query := __WCI_SCHEMA__.getBrowseDataVersionQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
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
