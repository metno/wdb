-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 - 2012 met.no
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


-- A data provider is the closest identifiable entity to the creation
-- of the data. In general, this will tend to be the creator or collector 
-- of the data.
CREATE TABLE __WDB_SCHEMA__.dataprovider (
    dataproviderid 				bigserial NOT NULL,
    dataprovidertype 			character varying(80) NOT NULL,
    spatialdomaindelivery		character varying(80) NOT NULL,
    dataproviderlife            interval NOT NULL,
    dataproviderstoretime 		timestamp with time zone NOT NULL,
    CONSTRAINT dataprovider_dataprovidertype_check 
	CHECK (	((dataprovidertype)::text = 'aeroplane'::text) OR 
			((dataprovidertype)::text = 'computer system'::text) OR
			((dataprovidertype)::text = 'data provider group'::text) OR
			((dataprovidertype)::text = 'named observation site'::text) OR
			((dataprovidertype)::text = 'organization'::text) OR
			((dataprovidertype)::text = 'person'::text) OR
			((dataprovidertype)::text = 'ship'::text) OR 
			((dataprovidertype)::text = 'wci user'::text) ),
    CONSTRAINT dataprovider_spatialdomaindelivery_check 
	CHECK (	((spatialdomaindelivery)::text = 'any'::text) OR
			((spatialdomaindelivery)::text = 'grid'::text) OR 
			((spatialdomaindelivery)::text = 'point'::text) )
);

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovider
    ADD CONSTRAINT dataprovider_pkey PRIMARY KEY (dataproviderid);

REVOKE ALL ON __WDB_SCHEMA__.dataprovider FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovider TO wdb_admin;
-- This sets startvalue to match that of a wci writer:
-- SELECT setval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass, 1000000);



CREATE TABLE __WDB_SCHEMA__.dataprovidername (
	dataproviderid 					bigint NOT NULL,
	dataprovidernamespaceid 		integer NOT NULL,
	dataprovidername 				character varying(255) NOT NULL,
    dataprovidernamevalidfrom 		TIMESTAMP WITH TIME ZONE NOT NULL,
    dataprovidernamevalidto 		TIMESTAMP WITH TIME ZONE NOT NULL,
	dataprovidernameleftset			bigint NOT NULL,
	dataprovidernamerightset		bigint NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_pkey PRIMARY KEY (dataproviderid, dataprovidernamespaceid);

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_skey UNIQUE (dataprovidernamespaceid, dataprovidername);

ALTER TABLE __WDB_SCHEMA__.dataprovidername
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;

ALTER TABLE __WDB_SCHEMA__.dataprovidername
	ADD FOREIGN KEY (dataprovidernamespaceid)
					REFERENCES __WDB_SCHEMA__.namespace
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.dataprovidername FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovidername TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.dataprovidercomment (
    dataproviderid 					bigint NOT NULL,
	dataprovidercomment				character varying(255) NOT NULL,
    dataprovidercommentstoretime	timestamp with time zone NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidercomment
    ADD CONSTRAINT dataprovidercomment_pkey PRIMARY KEY ( dataproviderid, dataprovidercommentstoretime );

ALTER TABLE __WDB_SCHEMA__.dataprovidercomment
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.dataprovidercomment FROM public;
GRANT ALL ON __WDB_SCHEMA__.dataprovidercomment TO wdb_admin;



CREATE TABLE __WDB_SCHEMA__.wciuserdataprovider
(
  	dataproviderid 				bigint NOT NULL,
  	rolname						text NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.wciuserdataprovider
    ADD CONSTRAINT wciuserdataprovider_pkey PRIMARY KEY ( dataproviderid );

ALTER TABLE __WDB_SCHEMA__.wciuserdataprovider
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;

REVOKE ALL ON __WDB_SCHEMA__.wciuserdataprovider FROM public;
GRANT ALL ON __WDB_SCHEMA__.wciuserdataprovider TO wdb_admin;
