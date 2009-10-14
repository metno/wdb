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
	sw.softwareversioncode AS version,
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


CREATE VIEW __WCI_SCHEMA__.dataprovidername_v AS
SELECT
    dataproviderid,
    dataprovidernamespaceid,
    dataprovidername
FROM
	__WDB_SCHEMA__.dataprovidername;

REVOKE ALL ON __WCI_SCHEMA__.dataprovidername_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.dataprovidername_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.dataprovidername_v TO wdb_read, wdb_write;


CREATE VIEW __WCI_SCHEMA__.placename_v AS
SELECT
    p.placeid,
    p.placenamespaceid,
    p.placename
FROM
	__WDB_SCHEMA__.placename p;

REVOKE ALL ON __WCI_SCHEMA__.placename_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_v TO wdb_read, wdb_write;


CREATE VIEW __WCI_SCHEMA__.placename AS
SELECT
    p.placeid,
    p.placenamespaceid,
    p.placename
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.placedefinition AS
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placename,
	pn.placenamespaceid,
	pg.originalsrid,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn,
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
	AND pd.placeid = pn.placeid
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placename,
	pn.placenamespaceid,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn
WHERE
	pd.placeid = pn.placeid
	AND pd.placegeometrytype <> 'Grid' 
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	0 AS placenamespaceid,
	pg.originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	lower(astext( pd.placegeometry )) as placename,
	0 AS placenamespaceid,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd
WHERE
	pd.placegeometrytype != 'Grid';
		
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



CREATE VIEW __WCI_SCHEMA__.placeregulargrid AS 
SELECT 
	pd.placeid,
	pn.placename,	
	pn.placenamespaceid,
	pd.placegeometry,
	pd.placegeometrytype,
	pit.placeindeterminatetype,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	srs.proj4text AS projdefinition,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	__WDB_SCHEMA__.placename pn,
	__WDB_SCHEMA__.placeregulargrid pg,
	spatial_ref_sys srs,
	__WDB_SCHEMA__.placeindeterminatetype pit
WHERE
	pd.placeid = pn.placeid 
	AND pd.placeid = pg.placeid
	AND srs.srid = pg.originalsrid
	AND pit.placeindeterminatecode = pd.placeindeterminatecode
UNION ALL
SELECT
	pd.placeid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	0 AS placenamespaceid,
	pd.placegeometry, 
	pd.placegeometrytype,
	pit.placeindeterminatetype,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	srs.proj4text AS projdefinition,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	__WDB_SCHEMA__.placeregulargrid pg,
	spatial_ref_sys srs,
	__WDB_SCHEMA__.placeindeterminatetype pit
WHERE
	pd.placeid = pg.placeid
	AND srs.srid = pg.originalsrid
	AND pit.placeindeterminatecode = pd.placeindeterminatecode;
	
REVOKE ALL ON TABLE __WCI_SCHEMA__.placeregulargrid FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placeregulargrid TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placeregulargrid TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.placeregulargrid_mv', '__WCI_SCHEMA__.placeregulargrid');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placeregulargrid_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placeregulargrid_mv TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.placeindeterminatetype AS
SELECT
	placeindeterminatecode,
	placeindeterminatetype
FROM
	__WDB_SCHEMA__.placeindeterminatetype;
	
REVOKE ALL ON TABLE __WCI_SCHEMA__.placeindeterminatetype FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placeindeterminatetype TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placeindeterminatetype TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.valueparameter AS
SELECT
	vpp.valueparameterid,
	0 AS parameternamespaceid,
	vpp.valueparameterusage || ' ' || unit.measure AS valueparametername,
	unit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuemeasureparameter AS vpp, 
	__WDB_SCHEMA__.unit AS unit
WHERE
	unit.unitname = vpp.valueparameterunitname AND
	vpp.parameterquantitytype = 'scalar'
UNION ALL
SELECT
	vpp.valueparameterid,
	0 AS parameternamespaceid,
	vpp.valueparameterusage || ' ' || unit.measure || ' (' || vpp.parameterquantitytype || ')' AS valueparametername,
	unit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuemeasureparameter AS vpp, 
	__WDB_SCHEMA__.unit AS unit
