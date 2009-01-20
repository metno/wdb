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

-- xmlload is a schema that contains views, functions and tables  
-- that are specific to the XmlLoad component 
CREATE SCHEMA xmlload;
REVOKE ALL ON SCHEMA xmlload FROM PUBLIC;
GRANT ALL ON SCHEMA xmlload TO wdb_admin;
GRANT USAGE ON SCHEMA xmlload TO wdb_xml;


--
-- Data Provider XREF
--
CREATE VIEW xmlload.dataproviderxref AS
SELECT
	dp.dataproviderid,
	dp.dataprovidertype,
	dp.spatialdomaindelivery,
	dn.dataprovidernamespaceid,
	dn.dataprovidername
FROM
	__WDB_SCHEMA__.dataprovider AS dp, 
	__WDB_SCHEMA__.dataprovidername AS dn
WHERE
	dp.dataproviderid = dn.dataproviderid;

REVOKE ALL ON xmlload.dataproviderxref FROM public;
GRANT ALL ON xmlload.dataproviderxref TO wdb_admin;
GRANT SELECT ON xmlload.dataproviderxref TO wdb_xml;


--
-- Get Data Provider Id
--
CREATE OR REPLACE FUNCTION 
xmlload.getdataproviderid(
	dataprov_ 	text,
	datans_		int
)
RETURNS SETOF bigint AS
$BODY$
	SELECT dataProviderId::bigint
	FROM   xmlload.dataproviderxref
	WHERE
		lower($1) = lower(dataprovidername) AND
		$2 = dataprovidernamespaceid;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';


--
-- PlaceDefinition XREF
--
CREATE VIEW xmlload.placexref AS
SELECT
	def.placeid, 
	def.placegeometry 
FROM
	__WDB_SCHEMA__.placedefinition def 
WHERE
	def.placegeometrytype = 'Point';

REVOKE ALL ON xmlload.placexref FROM public;
GRANT ALL ON xmlload.placexref TO wdb_admin;
GRANT SELECT ON xmlload.placexref TO wdb_xml;


--
-- Get Place Id
--
CREATE OR REPLACE FUNCTION 
xmlload.getplaceid(
	placegeo_ 	text,
	placesrid_ 	integer
)
RETURNS SETOF bigint AS
$BODY$
	SELECT PlaceId::bigint
	FROM   xmlload.placexref
	WHERE
		Equals(placegeometry, geomfromtext($1, $2));
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';


--
-- Add new placeId
--
CREATE OR REPLACE FUNCTION 
xmlload.setplaceid(
	placegeo_ 	text,
	placesrid_ 	integer
)
RETURNS bigint AS
$BODY$
DECLARE
	ret bigint;
BEGIN
	INSERT INTO __WDB_SCHEMA__.placedefinition ( placeindeterminatecode, placegeometrytype, placestoretime, placegeometry)
	VALUES ( 0, 'Point', CURRENT_TIMESTAMP, geomfromtext(placegeo_, placesrid_) );

	SELECT currval( '__WDB_SCHEMA__.placedefinition_placeid_seq' ) INTO ret;
	
	INSERT INTO __WDB_SCHEMA__.placename ( placeid, placenamespaceid, placename, placenamevalidfrom, placenamevalidto )
	VALUES ( ret, 0, 'Auto-insert xmlLoad '::text || CURRENT_TIMESTAMP::text, CURRENT_DATE, '12-31-2999' );

	RETURN ret;
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql' STRICT VOLATILE;


--
-- ValueParameter XREF
--
CREATE TABLE xmlload.valueparameterxref (
    xmlnode				character varying(80) NOT NULL,
	xmlvalue			character varying(80) NOT NULL,
	xmlunitretrieve		boolean NOT NULL,
	xmldefaultunit		character varying(80) NOT NULL,
	xmltimepointflag	boolean NOT NULL,
    valueparameterid	integer NOT NULL,
    loadvalueflag 		boolean NOT NULL
);

REVOKE ALL ON xmlload.valueparameterxref FROM public;
GRANT ALL ON xmlload.valueparameterxref TO wdb_admin;
GRANT SELECT ON xmlload.valueparameterxref TO wdb_xml;


CREATE OR REPLACE FUNCTION 
xmlload.getvalueparameter(
	xmlparam		character varying(80)
)
RETURNS SETOF xmlload.valueparameterxref AS
$BODY$
	SELECT *
	FROM xmlload.valueparameterxref
	WHERE
        xmlnode = $1;
$BODY$
LANGUAGE 'sql';


--
-- LevelParameter XREF
--
CREATE VIEW xmlload.levelparameterxref AS
SELECT
	lpp.levelparameterid,
	0 AS parameternamespaceid,
	lpp.levelparameterusage || ' ' || unit.physicalphenomenon AS levelparametername,
	unit.unitname AS levelunitname
FROM
	__WDB_SCHEMA__.levelphysicalparameter AS lpp, 
	__WDB_SCHEMA__.unit AS unit
WHERE
	unit.unitname = lpp.levelparameterunitname
UNION ALL
SELECT
	lcp.levelparameterid, 
	0 as parameternamespaceid, 
	lcp.levelcodeparametername AS levelparametername, 
	NULL as unitname 
FROM
	__WDB_SCHEMA__.levelcodeparameter as lcp;

REVOKE ALL ON xmlload.levelparameterxref FROM public;
GRANT ALL ON xmlload.levelparameterxref TO wdb_admin;
GRANT SELECT ON xmlload.levelparameterxref TO wdb_xml;


CREATE OR REPLACE FUNCTION 
xmlload.getlevelparameter(
	xmlparam		character varying(80)
)
RETURNS SETOF xmlload.levelparameterxref AS
$BODY$
	SELECT *
	FROM xmlload.levelparameterxref
	WHERE
        levelparametername = $1;
$BODY$
LANGUAGE 'sql';
