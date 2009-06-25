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


-- Measures (physical + angles)
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

ALTER TABLE ONLY __WDB_SCHEMA__.measure
    ADD CONSTRAINT measures_pkey PRIMARY KEY (measure);

REVOKE ALL ON __WDB_SCHEMA__.measure FROM public;
GRANT ALL ON __WDB_SCHEMA__.measure TO wdb_admin;



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

ALTER TABLE ONLY __WDB_SCHEMA__.unit
    ADD CONSTRAINT unit_pkey PRIMARY KEY (unitname);

ALTER TABLE __WDB_SCHEMA__.unit
	ADD FOREIGN KEY (measure)
					REFERENCES __WDB_SCHEMA__.measure
					ON DELETE RESTRICT
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.unit FROM public;
GRANT ALL ON __WDB_SCHEMA__.unit TO wdb_admin;



-- Conversion tables
CREATE TABLE __WDB_SCHEMA__.siunitconversion (
    unitname						character varying(80) NOT NULL,
    siunitconversioncoefficient		float NOT NULL,
    siunitconversionterm			float NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.siunitconversion
    ADD CONSTRAINT siunitconversion_pkey PRIMARY KEY (unitname);

ALTER TABLE __WDB_SCHEMA__.siunitconversion
	ADD FOREIGN KEY (unitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.siunitconversion FROM public;
GRANT ALL ON __WDB_SCHEMA__.siunitconversion TO wdb_admin;



-- Parameter functions
CREATE TABLE __WDB_SCHEMA__.parameterfunctiontype (
    parameterfunctiontype 			character varying(80) NOT NULL,
    parameterfunctiondescription 	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.parameterfunctiontype
    ADD CONSTRAINT parameterfunctiontype_pkey PRIMARY KEY (parameterfunctiontype);

REVOKE ALL ON __WDB_SCHEMA__.parameterfunctiontype FROM public;
GRANT ALL ON __WDB_SCHEMA__.parameterfunctiontype TO wdb_admin;



-- Parameter quantity types
CREATE TABLE __WDB_SCHEMA__.parameterquantitytype (
    parameterquantitytype				character varying(80) NOT NULL,
	parameterquantitytypedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.parameterquantitytype
    ADD CONSTRAINT parameterquantitytype_pkey PRIMARY KEY (parameterquantitytype);

REVOKE ALL ON __WDB_SCHEMA__.parameterquantitytype FROM public;
GRANT ALL ON __WDB_SCHEMA__.parameterquantitytype TO wdb_admin;



-- Value parameter usages
CREATE TABLE __WDB_SCHEMA__.valueparameterusage (
    valueparameterusage				character varying(80) NOT NULL,
	valueparameterusagedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.valueparameterusage
    ADD CONSTRAINT valueparameterusage_pkey PRIMARY KEY (valueparameterusage);

REVOKE ALL ON __WDB_SCHEMA__.valueparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparameterusage TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.valueparameter (
	valueparameterid 			serial NOT NULL,
	valueparametertype 			character varying(80) NOT NULL
    CONSTRAINT value_valueparametertype_check
	CHECK (	((valueparametertype)::text = 'Measure Parameter'::text) OR
			((valueparametertype)::text = 'Function Parameter'::text) OR
			((valueparametertype)::text = 'Code Parameter'::text) OR
			((valueparametertype)::text = 'Dimensionless Parameter'::text))
);

ALTER TABLE ONLY __WDB_SCHEMA__.valueparameter
    ADD CONSTRAINT valueparameter_pkey PRIMARY KEY (valueparameterid);

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

ALTER TABLE ONLY __WDB_SCHEMA__.valuemeasureparameter
    ADD CONSTRAINT valuemeasureparameter_pkey PRIMARY KEY (valueparameterid);

ALTER TABLE __WDB_SCHEMA__.valuemeasureparameter
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;

ALTER TABLE __WDB_SCHEMA__.valuemeasureparameter
	ADD FOREIGN KEY (valueparameterunitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;

ALTER TABLE __WDB_SCHEMA__.valuemeasureparameter
	ADD FOREIGN KEY (valueparameterusage)
					REFERENCES __WDB_SCHEMA__.valueparameterusage
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.valuemeasureparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuemeasureparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.valuemeasureparameter TO wdb_write;



CREATE TABLE __WDB_SCHEMA__.valuefunctionparameter (
	valueparameterid 				integer NOT NULL,
    parameterfunctiontype			character varying(80) NOT NULL,
	valueparameterusage 			character varying(80) NOT NULL,
	valueparameterunitname 			character varying(80) NOT NULL,
	parameterquantitytype			character varying(80) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.valuefunctionparameter
    ADD CONSTRAINT valuefunctionparameter_pkey PRIMARY KEY (valueparameterid);

REVOKE ALL ON __WDB_SCHEMA__.valuefunctionparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuefunctionparameter TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.valuecodeparameter (
	valueparameterid 			integer NOT NULL,
	valuecodeparametername 		character varying(255) NOT NULL,
	codeparameterreference 		character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.valuecodeparameter
    ADD CONSTRAINT valuecodeparameter_pkey PRIMARY KEY (valueparameterid);

REVOKE ALL ON __WDB_SCHEMA__.valuecodeparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuecodeparameter TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.valuedimensionlessparameter (
	valueparameterid 				integer NOT NULL,
	valuedimensionlessparametername character varying(255) NOT NULL,
	valueparameterdescription		character varying(255) NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.valuedimensionlessparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.valuedimensionlessparameter TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.valueparametername (
	valueparameterid 			integer NOT NULL,
	parameternamespaceid 		integer NOT NULL,
	valueparametername 			character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.valueparametername
    ADD CONSTRAINT valueparametername_pkey PRIMARY KEY (valueparameterid, parameternamespaceid);

REVOKE ALL ON __WDB_SCHEMA__.valueparametername FROM public;
GRANT ALL ON __WDB_SCHEMA__.valueparametername TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.levelparameterusage (
    levelparameterusage			character varying(80) NOT NULL,
    levelparameterusagedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelparameterusage
    ADD CONSTRAINT levelparameterusage_pkey PRIMARY KEY (levelparameterusage);

REVOKE ALL ON __WDB_SCHEMA__.levelparameterusage FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameterusage TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.levelparameter (
	levelparameterid 			serial NOT NULL,
	levelparametertype 			character varying(80) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelparameter
    ADD CONSTRAINT levelparameter_pkey PRIMARY KEY (levelparameterid);

REVOKE ALL ON __WDB_SCHEMA__.levelparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelparameter TO wdb_write;
--GRANT SELECT, INSERT, UPDATE ON __WDB_SCHEMA__.levelparameter_levelparameterid_seq TO wdb_write;


CREATE TABLE __WDB_SCHEMA__.levelmeasureparameter (
	levelparameterid 			integer NOT NULL,
	levelparameterunitname 		character varying(80) NOT NULL,
    levelparameterusage 		character varying(80) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelmeasureparameter
    ADD CONSTRAINT levelmeasureparameter_pkey PRIMARY KEY (levelparameterid);

REVOKE ALL ON __WDB_SCHEMA__.levelmeasureparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelmeasureparameter TO wdb_admin;
--GRANT INSERT ON __WDB_SCHEMA__.levelmeasureparameter TO wdb_write;



CREATE TABLE __WDB_SCHEMA__.levelcodeparameter (
	levelparameterid 			integer NOT NULL,
	levelcodeparametername 		character varying(255) NOT NULL,
	codeparameterreference 		character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelcodeparameter
    ADD CONSTRAINT levelcodeparameter_pkey PRIMARY KEY (levelparameterid);

REVOKE ALL ON __WDB_SCHEMA__.levelcodeparameter FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelcodeparameter TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.levelparametername (
	levelparameterid 			integer NOT NULL,
	parameternamespaceid 		integer NOT NULL,
	levelparametername 			character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelparametername
    ADD CONSTRAINT levelparametername_pkey PRIMARY KEY (levelparameterid, parameternamespaceid);

REVOKE ALL ON __WDB_SCHEMA__.levelparametername FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelparametername TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.levelindeterminatetype (
    levelindeterminatecode		integer NOT NULL,
    levelindeterminatetype		character varying(80) NOT NULL,
    levelindeterminatetypedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.levelindeterminatetype
    ADD CONSTRAINT levelindeterminatetype_pkey PRIMARY KEY (levelindeterminatecode);

REVOKE ALL ON __WDB_SCHEMA__.levelindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.levelindeterminatetype TO wdb_admin;
