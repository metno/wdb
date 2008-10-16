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


-- kvalobs is a schema that contains views, functions and tables
-- that are specific to the KvalObsLoad component
CREATE SCHEMA kvalobs;
REVOKE ALL ON SCHEMA kvalobs FROM PUBLIC;
GRANT ALL ON SCHEMA kvalobs TO wdb_admin;
GRANT USAGE ON SCHEMA kvalobs TO wdb_kvalobs;


CREATE TABLE __WDB_SCHEMA__.kvalobsparameterxref 
(
  kvalobsparameter int NOT NULL,
  statisticstype varchar(80) NOT NULL,
  unitname varchar(80) NOT NULL,
  valuedomain varchar(80) NOT NULL,
  valuedomaincodespaceid int4 NOT NULL,
  loadvalueflag bool NOT NULL DEFAULT true
);

CREATE TABLE __WDB_SCHEMA__.kvalobsvalidtimexref 
(
  parameter int NOT NULL,
  validTime interval NOT NULL
);

