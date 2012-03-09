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

--
-- Drop Dependent Views and Functions
--
DROP view wci_int.floatgridvalue;
DROP function wci_int.getlevelparametername(wci_int.gridvalue);
DROP function wci_int.lp_matches(wci_int.gridvalue,text[]);
DROP function wci_int.getparametername(wci_int.gridvalue);
DROP view wci_int.gridvalue;
DROP view wci_int.floatvalue;
SELECT wdb_int.dropMV('wci_int.placedefinition_mv');
DROP VIEW wci_int.placedefinition CASCADE;

--
-- Replace PlaceDefinition View
--
CREATE VIEW wci_int.placedefinition AS
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pn.placenamevalidfrom,
	pn.placenamevalidto,
	pg.originalsrid,
	pd.placestoretime
FROM 
	wdb_int.placedefinition pd, 
	wdb_int.placeregulargrid pg,
	wdb_int.placename pn
WHERE
	pd.placeid = pg.placeid
	AND pd.placeid = pn.placeid
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pn.placenamevalidfrom,
	pn.placenamevalidto,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 
	wdb_int.placedefinition pd, 
	wdb_int.placename pn
WHERE
	pd.placeid = pn.placeid
	AND pd.placegeometrytype <> 'grid' 
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	'-infinity' AS placenamevalidfrom,
	'infinity'  AS placenamevalidto,
	pg.originalsrid,
	pd.placestoretime	
FROM 	
	wdb_int.placedefinition pd, 
	wdb_int.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	lower(st_astext( pd.placegeometry )) as placename,
	'-infinity' AS placenamevalidfrom,
	'infinity'  AS placenamevalidto,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 	
	wdb_int.placedefinition pd
WHERE
	pd.placegeometrytype != 'grid';
		
REVOKE ALL ON wci_int.placedefinition FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition TO wdb_read, wdb_write;

--
-- Recreate Materialized view
--
SELECT wdb_int.createMV('wci_int.placedefinition_mv', 'wci_int.placedefinition');
SELECT wdb_int.refreshMV('wci_int.placedefinition_mv');

REVOKE ALL ON wci_int.placedefinition_mv FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition_mv TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition_mv TO wdb_read, wdb_write;

--
-- Index Materialized View
--
CREATE INDEX XIE0wci_placedefinition_mv ON wci_int.placedefinition_mv
USING GIST
(
    placeGeometry
);

CREATE INDEX XIE1wci_placedefinition_mv ON wci_int.placedefinition_mv
(
    placeNameValidFrom,
    placeNameValidTo,
	placeid
);

CREATE INDEX XIE2wci_placedefinition_mv ON wci_int.placedefinition_mv
(
	placeName,
	placeNameValidFrom,
	placeNameValidTo,
	placeNameSpaceId,
	placeId
);



--
-- New/Replacement WCI Functions
--
CREATE OR REPLACE FUNCTION 
wci.getPlaceDefinition( location 			text,	
						valid 				timestamp with time zone )
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	p.placeid,
			p.placegeometrytype,
			p.placegeometry,
			p.placeindeterminatecode,
			p.placenamespaceid,
			p.placename,
			p.placenamevalidfrom,
			p.placenamevalidto,
			p.originalsrid,
			p.placestoretime
	FROM	wci_int.placeDefinition_mv p,
			wci_int.getSessionData() s
	WHERE	p.placenamespaceid = s.placenamespaceid
	  AND	( $1 IS NULL OR placename LIKE lower($1) )
	  AND	( $2 IS NULL OR ( placenamevalidfrom <= $2 AND placenamevalidto >= $2 ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

CREATE OR REPLACE FUNCTION 
wci.getPlaceDefinition( location 			text )	
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	*
	FROM	wci.getPlaceDefinition($1, 'now');
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

-- Place Point Info
CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text,
				   valid 				timestamp with time zone )
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	p.placeid,
			p.placegeometrytype,
			p.placegeometry,
			p.placeindeterminatecode,
			p.placenamespaceid,
			p.placename,
			p.placenamevalidfrom,
			p.placenamevalidto,
			p.originalsrid,
			p.placestoretime
	FROM	wci_int.placeDefinition_mv p,
			wci_int.getSessionData() s
	WHERE	p.placenamespaceid = s.placenamespaceid
	  AND	( $1 IS NULL OR placename LIKE lower($1) )
	  AND	( $2 IS NULL OR ( placenamevalidfrom <= $2 AND placenamevalidto >= $2 ) )
	  AND	p.placegeometrytype = 'point';
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text )	
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	*
	FROM	wci.getPlacePoint($1, 'now');
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;



-- Replace view wci_int.floatvalue;
CREATE VIEW __WCI_SCHEMA__.floatvalue AS
SELECT	
	val.value,
	dp.dataproviderid,
	dp.dataprovidername,
	dp.dataprovidernameleftset,
	dp.dataprovidernamerightset,
	pl.placename,
	pl.placeid, 
	pl.placegeometry,
	pl.placeindeterminatecode,
	pl.originalsrid,
	val.referencetime, 
	val.validtimefrom, 
	val.validtimeto,
	val.validtimeindeterminatecode,
	val.valueparameterid,
	vp.parametername AS valueparametername, 
	vp.unitname AS valueunitname,
	val.levelparameterid,
	lp.parametername AS levelparametername,
	lp.unitname AS levelunitname,
	val.levelFrom, 
	val.levelTo,
	val.levelindeterminatecode,
	val.dataversion,
	val.maxdataversion,
	val.confidencecode, 
	val.valuestoretime,
	val.valueid,
	val.valuetype
