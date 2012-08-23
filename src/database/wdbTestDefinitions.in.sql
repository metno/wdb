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

--
-- Test View
--
CREATE OR REPLACE VIEW test.gridvalue AS
SELECT	
	val.value,
	dp.dataproviderid,
	dp.dataprovidername,
	place.placeid, 
	place.placegeometry,
	place.placeindeterminatecode,
	val.referencetime, 
	val.validtimefrom, 
	val.validtimeto,
	val.validtimeindeterminatecode,
	val.valueparameterid,
	vp.parametername as valueparametername, 
	vp.unitname as valueunitname,
	val.levelparameterid,
	vl.parametername as levelparametername,
	vl.unitname as levelunitname,
	val.levelFrom, 
	val.levelTo,
	val.levelindeterminatecode,
	val.dataversion,
	val.maxdataversion,
	val.confidencecode, 
	val.valuestoretime,
	val.valueid,
	0 as valuetype
FROM 	
	__WDB_SCHEMA__.gridvalue val,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WDB_SCHEMA__.placedefinition place,
	__WCI_SCHEMA__.parameter_mv vp,
	__WCI_SCHEMA__.parameter_mv vl
WHERE
	val.dataproviderid = dp.dataproviderid 
	AND val.placeid = place.placeid
	AND val.valueparameterid = vp.parameterid
	AND val.levelparameterid = vl.parameterid
	AND dp.dataprovidernamespaceid = 999
	AND vp.parameternamespaceid = 999
	AND vl.parameternamespaceid = 999;

REVOKE ALL ON test.gridvalue FROM public;
GRANT ALL ON test.gridvalue TO wdb_admin;
GRANT SELECT ON test.gridvalue TO wdb_test;


-- 
-- Data Provider

CREATE OR REPLACE VIEW test.dataprovider AS
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

REVOKE ALL ON test.dataprovider FROM PUBLIC;
GRANT ALL ON test.dataprovider TO wdb_admin;
GRANT SELECT ON test.dataprovider TO wdb_test;


--
-- Place Regular Grid

CREATE OR REPLACE VIEW test.placeregulargrid AS SELECT 
	pd.placeid,
	pn.placename,
	pn.placenamespaceid,
	pd.placegeometry, 
	pit.placeindeterminatetype,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
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

REVOKE ALL ON TABLE test.placeregulargrid FROM PUBLIC;
GRANT ALL ON TABLE test.placeregulargrid TO wdb_admin;
GRANT SELECT ON TABLE test.placeregulargrid TO wdb_test;


CREATE OR REPLACE FUNCTION 
test.cleanTestData( )
RETURNS void AS
$BODY$
	DELETE FROM __WDB_SCHEMA__.gridvalue 
	WHERE dataproviderid IN ( SELECT b.dataproviderid
					   		  FROM   __WCI_SCHEMA__.dataprovider_mv a,
					   		         __WCI_SCHEMA__.dataprovider_mv b
					   		  WHERE	 a.dataprovidername = 'test group'
					   		    AND	 a.dataprovidernameleftset <= b.dataprovidernameleftset
					   		    AND  a.dataprovidernamerightset >= b.dataprovidernamerightset);    
	DELETE FROM __WDB_SCHEMA__.floatvaluegroup 
	WHERE dataproviderid IN ( SELECT b.dataproviderid
					   		  FROM   __WCI_SCHEMA__.dataprovider_mv a,
					   		         __WCI_SCHEMA__.dataprovider_mv b
					   		  WHERE	 a.dataprovidername = 'test group'
					   		    AND	 a.dataprovidernameleftset <= b.dataprovidernameleftset
					   		    AND  a.dataprovidernamerightset >= b.dataprovidernamerightset);    
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.removePlaceDef( )
RETURNS void AS
$BODY$
DECLARE
	pid 	integer;
BEGIN
	SELECT placeid INTO pid FROM test.placeregulargrid 
	WHERE  numberX = 3 AND numberY = 3 AND
	       round(incrementX::numeric, 3) = 0.1 AND round(incrementY::numeric, 3) = 0.1 AND
	       startX = 0 AND startY = 0 AND
	       projdefinition='+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs'
		   LIMIT 1;
	DELETE FROM __WDB_SCHEMA__.gridvalue WHERE placeid=pid;
	DELETE FROM __WDB_SCHEMA__.placeregulargrid WHERE placeid=pid;
	DELETE FROM __WDB_SCHEMA__.placedefinition WHERE placeid=pid;
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.insertPlaceDef( )
RETURNS bigint AS
$BODY$
	select wci.addplaceregulargrid('test grid, gribload', 3, 3, 0.100,0.100,0.0000,0.0000,
								   '+proj=ob_tran +o_proj=longlat +lon_0=-40 +o_lat_p=22 +a=6367470.0 +no_defs');
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.countPlaceDef( )
RETURNS bigint AS
$BODY$
	SELECT count(*)
	FROM __WDB_SCHEMA__.placedefinition;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' STRICT;

