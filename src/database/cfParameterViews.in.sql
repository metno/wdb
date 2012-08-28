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

CREATE VIEW __WCI_SCHEMA__.cfsurface_v AS
SELECT
	cfsurface,
	cfsurfacecomment
FROM
	__WDB_SCHEMA__.cfsurface;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfsurface_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfsurface_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfsurface_v TO wdb_read, wdb_write;


	
CREATE VIEW __WCI_SCHEMA__.cfcomponent_v AS
SELECT
	cfcomponent,
	cfcomponentcomment
FROM
	__WDB_SCHEMA__.cfcomponent;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfcomponent_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfcomponent_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfcomponent_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.cfmedium_v AS
SELECT
	cfmedium,
	cfmediumcomment
FROM
	__WDB_SCHEMA__.cfmedium;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfmedium_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfmedium_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfmedium_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.cfprocess_v AS
SELECT
	cfprocess,
	cfprocesscomment
FROM
	__WDB_SCHEMA__.cfprocess;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfprocess_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfprocess_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfprocess_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.cfcondition_v AS
SELECT
	cfcondition,
	cfconditioncomment
FROM
	__WDB_SCHEMA__.cfcondition;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfcondition_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfcondition_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfcondition_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.cfmethods_v AS
SELECT
	cfmethods,
	cfmethodscomment,
	cfmethodsname
FROM
	__WDB_SCHEMA__.cfmethods;

REVOKE ALL ON TABLE __WCI_SCHEMA__.cfmethods_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.cfmethods_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.cfmethods_v TO wdb_read, wdb_write;
