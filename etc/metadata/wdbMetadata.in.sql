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
--COPY spatial_ref_sys FROM '__WDB_DATADIR__/sql/wdb_spatial_ref_sys.csv' CSV HEADER DELIMITER '|';

-- Party (Organizations and People)
COPY __WDB_SCHEMA__.party FROM '__WDB_DATADIR__/sql/wdb_party.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organizationtype FROM '__WDB_DATADIR__/sql/wdb_organizationtype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organization FROM '__WDB_DATADIR__/sql/wdb_organization.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organizationalias FROM '__WDB_DATADIR__/sql/wdb_organizationalias.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.person FROM '__WDB_DATADIR__/sql/wdb_person.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.softwareversion FROM '__WDB_DATADIR__/sql/wdb_softwareversion.csv' CSV HEADER DELIMITER '|';

-- Namespace (wci.addNameSpace, wci.getNameSpace, wci.begin, wci.setDefaultNameSpace )
COPY __WDB_SCHEMA__.namespace FROM '__WDB_DATADIR__/sql/wdb_namespace.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.defaultnamespace FROM '__WDB_DATADIR__/sql/wdb_defaultnamespace.csv' CSV HEADER DELIMITER '|';

-- Configuration (wci.version)
COPY __WDB_SCHEMA__.configuration FROM '__WDB_DATADIR__/sql/wdb_configuration.csv' CSV HEADER DELIMITER '|';

-- Indeterminate Types
COPY __WDB_SCHEMA__.placeindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_placeindeterminatetype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.levelindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_levelindeterminatetype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.timeindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_timeindeterminatetype.csv' CSV HEADER DELIMITER '|';

-- DataProvider (wci.adddataprovider, adddataprovidertogroup, addwciuser)
COPY __WDB_SCHEMA__.dataprovider FROM '__WDB_DATADIR__/sql/wdb_dataprovider.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.dataprovidercomment FROM '__WDB_DATADIR__/sql/wdb_dataprovidercomment.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.dataprovidername FROM '__WDB_DATADIR__/sql/wdb_dataprovidername.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.wciuserdataprovider FROM '__WDB_DATADIR__/sql/wdb_wciuserdataprovider.csv' CSV HEADER DELIMITER '|';

-- PlaceId (wci.addplace...)
COPY __WDB_SCHEMA__.placedefinition FROM '__WDB_DATADIR__/sql/wdb_placedefinition.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.placeregulargrid FROM '__WDB_DATADIR__/sql/wdb_placeregulargrid.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.placename FROM '__WDB_DATADIR__/sql/wdb_placename.csv' CSV HEADER DELIMITER '|';

-- Parameters & Units (wci.addparamater...)
COPY __WDB_SCHEMA__.measure FROM '__WDB_DATADIR__/sql/wdb_measure.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.unit FROM '__WDB_DATADIR__/sql/wdb_unit.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.siunitconversion FROM '__WDB_DATADIR__/sql/wdb_siunitconversion.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.parameterfunctiontype FROM '__WDB_DATADIR__/sql/wdb_parameterfunctiontype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.valueparameterusage FROM '__WDB_DATADIR__/sql/wdb_valueparameterusage.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelparameterusage FROM '__WDB_DATADIR__/sql/wdb_levelparameterusage.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valueparameter FROM '__WDB_DATADIR__/sql/wdb_valueparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuemeasureparameter FROM '__WDB_DATADIR__/sql/wdb_valuemeasureparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuedimensionlessparameter FROM '__WDB_DATADIR__/sql/wdb_valuedimensionlessparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuecodeparameter FROM '__WDB_DATADIR__/sql/wdb_valuecodeparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuefunctionparameter FROM '__WDB_DATADIR__/sql/wdb_valuefunctionparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelparameter FROM '__WDB_DATADIR__/sql/wdb_levelparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelmeasureparameter FROM '__WDB_DATADIR__/sql/wdb_levelmeasureparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelcodeparameter FROM '__WDB_DATADIR__/sql/wdb_levelcodeparameter.csv' CSV HEADER DELIMITER '|'; 
-- AutoGenerate
--COPY __WDB_SCHEMA__.valueparametername FROM '__WDB_DATADIR__/sql/wdb_valueparametername.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.levelparametername FROM '__WDB_DATADIR__/sql/wdb_levelparametername.csv' CSV HEADER DELIMITER '|';

-- Confidence Code
COPY __WDB_SCHEMA__.qualityconfidencecode FROM '__WDB_DATADIR__/sql/wdb_qualityconfidencecode.csv' CSV HEADER DELIMITER '|';
