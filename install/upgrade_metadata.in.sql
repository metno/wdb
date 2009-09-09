-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2008 met.no
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

-- Party (Organizations and People)
INSERT INTO __WDB_SCHEMA__.party SELECT * FROM __OLD_SCHEMA__.party src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.party WHERE partyid = src.partyid );
INSERT INTO __WDB_SCHEMA__.partycomment SELECT * FROM __OLD_SCHEMA__.partycomment src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.partycomment WHERE partyid = src.partyid and partycommentstoretime = src.partycommentstoretime );
INSERT INTO __WDB_SCHEMA__.organizationtype SELECT * FROM __OLD_SCHEMA__.organizationtype src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.organizationtype WHERE organizationtype = src.organizationtype );
INSERT INTO __WDB_SCHEMA__.organization SELECT * FROM __OLD_SCHEMA__.organization src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.organization WHERE partyid = src.partyid );
INSERT INTO __WDB_SCHEMA__.organizationalias SELECT * FROM __OLD_SCHEMA__.organizationalias src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.organizationalias WHERE partyid = src.partyid AND alternativeorganizationname = src.alternativeorganizationname );
INSERT INTO __WDB_SCHEMA__.person SELECT * FROM __OLD_SCHEMA__.person src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.person WHERE partyid = src.partyid );
INSERT INTO __WDB_SCHEMA__.softwareversion SELECT * FROM __OLD_SCHEMA__.softwareversion src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.softwareversion WHERE partyid = src.partyid );

-- Configuration
-- NOOP (Singleton)

-- Namespace
INSERT INTO __WDB_SCHEMA__.namespace SELECT * FROM __OLD_SCHEMA__.namespace src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.namespace WHERE namespaceid = src.namespaceid );

-- Indeterminate Types
INSERT INTO __WDB_SCHEMA__.placeindeterminatetype SELECT * FROM __OLD_SCHEMA__.placeindeterminatetype src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.placeindeterminatetype WHERE placeindeterminatecode = src.placeindeterminatecode );
INSERT INTO __WDB_SCHEMA__.timeindeterminatetype SELECT * FROM __OLD_SCHEMA__.timeindeterminatetype src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.timeindeterminatetype WHERE timeindeterminatecode = src.timeindeterminatecode );
INSERT INTO __WDB_SCHEMA__.levelindeterminatetype  SELECT * FROM __OLD_SCHEMA__.levelindeterminatetype src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.levelindeterminatetype WHERE levelindeterminatecode = src.levelindeterminatecode );

-- DataProvider
INSERT INTO __WDB_SCHEMA__.dataprovider SELECT * FROM __OLD_SCHEMA__.dataprovider src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.dataprovider WHERE dataproviderid = src.dataproviderid );
INSERT INTO __WDB_SCHEMA__.dataprovidername SELECT * FROM __OLD_SCHEMA__.dataprovidername src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.dataprovidername WHERE dataproviderid = src.dataproviderid AND dataprovidernamespaceid = src.dataprovidernamespaceid );
INSERT INTO __WDB_SCHEMA__.dataprovidercomment SELECT * FROM __OLD_SCHEMA__.dataprovidercomment src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.dataprovidercomment WHERE dataproviderid = src.dataproviderid AND dataprovidercommentstoretime = src.dataprovidercommentstoretime );
INSERT INTO __WDB_SCHEMA__.wciuserdataprovider  SELECT * FROM __OLD_SCHEMA__.wciuserdataprovider src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.wciuserdataprovider WHERE dataproviderid = src.dataproviderid );

-- PlaceId
INSERT INTO __WDB_SCHEMA__.placeregulargrid SELECT * FROM __OLD_SCHEMA__.placeregulargrid src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.placeregulargrid WHERE placeid = src.placeid );
INSERT INTO __WDB_SCHEMA__.placedefinition SELECT * FROM __OLD_SCHEMA__.placedefinition src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.placedefinition WHERE placeid = src.placeid );
INSERT INTO __WDB_SCHEMA__.placename SELECT * FROM __OLD_SCHEMA__.placename src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.placename WHERE placeid = src.placeid AND placenamespaceid = src.placenamespaceid );

