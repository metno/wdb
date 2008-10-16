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

-- gribload is a schema that contains views, functions and tables  
-- that are specific to the GribLoad component 
CREATE SCHEMA gribload;
REVOKE ALL ON SCHEMA gribload FROM PUBLIC;
GRANT ALL ON SCHEMA gribload TO wdb_admin;
GRANT USAGE ON SCHEMA gribload TO wdb_grib;


CREATE TABLE gribload.valueparameterxref (
    generatingcenterid integer NOT NULL,
    gribcodetable2version integer NOT NULL,
    gribparameter integer NOT NULL,
    gribtimerange integer NOT NULL,
    gribparameterthresholdindicator integer NOT NULL,
    gribparameterthresholdlower integer NOT NULL,
    gribparameterthresholdupper integer NOT NULL,
    gribparameterthresholdscale integer NOT NULL,
    griblevelparameter integer NOT NULL,
    valueparameterid integer NOT NULL,
    loadvalueflag boolean NOT NULL
);

REVOKE ALL ON gribload.valueparameterxref FROM public;
GRANT ALL ON gribload.valueparameterxref TO wdb_admin;
GRANT SELECT ON gribload.valueparameterxref TO wdb_grib;


CREATE TABLE gribload.levelparameterxref (
    griblevelparameter integer NOT NULL,
    levelparameterid integer NOT NULL,
    loadlevelflag boolean NOT NULL
);

REVOKE ALL ON gribload.levelparameterxref FROM public;
GRANT ALL ON gribload.levelparameterxref TO wdb_admin;
GRANT SELECT ON gribload.levelparameterxref TO wdb_grib;


CREATE TABLE gribload.parametertolevelxref (
    generatingcenterid integer NOT NULL,
    gribcodetable2version integer NOT NULL,
    gribparameter integer NOT NULL,
    gribtimerange integer NOT NULL,
    gribparameterthresholdindicator integer NOT NULL,
    gribparameterthresholdlower integer NOT NULL,
    gribparameterthresholdupper integer NOT NULL,
    gribparameterthresholdscale integer NOT NULL,
    levelparameterid integer NOT NULL,
    levelFrom real NOT NULL,
    levelTo real NOT NULL,
    levelIndeterminateCode integer NOT NULL,
    loadlevelflag boolean NOT NULL
);

REVOKE ALL ON gribload.parametertolevelxref FROM public;
GRANT ALL ON gribload.parametertolevelxref TO wdb_admin;
GRANT SELECT ON gribload.parametertolevelxref TO wdb_grib;



--
-- DataProvider XREF
--
CREATE VIEW gribload.gribgeneratingprocess AS
SELECT
    dataproviderid,
    generatingcenterid,
    generatingprocessid,
    generatingprocessvalidfrom,
    generatingprocessvalidto	
FROM
	__WDB_SCHEMA__.gribgeneratingprocess;

REVOKE ALL ON gribload.gribgeneratingprocess FROM public;
GRANT ALL ON gribload.gribgeneratingprocess TO wdb_admin;
GRANT SELECT ON gribload.gribgeneratingprocess TO wdb_grib;


CREATE OR REPLACE FUNCTION 
gribload.getdataprovider(
	genCenter integer,
	genProcess integer,
	refTime timestamp with time zone
)
RETURNS SETOF bigint AS
$BODY$
	SELECT dataproviderid
	FROM gribload.gribgeneratingprocess
	WHERE
		generatingcenterid = $1 AND
		generatingprocessid = $2 AND
		generatingprocessvalidfrom <= $3 AND
		generatingprocessvalidto >= $3;
$BODY$
LANGUAGE 'sql';


--
-- Add new placeId
--
CREATE OR REPLACE FUNCTION 
gribload.setplaceid(
	placegeo_ text,
	placesrid_ integer,
	inumber_ integer,
	jnumber_ integer,
	iincrement_ real,
	jincrement_ real,
	startLon_ real,
	startLat_ real,
	origsrid_ integer
)
RETURNS bigint AS
$BODY$
DECLARE
	ret bigint;
