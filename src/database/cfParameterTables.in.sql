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

-- Surface
CREATE TABLE __WDB_SCHEMA__.cfsurface (
    cfsurface					character varying(255) NOT NULL,
	cfsurfacecomment			character varying(255)
);

ALTER TABLE ONLY __WDB_SCHEMA__.cfsurface
    ADD CONSTRAINT cfsurface_pkey PRIMARY KEY (cfsurface);



-- Component
CREATE TABLE __WDB_SCHEMA__.cfcomponent (
    cfcomponent					character varying(255) NOT NULL,
	cfcomponentcomment			character varying(255)
);

ALTER TABLE ONLY __WDB_SCHEMA__.cfcomponent
    ADD CONSTRAINT cfcomponent_pkey PRIMARY KEY (cfcomponent);



-- Medium
CREATE TABLE __WDB_SCHEMA__.cfmedium (
    cfmedium					character varying(255) NOT NULL,
	cfmediumcomment				character varying(255)
);

ALTER TABLE ONLY __WDB_SCHEMA__.cfmedium
    ADD CONSTRAINT cfmedium_pkey PRIMARY KEY (cfmedium);



-- Process
CREATE TABLE __WDB_SCHEMA__.cfprocess (
    cfprocess					character varying(255) NOT NULL,
	cfprocesscomment			character varying(255)
);

ALTER TABLE ONLY __WDB_SCHEMA__.cfprocess
    ADD CONSTRAINT cfprocess_pkey PRIMARY KEY (cfprocess);



-- Condition
CREATE TABLE __WDB_SCHEMA__.cfcondition (
    cfcondition					character varying(255) NOT NULL,
	cfconditioncomment			character varying(255)
);	

ALTER TABLE ONLY __WDB_SCHEMA__.cfcondition
    ADD CONSTRAINT cfcondition_pkey PRIMARY KEY (cfcondition);



-- Surface
CREATE TABLE __WDB_SCHEMA__.cfmethods (
    cfmethods				character varying(255) NOT NULL,
	cfmethodscomment		character varying(255),
    cfmethodsname			character varying(255) NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.cfmethods
    ADD CONSTRAINT cfmethods_pkey PRIMARY KEY (cfmethods);
