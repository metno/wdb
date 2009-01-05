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

-- SRIDs
COPY spatial_ref_sys FROM '__WDB_DATADIR__/sql/wdb_spatial_ref_sys.csv' CSV HEADER DELIMITER '|';

-- Party (Organizations and People)
COPY __WDB_SCHEMA__.party FROM '__WDB_DATADIR__/sql/wdb_party.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organizationtype FROM '__WDB_DATADIR__/sql/wdb_organizationtype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organization FROM '__WDB_DATADIR__/sql/wdb_organization.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.organizationalias FROM '__WDB_DATADIR__/sql/wdb_organizationalias.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.person FROM '__WDB_DATADIR__/sql/wdb_person.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.softwareversion FROM '__WDB_DATADIR__/sql/wdb_softwareversion.csv' CSV HEADER DELIMITER '|';

-- Codespace
COPY __WDB_SCHEMA__.namespace FROM '__WDB_DATADIR__/sql/wdb_namespace.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.codespace FROM '__WDB_DATADIR__/sql/wdb_codespace.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.defaultCodespace FROM '__WDB_DATADIR__/sql/wdb_defaultCodespace.csv' CSV HEADER DELIMITER '|';

-- Configuration
COPY __WDB_SCHEMA__.currentconfiguration FROM '__WDB_DATADIR__/sql/wdb_currentconfiguration.csv' CSV HEADER DELIMITER '|';

-- Indeterminate Types
COPY __WDB_SCHEMA__.placeindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_placeindeterminatetype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.levelindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_levelindeterminatetype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.timeindeterminatetype FROM '__WDB_DATADIR__/sql/wdb_timeindeterminatetype.csv' CSV HEADER DELIMITER '|';

-- DataProvider
COPY __WDB_SCHEMA__.dataprovider FROM '__WDB_DATADIR__/sql/wdb_dataprovider.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.dataprovidercomment FROM '__WDB_DATADIR__/sql/wdb_dataprovidercomment.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.dataprovidername FROM '__WDB_DATADIR__/sql/wdb_dataprovidername.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.wciuserdataprovider FROM '__WDB_DATADIR__/sql/wdb_wciuserdataprovider.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.gribgeneratingprocess FROM '__WDB_DATADIR__/sql/wdb_gribgeneratingprocess.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.feltgeneratingprocess FROM '__WDB_DATADIR__/sql/wdb_feltgeneratingprocess.csv' CSV HEADER DELIMITER '|';

-- PlaceId
COPY __WDB_SCHEMA__.placedefinition FROM '__WDB_DATADIR__/sql/wdb_placedefinition.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.placeregulargrid FROM '__WDB_DATADIR__/sql/wdb_placeregulargrid.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.placename FROM '__WDB_DATADIR__/sql/wdb_placename.csv' CSV HEADER DELIMITER '|';

-- Parameters & Units
COPY __WDB_SCHEMA__.parameterfunctiontype FROM '__WDB_DATADIR__/sql/wdb_parameterfunctiontype.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.physicalphenomenon FROM '__WDB_DATADIR__/sql/wdb_physicalphenomenon.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.unit FROM '__WDB_DATADIR__/sql/wdb_unit.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.siunitconversion FROM '__WDB_DATADIR__/sql/wdb_siunitconversion.csv' CSV HEADER DELIMITER '|';
COPY __WDB_SCHEMA__.valueparameterusage FROM '__WDB_DATADIR__/sql/wdb_valueparameterusage.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelparameterusage FROM '__WDB_DATADIR__/sql/wdb_levelparameterusage.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valueparameter FROM '__WDB_DATADIR__/sql/wdb_valueparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuephysicalparameter FROM '__WDB_DATADIR__/sql/wdb_valuephysicalparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuedimensionlessparameter FROM '__WDB_DATADIR__/sql/wdb_valuedimensionlessparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuecodeparameter FROM '__WDB_DATADIR__/sql/wdb_valuecodeparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.valuestatisticsparameter FROM '__WDB_DATADIR__/sql/wdb_valuestatisticsparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelparameter FROM '__WDB_DATADIR__/sql/wdb_levelparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelphysicalparameter FROM '__WDB_DATADIR__/sql/wdb_levelphysicalparameter.csv' CSV HEADER DELIMITER '|'; 
COPY __WDB_SCHEMA__.levelcodeparameter FROM '__WDB_DATADIR__/sql/wdb_levelcodeparameter.csv' CSV HEADER DELIMITER '|'; 
-- AutoGenerate
--COPY __WDB_SCHEMA__.valueparametername FROM '__WDB_DATADIR__/sql/wdb_valueparametername.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.levelparametername FROM '__WDB_DATADIR__/sql/wdb_levelparametername.csv' CSV HEADER DELIMITER '|';

-- Quality Processes
--COPY __WDB_SCHEMA__.qualitycontrolstage FROM '__WDB_DATADIR__/sql/wdb_qualitycontrolstage.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.qualityprocess FROM '__WDB_DATADIR__/sql/wdb_qualityprocess.csv' CSV HEADER DELIMITER '|';

-- Confidence Code
COPY __WDB_SCHEMA__.qualityconfidencecode FROM '__WDB_DATADIR__/sql/wdb_qualityconfidencecode.csv' CSV HEADER DELIMITER '|';

-- GRIB Metadata
COPY gribload.valueparameterxref FROM '__WDB_DATADIR__/sql/wdb_gribparameterxref.csv' CSV HEADER DELIMITER '|'; 
COPY gribload.parametertolevelxref FROM '__WDB_DATADIR__/sql/wdb_gribparametertolevelxref.csv' CSV HEADER DELIMITER '|'; 
COPY gribload.levelparameterxref FROM '__WDB_DATADIR__/sql/wdb_griblevelparameterxref.csv' CSV HEADER DELIMITER '|';

-- FELT Metadata
COPY feltload.valueparameterxref FROM '__WDB_DATADIR__/sql/wdb_feltparameterxref.csv' CSV HEADER DELIMITER '|'; 
COPY feltload.parametertolevelxref FROM '__WDB_DATADIR__/sql/wdb_feltparametertolevelxref.csv' CSV HEADER DELIMITER '|'; 
COPY feltload.levelparameterxref FROM '__WDB_DATADIR__/sql/wdb_feltlevelparameterxref.csv' CSV HEADER DELIMITER '|';
COPY feltload.feltparametertovaliddurationxref FROM '__WDB_DATADIR__/sql/wdb_feltparametertovaliddurationxref.csv' CSV HEADER DELIMITER '|';

-- XML Metadata
COPY xmlload.valueparameterxref FROM '__WDB_DATADIR__/sql/wdb_xmlparameterxref.csv' CSV HEADER DELIMITER '|'; 

--COPY __WDB_SCHEMA__.kvalobsparameterxref FROM '__WDB_DATADIR__/sql/wdb_kvalobsparameterxref.csv' CSV HEADER DELIMITER '|';
--COPY __WDB_SCHEMA__.kvalobsvalidtimexref FROM '__WDB_DATADIR__/sql/wdb_kvalobsvalidtimexref.csv' CSV HEADER DELIMITER '|';

