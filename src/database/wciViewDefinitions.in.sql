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


-- wci_internal is a schema that contains the internal functions,
-- views, and tables utilized by the WCI. 
CREATE SCHEMA __WCI_SCHEMA__;
REVOKE ALL ON SCHEMA __WCI_SCHEMA__ FROM PUBLIC;
GRANT ALL ON SCHEMA __WCI_SCHEMA__ TO wdb_admin;
GRANT USAGE ON SCHEMA __WCI_SCHEMA__ TO wdb_write;
GRANT USAGE ON SCHEMA __WCI_SCHEMA__ TO wdb_read;

CREATE VIEW __WCI_SCHEMA__.configuration AS
SELECT 
	sw.softwareversioncode as version,
	conf.productionstatus
FROM
	__WDB_SCHEMA__.currentconfiguration conf,
	__WDB_SCHEMA__.softwareversion sw
WHERE
	conf.softwareversionpartyid = sw.partyid;
REVOKE ALL ON __WCI_SCHEMA__.configuration FROM public;
GRANT ALL ON __WCI_SCHEMA__.configuration TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.configuration TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.sessiondata AS
SELECT
	dataprovidernamespaceid,
	placenamespaceid,
	parameternamespaceid	
FROM
	__WDB_SCHEMA__.defaultnamespace
WHERE 
	rolname = "current_user"() OR rolname=''
ORDER BY 
	rolname DESC
LIMIT 
	1;

REVOKE ALL ON __WCI_SCHEMA__.sessiondata FROM public;
GRANT ALL ON __WCI_SCHEMA__.sessiondata TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.sessiondata TO wdb_read, wdb_write;

-- Needs to be defined here to support view creation
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getSessionData
()
RETURNS __WCI_SCHEMA__.sessionData AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_get'
LANGUAGE 'c' STABLE;



CREATE VIEW __WCI_SCHEMA__.dataprovider AS
SELECT
	dp.dataproviderid,
	dp.dataprovidertype,
	dp.spatialdomaindelivery,
	dn.dataprovidernamespaceid,
	dn.dataprovidername,
	dn.dataprovidernameleftset,
	dn.dataprovidernamerightset,
	dp.dataproviderstoretime,
	dc.dataprovidercomment
FROM
	__WDB_SCHEMA__.dataprovider AS dp, 
	__WDB_SCHEMA__.dataprovidername AS dn,
	__WDB_SCHEMA__.dataprovidercomment AS dc
WHERE
	dp.dataproviderid = dn.dataproviderid AND
	dp.dataproviderid = dc.dataproviderid;

REVOKE ALL ON __WCI_SCHEMA__.dataprovider FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.dataprovider TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.dataprovider TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.dataprovider_mv', '__WCI_SCHEMA__.dataprovider');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.dataprovider_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.dataprovider_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.dataprovider_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.dataprovider_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_dataprovider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
    dataprovidernameleftset,
	dataprovidernamerightset,
	dataproviderid,
	dataprovidernamespaceid
);

CREATE INDEX XIE1wci_dataprovider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
	dataprovidernamespaceid,
	dataproviderid,
	dataprovidernameleftset,
	dataprovidernamerightset
);

CREATE INDEX XIE2wci_dataprovider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
    dataprovidername
);

CREATE INDEX XIE3wci_dataprovider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
    dataprovidernameleftset,
	dataprovidernamerightset
);

CREATE VIEW __WCI_SCHEMA__.wciuserdataprovider AS
SELECT
	dataproviderid,
	rolname
FROM
	__WDB_SCHEMA__.wciuserdataprovider;

REVOKE ALL ON __WCI_SCHEMA__.wciuserdataprovider FROM public;
GRANT ALL ON __WCI_SCHEMA__.wciuserdataprovider TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.wciuserdataprovider TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.placename AS
SELECT
    placeid,
    placename
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename TO wdb_read, wdb_write;

CREATE VIEW __WCI_SCHEMA__.placedefinition AS
SELECT pdd.placeid, placegeometrytype, placegeometry, placeindeterminatecode, originalsrid, placename, placenamespaceid
FROM ( SELECT  
		pd.placeid,
		pd.placegeometrytype,
		pd.placegeometry,
		pd.placeindeterminatecode,
		grd.originalsrid
	FROM 	
		__WDB_SCHEMA__.placedefinition pd, 
		__WDB_SCHEMA__.placeregulargrid grd
	WHERE
		pd.placeid = grd.placeid
	UNION ALL
	SELECT  
		pd.placeid,
		pd.placegeometrytype,
		pd.placegeometry,
		pd.placeindeterminatecode,
		4030 as originalsrid
	FROM 	
		__WDB_SCHEMA__.placedefinition pd
	WHERE
		pd.placegeometrytype != 'Grid' ) pdd, __WDB_SCHEMA__.placename pn
