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


-- Needs to be defined here to support view creation
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getSessionData
()
RETURNS __WCI_SCHEMA__.sessionData AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_get'
LANGUAGE 'c' STABLE;

