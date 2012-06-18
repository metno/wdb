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
ALTER TABLE ONLY __WDB_SCHEMA__.floatvaluegroup
    ADD CONSTRAINT floatvaluegroup_pkey PRIMARY KEY (valuegroupid);


ALTER TABLE ONLY __WDB_SCHEMA__.floatvalueitem
    ADD CONSTRAINT floatvalueitem_pkey PRIMARY KEY (valuegroupid, referencetime);


ALTER TABLE ONLY __WDB_SCHEMA__.gridvalue
    ADD CONSTRAINT gridvalue_pkey PRIMARY KEY (valueid, valuetype);


ALTER TABLE ONLY __WDB_SCHEMA__.valueadditionallevelinfo
    ADD CONSTRAINT valueadditionallevelinfo_pkey PRIMARY KEY (valueid, valuetype);



--
-- Foreign Key Constraints
--

ALTER TABLE __WDB_SCHEMA__.parametername
	ADD FOREIGN KEY (parameternamespaceid)
					REFERENCES __WDB_SCHEMA__.namespace
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (validtimeindeterminatecode)
					REFERENCES __WDB_SCHEMA__.timeindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.parameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.parameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvaluegroup
	ADD FOREIGN KEY (levelindeterminatecode)
					REFERENCES __WDB_SCHEMA__.levelindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalueitem
	ADD FOREIGN KEY (confidencecode)
					REFERENCES __WDB_SCHEMA__.qualityconfidencecode
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.floatvalueitem
	ADD FOREIGN KEY (valuegroupid)
					REFERENCES __WDB_SCHEMA__.floatvaluegroup
					ON DELETE CASCADE
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (dataproviderid)
					REFERENCES __WDB_SCHEMA__.dataprovider
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (placeid)
					REFERENCES __WDB_SCHEMA__.placedefinition
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (validtimeindeterminatecode)
					REFERENCES __WDB_SCHEMA__.timeindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (valueparameterid)
					REFERENCES __WDB_SCHEMA__.parameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (levelparameterid)
					REFERENCES __WDB_SCHEMA__.parameter
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (levelindeterminatecode)
					REFERENCES __WDB_SCHEMA__.levelindeterminatetype
					ON DELETE RESTRICT
					ON UPDATE CASCADE;


ALTER TABLE __WDB_SCHEMA__.gridvalue
	ADD FOREIGN KEY (confidencecode)
					REFERENCES __WDB_SCHEMA__.qualityconfidencecode
					ON DELETE RESTRICT
					ON UPDATE CASCADE;
