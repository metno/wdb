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


--
-- Primary Key Constraints
--
ALTER TABLE ONLY __WDB_SCHEMA__.currentconfiguration
    ADD CONSTRAINT currentconfiguration_pkey PRIMARY KEY (singleton);


ALTER TABLE ONLY __WDB_SCHEMA__.namespace
    ADD CONSTRAINT namespace_pkey PRIMARY KEY (namespaceid);


ALTER TABLE ONLY __WDB_SCHEMA__.party
    ADD CONSTRAINT party_pkey PRIMARY KEY (partyid);


ALTER TABLE ONLY __WDB_SCHEMA__.partycomment
    ADD CONSTRAINT partycomment_pkey PRIMARY KEY (partyid, partycommentstoretime);


ALTER TABLE ONLY __WDB_SCHEMA__.organization
    ADD CONSTRAINT organization_pkey PRIMARY KEY (partyid);


ALTER TABLE ONLY __WDB_SCHEMA__.organizationtype
    ADD CONSTRAINT organizationtype_pkey PRIMARY KEY (organizationtype);


ALTER TABLE ONLY __WDB_SCHEMA__.organizationalias
    ADD CONSTRAINT organizationalias_pkey PRIMARY KEY (partyid, alternativeorganizationname);


ALTER TABLE ONLY __WDB_SCHEMA__.person
    ADD CONSTRAINT person_pkey PRIMARY KEY (partyid);


ALTER TABLE ONLY __WDB_SCHEMA__.softwareversion
    ADD CONSTRAINT softwareversion_pkey PRIMARY KEY (partyid);


ALTER TABLE ONLY __WDB_SCHEMA__.dataprovider
    ADD CONSTRAINT dataprovider_pkey PRIMARY KEY (dataproviderid);


ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidercomment
    ADD CONSTRAINT dataprovidercomment_pkey PRIMARY KEY (dataproviderid, dataprovidercommentstoretime);


ALTER TABLE ONLY __WDB_SCHEMA__.gribgeneratingprocess
    ADD CONSTRAINT gribgeneratingprocess_pkey PRIMARY KEY (dataproviderid);


ALTER TABLE ONLY __WDB_SCHEMA__.feltgeneratingprocess
    ADD CONSTRAINT feltgeneratingprocess_pkey PRIMARY KEY (dataproviderid);


ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_pkey PRIMARY KEY (dataproviderid, dataprovidernamespaceid);

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_skey UNIQUE (dataprovidername);


ALTER TABLE ONLY __WDB_SCHEMA__.placedefinition
    ADD CONSTRAINT placedefiniton_pkey PRIMARY KEY (placeid);


ALTER TABLE ONLY __WDB_SCHEMA__.placeindeterminatetype
    ADD CONSTRAINT placeindeterminatetype_pkey PRIMARY KEY (placeindeterminatecode);


ALTER TABLE ONLY __WDB_SCHEMA__.placeregulargrid
    ADD CONSTRAINT placeregulargrid_pkey PRIMARY KEY (placeid);


ALTER TABLE ONLY __WDB_SCHEMA__.placename
    ADD CONSTRAINT placename_pkey PRIMARY KEY (placeid, placenamespaceid);

ALTER TABLE ONLY __WDB_SCHEMA__.placename
	ADD CONSTRAINT placename_unique UNIQUE (placename);

ALTER TABLE ONLY __WDB_SCHEMA__.timeindeterminatetype
    ADD CONSTRAINT timeindeterminatetype_pkey PRIMARY KEY (timeindeterminatecode);


ALTER TABLE ONLY __WDB_SCHEMA__.statisticstype
    ADD CONSTRAINT statisticstype_pkey PRIMARY KEY (statisticstype);


ALTER TABLE ONLY __WDB_SCHEMA__.physicalphenomenon
    ADD CONSTRAINT physicalphenomenon_pkey PRIMARY KEY (physicalphenomenon);


ALTER TABLE ONLY __WDB_SCHEMA__.valueparameterusage
    ADD CONSTRAINT valueparameterusage_pkey PRIMARY KEY (valueparameterusage);


ALTER TABLE ONLY __WDB_SCHEMA__.levelparameterusage
    ADD CONSTRAINT levelparameterusage_pkey PRIMARY KEY (levelparameterusage);


ALTER TABLE ONLY __WDB_SCHEMA__.unit
    ADD CONSTRAINT unit_pkey PRIMARY KEY (unitname);


ALTER TABLE ONLY __WDB_SCHEMA__.unitabbreviation
    ADD CONSTRAINT unitabbreviation_pkey PRIMARY KEY (unitname);


ALTER TABLE ONLY __WDB_SCHEMA__.siunitconversion
    ADD CONSTRAINT siunitconversion_pkey PRIMARY KEY (unitname);


