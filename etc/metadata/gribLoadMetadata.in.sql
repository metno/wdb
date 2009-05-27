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

-- GRIB Metadata
COPY gribload.generatingprocess FROM '__WDB_DATADIR__/sql/wdb_gribgeneratingprocess.csv' CSV HEADER DELIMITER '|';
COPY gribload.valueparameterxref FROM '__WDB_DATADIR__/sql/wdb_gribparameterxref.csv' CSV HEADER DELIMITER '|'; 
COPY gribload.parametertolevelxref FROM '__WDB_DATADIR__/sql/wdb_gribparametertolevelxref.csv' CSV HEADER DELIMITER '|'; 
COPY gribload.levelparameterxref FROM '__WDB_DATADIR__/sql/wdb_griblevelparameterxref.csv' CSV HEADER DELIMITER '|';
