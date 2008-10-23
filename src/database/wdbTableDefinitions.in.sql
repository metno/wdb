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

-- wdb is the core schema of the WDB system. It contains the core data 
-- tables, functions, and views
CREATE SCHEMA __WDB_SCHEMA__;
REVOKE ALL ON SCHEMA __WDB_SCHEMA__ FROM PUBLIC;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_admin;
GRANT USAGE ON SCHEMA __WDB_SCHEMA__ TO wdb_write;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_clean;


CREATE TABLE __WDB_SCHEMA__.currentconfiguration (
    singleton					integer NOT NULL,
    softwareversionpartyid		integer NOT NULL,
    productionstatus			character varying(80) NOT NULL,
    CONSTRAINT currentconfiguration_productionstatus_check
	CHECK (	((productionstatus)::text = 'Development'::text) OR 
			((productionstatus)::text = 'Integration Test'::text) OR
			((productionstatus)::text = 'Production Test'::text) OR
			((productionstatus)::text = 'Production'::text) ),
    CONSTRAINT currentconfiguration_singleton_check 
	CHECK ( singleton = 1 )
);

REVOKE ALL ON __WDB_SCHEMA__.currentconfiguration FROM public;
GRANT ALL ON __WDB_SCHEMA__.currentconfiguration TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.namespace (
    namespaceid					integer NOT NULL,
    namespacename				character varying(80) NOT NULL,
    namespacedescription		character varying(255) NOT NULL,
    namespacefieldofapplication character varying(255) NOT NULL,
    namespaceownerid			integer NOT NULL,
    namespacecontactid			integer NOT NULL,
    namespacevalidfrom 			date NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.namespace FROM public;
GRANT ALL ON __WDB_SCHEMA__.namespace TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.party (
    partyid						serial NOT NULL,
    partytype 					character varying(80) NOT NULL,
    partyvalidfrom 				date NOT NULL,
    partyvalidto 				date NOT NULL,
    CONSTRAINT party_partytype_check 
	CHECK (	((partytype)::text = 'Organization'::text) OR 
			((partytype)::text = 'Person'::text) OR 
			((partytype)::text = 'Software'::text) )
);

REVOKE ALL ON __WDB_SCHEMA__.party FROM public;
GRANT ALL ON __WDB_SCHEMA__.party TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.partycomment (
	partyid						integer NOT NULL,
	partycomment				character varying(255) NOT NULL,
    partycommentstoretime		timestamp with time zone NOT NULL
);


REVOKE ALL ON __WDB_SCHEMA__.partycomment FROM public;
GRANT ALL ON __WDB_SCHEMA__.partycomment TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.organization (
    partyid 					integer NOT NULL,
    organizationname 			character varying(80) NOT NULL,
    organizationtype 			character varying(80) NOT NULL,
    organizationdescription 	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.organization FROM public;
GRANT ALL ON __WDB_SCHEMA__.organization TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.organizationtype (
    organizationtype 			character varying(80) NOT NULL,
	organizationtypedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.organizationtype FROM public;
GRANT ALL ON __WDB_SCHEMA__.organizationtype TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.organizationalias (
    partyid 					integer NOT NULL,
    alternativeorganizationname	character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.organizationalias FROM public;
GRANT ALL ON __WDB_SCHEMA__.organizationalias TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.person (
    partyid 					integer NOT NULL,
    firstname 					character varying(80),
    lastname 					character varying(80),
    title 						character varying(80),
    salutation 					character varying(80),
    initials 					character varying(80),
    signum 						character varying(80),
    gender 						character varying(80),
    namesuffix 					character varying(80),
    maritalstatus 				character varying(80),
    CONSTRAINT person_gender_check 
	CHECK ( ((gender)::text = 'Male'::text) OR 
			((gender)::text = 'Female'::text) ),
    CONSTRAINT person_maritalstatus_check 
	CHECK (	((maritalstatus)::text = 'Married'::text) OR 
			((maritalstatus)::text = 'Single'::text) )
);

REVOKE ALL ON __WDB_SCHEMA__.person FROM public;
GRANT ALL ON __WDB_SCHEMA__.person TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.softwareversion
(
	partyid						integer NOT NULL,
	softwarename 				character varying(80) NOT NULL,
	softwareversioncode			character varying(80) NOT NULL,
	softwaredescription 		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.softwareversion FROM public;
GRANT ALL ON __WDB_SCHEMA__.softwareversion TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.dataprovider (
    dataproviderid 				bigserial NOT NULL,
    dataprovidertype 			character varying(80) NOT NULL,
    spatialdomaindelivery		character varying(80) NOT NULL,
    dataproviderstoretime 		timestamp with time zone NOT NULL,
    CONSTRAINT dataprovider_dataprovidertype_check 
	CHECK (	((dataprovidertype)::text = 'Data Provider Group'::text) OR
			((dataprovidertype)::text = 'GRIB Generating Process'::text) OR 
			((dataprovidertype)::text = 'WCI User'::text) OR 
			((dataprovidertype)::text = 'Named Observation Site'::text) OR
			((dataprovidertype)::text = 'Ship'::text) OR 
			((dataprovidertype)::text = 'Aeroplane'::text) OR 
			((dataprovidertype)::text = 'Computer System'::text) ),
    CONSTRAINT dataprovider_spatialdomaindelivery_check 
	CHECK (	((spatialdomaindelivery)::text = 'Any'::text) OR
			((spatialdomaindelivery)::text = 'Point'::text) OR 
			((spatialdomaindelivery)::text = 'Grid'::text) )
);

REVOKE ALL ON __WDB_SCHEMA__.dataprovider FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovider TO wdb_admin;
-- This sets startvalue to match that of a wci writer:
SELECT setval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass, 1000000);



CREATE TABLE __WDB_SCHEMA__.dataprovidercomment (
    dataproviderid 					bigserial NOT NULL,
	dataprovidercomment				character varying(254) NOT NULL,
    dataprovidercommentstoretime	timestamp with time zone NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.dataprovidercomment FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovidercomment TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.gribgeneratingprocess
(
    dataproviderid  			bigint NOT NULL,
    generatingcenterid 			integer NOT NULL,
    generatingprocessid 		integer NOT NULL,
    generatingprocessvalidfrom 	timestamp with time zone NOT NULL,
    generatingprocessvalidto 	timestamp with time zone NOT NULL	
);

REVOKE ALL ON __WDB_SCHEMA__.gribgeneratingprocess FROM public;
GRANT ALL ON __WDB_SCHEMA__.gribgeneratingprocess TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.feltgeneratingprocess
(
    dataproviderid  			bigint NOT NULL,
    producerid 					integer NOT NULL,
    gridareanumber		 		integer NOT NULL,
    feltprocessvalidfrom	 	timestamp with time zone NOT NULL,
    feltprocessvalidto		 	timestamp with time zone NOT NULL	
);

REVOKE ALL ON __WDB_SCHEMA__.feltgeneratingprocess FROM public;
GRANT ALL ON __WDB_SCHEMA__.feltgeneratingprocess TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.wciuserdataprovider
(
  	dataproviderid 				bigint NOT NULL,
  	rolname						text NOT NULL
	--, partyid						integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.wciuserdataprovider FROM public;
GRANT ALL ON __WDB_SCHEMA__.wciuserdataprovider TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.dataprovidername (
	dataproviderid 				bigint NOT NULL,
	dataprovidernamespaceid 	integer NOT NULL,
	dataprovidername 			character varying(255) NOT NULL,
    dataprovidernamevalidfrom 	timestamp with time zone NOT NULL,
    dataprovidernamevalidto 	timestamp with time zone NOT NULL,
	dataprovidernameleftset		bigint NOT NULL,
	dataprovidernamerightset	bigint NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.dataprovidername FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovidername TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.placedefinition (
    placeid						bigserial NOT NULL,
    placeindeterminatecode 		integer NOT NULL,
    placegeometrytype 			character varying(80) NOT NULL,
    placestoretime				timestamp with time zone NOT NULL,
    CONSTRAINT placedefinition_placegeometrytype_check 
	CHECK (	((placegeometrytype)::text = 'Point'::text) OR
			((placegeometrytype)::text = 'Grid'::text) OR
			((placegeometrytype)::text = 'Line'::text) )
);
SELECT AddGeometryColumn('__WDB_SCHEMA__', 'placedefinition', 'placegeometry', 4030, 'GEOMETRY', 2);
ALTER SEQUENCE __WDB_SCHEMA__.placedefinition_placeid_seq RESTART WITH 100000;

REVOKE ALL ON __WDB_SCHEMA__.placedefinition FROM public;
GRANT ALL ON __WDB_SCHEMA__.placedefinition TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.placedefinition TO wdb_clean;
GRANT INSERT ON __WDB_SCHEMA__.placedefinition TO wdb_write;
REVOKE ALL ON __WDB_SCHEMA__.placedefinition_placeid_seq FROM public;
GRANT ALL ON __WDB_SCHEMA__.placedefinition_placeid_seq TO wdb_admin;
GRANT SELECT,UPDATE ON __WDB_SCHEMA__.placedefinition_placeid_seq TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.placeindeterminatetype (
    placeindeterminatecode		integer NOT NULL,
    placeindeterminatetype		character varying(80) NOT NULL,
    placeindeterminatedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.placeindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.placeindeterminatetype TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.placeregulargrid (
    placeid						bigint NOT NULL,
    inumber						integer NOT NULL,
    jnumber						integer NOT NULL,
    iincrement					real NOT NULL,
    jincrement					real NOT NULL,
    startLongitude				real NOT NULL,
    startLatitude				real NOT NULL,
    originalsrid				integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.placeregulargrid FROM public;
GRANT ALL ON __WDB_SCHEMA__.placeregulargrid TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.placeregulargrid TO wdb_write;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.placeregulargrid TO wdb_clean;


CREATE TABLE __WDB_SCHEMA__.placename (
    placeid 					bigint NOT NULL,
    placenamespaceid 			integer NOT NULL,
    placename 					character varying(255) NOT NULL,
    placenamevalidfrom 			timestamp with time zone NOT NULL,
    placenamevalidto 			timestamp with time zone NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.placename FROM public;
GRANT ALL ON __WDB_SCHEMA__.placename TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.timeindeterminatetype (
    timeindeterminatecode		integer NOT NULL,
    timeindeterminatetype		character varying(80) NOT NULL,
    timeindeterminatedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.timeindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.timeindeterminatetype TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.statisticstype (
    statisticstype 				character varying(80) NOT NULL,
    statisticsdescription 		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.statisticstype FROM public;
GRANT ALL ON __WDB_SCHEMA__.statisticstype TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.physicalphenomenon (
    physicalphenomenon			character varying(80) NOT NULL,
    lengthpower					integer NOT NULL,
    masspower					integer NOT NULL,
    timepower					integer NOT NULL,
    luminositypower				integer NOT NULL,
    electriccurrentpower		integer NOT NULL,
    temperaturepower			integer NOT NULL,
    substanceamountpower		integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.physicalphenomenon FROM public;
GRANT ALL ON __WDB_SCHEMA__.physicalphenomenon TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.valueparameterusage (
    valueparameterusage			character varying(80) NOT NULL,
	valueparameterusagedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparameterusage TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelparameterusage (
    levelparameterusage			character varying(80) NOT NULL,
    levelparameterusagedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameterusage TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.unit (
    unitname					character varying(80) NOT NULL,
    unittype					character varying(80) NOT NULL,
    physicalphenomenon			character varying(80) NOT NULL,
    CONSTRAINT unit_unittype_check
	CHECK (	((unittype)::text = 'SI Unit'::text) OR
			((unittype)::text = 'Conventional Unit'::text) )
);

REVOKE ALL ON __WDB_SCHEMA__.unit FROM public;
GRANT ALL ON __WDB_SCHEMA__.unit TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.unitabbreviation (
    unitabbreviation			character varying(80) NOT NULL,
    unitname					character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.unitabbreviation FROM public;
GRANT ALL ON __WDB_SCHEMA__.unitabbreviation TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.siunitconversion (
    unitname						character varying(80) NOT NULL,
    siunitconversioncoefficient		real NOT NULL,
    siunitconversionterm			real NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.siunitconversion FROM public;
GRANT ALL ON __WDB_SCHEMA__.siunitconversion TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.valueparameter (
	valueparameterid 			serial NOT NULL,
	valueparametertype 			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.valueparameter TO wdb_write;
--GRANT SELECT, INSERT, UPDATE ON __WDB_SCHEMA__.valueparameter_valueparameterid_seq TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.valuephysicalparameter (
	valueparameterid 				integer NOT NULL,
	valueparameterunitname 			character varying(80) NOT NULL,
	valueparameterusage 			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuephysicalparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuephysicalparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.valuephysicalparameter TO wdb_write;

CREATE TABLE __WDB_SCHEMA__.valuedimensionlessparameter (
	valueparameterid 				integer NOT NULL,
	valuedimensionlessparametername character varying(255) NOT NULL,
	valueparameterdescription		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuedimensionlessparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuedimensionlessparameter TO wdb_admin;

CREATE TABLE __WDB_SCHEMA__.valuecodeparameter (
	valueparameterid 			integer NOT NULL,
	valuecodeparametername 		character varying(255) NOT NULL,
	codeparameterreference 		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuecodeparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuecodeparameter TO wdb_admin;

CREATE TABLE __WDB_SCHEMA__.valuestatisticsparameter (
	valueparameterid 				integer NOT NULL,
    valueparameterstatisticstype	character varying(80) NOT NULL,
	basevalueparameterid 			integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuestatisticsparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuestatisticsparameter TO wdb_admin;

CREATE TABLE __WDB_SCHEMA__.valueparametername (
	valueparameterid 			integer NOT NULL,
	parameternamespaceid 		integer NOT NULL,
	valueparametername 			character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueparametername FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparametername TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelparameter (
	levelparameterid 			serial NOT NULL,
	levelparametertype 			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelparameter TO wdb_write;
--GRANT SELECT, INSERT, UPDATE ON __WDB_SCHEMA__.levelparameter_levelparameterid_seq TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.levelphysicalparameter (
	levelparameterid 			integer NOT NULL,
	levelparameterunitname 		character varying(80) NOT NULL,
    levelparameterusage 		character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelphysicalparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelphysicalparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelphysicalparameter TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.levelcodeparameter (
	levelparameterid 			integer NOT NULL,
	levelcodeparametername 		character varying(255) NOT NULL,
	codeparameterreference 		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelcodeparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelcodeparameter TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelparametername (
	levelparameterid 			integer NOT NULL,
	parameternamespaceid 		integer NOT NULL,
	levelparametername 			character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelparametername FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparametername TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelindeterminatetype (
    levelindeterminatecode		integer NOT NULL,
    levelindeterminatetype		character varying(80) NOT NULL,
    levelindeterminatetypedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelindeterminatetype TO wdb_admin;


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



--
-- PlacePoint
--
CREATE TABLE __WDB_SCHEMA__.placepoint (
    placeid bigint NOT NULL,
    i integer NOT NULL,
    j integer NOT NULL
);
SELECT AddGeometryColumn( '__WDB_SCHEMA__', 'placepoint', 'location', 4030, 'POINT', 2 );

REVOKE ALL ON TABLE __WDB_SCHEMA__.placepoint FROM PUBLIC;
GRANT ALL ON TABLE __WDB_SCHEMA__.placepoint TO wdb_admin;
GRANT INSERT ON TABLE __WDB_SCHEMA__.placepoint TO wdb_write;

