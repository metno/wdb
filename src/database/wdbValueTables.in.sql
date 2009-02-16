-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007-2009 met.no
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

REVOKE ALL ON __WDB_SCHEMA__.qualityconfidencecode FROM public;
GRANT ALL ON __WDB_SCHEMA__.qualityconfidencecode TO wdb_admin;


--
-- Name: FloatValue 
--
CREATE TABLE __WDB_SCHEMA__.floatvalue (
    valueid						bigserial NOT NULL,
    valuetype 					character varying(80) NOT NULL,
    dataproviderid 				bigint NOT NULL,
    placeid						bigint NOT NULL,
    referencetime				timestamp with time zone NOT NULL,
    validtimefrom				timestamp with time zone NOT NULL,
    validtimeto					timestamp with time zone NOT NULL,
    validtimeindeterminatecode	integer NOT NULL,
    valueparameterid			integer NOT NULL,
    levelparameterid			integer NOT NULL,
    levelfrom					real NOT NULL,
    levelto						real NOT NULL,
    levelindeterminatecode		integer NOT NULL,
    dataversion					integer NOT NULL,
    maxdataversion				integer NOT NULL,
    confidencecode				integer NOT NULL,
    value						real NOT NULL,
    valuestoretime				timestamp with time zone NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.floatvalue FROM public;
GRANT ALL ON __WDB_SCHEMA__.floatvalue TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.floatvalue TO wdb_clean;
--GRANT INSERT ON __WDB_SCHEMA__.floatvalue TO wdb_write;
--REVOKE ALL ON __WDB_SCHEMA__.floatvalue_valueid_seq FROM public;
--GRANT ALL ON __WDB_SCHEMA__.floatvalue_valueid_seq TO wdb_admin;
--GRANT SELECT,UPDATE ON __WDB_SCHEMA__.floatvalue_valueid_seq TO wdb_write;


--
-- Name: OIDValue 
--
CREATE TABLE __WDB_SCHEMA__.oidvalue (
    valueid 					bigserial NOT NULL,
    valuetype 					character varying(80) NOT NULL,   
	dataproviderid 				bigint NOT NULL,
    placeid 					bigint NOT NULL,
    referencetime				timestamp with time zone NOT NULL,
	validtimefrom 				timestamp with time zone NOT NULL,
    validtimeto 				timestamp with time zone NOT NULL,
    validtimeindeterminatecode	integer NOT NULL,  
	valueparameterid 			integer NOT NULL, 
	levelparameterid 			integer NOT NULL,
    levelfrom 					real NOT NULL,
    levelto 					real NOT NULL,
    levelindeterminatecode		integer NOT NULL,
    dataversion 				integer NOT NULL,
	maxdataversion 				integer NOT NULL,
    confidencecode 				integer NOT NULL,
    value 						oid NOT NULL,
    valuestoretime 				timestamp with time zone NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.oidvalue FROM public;
GRANT ALL ON __WDB_SCHEMA__.oidvalue TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.oidvalue TO wdb_clean;
REVOKE ALL ON __WDB_SCHEMA__.oidvalue_valueid_seq FROM public;
GRANT ALL ON __WDB_SCHEMA__.oidvalue_valueid_seq TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.valueadditionallevelinfo (
    valueid 					bigint NOT NULL,
    valuetype					character varying(80) NOT NULL,
    levelparameterid			integer NOT NULL,
    levelfrom					real NOT NULL,
    levelto						real NOT NULL,
    levelindeterminatecode		integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueadditionallevelinfo FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueadditionallevelinfo TO wdb_admin;
--GRANT INSERT ON  __WDB_SCHEMA__.valueadditionallevelinfo TO wdb_write;
--GRANT SELECT, DELETE ON __WDB_SCHEMA__.valueadditionallevelinfo TO wdb_clean;


--
-- WCI Tables
--
CREATE TABLE __WDB_SCHEMA__.defaultnamespace (
	rolname 				NAME NOT NULL,
	dataprovidernamespaceid integer NOT NULL,
	placenamespaceid		integer NOT NULL,
	parameternamespaceid	integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.defaultnamespace FROM public;
GRANT ALL ON __WDB_SCHEMA__.defaultnamespace TO wdb_admin;
