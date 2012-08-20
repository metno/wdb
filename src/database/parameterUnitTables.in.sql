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

-- Base Unit Table
CREATE TABLE __WDB_SCHEMA__.unit (
    unitname					character varying(80) NOT NULL,
    unittype					character varying(80) NOT NULL,
	description					character varying(255) NOT NULL
    CONSTRAINT unit_unittype_check
	CHECK (	((unittype)::text = 'derived unit'::text) OR
			((unittype)::text = 'base unit'::text))
);

ALTER TABLE ONLY __WDB_SCHEMA__.unit
    ADD CONSTRAINT unit_pkey PRIMARY KEY (unitname);

REVOKE ALL ON __WDB_SCHEMA__.unit FROM public;
GRANT ALL ON __WDB_SCHEMA__.unit TO wdb_admin;


-- Conversion tables
CREATE TABLE __WDB_SCHEMA__.baseunitconversion (
    unitname						character varying(80) NOT NULL,
    baseunitname					character varying(80) NOT NULL,
    baseunitconversioncoefficient	float NOT NULL,
    baseunitconversionterm			float NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.baseunitconversion
    ADD CONSTRAINT baseunitconversion_pkey PRIMARY KEY (unitname);

ALTER TABLE __WDB_SCHEMA__.baseunitconversion
	ADD FOREIGN KEY (unitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.baseunitconversion FROM public;
GRANT ALL ON __WDB_SCHEMA__.baseunitconversion TO wdb_admin;
