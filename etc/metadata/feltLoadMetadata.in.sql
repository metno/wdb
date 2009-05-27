-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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

-- FELT Metadata
COPY feltload.generatingprocess FROM '__WDB_DATADIR__/sql/wdb_feltgeneratingprocess.csv' CSV HEADER DELIMITER '|';
COPY feltload.valueparameterxref FROM '__WDB_DATADIR__/sql/wdb_feltparameterxref.csv' CSV HEADER DELIMITER '|'; 
COPY feltload.parametertolevelxref FROM '__WDB_DATADIR__/sql/wdb_feltparametertolevelxref.csv' CSV HEADER DELIMITER '|'; 
COPY feltload.levelparameterxref FROM '__WDB_DATADIR__/sql/wdb_feltlevelparameterxref.csv' CSV HEADER DELIMITER '|';
COPY feltload.feltparametertovaliddurationxref FROM '__WDB_DATADIR__/sql/wdb_feltparametertovaliddurationxref.csv' CSV HEADER DELIMITER '|';
