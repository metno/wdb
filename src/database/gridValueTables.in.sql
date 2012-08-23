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


-- Name: GRIDValue 
CREATE TABLE __WDB_SCHEMA__.gridvalue (
    valueid 					bigserial NOT NULL,
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
    value 						bigint NOT NULL,
    valuestoretime 				timestamp with time zone NOT NULL
);

ALTER TABLE ONLY __WDB_SCHEMA__.gridvalue
    ADD CONSTRAINT gridvalue_pkey PRIMARY KEY (valueid);

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
					
CREATE UNIQUE INDEX XAK1Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
       DataProviderId,
       ReferenceTime,
       PlaceId,
       ValueParameterId,
       LevelParameterId,
       LevelFrom,
       LevelTo,
       LevelIndeterminateCode,
       ValidTimeFrom,
       ValidTimeTo,
       ValidTimeIndeterminateCode,
       DataVersion
);

CREATE INDEX XIE1Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
	DataProviderId
);


CREATE INDEX XIE2Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
	PlaceId
);

CREATE INDEX XIE4Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
	ReferenceTime
);

CREATE INDEX XIE5Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
       ValidTimeFrom,
       ValidTimeTo
);

CREATE INDEX XIE6Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
       LevelParameterId,
       LevelFrom,
       LevelTo
);

CREATE INDEX XIE7Wdb_gridvalue ON __WDB_SCHEMA__.gridvalue
(
       (DataVersion - MaxDataVersion - 1)
);

CREATE INDEX cascadingDeleteOids_gridvalue ON __WDB_SCHEMA__.gridvalue
(
	value
);

REVOKE ALL ON __WDB_SCHEMA__.gridvalue FROM public;
GRANT ALL ON __WDB_SCHEMA__.gridvalue TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.gridvalue TO wdb_clean;
REVOKE ALL ON __WDB_SCHEMA__.gridvalue_valueid_seq FROM public;
GRANT ALL ON __WDB_SCHEMA__.gridvalue_valueid_seq TO wdb_admin;
