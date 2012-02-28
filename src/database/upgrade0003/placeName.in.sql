-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2012 met.no
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
--
-- Update Schema to use Place Definition with Valid Times
-- 

SET SESSION client_min_messages TO 'warning';

-- Place Name Table
ALTER TABLE __WDB_SCHEMA__.placename ADD COLUMN 
    placenameupdatetime	timestamp with time zone NOT NULL default 'now';

ALTER TABLE ONLY __WDB_SCHEMA__.placename
    DROP CONSTRAINT placename_pkey;
ALTER TABLE ONLY __WDB_SCHEMA__.placename
    ADD CONSTRAINT placename_pkey PRIMARY KEY (placeid, placenamespaceid, placename, placenamevalidfrom);

ALTER TABLE ONLY __WDB_SCHEMA__.placename
    DROP CONSTRAINT placename_unique;
ALTER TABLE ONLY __WDB_SCHEMA__.placename
	ADD CONSTRAINT placename_unique UNIQUE (placenamespaceid, placename, placenamevalidfrom);



CREATE VIEW __WCI_SCHEMA__.placename_valid_v AS
SELECT
    p.placeid,
    p.placename,
    p.placenamevalidfrom,
    p.placenamevalidto
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename_valid_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_valid_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_valid_V TO wdb_read, wdb_write;
