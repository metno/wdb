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

-- __WCI_SCHEMA__ (usually wci_int) is a schema that contains the internal
-- functions, views, and tables utilized by the WCI 
CREATE SCHEMA __WCI_SCHEMA__;
REVOKE ALL ON SCHEMA __WCI_SCHEMA__ FROM PUBLIC;
GRANT ALL ON SCHEMA __WCI_SCHEMA__ TO wdb_admin;
GRANT USAGE ON SCHEMA __WCI_SCHEMA__ TO wdb_write;
GRANT USAGE ON SCHEMA __WCI_SCHEMA__ TO wdb_read;


CREATE VIEW __WCI_SCHEMA__.configuration AS
SELECT
	sw.softwarename AS name,
	sw.softwareversioncode AS softwareversion,
	cf.packageversion AS packageversion,
	pc.partycomment AS description,
	cf.installtime AS installtime
FROM
	(__WDB_SCHEMA__.configuration AS cf JOIN __WDB_SCHEMA__.softwareversion AS sw ON cf.softwareversionpartyid = sw.partyid)
	LEFT OUTER JOIN __WDB_SCHEMA__.partycomment AS pc ON softwareversionpartyid = pc.partyid; 

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



CREATE VIEW __WCI_SCHEMA__.person AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
    pn.firstname,
    pn.lastname,
    pn.title,
    pn.salutation,
    pn.initials,
    pn.signum,
    pn.gender,
    pn.namesuffix,
    pn.maritalstatus,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	(__WDB_SCHEMA__.party AS py JOIN __WDB_SCHEMA__.person AS pn ON py.partyid = pn.partyid)  
	LEFT OUTER JOIN __WDB_SCHEMA__.partycomment AS pc ON py.partyid = pc.partyid;
	

REVOKE ALL ON __WCI_SCHEMA__.person FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.person TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.person TO wdb_read, wdb_write;


CREATE VIEW __WCI_SCHEMA__.organization AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
    po.organizationname,
    po.organizationalias,
    po.organizationtype,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	(__WDB_SCHEMA__.party AS py JOIN __WDB_SCHEMA__.organization AS po ON py.partyid = po.partyid) 
	LEFT OUTER JOIN __WDB_SCHEMA__.partycomment AS pc ON py.partyid = pc.partyid;

REVOKE ALL ON __WCI_SCHEMA__.organization FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.organization TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.organization TO wdb_read, wdb_write;


CREATE VIEW __WCI_SCHEMA__.softwareversion AS
SELECT
    py.partyid,
    py.partytype,
    py.partyvalidfrom,
    py.partyvalidto,
	ps.softwarename,
	ps.softwareversioncode,
	pc.partycomment,
    pc.partycommentstoretime
FROM
	(__WDB_SCHEMA__.party AS py JOIN __WDB_SCHEMA__.softwareversion AS ps ON py.partyid = ps.partyid )
	LEFT OUTER JOIN  __WDB_SCHEMA__.partycomment AS pc ON py.partyid = pc.partyid;

REVOKE ALL ON __WCI_SCHEMA__.softwareversion FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.softwareversion TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.softwareversion TO wdb_read, wdb_write;






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
	dp.dataproviderid = dc.dataproviderid AND
	dc.dataprovidercommentstoretime = (
		SELECT max(dataprovidercommentstoretime) 
		FROM __WDB_SCHEMA__.dataprovidercomment c 
		WHERE c.dataproviderid = dp.dataproviderid
	);

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


CREATE VIEW __WCI_SCHEMA__.placename_valid_v AS
SELECT
    p.placeid,
    p.placename,
    p.placenamevalidfrom,
    p.placenamevalidto
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename_valid_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_valid_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_valid_V TO wdb_read, wdb_write;


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


CREATE VIEW __WCI_SCHEMA__.placeregulargrid AS 
SELECT 
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pit.placeindeterminatetype,
	pn.placenamespaceid,
	pn.placename,	
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	btrim(srs.proj4text) AS projdefinition,
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
	pd.placegeometrytype,
	pd.placegeometry, 
	pit.placeindeterminatetype,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	btrim(srs.proj4text) AS projdefinition,
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


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getCanonicalParameterName( 
				  cfstandardname_		text,
				  cfsurface_ 			text,
				  cfcomponent_			text,
				  cfmedium_				text,
				  cfprocess_			text,
				  cfcondition_			text,
				  cfmethods_			text )	
RETURNS text AS
$BODY$
DECLARE
	parametername 		text;
