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


CREATE TABLE __WDB_SCHEMA__.gribgeneratingprocess
(
    dataproviderid  				bigint NOT NULL,
    generatingcenterid 				integer NOT NULL,
    generatingprocessid 			integer NOT NULL,
    generatingprocessvalidfrom 		timestamp with time zone NOT NULL,
    generatingprocessvalidto 		timestamp with time zone NOT NULL	
);

REVOKE ALL ON __WDB_SCHEMA__.gribgeneratingprocess FROM public;
GRANT ALL ON __WDB_SCHEMA__.gribgeneratingprocess TO wdb_admin;

ALTER TABLE ONLY __WDB_SCHEMA__.gribgeneratingprocess
    ADD CONSTRAINT gribgeneratingprocess_pkey PRIMARY KEY (dataproviderid);

ALTER TABLE __WDB_SCHEMA__.gribgeneratingprocess
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;

CREATE UNIQUE INDEX XAK1Wdb_GribGeneratingprocess ON __WDB_SCHEMA__.gribgeneratingprocess
(
	GeneratingCenterId,
	GeneratingProcessId,
	GeneratingProcessValidFrom
);



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
	valueparameterunit character varying(80) NOT NULL,
    loadvalueflag boolean NOT NULL
);

ALTER TABLE gribload.valueparameterxref
	ADD FOREIGN KEY (valueparameterunit)
					REFERENCES __WDB_SCHEMA__.unit(unitname)
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON gribload.valueparameterxref FROM public;
GRANT ALL ON gribload.valueparameterxref TO wdb_admin;
GRANT SELECT ON gribload.valueparameterxref TO wdb_grib;


CREATE TABLE gribload.levelparameterxref (
    griblevelparameter 	integer NOT NULL,
    levelparameterid 	integer NOT NULL,
	levelparameterunit	character varying(80) NOT NULL,
    loadlevelflag 		boolean NOT NULL
);

ALTER TABLE gribload.levelparameterxref
	ADD FOREIGN KEY (levelparameterunit)
					REFERENCES __WDB_SCHEMA__.unit(unitname)
					ON DELETE CASCADE
					ON UPDATE CASCADE;

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


CREATE TYPE gribload.valueparameter AS (
	valueparameterid int,
	valueparameterunit text
);

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
RETURNS SETOF gribload.valueparameter AS
$BODY$
DECLARE
	ret gribload.valueparameter;
	load boolean;
BEGIN
	SELECT valueparameterid, valueparameterunit, loadvalueflag INTO ret.valueparameterid, ret.valueparameterunit, load
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
		ret.valueparameterid = -1;
		RETURN NEXT ret;
	END IF;
	IF load = true THEN
		RETURN NEXT ret;
	END IF;
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


CREATE TYPE gribload.levelparameter AS (
	levelparameterid int,
	levelparameterunit text
);

--
-- LevelParameter XREF
--
CREATE OR REPLACE FUNCTION 
gribload.getlevelparameter( levelParam integer )
RETURNS SETOF gribload.levelparameter AS
$BODY$
DECLARE
	ret gribload.levelparameter;
	load boolean;
BEGIN
	SELECT levelparameterid, levelparameterunit, loadlevelflag INTO ret.levelparameterid, ret.levelparameterunit, load
	FROM gribload.levelparameterxref
	WHERE
		griblevelparameter = levelParam;
	-- Check load
	IF load = false THEN
		ret.levelparameterid = -1;
		RETURN NEXT ret;
	END IF;
	IF load = true THEN
		RETURN NEXT ret;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql';