ALTER TABLE ONLY __WDB_SCHEMA__.valueparameter
    ADD CONSTRAINT valueparameter_pkey PRIMARY KEY (valueparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.valuephysicalparameter
    ADD CONSTRAINT valuephysicalparameter_pkey PRIMARY KEY (valueparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.valuecodeparameter
    ADD CONSTRAINT valuecodeparameter_pkey PRIMARY KEY (valueparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.valueparametername
    ADD CONSTRAINT valueparametername_pkey PRIMARY KEY (valueparameterid, parameternamespaceid);


ALTER TABLE ONLY __WDB_SCHEMA__.levelparameter
    ADD CONSTRAINT levelparameter_pkey PRIMARY KEY (levelparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.levelphysicalparameter
    ADD CONSTRAINT levelphysicalparameter_pkey PRIMARY KEY (levelparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.levelcodeparameter
    ADD CONSTRAINT levelcodeparameter_pkey PRIMARY KEY (levelparameterid);


ALTER TABLE ONLY __WDB_SCHEMA__.levelparametername
    ADD CONSTRAINT levelparametername_pkey PRIMARY KEY (levelparameterid, parameternamespaceid);


ALTER TABLE ONLY __WDB_SCHEMA__.levelindeterminatetype
    ADD CONSTRAINT levelindeterminatetype_pkey PRIMARY KEY (levelindeterminatecode);


ALTER TABLE ONLY __WDB_SCHEMA__.qualityconfidencecode
    ADD CONSTRAINT qualityconfidencecode_pkey PRIMARY KEY (confidencecode);


ALTER TABLE ONLY __WDB_SCHEMA__.floatvalue
    ADD CONSTRAINT floatvalue_pkey PRIMARY KEY (valueid, valuetype);


ALTER TABLE ONLY __WDB_SCHEMA__.oidvalue
    ADD CONSTRAINT oidvalue_pkey PRIMARY KEY (valueid, valuetype);


ALTER TABLE ONLY __WDB_SCHEMA__.valueadditionallevelinfo
    ADD CONSTRAINT valueadditionallevelinfo_pkey PRIMARY KEY (valueid, valuetype);



--
-- Foreign Key Constraints
--
ALTER TABLE __WDB_SCHEMA__.currentconfiguration
	ADD FOREIGN KEY (softwareversionpartyid)
					REFERENCES __WDB_SCHEMA__.softwareversion
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;


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


ALTER TABLE __WDB_SCHEMA__.partycomment
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.organization
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.organizationtype
	ADD FOREIGN KEY (organizationtype)
					REFERENCES __WDB_SCHEMA__.organizationtype
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.organizationalias
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.organization
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.person
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.softwareversion
	ADD FOREIGN KEY (partyid)
					REFERENCES __WDB_SCHEMA__.party
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gribgeneratingprocess
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.feltgeneratingprocess
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE CASCADE
					ON UPDATE CASCADE;


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


ALTER TABLE __WDB_SCHEMA__.placedefinition
	ADD FOREIGN KEY (placeindeterminatecode)
					REFERENCES __WDB_SCHEMA__.placeindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE RESTRICT;


ALTER TABLE __WDB_SCHEMA__.placeregulargrid
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.placename
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.placename
	ADD FOREIGN KEY (placenamespaceid)
					REFERENCES __WDB_SCHEMA__.namespace
					ON DELETE RESTRICT
					ON UPDATE CASCADE;

ALTER TABLE __WDB_SCHEMA__.unit
	ADD FOREIGN KEY (physicalphenomenon)
					REFERENCES __WDB_SCHEMA__.physicalphenomenon
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.unitabbreviation
	ADD FOREIGN KEY (unitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.siunitconversion
	ADD FOREIGN KEY (unitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valuephysicalparameter
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valuestatisticsparameter
	ADD FOREIGN KEY (valueparameterstatisticstype)
					REFERENCES __WDB_SCHEMA__.statisticstype
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valuephysicalparameter
	ADD FOREIGN KEY (valueparameterunitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valuephysicalparameter
	ADD FOREIGN KEY (valueparameterusage)
					REFERENCES __WDB_SCHEMA__.valueparameterusage
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valuecodeparameter
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valueparametername
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.valueparametername
	ADD FOREIGN KEY (parameternamespaceid)
					REFERENCES __WDB_SCHEMA__.namespace
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelphysicalparameter
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.levelparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelphysicalparameter
	ADD FOREIGN KEY (levelparameterunitname)
					REFERENCES __WDB_SCHEMA__.unit
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelphysicalparameter
	ADD FOREIGN KEY (levelparameterusage)
					REFERENCES __WDB_SCHEMA__.levelparameterusage
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelcodeparameter
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.levelparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelparametername
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.levelparameter
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.levelparametername
	ADD FOREIGN KEY (parameternamespaceid)
					REFERENCES __WDB_SCHEMA__.namespace
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (validtimeindeterminatecode)
					REFERENCES __WDB_SCHEMA__.timeindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.levelparameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (levelindeterminatecode)
					REFERENCES __WDB_SCHEMA__.levelindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalue
	ADD FOREIGN KEY (confidencecode)
					REFERENCES __WDB_SCHEMA__.qualityconfidencecode
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (validtimeindeterminatecode)
					REFERENCES __WDB_SCHEMA__.timeindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.valueparameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.levelparameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (levelindeterminatecode)
					REFERENCES __WDB_SCHEMA__.levelindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.oidvalue
	ADD FOREIGN KEY (confidencecode)
					REFERENCES __WDB_SCHEMA__.qualityconfidencecode
					ON DELETE RESTRICT
					ON UPDATE CASCADE;