WHERE
	pdd.placeid = pn.placeid;
		
REVOKE ALL ON __WCI_SCHEMA__.placedefinition FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.placedefinition TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placedefinition TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.placedefinition_mv', '__WCI_SCHEMA__.placedefinition');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placedefinition_mv');

REVOKE ALL ON __WCI_SCHEMA__.placedefinition_mv FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.placedefinition_mv TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placedefinition_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_placedefinition_mv ON __WCI_SCHEMA__.placedefinition_mv
USING GIST
(
    PlaceGeometry
);

CREATE INDEX XIE1wci_placedefinition_mv ON __WCI_SCHEMA__.placedefinition_mv
(
       PlaceId
);

CREATE INDEX XIE2wci_placedefinition_mv ON __WCI_SCHEMA__.placedefinition_mv
(
       PlaceName,
	   PlaceNameSpaceId,
	   PlaceId
);

CREATE VIEW __WCI_SCHEMA__.placepoint AS
SELECT 
	pp.placeid, 
	pp.i, 
	pp.j, 
	pp."location" 
FROM
	__WDB_SCHEMA__.placepoint pp;

REVOKE ALL ON TABLE __WCI_SCHEMA__.placepoint FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placepoint TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placepoint TO wdb_read;



CREATE VIEW __WCI_SCHEMA__.valueparameter AS
SELECT
	vpp.valueparameterid,
	0 AS parameternamespaceid,
	unit.physicalphenomenon || ' ' || vpp.valueparameterusage AS valueparametername,
	unit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuephysicalparameter AS vpp, 
	__WDB_SCHEMA__.unit AS unit
WHERE
	unit.unitname = vpp.valueparameterunitname
UNION ALL
SELECT
	vdp.valueparameterid,
	0 AS parameternamespaceid,
	vdp.valuedimensionlessparametername,
	'ratio' AS valueunitname
FROM
	__WDB_SCHEMA__.valuedimensionlessparameter AS vdp
UNION ALL
SELECT
	vcp.valueparameterid,
	0 AS parameternamespaceid,
	vcp.valuecodeparametername,
	NULL AS valueunitname
FROM
	__WDB_SCHEMA__.valuecodeparameter AS vcp
UNION ALL
SELECT
	vsp.valueparameterid,
	0 AS parameternamespaceid,
	vsp.valueparameterstatisticstype || ' ' || vsunit.physicalphenomenon || ' ' || vspp.valueparameterusage AS valueparametername,
	vsunit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuestatisticsparameter AS vsp,
	__WDB_SCHEMA__.valuephysicalparameter AS vspp, 
	__WDB_SCHEMA__.unit AS vsunit
WHERE
	vsunit.unitname = vspp.valueparameterunitname AND
	vsp.basevalueparameterid = vspp.valueparameterid;

REVOKE ALL ON __WCI_SCHEMA__.valueparameter FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.valueparameter TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.valueparameter TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.valueparameter_mv', '__WCI_SCHEMA__.valueparameter');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.valueparameter_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.valueparameter_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.valueparameter_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.valueparameter_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_valueparameter_mv ON __WCI_SCHEMA__.valueparameter_mv
(
    valueparameterid,
	valueparametername,
	parameternamespaceid
);



CREATE VIEW __WCI_SCHEMA__.levelparameter AS
SELECT
	lpp.levelparameterid,
	0 AS parameternamespaceid,
	unit.physicalphenomenon || ' ' || lpp.levelparameterusage AS levelparametername,
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
	lcp.levelcodeparametername, 
	NULL as unitname 
FROM
	__WDB_SCHEMA__.levelcodeparameter as lcp;

REVOKE ALL ON __WCI_SCHEMA__.levelparameter FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.levelparameter TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.levelparameter TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.levelparameter_mv', '__WCI_SCHEMA__.levelparameter');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.levelparameter_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.levelparameter_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.levelparameter_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.levelparameter_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_levelparameter_mv ON __WCI_SCHEMA__.levelparameter_mv
(
	levelparameterid,
    levelparametername,
	parameternamespaceid
);



CREATE VIEW __WCI_SCHEMA__.oidvalue AS
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
	vp.valueparametername, 
	vp.valueunitname,
	val.levelparameterid,
	vl.levelparametername,
	vl.levelunitname,
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
	__WDB_SCHEMA__.oidvalue val,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.valueparameter_mv vp,
	__WCI_SCHEMA__.levelparameter_mv vl,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	val.dataproviderid = dp.dataproviderid 
	AND val.placeid = pl.placeid
	AND val.valueparameterid = vp.valueparameterid
	AND val.levelparameterid = vl.levelparameterid
	AND dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND vl.parameternamespaceid = s.parameternamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.oidvalue FROM public;
