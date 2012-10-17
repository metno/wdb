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

-- DataProvider
CREATE VIEW __WCI_SCHEMA__.dataprovider_v AS
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

REVOKE ALL ON __WCI_SCHEMA__.dataprovider_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.dataprovider_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.dataprovider_v TO wdb_read, wdb_write;



-- Create Materialized View
SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.dataprovider_mv', '__WCI_SCHEMA__.dataprovider_v');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.dataprovider_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.dataprovider_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.dataprovider_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.dataprovider_mv TO wdb_read, wdb_write;

CREATE INDEX XIE1Wci_DataProvider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
       DataProviderId,
	   DataProviderNamespaceId
);

CREATE INDEX XIE0Wci_DataProvider_mv ON wci_int.dataprovider_mv 
(
	dataprovidernamespaceid, 
	dataprovidername
);


CREATE INDEX XIE2Wci_DataProvider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
	   DataProviderName
);

CREATE INDEX XIE3wci_dataprovider_mv ON __WCI_SCHEMA__.dataprovider_mv
(
    dataprovidernameleftset,
	dataprovidernamerightset
);



CREATE VIEW __WCI_SCHEMA__.wciuserdataprovider_v AS
SELECT
	dataproviderid,
	rolname
FROM
	__WDB_SCHEMA__.wciuserdataprovider;

REVOKE ALL ON __WCI_SCHEMA__.wciuserdataprovider_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.wciuserdataprovider_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.wciuserdataprovider_v TO wdb_write;



-- Dataprovider Name
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