BEGIN
--	INSERT INTO __WDB_SCHEMA__.placedefinition ( placeindeterminatecode, placegeometrytype, placestoretime, placegeometry)
--	VALUES ( 0, 'Grid', CURRENT_TIMESTAMP, geomfromtext(placegeo_, 4030) );

	SELECT nextval ( '__WDB_SCHEMA__.placedefinition_placeid_seq' ) INTO ret;
	
	INSERT INTO __WDB_SCHEMA__.placeregulargrid ( placeid, inumber, jnumber, iincrement, jincrement, startlongitude, startlatitude, originalsrid)
	VALUES ( ret, inumber_, jnumber_, iincrement_, jincrement_, startLon_, startLat_, origsrid_ );

	INSERT INTO __WDB_SCHEMA__.placename ( placeid, placenamespaceid, placename, placenamevalidfrom, placenamevalidto )
	VALUES ( ret, 0, 'Automatic insertion by gribLoad '::text || CURRENT_TIMESTAMP::text, CURRENT_DATE, '12-31-2999' );

	RETURN ret;
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql' STRICT VOLATILE;

--
-- ValueParameter XREF
--
CREATE OR REPLACE FUNCTION 
gribload.getvalueparameter(
	genCenter integer,
	codeTable2 integer,
	gribParam integer,
	gribTR integer,
	gribPTI integer,
	gribPTL integer,
	gribPTU integer,
	gribPTS integer,
	gribLevel integer
)
RETURNS SETOF integer AS
$BODY$
DECLARE
	ret integer;
	load boolean;
BEGIN
	SELECT valueparameterid, loadvalueflag INTO ret, load
	FROM gribload.valueparameterxref
	WHERE
		generatingcenterid = genCenter AND
		gribcodetable2version = codeTable2 AND
		gribparameter = gribParam AND
        gribtimerange = gribTR AND 
        GribParameterThresholdIndicator = gribPTI AND
		GribParameterThresholdLower = gribPTL AND
		GribParameterThresholdUpper = gribPTU AND
		GribParameterThresholdScale = gribPTS AND
		GribLevelParameter = gribLevel;
	-- Check load
	IF load = false THEN
		RETURN NEXT -1;
	END IF;
	IF load = true THEN
		RETURN NEXT ret;
	END IF;
	RETURN;
END;
$BODY$
LANGUAGE 'plpgsql' STRICT STABLE;


--
-- Get additional level information
-- Dependent on the parameter!
--
CREATE OR REPLACE FUNCTION 
gribload.getlevelfromparameter(
	genCenter integer,
	codeTable2 integer,
	gribParam integer,
	gribTR integer,
	gribPTI integer,
	gribPTL integer,
	gribPTU integer,
	gribPTS integer )
RETURNS SETOF gribload.parametertolevelxref AS
$BODY$
	SELECT *
	FROM gribload.parametertolevelxref
	WHERE
		GeneratingCenterId = $1 AND
		GribCodeTable2Version = $2 AND
		GribParameter = $3 AND
		GribTimeRange = $4 AND
		GribParameterThresholdIndicator = $5 AND
		GribParameterThresholdLower = $6 AND
		GribParameterThresholdUpper = $7 AND
		GribParameterThresholdScale = $8;
$BODY$
LANGUAGE 'sql';


--
-- LevelParameter XREF
--
CREATE OR REPLACE FUNCTION 
gribload.getlevelparameter( levelParam integer )
RETURNS SETOF integer AS
$BODY$
DECLARE
	ret integer;
	load boolean;
BEGIN
	SELECT levelparameterid, loadlevelflag INTO ret, load
	FROM gribload.levelparameterxref
	WHERE
		griblevelparameter = levelParam;
	-- Check load
	IF load = false THEN
		RETURN NEXT -1;
	END IF;
	IF load = true THEN
		RETURN NEXT ret;
	END IF;
	RETURN;
END;
$BODY$
LANGUAGE 'plpgsql';