GRANT ALL ON __WCI_SCHEMA__.oidvalue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.oidvalue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.oidvalue TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.floatgridvalue AS SELECT * FROM __WCI_SCHEMA__.oidvalue;

REVOKE ALL ON __WCI_SCHEMA__.floatGridValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatGridValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatGridValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatGridValue TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.floatvalue AS
SELECT	
	val.value,
	dp.dataproviderid,
	dp.dataprovidername,
	dp.dataprovidernameleftset,
	dp.dataprovidernamerightset,
	plname.placename,
	place.placeid, 
	place.placegeometry,
	place.placeindeterminatecode,
	place.originalsrid,
	val.referencetime, 
	val.validtimefrom, 
	val.validtimeto,
	val.validtimeindeterminatecode,
	val.valueparameterid,
	vp.valueparametername, 
	vp.valueunitname,
	val.levelparameterid,
	vl.levelparametername,
	vl.levelunitname,
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
	__WCI_SCHEMA__.placedefinition_mv place,
	__WDB_SCHEMA__.placename plname,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.valueparameter_mv vp,
	__WCI_SCHEMA__.levelparameter_mv vl,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	val.dataproviderid = dp.dataproviderid 
	AND val.placeid = place.placeid
	AND place.placeid = plname.placeid
	AND val.valueparameterid = vp.valueparameterid
	AND val.levelparameterid = vl.levelparameterid
	AND dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND plname.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND vl.parameternamespaceid = s.parameternamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.floatValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.unit AS
	SELECT  
		unitname,
		unittype,
		physicalphenomenon
	FROM 	
		__WDB_SCHEMA__.unit;
		
REVOKE ALL ON __WCI_SCHEMA__.unit FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.unit TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.unit TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.unit TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.parameterfunctiontype AS
	SELECT  
		parameterfunctiontype,
		parameterfunctiondescription
	FROM 	
		__WDB_SCHEMA__.parameterfunctiontype;
		
REVOKE ALL ON __WCI_SCHEMA__.parameterfunctiontype FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.parameterfunctiontype TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.parameterfunctiontype TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.parameterfunctiontype TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.physicalphenomenon AS
	SELECT  
		physicalphenomenon
	FROM 	
		__WDB_SCHEMA__.physicalphenomenon;
		
REVOKE ALL ON __WCI_SCHEMA__.physicalphenomenon FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.physicalphenomenon TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.physicalphenomenon TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.physicalphenomenon TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.valuedomain AS
	SELECT  
		valueparameterusage as valuedomain,
		valueparameterusagedescription as valuedomaindescription
	FROM 	
		__WDB_SCHEMA__.valueparameterusage;
		
REVOKE ALL ON __WCI_SCHEMA__.valuedomain FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.valuedomain TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.valuedomain TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.valuedomain TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.leveldomain AS
	SELECT  
		levelparameterusage as leveldomain,
		levelparameterusagedescription as leveldomaindescription
	FROM 	
		__WDB_SCHEMA__.levelparameterusage;
		
REVOKE ALL ON __WCI_SCHEMA__.leveldomain FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.leveldomain TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.leveldomain TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.valuedomain TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.placegeo AS
	SELECT  
		placeid,
		placeindeterminatecode,
		placegeometrytype,
		astext(placegeometry) AS placegeometry,
		placestoretime
	FROM 	
		__WDB_SCHEMA__.placedefinition;
		
REVOKE ALL ON __WCI_SCHEMA__.placegeo FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.placegeo TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placegeo TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.placegeo TO wdb_write;

CREATE VIEW __WCI_SCHEMA__.placespec_v AS SELECT 
	pd.placeid,
	pn.placename,
	pn.placenamespaceid,
	pd.placegeometry, 
	pit.placeindeterminatetype,
	pg.inumber, 
	pg.jnumber, 
	pg.iincrement, 
	pg.jincrement, 
	pg.startlongitude, 
	pg.startlatitude, 
	pg.originalsrid,
	srs.proj4text AS projdefinition 
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	__WDB_SCHEMA__.placename pn,
	spatial_ref_sys srs, 
	__WDB_SCHEMA__.placeregulargrid pg,
	__WDB_SCHEMA__.placeindeterminatetype pit
WHERE 
	((srs.srid = pg.originalsrid)
	AND (pd.placeid = pg.placeid))
	AND (pd.placeid = pn.placeid)
	AND (pit.placeindeterminatecode = pd.placeindeterminatecode);

REVOKE ALL ON TABLE __WCI_SCHEMA__.placespec_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placespec_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placespec_v TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.placespec', '__WCI_SCHEMA__.placespec_v');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placespec');

REVOKE ALL ON TABLE __WCI_SCHEMA__.placespec FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placespec TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placespec TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_placespec_mv ON __WCI_SCHEMA__.placespec
(
	placeid
);
