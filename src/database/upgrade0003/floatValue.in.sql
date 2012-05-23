-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2012 met.no
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
-- Name: FloatValue Group 
--
CREATE TABLE __WDB_SCHEMA__.floatvaluegroup (
    valuegroupid				serial NOT NULL,
    dataproviderid 				bigint NOT NULL,
    placeid						bigint NOT NULL,
    validtimefrom				interval NOT NULL,
    validtimeto					interval NOT NULL,
    validtimeindeterminatecode	integer NOT NULL,
    valueparameterid			integer NOT NULL,
    levelparameterid			integer NOT NULL,
    levelfrom					real NOT NULL,
    levelto						real NOT NULL,
    levelindeterminatecode		integer NOT NULL,
    dataversion					integer NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.floatvaluegroup FROM public;
GRANT ALL ON __WDB_SCHEMA__.floatvaluegroup TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.floatvaluegroup TO wdb_clean;


--
-- Name: FloatValue Detail
--
CREATE TABLE __WDB_SCHEMA__.floatvalueitem (
    valuegroupid				integer NOT NULL,
    referencetime				timestamp with time zone NOT NULL,
    maxdataversion				integer NOT NULL,
    confidencecode				integer NOT NULL,
    value						real NOT NULL,
    valuestoretime				timestamp with time zone NOT NULL
);

REVOKE ALL ON __WDB_SCHEMA__.floatvalueitem FROM public;
GRANT ALL ON __WDB_SCHEMA__.floatvalueitem TO wdb_admin;
GRANT SELECT, DELETE ON __WDB_SCHEMA__.floatvalueitem TO wdb_clean;


DROP TABLE __WDB_SCHEMA__.floatvalue CASCADE;

--
-- Constraints
--
ALTER TABLE ONLY __WDB_SCHEMA__.floatvaluegroup
    ADD CONSTRAINT floatvaluegroup_pkey PRIMARY KEY (valuegroupid);

ALTER TABLE ONLY __WDB_SCHEMA__.floatvalueitem
    ADD CONSTRAINT floatvalueitem_pkey PRIMARY KEY (valuegroupid, referencetime);
    
--
-- Foreign Key Constraints
--				
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


CREATE INDEX i_FloatValueGroup ON wdb_int.FloatValueGroup
(
	valuegroupid,
	dataproviderid,
	placeid,
	valueparameterid
);

CREATE INDEX i_FloatValueItem ON wdb_int.FloatValueItem
(
	valuegroupid,
	referencetime
);


CREATE UNIQUE INDEX XAK1Wdb_FloatValueGroup ON wdb_int.FloatValueGroup
(
       DataProviderId,
       DataVersion,
       PlaceId,
       ValueParameterId,
       LevelParameterId,
       LevelFrom,
       LevelTo,
       LevelIndeterminateCode,
       ValidTimeFrom,
       ValidTimeTo,
       ValidTimeIndeterminateCode
);

--CREATE INDEX i_FloatValue_TABLE_EXTENSION ON wdb_partition.FloatValue_TABLE_EXTENSION
--(
--        dataproviderid,
--        referencetime,
--        placeid,
--        valueparameterid
--);


DROP VIEW IF EXISTS __WCI_SCHEMA__.floatvalue;
CREATE VIEW __WCI_SCHEMA__.floatvalue AS
SELECT
	vli.value,
	dp.dataproviderid,
	dp.dataprovidername,
	dp.dataprovidernameleftset,
	dp.dataprovidernamerightset,
	pl.placename,
	pl.placeid,
	pl.placegeometry,
	pl.placeindeterminatecode,
	pl.originalsrid,
	vli.referencetime,
	(vli.referencetime + vlg.validtimefrom) AS validtimefrom,
	(vli.referencetime + vlg.validtimeto) AS validtimeto,
	vlg.validtimeindeterminatecode,
	vlg.valueparameterid,
	vp.parametername AS valueparametername, 
	vp.unitname AS valueunitname,
	vlg.levelparameterid,
	lp.parametername AS levelparametername,
	lp.unitname AS levelunitname,
	vlg.levelFrom, 
	vlg.levelTo,
	vlg.levelindeterminatecode,
	vlg.dataversion,
	vli.maxdataversion,
	vli.confidencecode,
	vli.valuestoretime,
	0::bigint AS valueid,
	1 AS valuetype
FROM 	
	__WDB_SCHEMA__.floatvalueitem vli,
	__WDB_SCHEMA__.floatvaluegroup vlg,
	__WCI_SCHEMA__.dataprovider_mv dp,
	__WCI_SCHEMA__.placedefinition_mv pl,
	__WCI_SCHEMA__.parameter_mv vp,
	__WCI_SCHEMA__.parameter_mv lp,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	vli.valuegroupid = vlg.valuegroupid
	AND dp.dataprovidernamespaceid = s.dataprovidernamespaceid
	AND pl.placenamespaceid = s.placenamespaceid
	AND vp.parameternamespaceid = s.parameternamespaceid
	AND lp.parameternamespaceid = s.parameternamespaceid
	AND vlg.dataproviderid = dp.dataproviderid 
	AND vlg.placeid = pl.placeid
	AND vlg.valueparameterid = vp.parameterid
	AND vlg.levelparameterid = lp.parameterid ;


REVOKE ALL ON __WCI_SCHEMA__.floatValue FROM public;
GRANT ALL ON __WCI_SCHEMA__.floatValue TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.floatValue TO wdb_read;
GRANT SELECT, INSERT ON __WCI_SCHEMA__.floatValue TO wdb_write;


CREATE OR REPLACE RULE 
wci_internal_floatvalue_insert
AS ON INSERT TO __WCI_SCHEMA__.floatvalue
DO INSTEAD 
SELECT
__WCI_SCHEMA__.write(
	NEW.dataproviderid, 
	NEW.placeid, 
	NEW.referencetime, 
	NEW.validtimefrom, 
	NEW.validtimeto,
	NEW.validtimeindeterminatecode,
	NEW.valueparameterid, 
	NEW.levelparameterid, 
	NEW.levelfrom, 
	NEW.levelto,
	NEW.levelindeterminatecode,
	NEW.dataversion,
	NEW.confidencecode,
	NEW.value );



--
-- __WCI_SCHEMA__.write
-- This is the main work function of the wci.write call (for FLOAT)
-- 
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.write(
	dataproviderid_ bigint, 
	placeid_ 		bigint,
	referencetime_ 	timestamp with time zone, 
	validfrom_ 		timestamp with time zone,
	validto_ 		timestamp with time zone,
	validic_ 		integer,
	valueparameter_ integer,
	levelparameter_ integer,
	levelfrom_ 		real,
	levelto_ 		real,
	levelic_ 		integer,
	dataversion_ 	integer,
	confidencecode_ integer,
	value_ 			float
)
RETURNS void AS
$BODY$
DECLARE
	groupid_ 	bigint;
	max_		int;
BEGIN
	-- Todo: Set Time by timeIndeterminateCode
	-- Todo: Set Levels by levelIndeterminateCode
	-- Insert value group row
	BEGIN
		INSERT INTO __WDB_SCHEMA__.floatvaluegroup (
    		dataproviderid,
    		placeid,
    		validtimefrom,
    		validtimeto,
    		validtimeindeterminatecode,
			valueparameterid,
			levelparameterid,
			levelfrom, 
			levelto, 
			levelindeterminatecode, 
			dataversion 
		)
		VALUES (
			dataproviderid_, 
			placeid_, 
			(validfrom_ - referencetime_),
			(validto_ - referencetime_),
			validic_,
	 		valueparameter_, 
			levelparameter_,
			levelfrom_,
			levelto_,
			levelic_,
			dataversion_
		)
		RETURNING valuegroupid INTO groupid_;
	EXCEPTION
		WHEN unique_violation THEN
			-- Do not abort on unique violations
			SELECT valuegroupid INTO groupid_
			FROM __WCI_SCHEMA__.floatvaluegroup
			WHERE dataproviderid = dataproviderid_
			AND placeid = placeid_
			AND validtimefrom = (validfrom_ - referencetime_)
			AND validtimeto = (validto_ - referencetime_)
			AND validtimeindeterminatecode = validic_
	 		AND valueparameterid = valueparameter_ 
			AND levelparameterid = levelparameter_
			AND levelfrom = levelfrom_
			AND levelto = levelto_
			AND levelindeterminatecode = levelic_
			AND dataversion = dataversion_;
	END;
	-- Insert value item row
	INSERT INTO __WDB_SCHEMA__.floatvalueitem (
    	valuegroupid,
    	referencetime,
    	maxdataversion,
    	confidencecode,
    	value,
    	valuestoretime
	)
	VALUES (
		groupid_, 
		referencetime_,
		dataversion_,
		confidencecode_,
		value_,
		'now'
	);
	-- Get Max Data Version
	IF (dataversion_ > 0) THEN
		SELECT max(dataversion) INTO max_
		FROM wci_int.floatvalue
		WHERE dataproviderid = dataproviderid_
		AND placeid = placeid_
		AND referencetime = referencetime_	
		AND validtimefrom = validfrom_
		AND validtimeto = validto_
		AND validtimeindeterminatecode = validic_
		AND valueparameterid = valueparameter_ 
		AND levelparameterid = levelparameter_
		AND levelfrom = levelfrom_
		AND levelto = levelto_
		AND levelindeterminatecode = levelic_;
		-- Set Max Data Version
		UPDATE 	__WDB_SCHEMA__.floatvalueitem 
		SET 	maxdataversion = max_
		WHERE   referencetime = referencetime_	
		AND		valuegroupid IN ( SELECT valuegroupid
								  FROM	wdb_int.floatvaluegroup vg		
								  WHERE	vg.dataproviderid = dataproviderid_
								  AND 	vg.placeid = placeid_
								  AND 	vg.validtimefrom = (validfrom_ - referencetime_)
								  AND 	vg.validtimeto = (validto_ - referencetime_)
								  AND 	vg.validtimeindeterminatecode = validic_
								  AND 	vg.valueparameterid = valueparameter_ 
								  AND 	vg.levelparameterid = levelparameter_
								  AND 	vg.levelfrom = levelfrom_
								  AND 	vg.levelto = levelto_
								  AND 	vg.levelindeterminatecode = levelic_ );
	END IF;	
END;
$BODY$
SECURITY DEFINER
LANGUAGE 'plpgsql';



CREATE OR REPLACE FUNCTION 
test.cleanTestData( )
RETURNS void AS
$BODY$
	DELETE FROM __WDB_SCHEMA__.gridvalue 
	WHERE dataproviderid IN ( SELECT b.dataproviderid
					   		  FROM   __WCI_SCHEMA__.dataprovider_mv a,
					   		         __WCI_SCHEMA__.dataprovider_mv b
					   		  WHERE	 a.dataprovidername = 'test group'
					   		    AND	 a.dataprovidernameleftset <= b.dataprovidernameleftset
					   		    AND  a.dataprovidernamerightset >= b.dataprovidernamerightset);    
	DELETE FROM __WDB_SCHEMA__.floatvaluegroup 
	WHERE dataproviderid IN ( SELECT b.dataproviderid
					   		  FROM   __WCI_SCHEMA__.dataprovider_mv a,
					   		         __WCI_SCHEMA__.dataprovider_mv b
					   		  WHERE	 a.dataprovidername = 'test group'
					   		    AND	 a.dataprovidernameleftset <= b.dataprovidernameleftset
					   		    AND  a.dataprovidernamerightset >= b.dataprovidernamerightset);    
$BODY$
SECURITY DEFINER
LANGUAGE 'sql' VOLATILE;


