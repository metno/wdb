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
