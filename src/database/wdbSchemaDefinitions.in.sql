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
SET SESSION client_min_messages TO 'warning';

-- wdb_int is the core schema of the WDB system. It contains the core data 
-- tables, functions, and views
CREATE SCHEMA __WDB_SCHEMA__;
REVOKE ALL ON SCHEMA __WDB_SCHEMA__ FROM PUBLIC;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_admin;
GRANT USAGE ON SCHEMA __WDB_SCHEMA__ TO wdb_write;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_clean;



