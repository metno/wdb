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

CREATE TYPE wci.infodataprovider AS 
(
	dataprovidername		text,
	dataprovidertype		character varying(80),
	spatialdomaindelivery	character varying(80),
	dataprovidercomment		character varying(254),
	dataproviderstoretime 	timestamp with time zone
);

CREATE TYPE wci.infoplace AS 
(
	placename				text,
	placegeometry			text,
	placeindeterminatetype	character varying(80),
	placegeometrytype		character varying(80),
	placestoretime			timestamp with time zone
);

CREATE TYPE wci.inforegulargrid AS
(
	placename 		text,
	numberX			int,
	numberY			int,
	incrementX 		real,
	incrementY 		real,
	startX			real,
	startY			real,
	projDefinition 	text
);

CREATE TYPE wci.infovalueparameter AS 
(
	valueparametername	text,
	valueunitname		text
);

CREATE TYPE wci.infolevelparameter AS 
(
	levelparametername	text,
	levelunitname		text
);

CREATE TYPE wci.infoparameterunit AS 
(
	unitname						text,
	unittype						text,
	measure							text,
	description						text,
	siunitconversioncoefficient		float,
	siunitconversionterm			float
);



-- dataprovider info by name
-- returns wci.infodataprovider
CREATE OR REPLACE FUNCTION 
wci.info( dataprovider 		text, 
		  returntype 		 wci.infodataprovider )	
RETURNS SETOF wci.infodataprovider AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.infodataprovider;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT dataprovidername, dataprovidertype, spatialdomaindelivery,
						 dataprovidercomment, dataproviderstoretime 
			      FROM   __WCI_SCHEMA__.dataprovider_mv dp 
						 INNER JOIN __WCI_SCHEMA__.getSessionData() s ON (dp.dataprovidernamespaceid = s.dataprovidernamespaceid)';
						 --LEFT OUTER JOIN __WDB_SCHEMA__.dataprovidercomment dc ON (dp.dataproviderid = dc.dataproviderid)';
	IF dataprovider IS NOT NULL THEN
		infoQuery := infoQuery || ' WHERE dataprovidername LIKE lower($$' || dataprovider || '$$)';
	END IF;
	RAISE DEBUG 'WCI.INFO.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



-- place info by name
-- returns wci.infoplace
CREATE OR REPLACE FUNCTION 
wci.info( location 			text,
		  returntype 		wci.infoplace
)	
RETURNS SETOF wci.infoplace AS
$BODY$
DECLARE
	infoQuery 		text;
	entry 			wci.infoplace;
BEGIN
	-- Create Query to Run
	infoQuery := 'SELECT placename, 
						 astext(placegeometry), 
						 placeindeterminatetype, 
						 placegeometrytype,
						 placestoretime 
				  FROM __WCI_SCHEMA__.placedefinition_mv ps,
					   __WCI_SCHEMA__.placeindeterminatetype pi,
					   __WCI_SCHEMA__.getSessionData() s 
				  WHERE  
						s.placenamespaceid = ps.placenamespaceid
						AND pi.placeindeterminatecode = ps.placeindeterminatecode';

	IF location IS NOT NULL THEN
		infoQuery := infoQuery || ' AND placename LIKE lower($$' || location || '$$)';
	END IF;
	RAISE DEBUG 'WCI.INFO.Query: %', infoQuery;

	<<main_select>>
	FOR entry IN
		EXECUTE infoQuery
	LOOP
		RETURN NEXT entry;
	END LOOP main_select;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;


CREATE OR REPLACE FUNCTION 
wci.info( placename_ 		text,
		  returntype 		wci.inforegulargrid
)	
RETURNS SETOF wci.inforegulargrid AS
$BODY$
DECLARE
	entry 			wci.inforegulargrid;
BEGIN
	IF placename_ IS NULL THEN
		FOR entry IN 
		SELECT
			placename,
			numberX,
			numberY,
			incrementX,
			incrementY,
			startX,
			startY,
			projDefinition
		FROM 
			__WCI_SCHEMA__.placespec 
		LOOP
			RETURN NEXT entry;
		END LOOP;
	ELSE
		FOR entry IN 
		SELECT
			placename,
			numberX,
			numberY,
			incrementX,
			incrementY,
			startX,
			startY,
			projDefinition
		FROM 
			__WCI_SCHEMA__.placespec
		WHERE
			placename_=lower(placename)
		LOOP
			RETURN NEXT entry;
		END LOOP;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



-- place info by name
-- returns wci.infovalueparameter
--   valueparametername
--   valueunitname
--   count 
CREATE OR REPLACE FUNCTION
wci.info( parameter 		text,
		  returntype 		wci.infovalueparameter )	
RETURNS SETOF wci.infovalueparameter AS
$BODY$
	SELECT 
		valueparametername, valueunitname as valueunitnameorref
	FROM 
		__WCI_SCHEMA__.valueparameter_mv vp, __WCI_SCHEMA__.getSessionData() s
	WHERE 
		vp.parameternamespaceid = s.parameternamespaceid AND
		($1 IS NULL OR valueparametername LIKE lower($1));
$BODY$
LANGUAGE sql;


-- place info by name
-- returns wci.infolevelparameter
--   levelparametername
--   levelunitname
--   levelfrom
--   levelto
--   count 
CREATE OR REPLACE FUNCTION 
wci.info( parameter 		text,
		  returntype 		wci.infolevelparameter )	
RETURNS SETOF wci.infolevelparameter AS
$BODY$
	SELECT 
		levelparametername, levelunitname as levelunitnameorref
	FROM 
		__WCI_SCHEMA__.levelparameter_mv lp, __WCI_SCHEMA__.getSessionData() s
	WHERE 
		lp.parameternamespaceid = s.parameternamespaceid AND
		($1 IS NULL OR levelparametername LIKE lower($1));
$BODY$
LANGUAGE sql;


CREATE OR REPLACE FUNCTION 
wci.info(
	parameterunit	text,
	returnType		wci.infoparameterunit	
)
RETURNS SETOF wci.infoparameterunit AS
$BODY$
	SELECT *
	FROM __WCI_SCHEMA__.unitwithconversion
	WHERE $1 = unitname;
$BODY$
SECURITY DEFINER
LANGUAGE sql;
