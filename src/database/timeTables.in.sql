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


-- Indeterminate type for time
CREATE TABLE __WDB_SCHEMA__.timeindeterminatetype (
    timeindeterminatecode			integer NOT NULL,
    timeindeterminatetype			character varying(80) NOT NULL,
    timeindeterminatedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.timeindeterminatetype
    ADD CONSTRAINT timeindeterminatetype_pkey
    PRIMARY KEY (timeindeterminatecode);

REVOKE ALL ON __WDB_SCHEMA__.timeindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.timeindeterminatetype TO wdb_admin;

INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 0, 'any', 'Values are universal constants (valid for any time)');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 1, 'exact', 'The time definition is exact');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 2, 'inside', 'The time definition is inside the given interval');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 3, 'outside', 'The time definition is outside the given interval');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 4, 'before', 'The time definition is before the given interval');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 5, 'after', 'The time definition is after the given interval');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 6, 'withheld', 'The time definition is known but withheld');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 7, 'withdrawn', 'The time definition has been removed');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 8, 'unknown', 'The time definition is unknown');
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype
	VALUES ( 9, 'delayed', 'The time definition will be given later');