BEGIN
	parametername := cfstandardname_;	
	IF cfcomponent_ IS NOT NULL then
		parametername := cfcomponent_ || ' ' || parametername;  
	END IF;
	IF cfsurface_ IS NOT NULL then
		IF ( position( ' ' IN cfsurface_ ) < 1 ) THEN
			parametername := cfsurface_ || ' ' || parametername;
		ELSE
			parametername := parametername || ' at ' || cfsurface_;
		END IF;			
	END IF;
	IF cfmedium_ IS NOT NULL then
		parametername := parametername || ' in ' || cfmedium_;  
	END IF;
	IF cfprocess_ IS NOT NULL then
		parametername := parametername || ' due to ' || cfprocess_;  
	END IF;
	IF cfcondition_ IS NOT NULL then
		parametername := parametername || ' assuming ' || cfcondition_;  
	END IF;
	IF cfmethods_ IS NOT NULL then
		parametername := parametername || ' [' || cfmethods_ || ']';  
	END IF;
	RETURN lower(parametername);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getParameterName( 
				  cfstandardname_		text,
				  cfsurface_ 			text,
				  cfcomponent_			text,
				  cfmedium_				text,
				  cfprocess_			text,
				  cfcondition_			text,
				  cfmethods_			text )	
RETURNS text AS
$BODY$
DECLARE
	parametername 		text;
	functionname 		text;
BEGIN
	parametername := cfstandardname_;	
	IF cfcomponent_ IS NOT NULL then
		parametername := cfcomponent_ || ' ' || parametername;  
	END IF;
	IF cfsurface_ IS NOT NULL then
		IF ( position( ' ' IN cfsurface_ ) < 1 ) THEN
			parametername := cfsurface_ || ' ' || parametername;
		ELSE
			parametername := parametername || ' at ' || cfsurface_;
		END IF;			
	END IF;
	IF cfmethods_ IS NOT NULL then
		SELECT cfmethodsname INTO functionname
		FROM   __WCI_SCHEMA__.cfmethods
		WHERE  cfmethods = cfmethods_;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Could not identify the cfmethods description %', cfmethods_;
		END IF;
		parametername := functionname || ' ' || parametername;  
	END IF;
	IF cfmedium_ IS NOT NULL then
		parametername := parametername || ' in ' || cfmedium_;  
	END IF;
	IF cfprocess_ IS NOT NULL then
		parametername := parametername || ' due to ' || cfprocess_;  
	END IF;
	IF cfcondition_ IS NOT NULL then
		parametername := parametername || ' assuming ' || cfcondition_;  
	END IF;
	RETURN lower(parametername);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;





CREATE VIEW __WCI_SCHEMA__.parameter AS
SELECT
	v.parameterid,
  	v.cfstandardname,
 	v.cfsurface,
    v.cfcomponent,
    v.cfmedium,
    v.cfprocess,
    v.cfcondition,
    v.cfmethods,
    v.unitname,
	n.parameternamespaceid,
	n.parametername
FROM
	__WDB_SCHEMA__.parameter AS v, 
	__WDB_SCHEMA__.parametername AS n
WHERE 	v.parameterid = n.parameterid
UNION ALL
SELECT
	v.parameterid,
  	v.cfstandardname,
 	v.cfsurface,
    v.cfcomponent,
    v.cfmedium,
    v.cfprocess,
    v.cfcondition,
    v.cfmethods,
    v.unitname,
	0 AS parameternamespaceid,
	__WCI_SCHEMA__.getCanonicalParameterName( v.cfstandardname, v.cfsurface, v.cfcomponent, 
			v.cfmedium, v.cfprocess, v.cfcondition, v.cfmethods ) AS parametername
FROM
	__WDB_SCHEMA__.parameter AS v;
  
REVOKE ALL ON __WCI_SCHEMA__.parameter FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.parameter TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.parameter TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.parameter_mv', '__WCI_SCHEMA__.parameter');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.parameter_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.parameter_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.parameter_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.parameter_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_parameter_mv ON __WCI_SCHEMA__.parameter_mv
(
    parameterid,
	parametername,
	parameternamespaceid
);



--
-- Unit and Unit Conversions
--
CREATE VIEW __WCI_SCHEMA__.unitwithconversion AS
SELECT un.unitname, 
	   un.unittype,
	   un.description,
	   co.baseunitname,
	   co.baseunitconversioncoefficient,
	   co.baseunitconversionterm
FROM  	__WDB_SCHEMA__.unit un,
		__WDB_SCHEMA__.baseunitconversion co
WHERE	un.unitname = co.unitname
UNION ALL
SELECT un.unitname, 
	   un.unittype,
	   un.description,
	   un.unitname AS baseunitname,
	   1.0 AS baseunitconversioncoefficient,
	   0.0 AS baseunitconversionterm
