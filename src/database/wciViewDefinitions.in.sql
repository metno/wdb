-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 - 2012 met.no
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
	AND pl.placenamevalidfrom <= val.referencetime
	AND pl.placenamevalidto > val.referencetime
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
	AND pl.placenamevalidfrom <= vli.referencetime
	AND pl.placenamevalidto > vli.referencetime
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







