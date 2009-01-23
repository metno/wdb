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


-- wdb_x_x_x is the core schema of the WDB system. It contains the core data 
-- tables, functions, and views
CREATE SCHEMA __WDB_SCHEMA__;
REVOKE ALL ON SCHEMA __WDB_SCHEMA__ FROM PUBLIC;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_admin;
GRANT USAGE ON SCHEMA __WDB_SCHEMA__ TO wdb_write;
GRANT ALL ON SCHEMA __WDB_SCHEMA__ TO wdb_clean;



CREATE TABLE __WDB_SCHEMA__.measure (
    measure						character varying(80) NOT NULL,
    distancepower				integer NOT NULL,
    masspower					integer NOT NULL,
    timepower					integer NOT NULL,
    luminositypower				integer NOT NULL,
    electriccurrentpower		integer NOT NULL,
    temperaturepower			integer NOT NULL,
    substanceamountpower		integer NOT NULL,
	anglepower					integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.measure FROM public;
GRANT ALL ON __WDB_SCHEMA__.measure TO wdb_admin;

ALTER TABLE ONLY __WDB_SCHEMA__.measure
    ADD CONSTRAINT measures_pkey PRIMARY KEY (measure);



CREATE TABLE __WDB_SCHEMA__.unit (
    unitname					character varying(80) NOT NULL,
    unittype					character varying(80) NOT NULL,
    measure						character varying(80) NOT NULL,
	description					character varying(255) NOT NULL
    CONSTRAINT unit_unittype_check
	CHECK (	((unittype)::text = 'SI Unit'::text) OR
			((unittype)::text = 'Conventional Unit'::text) OR
			((unittype)::text = 'Base Unit'::text))
);

REVOKE ALL ON __WDB_SCHEMA__.unit FROM public;
GRANT ALL ON __WDB_SCHEMA__.unit TO wdb_admin;

ALTER TABLE ONLY __WDB_SCHEMA__.unit
    ADD CONSTRAINT unit_pkey PRIMARY KEY (unitname);

ALTER TABLE __WDB_SCHEMA__.unit
	ADD FOREIGN KEY (measure)
					REFERENCES __WDB_SCHEMA__.measure
					ON DELETE RESTRICT
					ON UPDATE CASCADE;



CREATE TABLE __WDB_SCHEMA__.parameterfunctiontype (
    parameterfunctiontype 			character varying(80) NOT NULL,
    parameterfunctiondescription 	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.parameterfunctiontype FROM public;
GRANT ALL ON __WDB_SCHEMA__.parameterfunctiontype TO wdb_admin;

ALTER TABLE ONLY __WDB_SCHEMA__.parameterfunctiontype
    ADD CONSTRAINT parameterfunctiontype_pkey PRIMARY KEY (parameterfunctiontype);



CREATE TABLE __WDB_SCHEMA__.parameterquantitytype (
    parameterquantitytype				character varying(80) NOT NULL,
	parameterquantitytypedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.parameterquantitytype FROM public;
GRANT ALL ON __WDB_SCHEMA__.parameterquantitytype TO wdb_admin;

ALTER TABLE ONLY __WDB_SCHEMA__.parameterquantitytype
    ADD CONSTRAINT parameterquantitytype_pkey PRIMARY KEY (parameterquantitytype);



CREATE TABLE __WDB_SCHEMA__.valueparameterusage (
    valueparameterusage				character varying(80) NOT NULL,
	valueparameterusagedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparameterusage TO wdb_admin;




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


CREATE TABLE __WDB_SCHEMA__.valuemeasureparameter (
	valueparameterid 				integer NOT NULL,
	valueparameterusage 			character varying(80) NOT NULL,
	valueparameterunitname 			character varying(80) NOT NULL,
	parameterquantitytype			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuemeasureparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuemeasureparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.valuemeasureparameter TO wdb_write;

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

CREATE TABLE __WDB_SCHEMA__.valuefunctionparameter (
	valueparameterid 				integer NOT NULL,
    parameterfunctiontype			character varying(80) NOT NULL,
	valueparameterusage 			character varying(80) NOT NULL,
	valueparameterunitname 			character varying(80) NOT NULL,
	parameterquantitytype			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuefunctionparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuefunctionparameter TO wdb_admin;

CREATE TABLE __WDB_SCHEMA__.valueparametername (
	valueparameterid 			integer NOT NULL,
	parameternamespaceid 		integer NOT NULL,
	valueparametername 			character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valueparametername FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparametername TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelparameterusage (
    levelparameterusage			character varying(80) NOT NULL,
    levelparameterusagedescription	character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameterusage TO wdb_admin;


CREATE TABLE __WDB_SCHEMA__.levelparameter (
	levelparameterid 			serial NOT NULL,
	levelparametertype 			character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelparameter TO wdb_write;
--GRANT SELECT, INSERT, UPDATE ON __WDB_SCHEMA__.levelparameter_levelparameterid_seq TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.levelmeasureparameter (
	levelparameterid 			integer NOT NULL,
	levelparameterunitname 		character varying(80) NOT NULL,
    levelparameterusage 		character varying(80) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.levelmeasureparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelmeasureparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelmeasureparameter TO wdb_write;


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