FROM  	__WDB_SCHEMA__.unit un
WHERE 	NOT EXISTS ( SELECT * 
					 FROM 	__WDB_SCHEMA__.baseunitconversion co
					 WHERE	un.unitname = co.unitname );

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


CREATE VIEW __WCI_SCHEMA__.floatgridvalue AS SELECT * FROM __WCI_SCHEMA__.gridvalue;

REVOKE ALL ON __WCI_SCHEMA__.floatGridValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatGridValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatGridValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatGridValue TO wdb_write;


DROP VIEW IF EXISTS __WCI_SCHEMA__.floatvalue;
CREATE VIEW __WCI_SCHEMA__.floatvalue AS
SELECT
	vli.value,
	dp.dataproviderid,
	dp.dataprovidername,
	dp.dataprovidernameleftset,
	dp.dataprovidernamerightset,
	pl.placename,
	pl.placeid,
	pl.placegeometry,
	pl.placeindeterminatecode,
	pl.originalsrid,
	vli.referencetime,
	(vli.referencetime + vlg.validtimefrom) AS validtimefrom,
	(vli.referencetime + vlg.validtimeto) AS validtimeto,
	vlg.validtimeindeterminatecode,
	vlg.valueparameterid,
	vp.parametername AS valueparametername, 
	vp.unitname AS valueunitname,
	vlg.levelparameterid,
	lp.parametername AS levelparametername,
	lp.unitname AS levelunitname,
	vlg.levelFrom, 
	vlg.levelTo,
	vlg.levelindeterminatecode,
	vlg.dataversion,
	vli.maxdataversion,
	vli.confidencecode,
	vli.valuestoretime,
	0::bigint AS valueid,
	1 AS valuetype
FROM 	
	__WDB_SCHEMA__.floatvalueitem vli,
	__WDB_SCHEMA__.floatvaluegroup vlg,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.parameter_mv vp,
	__WCI_SCHEMA__.parameter_mv lp,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	vli.valuegroupid = vlg.valuegroupid
	AND dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND lp.parameternamespaceid = s.parameternamespaceid
	AND vlg.dataproviderid = dp.dataproviderid 
	AND vlg.placeid = pl.placeid
	AND vlg.valueparameterid = vp.parameterid
	AND vlg.levelparameterid = lp.parameterid ;


REVOKE ALL ON __WCI_SCHEMA__.floatValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue TO wdb_write;


CREATE VIEW __WCI_SCHEMA__.unit AS
	SELECT  
		unitname,
		unittype,
		description
	FROM 	
		__WDB_SCHEMA__.unit;
		
REVOKE ALL ON __WCI_SCHEMA__.unit FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.unit TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.unit TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.unit TO wdb_write;





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
	btrim(srs.proj4text) AS projdefinition,
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







CREATE VIEW __WCI_SCHEMA__.cfsurface AS
SELECT
	cfsurface,
	cfsurfacecomment
FROM
	__WDB_SCHEMA__.cfsurface;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfsurface FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfsurface TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfsurface TO wdb_read, wdb_write;
	
CREATE VIEW __WCI_SCHEMA__.cfcomponent AS
SELECT
	cfcomponent,
	cfcomponentcomment
FROM
	__WDB_SCHEMA__.cfcomponent;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfcomponent FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfcomponent TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfcomponent TO wdb_read, wdb_write;

CREATE VIEW __WCI_SCHEMA__.cfmedium AS
SELECT
	cfmedium,
	cfmediumcomment
FROM
	__WDB_SCHEMA__.cfmedium;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfmedium FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfmedium TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfmedium TO wdb_read, wdb_write;

CREATE VIEW __WCI_SCHEMA__.cfprocess AS
SELECT
	cfprocess,
	cfprocesscomment
FROM
	__WDB_SCHEMA__.cfprocess;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfprocess FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfprocess TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfprocess TO wdb_read, wdb_write;

CREATE VIEW __WCI_SCHEMA__.cfcondition AS
SELECT
	cfcondition,
	cfconditioncomment
FROM
	__WDB_SCHEMA__.cfcondition;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfcondition FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfcondition TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfcondition TO wdb_read, wdb_write;

CREATE VIEW __WCI_SCHEMA__.cfmethods AS
SELECT
	cfmethods,
	cfmethodscomment,
	cfmethodsname
FROM
	__WDB_SCHEMA__.cfmethods;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfmethods FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfmethods TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfmethods TO wdb_read, wdb_write;