-- Parameters & Units
INSERT INTO __WDB_SCHEMA__.measure SELECT * FROM __OLD_SCHEMA__.measure src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.measure WHERE measure = src.measure );
INSERT INTO __WDB_SCHEMA__.unit SELECT * FROM __OLD_SCHEMA__.unit src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.unit WHERE unitname = src.unitname );
INSERT INTO __WDB_SCHEMA__.siunitconversion SELECT * FROM __OLD_SCHEMA__.siunitconversion src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.siunitconversion WHERE unitname = src.unitname );
INSERT INTO __WDB_SCHEMA__.parameterfunctiontype SELECT * FROM __OLD_SCHEMA__.parameterfunctiontype src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.parameterfunctiontype WHERE parameterfunctiontype = src.parameterfunctiontype );
INSERT INTO __WDB_SCHEMA__.valueparameterusage SELECT * FROM __OLD_SCHEMA__.valueparameterusage src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valueparameterusage WHERE valueparameterusage = src.valueparameterusage ); 
INSERT INTO __WDB_SCHEMA__.valueparameter SELECT * FROM __OLD_SCHEMA__.valueparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valueparameter WHERE valueparameterid = src.valueparameterid ); 
INSERT INTO __WDB_SCHEMA__.valuemeasureparameter SELECT * FROM __OLD_SCHEMA__.valuemeasureparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valuemeasureparameter WHERE valueparameterid = src.valueparameterid ); 
INSERT INTO __WDB_SCHEMA__.valuedimensionlessparameter SELECT * FROM __OLD_SCHEMA__.valuedimensionlessparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valuedimensionlessparameter WHERE valueparameterid = src.valueparameterid ); 
INSERT INTO __WDB_SCHEMA__.valuecodeparameter SELECT * FROM __OLD_SCHEMA__.valuecodeparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valuecodeparameter WHERE valueparameterid = src.valueparameterid ); 
INSERT INTO __WDB_SCHEMA__.valuefunctionparameter SELECT * FROM __OLD_SCHEMA__.valuefunctionparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valuefunctionparameter WHERE valueparameterid = src.valueparameterid ); 
INSERT INTO __WDB_SCHEMA__.valueparametername SELECT * FROM __OLD_SCHEMA__.valueparametername src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.valueparametername WHERE valueparameterid = src.valueparameterid AND parameternamespaceid = src.parameternamespaceid ); 
INSERT INTO __WDB_SCHEMA__.levelparameterusage SELECT * FROM __OLD_SCHEMA__.levelparameterusage src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.levelparameterusage WHERE levelparameterusage = src.levelparameterusage ); 
INSERT INTO __WDB_SCHEMA__.levelparameter SELECT * FROM __OLD_SCHEMA__.levelparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.levelparameter WHERE levelparameterid = src.levelparameterid ); 
INSERT INTO __WDB_SCHEMA__.levelmeasureparameter SELECT * FROM __OLD_SCHEMA__.levelmeasureparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.levelmeasureparameter WHERE levelparameterid = src.levelparameterid ); 
INSERT INTO __WDB_SCHEMA__.levelcodeparameter SELECT * FROM __OLD_SCHEMA__.levelcodeparameter src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.levelcodeparameter WHERE levelparameterid = src.levelparameterid ); 

-- Confidence Code
INSERT INTO __WDB_SCHEMA__.qualityconfidencecode SELECT * FROM __OLD_SCHEMA__.qualityconfidencecode src WHERE NOT EXISTS ( SELECT * FROM __WDB_SCHEMA__.qualityconfidencecode WHERE confidencecode = src.confidencecode );
