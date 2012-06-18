-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007-2012 met.no
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


-- configuration stores the WDB version information in the database
CREATE TABLE __WDB_SCHEMA__.configuration (
    softwareversionpartyid		integer NOT NULL,
    packageversion				integer NOT NULL,
    installtime					timestamp with time zone NOT NULL
);

ALTER TABLE __WDB_SCHEMA__.configuration
	ADD FOREIGN KEY (softwareversionpartyid)
					REFERENCES __WDB_SCHEMA__.softwareversion
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

REVOKE ALL ON __WDB_SCHEMA__.configuration FROM public;
GRANT ALL ON __WDB_SCHEMA__.configuration TO wdb_admin;
