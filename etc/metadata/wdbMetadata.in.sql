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


-- SRIDs (wci.addSrid, wci.getSrid)
\i __WDB_DATADIR__/sql/wdb_spatial_ref_sys.sql

-- Party (Organizations and People)
\i __WDB_DATADIR__/sql/wdb_party.sql

-- Namespace (wci.addNameSpace, wci.getNameSpace, wci.begin, wci.setDefaultNameSpace )
\i __WDB_DATADIR__/sql/wdb_namespace.sql

-- Configuration (wci.version)
--COPY __WDB_SCHEMA__.configuration FROM '__WDB_DATADIR__/sql/wdb_configuration.csv' CSV HEADER DELIMITER '|';
SELECT wci.setConfiguration( 'WDB', '__WDB_VERSION__', __SCHEMA_VERSION__ );

-- DataProvider (wci.adddataprovider, adddataprovidertogroup, addwciuser)
\i __WDB_DATADIR__/sql/wdb_dataprovider.sql

-- PlaceId (wci.addplace...)
\i __WDB_DATADIR__/sql/wdb_placepoint.sql
\i __WDB_DATADIR__/sql/wdb_placepolygon.sql
\i __WDB_DATADIR__/sql/wdb_placeregulargrid.sql

-- Parameters & Units (wci.addparamater...)
\i __WDB_DATADIR__/sql/wdb_unit.sql
\i __WDB_DATADIR__/sql/wdb_cfsurface.sql
\i __WDB_DATADIR__/sql/wdb_cfcomponent.sql
\i __WDB_DATADIR__/sql/wdb_cfmedium.sql
\i __WDB_DATADIR__/sql/wdb_cfmethods.sql
\i __WDB_DATADIR__/sql/wdb_parameter.sql
