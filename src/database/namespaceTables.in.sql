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


-- Namespace descriptors
CREATE TABLE __WDB_SCHEMA__.namespace (
    namespaceid					integer NOT NULL,
    namespacename				character varying(80) NOT NULL,
    namespacedescription		character varying(255) NOT NULL,
    namespacefieldofapplication character varying(255) NOT NULL,
    namespaceownerid			integer NOT NULL,
    namespacecontactid			integer NOT NULL,
    namespacevalidfrom 			date NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.namespace
    ADD CONSTRAINT namespace_pkey
    PRIMARY KEY (namespaceid);

ALTER TABLE __WDB_SCHEMA__.namespace
	ADD FOREIGN KEY (namespaceownerid)
					REFERENCES __WDB_SCHEMA__.organization
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

ALTER TABLE __WDB_SCHEMA__.namespace
	ADD FOREIGN KEY (namespacecontactid)
					REFERENCES __WDB_SCHEMA__.person
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

REVOKE ALL ON __WDB_SCHEMA__.namespace FROM public;
GRANT ALL ON __WDB_SCHEMA__.namespace TO wdb_admin;

--
-- WCI Tables
--
CREATE TABLE __WDB_SCHEMA__.defaultnamespace (
	rolname 				NAME NOT NULL PRIMARY KEY,
	dataprovidernamespaceid integer NOT NULL REFERENCES __WDB_SCHEMA__.namespace (namespaceid),
	placenamespaceid		integer NOT NULL REFERENCES __WDB_SCHEMA__.namespace (namespaceid),
	parameternamespaceid	integer NOT NULL REFERENCES __WDB_SCHEMA__.namespace (namespaceid)
);

REVOKE ALL ON __WDB_SCHEMA__.defaultnamespace FROM public;
GRANT ALL ON __WDB_SCHEMA__.defaultnamespace TO wdb_admin;