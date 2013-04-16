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
SET SESSION client_min_messages TO 'warning';


CREATE VIEW __WCI_SCHEMA__.floatvalue_v AS
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
	vlg.valuegroupid AS valueid,
	0 AS valuetype
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

REVOKE ALL ON __WCI_SCHEMA__.floatValue_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue_v TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue_v TO wdb_write;