FROM 	
	__WDB_SCHEMA__.floatvalue val,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.parameter_mv vp,
	__WCI_SCHEMA__.parameter_mv lp,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND lp.parameternamespaceid = s.parameternamespaceid
	AND val.dataproviderid = dp.dataproviderid 
	AND val.placeid = pl.placeid
	AND val.valueparameterid = vp.parameterid
	AND val.levelparameterid = lp.parameterid ;

REVOKE ALL ON __WCI_SCHEMA__.floatValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue TO wdb_write;

CREATE OR REPLACE RULE 
wci_internal_floatvalue_insert
AS ON INSERT TO __WCI_SCHEMA__.floatvalue
DO INSTEAD 
SELECT
__WCI_SCHEMA__.write(
	NEW.dataproviderid, 
	NEW.placeid, 
	NEW.referencetime, 
	NEW.validtimefrom, 
	NEW.validtimeto,
	NEW.validtimeindeterminatecode,
	NEW.valueparameterid, 
	NEW.levelparameterid, 
	NEW.levelfrom, 
	NEW.levelto,
	NEW.levelindeterminatecode,
	NEW.dataversion,
	NEW.confidencecode,
	NEW.value );



-- Replace view wci_int.gridvalue;
CREATE VIEW __WCI_SCHEMA__.gridvalue AS
SELECT	
	val.value,
	dp.dataproviderid,
	dp.dataprovidername,
	dp.dataprovidernameleftset,
	dp.dataprovidernamerightset,
	pl.placename,
	pl.placeid, 
	pl.placegeometry,
	pl.placeindeterminatecode,
	pl.originalsrid,
	val.referencetime, 
	val.validtimefrom, 
	val.validtimeto,
	val.validtimeindeterminatecode,
	val.valueparameterid,
	vp.parametername as valueparametername, 
	vp.unitname as valueunitname,
	val.levelparameterid,
	lp.parametername as levelparametername,
	lp.unitname as levelunitname,
	val.levelFrom,
	val.levelTo,
	val.levelindeterminatecode,
	val.dataversion,
	val.maxdataversion,
	val.confidencecode, 
	val.valuestoretime,
	val.valueid,
	val.valuetype
FROM 	
	__WDB_SCHEMA__.gridvalue val,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.parameter_mv vp,
	__WCI_SCHEMA__.parameter_mv lp,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND lp.parameternamespaceid = s.parameternamespaceid
	AND val.dataproviderid = dp.dataproviderid 
	AND val.placeid = pl.placeid
	AND val.valueparameterid = vp.parameterid
	AND val.levelparameterid = lp.parameterid ;

REVOKE ALL ON __WCI_SCHEMA__.gridvalue FROM public;
GRANT ALL ON __WCI_SCHEMA__.gridvalue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.gridvalue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.gridvalue TO wdb_write;

CREATE OR REPLACE RULE 
wci_internal_gridvalue_insert
AS ON INSERT TO __WCI_SCHEMA__.gridvalue
DO INSTEAD 
SELECT
__WCI_SCHEMA__.write(
	NEW.dataproviderid, 
	NEW.placeid, 
	NEW.referencetime, 
	NEW.validtimefrom, 
	NEW.validtimeto,
	NEW.validtimeindeterminatecode,
	NEW.valueparameterid, 
	NEW.levelparameterid, 
	NEW.levelfrom, 
	NEW.levelto,
	NEW.levelindeterminatecode,
	NEW.dataversion,
	NEW.confidencecode,
	NEW.value );



-- Replace function wci_int.getparametername(wci_int.gridvalue);
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getParameterName( data __WCI_SCHEMA__.gridvalue )
RETURNS text AS
$BODY$
DECLARE
	ret text;
BEGIN
	ret := data.valueparametername;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;



-- Replace function wci_int.lp_matches(wci_int.gridvalue,text[]);
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.lp_matches( val __WCI_SCHEMA__.gridvalue, param text[] )
RETURNS boolean AS
$BODY$
DECLARE
	currentParam __WCI_SCHEMA__.levelParameterTuple;
	item text;
	i int;
BEGIN
	-- Not implemented yet
	RETURN false;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;



-- Replace function wci_int.getlevelparametername(wci_int.gridvalue);
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.getLevelParameterName( data __WCI_SCHEMA__.gridvalue )
RETURNS text AS
$BODY$
DECLARE
	ret text;
BEGIN
	ret := data.levelparametername;
	RETURN ret;
END;
$BODY$
LANGUAGE 'plpgsql' IMMUTABLE STRICT;



-- Replace view wci_int.floatgridvalue;
CREATE VIEW __WCI_SCHEMA__.floatgridvalue AS SELECT * FROM __WCI_SCHEMA__.gridvalue;

REVOKE ALL ON __WCI_SCHEMA__.floatGridValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatGridValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatGridValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatGridValue TO wdb_write;
