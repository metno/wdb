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


CREATE VIEW __WCI_SCHEMA__.gridvalue_v AS
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
	1 as valuetype
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

REVOKE ALL ON __WCI_SCHEMA__.gridvalue_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.gridvalue_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.gridvalue_v TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.gridvalue_v TO wdb_write;
