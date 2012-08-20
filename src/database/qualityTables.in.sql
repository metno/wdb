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

CREATE TABLE __WDB_SCHEMA__.qualityconfidencecode (
    confidencecode				integer NOT NULL,
    confidencecodedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.qualityconfidencecode
    ADD CONSTRAINT confidencecode_pkey PRIMARY KEY (confidencecode);

REVOKE ALL ON __WDB_SCHEMA__.qualityconfidencecode FROM public;
GRANT ALL ON __WDB_SCHEMA__.qualityconfidencecode TO wdb_admin;

INSERT INTO __WDB_SCHEMA__.qualityconfidencecode VALUES ( 0, 'not defined' );