WHERE
	unit.unitname = vpp.valueparameterunitname AND
	vpp.parameterquantitytype <> 'scalar'
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
	vfp.valueparameterid,
	0 AS parameternamespaceid,
	vfp.parameterfunctiontype || ' ' || vfp.valueparameterusage || ' ' || vsunit.measure  AS valueparametername,
	vsunit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuefunctionparameter AS vfp,
	__WDB_SCHEMA__.unit AS vsunit
WHERE
	vsunit.unitname = vfp.valueparameterunitname AND
	vfp.parameterquantitytype = 'scalar'
UNION ALL
SELECT
	vfp.valueparameterid,
	0 AS parameternamespaceid,
	vfp.parameterfunctiontype || ' ' || vfp.valueparameterusage || ' ' || vsunit.measure || ' (' || vfp.parameterquantitytype || ')' AS valueparametername,
	vsunit.unitname AS valueunitname
FROM
	__WDB_SCHEMA__.valuefunctionparameter AS vfp,
	__WDB_SCHEMA__.unit AS vsunit
WHERE
	vsunit.unitname = vfp.valueparameterunitname AND
	vfp.parameterquantitytype <> 'scalar';

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
	lpp.levelparameterusage || ' ' || unit.measure AS levelparametername,
	unit.unitname AS levelunitname
FROM
	__WDB_SCHEMA__.levelmeasureparameter AS lpp, 
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



--
-- Unit and Unit Conversions
--
CREATE VIEW __WCI_SCHEMA__.unitwithconversion AS
SELECT un.unitname, 
	   un.unittype, 
	   co.siunitconversioncoefficient,
	   co.siunitconversionterm
FROM  
	   __WDB_SCHEMA__.unit un
LEFT OUTER JOIN
	   __WDB_SCHEMA__.siunitconversion co
ON 	   (un.unitname = co.unitname);

REVOKE ALL ON __WCI_SCHEMA__.unitwithconversion FROM public;
GRANT ALL ON __WCI_SCHEMA__.unitwithconversion TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.unitwithconversion TO wdb_felt;
GRANT SELECT ON __WCI_SCHEMA__.unitwithconversion TO wdb_grib;



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
	__WDB_SCHEMA__.gridvalue val,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.valueparameter_mv vp,
	__WCI_SCHEMA__.levelparameter_mv vl,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND vl.parameternamespaceid = s.parameternamespaceid
	AND val.dataproviderid = dp.dataproviderid 
	AND val.placeid = pl.placeid
	AND val.valueparameterid = vp.valueparameterid
	AND val.levelparameterid = vl.levelparameterid ;

REVOKE ALL ON __WCI_SCHEMA__.gridvalue FROM public;
GRANT ALL ON __WCI_SCHEMA__.gridvalue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.gridvalue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.gridvalue TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.floatgridvalue AS SELECT * FROM __WCI_SCHEMA__.gridvalue;

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
	__WDB_SCHEMA__.floatvalue val,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.valueparameter_mv vp,
	__WCI_SCHEMA__.levelparameter_mv vl,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND vl.parameternamespaceid = s.parameternamespaceid
	AND val.dataproviderid = dp.dataproviderid 
	AND val.placeid = pl.placeid
	AND val.valueparameterid = vp.valueparameterid
	AND val.levelparameterid = vl.levelparameterid ;

	

REVOKE ALL ON __WCI_SCHEMA__.floatValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue TO wdb_write;



CREATE VIEW __WCI_SCHEMA__.unit AS
	SELECT  
		unitname,
		unittype,
		measure
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


CREATE VIEW __WCI_SCHEMA__.measure AS
	SELECT  
		measure
	FROM 	
		__WDB_SCHEMA__.measure;
		
REVOKE ALL ON __WCI_SCHEMA__.measure FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.measure TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.measure TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.measure TO wdb_write;


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



CREATE VIEW __WCI_SCHEMA__.placespec_v AS SELECT 
	pd.placeid,
	pn.placename,
	pn.placenamespaceid,
	pd.placegeometry, 
	pd.placegeometrytype,
	pit.placeindeterminatetype,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	srs.proj4text AS projdefinition,
	pd.placestoretime
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



