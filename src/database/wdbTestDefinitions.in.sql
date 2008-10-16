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


-- test is a schema that contains views, functions and tables that
-- are specific to the testing of the WDB system
CREATE SCHEMA test;
REVOKE ALL ON SCHEMA test FROM PUBLIC;
GRANT ALL ON SCHEMA test TO wdb_admin;
GRANT USAGE ON SCHEMA test TO wdb_test;


--
-- Test View
--
CREATE VIEW test.oidvalue AS
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
	__WDB_SCHEMA__.placedefinition place,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.valueparameter_mv vp,
	__WCI_SCHEMA__.levelparameter_mv vl
WHERE
	val.dataproviderid = dp.dataproviderid 
	AND val.placeid = place.placeid
	AND val.valueparameterid = vp.valueparameterid
	AND val.levelparameterid = vl.levelparameterid;

REVOKE ALL ON test.oidvalue FROM public;
GRANT ALL ON test.oidvalue TO wdb_admin;
GRANT SELECT ON test.oidvalue TO wdb_test;


-- 
-- Data Provider

CREATE VIEW test.dataprovider AS
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

CREATE VIEW test.placeregulargrid AS SELECT 
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

REVOKE ALL ON TABLE test.placeregulargrid FROM PUBLIC;
GRANT ALL ON TABLE test.placeregulargrid TO wdb_admin;
GRANT SELECT ON TABLE test.placeregulargrid TO wdb_test;


CREATE OR REPLACE FUNCTION 
test.removePlaceDef( )
RETURNS void AS
$BODY$
	DELETE FROM __WDB_SCHEMA__.oidvalue WHERE placeid=500;
	DELETE FROM __WDB_SCHEMA__.placeregulargrid WHERE placeid=500;
	DELETE FROM __WDB_SCHEMA__.placedefinition WHERE placeid=500;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.insertPlaceDef( )
RETURNS void AS
$BODY$
	INSERT INTO __WDB_SCHEMA__.placeregulargrid 
	VALUES (500, 2, 2, 0.1, 0.1, 0, 0, 50001);
	INSERT INTO __WDB_SCHEMA__.placename
	VALUES (
		500,0,'test grid, rotated','2000-01-01','2999-01-01'
	);
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.removeInsertedPlaceDef( )
RETURNS void AS
$BODY$
DECLARE
	pid 	integer;
BEGIN
	SELECT placeid INTO pid FROM test.oidvalue WHERE dataproviderid = 30 LIMIT 1;
	DELETE FROM __WDB_SCHEMA__.oidvalue WHERE dataproviderid = 30;
	DELETE FROM __WDB_SCHEMA__.placedefinition WHERE placeid = pid;
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql' VOLATILE;


CREATE OR REPLACE FUNCTION 
test.countPlaceDef( )
RETURNS bigint AS
$BODY$
	SELECT count(*)
	FROM __WDB_SCHEMA__.placedefinition;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' STRICT;

