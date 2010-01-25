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


-- A party represents an actor wrt to the data in the database; either
-- a person, an organization of a piece of software
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

ALTER TABLE ONLY __WDB_SCHEMA__.party
    ADD CONSTRAINT party_pkey PRIMARY KEY (partyid);

REVOKE ALL ON __WDB_SCHEMA__.party FROM public;
GRANT ALL ON __WDB_SCHEMA__.party TO wdb_admin;



-- Comment box for partyid
CREATE TABLE __WDB_SCHEMA__.partycomment (
	partyid						integer NOT NULL,
	partycomment				character varying(255) NOT NULL,
    partycommentstoretime		timestamp with time zone NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.partycomment
    ADD CONSTRAINT partycomment_pkey PRIMARY KEY (partyid, partycommentstoretime);

ALTER TABLE __WDB_SCHEMA__.partycomment
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.partycomment FROM public;
GRANT ALL ON __WDB_SCHEMA__.partycomment TO wdb_admin;



-- Organization types
CREATE TABLE __WDB_SCHEMA__.organizationtype (
    organizationtype 			character varying(80) NOT NULL,
	organizationtypedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.organizationtype
    ADD CONSTRAINT organizationtype_pkey PRIMARY KEY (organizationtype);

REVOKE ALL ON __WDB_SCHEMA__.organizationtype FROM public;
GRANT ALL ON __WDB_SCHEMA__.organizationtype TO wdb_admin;



-- Organizations
CREATE TABLE __WDB_SCHEMA__.organization (
    partyid 					integer NOT NULL,
    organizationname 			character varying(80) NOT NULL,
    organizationtype 			character varying(80) NOT NULL,
    organizationdescription 	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.organization
    ADD CONSTRAINT organization_pkey PRIMARY KEY (partyid);

ALTER TABLE __WDB_SCHEMA__.organization
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;

ALTER TABLE __WDB_SCHEMA__.organizationtype
	ADD FOREIGN KEY (organizationtype)
					REFERENCES __WDB_SCHEMA__.organizationtype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.organization FROM public;
GRANT ALL ON __WDB_SCHEMA__.organization TO wdb_admin;



-- Alternative names for organizations
CREATE TABLE __WDB_SCHEMA__.organizationalias (
    partyid 					integer NOT NULL,
    alternativeorganizationname	character varying(80) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.organizationalias
    ADD CONSTRAINT organizationalias_pkey PRIMARY KEY (partyid, alternativeorganizationname);

ALTER TABLE __WDB_SCHEMA__.organizationalias
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.organization
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.organizationalias FROM public;
GRANT ALL ON __WDB_SCHEMA__.organizationalias TO wdb_admin;



-- This is a standard person schema
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

ALTER TABLE ONLY __WDB_SCHEMA__.person
    ADD CONSTRAINT person_pkey PRIMARY KEY (partyid);

ALTER TABLE __WDB_SCHEMA__.person
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.person FROM public;
GRANT ALL ON __WDB_SCHEMA__.person TO wdb_admin;



-- Software versions
CREATE TABLE __WDB_SCHEMA__.softwareversion
(
	partyid						integer NOT NULL,
	softwarename 				character varying(80) NOT NULL,
	softwareversioncode			character varying(80) NOT NULL,
	softwaredescription 		character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.softwareversion
    ADD CONSTRAINT softwareversion_pkey PRIMARY KEY (partyid);

ALTER TABLE __WDB_SCHEMA__.softwareversion
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.softwareversion FROM public;
GRANT ALL ON __WDB_SCHEMA__.softwareversion TO wdb_admin;


-- currentconfiguration stores the WDB version information in
-- the database
CREATE TABLE __WDB_SCHEMA__.configuration (
    softwareversionpartyid		integer NOT NULL,
    packageversion				integer NOT NULL,
    installtime					timestamp with time zone NOT NULL
);

ALTER TABLE __WDB_SCHEMA__.configuration
	ADD FOREIGN KEY (softwareversionpartyid)
					REFERENCES __WDB_SCHEMA__.softwareversion
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

REVOKE ALL ON __WDB_SCHEMA__.configuration FROM public;
GRANT ALL ON __WDB_SCHEMA__.configuration TO wdb_admin;


-- Namespace descriptors
CREATE TABLE __WDB_SCHEMA__.namespace (
    namespaceid					integer NOT NULL,
    namespacename				character varying(80) NOT NULL,
    namespacedescription		character varying(255) NOT NULL,
    namespacefieldofapplication character varying(255) NOT NULL,
    namespaceownerid			integer NOT NULL,
    namespacecontactid			integer NOT NULL,
    namespacevalidfrom 			date NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.namespace
    ADD CONSTRAINT namespace_pkey PRIMARY KEY (namespaceid);

ALTER TABLE __WDB_SCHEMA__.namespace
	ADD FOREIGN KEY (namespaceownerid)
					REFERENCES __WDB_SCHEMA__.organization
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

ALTER TABLE __WDB_SCHEMA__.namespace
	ADD FOREIGN KEY (namespacecontactid)
					REFERENCES __WDB_SCHEMA__.person
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;

REVOKE ALL ON __WDB_SCHEMA__.namespace FROM public;
GRANT ALL ON __WDB_SCHEMA__.namespace TO wdb_admin;




-- Indeterminate type for time
CREATE TABLE __WDB_SCHEMA__.timeindeterminatetype (
    timeindeterminatecode			integer NOT NULL,
    timeindeterminatetype			character varying(80) NOT NULL,
    timeindeterminatedescription	character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.timeindeterminatetype
    ADD CONSTRAINT timeindeterminatetype_pkey PRIMARY KEY (timeindeterminatecode);

REVOKE ALL ON __WDB_SCHEMA__.timeindeterminatetype FROM public;
GRANT ALL ON __WDB_SCHEMA__.timeindeterminatetype TO wdb_admin;
