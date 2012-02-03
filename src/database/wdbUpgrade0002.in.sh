#!/bin/sh
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# 
# wdb - weather and water data storage
#
# Copyright (C) 2007-2010 met.no
#
#  Contact information:
#  Norwegian Meteorological Institute
#  Box 43 Blindern
#  0313 OSLO
#  NORWAY
#  E-mail: wdb@met.no
#
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#
# This is the update file for WDB from schema version 1 to schema version 2
#
# Set Variables
export WDB_NAME=$1
export WDB_USER=$2
export WDB_PORT=$3
export WDB_PATH=$4
export WCI_PATH=$5
export WDB_LOGS=$6

# Run PSQL Query
psql -U $WDB_USER -p $WDB_PORT -d $WDB_NAME -q <<EOF

SET CLIENT_MIN_MESSAGES TO "WARNING";

\set ON_ERROR_STOP

\o $WDB_LOGS/wdb_upgrade_datamodel.log

-- PlaceDefinition Tables
ALTER TABLE __WDB_SCHEMA__.placedefinition
DROP CONSTRAINT placedefinition_placegeometrytype_check;
ALTER TABLE __WDB_SCHEMA__.placedefinition
ADD CONSTRAINT placedefinition_placegeometrytype_check 
CHECK (	((placegeometrytype)::text = 'grid'::text) OR
		((placegeometrytype)::text = 'line'::text) OR
		((placegeometrytype)::text = 'point'::text) OR
		((placegeometrytype)::text = 'polygon'::text) );
-- UserDefinitions
--ALTER ROLE wdb_test INHERIT wdb_read;
--WCI View
CREATE OR REPLACE VIEW __WCI_SCHEMA__.placedefinition AS
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pg.originalsrid,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn,
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
	AND pd.placeid = pn.placeid
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placename pn
WHERE
	pd.placeid = pn.placeid
	AND pd.placegeometrytype <> 'grid' 
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	pg.originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd, 
	__WDB_SCHEMA__.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	lower(st_astext( pd.placegeometry )) as placename,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 	
	__WDB_SCHEMA__.placedefinition pd
WHERE
	pd.placegeometrytype != 'grid';
		
REVOKE ALL ON __WCI_SCHEMA__.placedefinition FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.placedefinition TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placedefinition TO wdb_read, wdb_write;
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placedefinition_mv');


-- WCI Types
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.dwithin(geometry, geometry, float8)
    RETURNS boolean
    AS 'SELECT \$1 && st_expand(\$2,\$3) AND \$2 && st_expand(\$1,\$3) AND st_distance(\$1, \$2) < \$3'
    LANGUAGE 'SQL' IMMUTABLE; 

-- WCI Core
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.expandoverlap_metric(a geometry,
								    b geometry,
								    maxe double precision,
								    maxslice double precision)
RETURNS integer AS
\$BODY$
BEGIN
    FOR i IN 0..maxslice LOOP
        IF st_expand(a,maxe*i/maxslice) && b THEN
            RETURN i;
        END IF;
    END LOOP; 
    RETURN 99999999;
END;
\$BODY$
LANGUAGE 'plpgsql' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__._pgis_fn_nn(geom1 geometry,
						   distguess double precision,
						   numnn integer,
						   maxslices integer,
						   lookupset varchar(150),
						   swhere varchar(5000),
						   sgid2field varchar(100),
						   sgeom2field varchar(100))
RETURNS SETOF __WCI_SCHEMA__.pgis_nn AS
\$BODY$
DECLARE
    strsql text;
    rec __WCI_SCHEMA__.pgis_nn;
    ncollected integer;
    it integer;
--NOTE: it: the iteration we are currently at 
--start at the bounding box of the object (expand 0) and move up until it has collected more objects than we need or it = maxslices whichever event happens first
BEGIN
	IF geom1 IS NULL THEN
		RETURN;
	END IF;	
    ncollected := 0; it := 0;
    WHILE ncollected < numnn AND it <= maxslices LOOP
        strsql := 'SELECT currentit.' || sgid2field || ', st_distance(ref.geom, currentit.' || sgeom2field || ') as dist FROM ' || lookupset || '  as currentit, (SELECT geometry(''' || CAST(geom1 As text) || ''') As geom) As ref WHERE ' || swhere || ' AND st_distance(ref.geom, currentit.' || sgeom2field || ') <= ' || CAST(distguess As varchar(200)) || ' AND st_expand(ref.geom, ' || CAST(distguess*it/maxslices As varchar(100)) ||  ') && currentit.' || sgeom2field || ' AND __WCI_SCHEMA__.expandoverlap_metric(ref.geom, currentit.' || sgeom2field || ', ' || CAST(distguess As varchar(200)) || ', ' || CAST(maxslices As varchar(200)) || ') = ' || CAST(it As varchar(100)) || ' ORDER BY st_distance(ref.geom, currentit.' || sgeom2field || ') LIMIT ' || 
        CAST((numnn - ncollected) As varchar(200));
        -- RAISE NOTICE 'sql: %', strsql;
        FOR rec in EXECUTE (strsql) LOOP
            IF ncollected < numnn THEN
                ncollected := ncollected + 1;
                RETURN NEXT rec;
            ELSE
                EXIT;
            END IF;
        END LOOP;
        it := it + 1;
    END LOOP;
END
\$BODY$
LANGUAGE 'plpgsql' STABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQuery(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQuery'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION 
verifyBlobSize(
	value bytea,
	placename_ text
)
RETURNS void AS
\$BODY$
DECLARE
	blobSize real := length(value) / 4.0;
	specSize int;
BEGIN
	SELECT 
		g.numberx*g.numbery INTO specSize
	FROM 
		__WCI_SCHEMA__.placeregulargrid g, 
		__WCI_SCHEMA__.getSessionData() s
	WHERE
		g.placename = placename_ AND
		g.placenamespaceid = s.placenamespaceid;

	IF blobSize != specSize THEN
		RAISE EXCEPTION 'verifyBlobSize: Wrong size of data field (size=%, should be %)', blobSize, specSize;
	END IF;
END;
\$BODY$
LANGUAGE plpgsql STABLE;


-- WCI Api
CREATE OR REPLACE FUNCTION
wci.setConfiguration( name_				varchar(80),
				  	  code_				varchar(80),
				  	  versionNumber_	integer )
RETURNS VOID AS
\$BODY$
DECLARE
	sid_		int;
BEGIN
	-- Get OwnerID
	SELECT partyid INTO sid_
	FROM __WCI_SCHEMA__.softwareversion
	WHERE softwarename = name_ AND
		  softwareversioncode = code_;
	-- Update or INSERT
	IF NOT FOUND THEN
		sid_ := nextval('__WDB_SCHEMA__.party_partyid_seq'::regclass);
		
		INSERT INTO __WDB_SCHEMA__.party( PartyId, PartyType, PartyValidFrom, PartyValidTo ) 
			VALUES ( sid_, 'software', 'now',  'infinity' );
	
		INSERT INTO __WDB_SCHEMA__.softwareversion VALUES
		( sid_, name_, code_ );
		
		INSERT INTO __WDB_SCHEMA__.configuration
		VALUES ( sid_,
				 versionNumber_,
				 'now' );
	ELSE
		PERFORM *
		FROM __WDB_SCHEMA__.configuration
		WHERE softwareversionpartyid = sid_;
		IF NOT FOUND THEN
			INSERT INTO __WDB_SCHEMA__.configuration
			VALUES ( sid_,
					 versionNumber_,
					 'now' );
		ELSE
			UPDATE  __WDB_SCHEMA__.configuration
			SET packageversion = versionNumber_;
			UPDATE  __WDB_SCHEMA__.configuration
			SET installtime = 'now';
		END IF;
	END IF;
END;
\$BODY$
LANGUAGE 'plpgsql' VOLATILE;

DROP TYPE IF EXISTS wci.browsedataversion CASCADE;
CREATE TYPE wci.browsedataversion AS
(
	dataversion integer,
	numberoftuples integer
);

CREATE OR REPLACE FUNCTION 
wci.browse( dataprovider 		text[],
		    location 			text,
		    referencetime 		text,
		    validtime 			text,
		    parameter 			text[],
		    level 				text,
		    dataversion 		integer[],
		    returntype 			wci.browsedataversion
)
RETURNS SETOF wci.browsedataversion
AS 
\$BODY$
DECLARE
	query 	text;
	ret 	wci.browsedataversion;
BEGIN
	query := __WCI_SCHEMA__.getBrowseDataVersionQuery(dataprovider, location, referencetime, validtime, parameter, level, dataversion);
	RAISE DEBUG '%', query;

	FOR ret IN EXECUTE query LOOP
		RETURN NEXT ret;
	END LOOP;
END;
\$BODY$
LANGUAGE 'plpgsql' STABLE;

CREATE OR REPLACE FUNCTION
wci.browse(returntype wci.browsedataversion)
RETURNS SETOF wci.browsedataversion AS
\$BODY$
	SELECT *
	FROM wci.browse( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.browsedataversion);
\$BODY$
LANGUAGE sql STABLE;

--
-- Get a list of all sub-dataproviders for the given data provider name.
--
CREATE OR REPLACE FUNCTION
wci.getdataprovidersingroup(dataprovidername text)
RETURNS SETOF text AS
\$BODY$
	SELECT 
		p.dataprovidername 
	FROM 
		wdb_int.dataprovidername p, 
		wdb_int.dataprovidername pb, 
		wci_int.getsessiondata() s 
	WHERE 
		p.dataprovidernamespaceid = s.dataprovidernamespaceid AND 
		pb.dataprovidernamespaceid = s.dataprovidernamespaceid AND 
		p.dataprovidernameleftset >= pb.dataprovidernameleftset AND 
		p.dataprovidernamerightset <= pb.dataprovidernamerightset AND
		pb.dataprovidername=\$1
	ORDER BY 
		p.dataproviderid;
\$BODY$
SECURITY DEFINER
LANGUAGE sql STRICT;

--
-- add new data provider name in namespace
-- 
CREATE OR REPLACE FUNCTION
wci.setDataProviderName
(
	fromNameSpaceId_	int,
	fromName_			text,
	dataProviderName_ 	text
)
RETURNS void AS
\$BODY$
DECLARE	
	namespace_ 		int;
	dataProviderId_ int;
	gpid_			bigint;
BEGIN
	-- Get namespace
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Get dataproviderid
	SELECT dataproviderid INTO dataProviderId_
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = lower(fromName_) AND
		  dataprovidernamespaceid = fromNameSpaceId_;
	-- Failed to find dpid
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify the dataprovider in the specified namespace';
	END IF;
	-- Delete old name if any exist
	DELETE FROM __WDB_SCHEMA__.dataprovidername
	WHERE dataprovidernamespaceid = namespace_ AND
		  dataproviderid = dataProviderId_;
	-- Get Group ID
	SELECT max(dataprovidernamerightset) INTO gpid_ 
	FROM   __WCI_SCHEMA__.dataprovider
	WHERE  dataprovidernamespaceid = namespace_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.dataprovidername
	VALUES ( dataProviderId_,
			 namespace_,
			 lower(dataProviderName_),
			 'today'::TIMESTAMP WITH TIME ZONE, 
			 'infinity'::TIMESTAMP WITH TIME ZONE, 
			 gpid_ + 1,
			 gpid_ + 2 );
END;
\$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;

--
-- add New Place Definition
-- 
CREATE OR REPLACE FUNCTION
wci.addPlacePolygon(
	placeName_ 		text,
	placeGeometry_ 	geometry
)
RETURNS bigint AS
\$BODY$
DECLARE
	placeId_ 	bigint;
	namespace_	int;
	newname_	text;
	indCode_ 	int;
BEGIN
	-- Get namespace
	SELECT placenamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Indeterminate Code 1 = Exact
	indCode_ := 1;	
	-- Check that geometry is POINT
	IF GeometryType( placeGeometry_ ) <> 'POLYGON' THEN
		RAISE EXCEPTION 'Place geometry passed to function is not a WKB polygon';
	END IF;	
	-- Get placedef
	SELECT placeid INTO placeId_ 
	FROM  __WCI_SCHEMA__.placedefinition
	WHERE st_equals( placegeometry, placeGeometry_) AND
		  placenamespaceid = 0;
	-- Add dataprovider
	IF NOT FOUND THEN
		placeId_ := nextval('__WDB_SCHEMA__.placedefinition_placeid_seq');
		INSERT INTO __WDB_SCHEMA__.placedefinition VALUES
		( placeId_, indCode_, 'polygon', 'now', placeGeometry_ );
		IF namespace_ <> 0 THEN
			INSERT INTO __WDB_SCHEMA__.placename VALUES
			( placeId_, namespace_, lower(placeName_), 'today', 'infinity' );
		END IF;
	ELSE
		IF namespace_ <> 0 THEN
			PERFORM * 
			FROM  __WDB_SCHEMA__.placename
			WHERE placenamespaceid = namespace_;
			IF NOT FOUND THEN
				INSERT INTO __WDB_SCHEMA__.placename VALUES
				( placeId_, namespace_, lower(placeName_), 'today', 'infinity' );
			ELSE
				UPDATE __WDB_SCHEMA__.placename 
				SET placename = lower(placeName_)
				WHERE placeid = placeId_;
			END IF;
		END IF;
	END IF;
	RETURN placeId_;
END;
\$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;

-- Write GRID Value
-- Limited Specification
CREATE OR REPLACE FUNCTION 
wci.write(
	data 			bytea,
	placename_ 		text,
	referencetime_ 	timestamp with time zone,
	validFrom_ 		timestamp with time zone,
	validTo_ 		timestamp with time zone,
	valueparameter_ text,
	levelparameter_ text,
	levelFrom_ 		real,
	levelTo_ 		real
)
RETURNS void AS
\$BODY$
DECLARE
	myDataProviderId_ bigint := __WCI_SCHEMA__.wcidataproviderid();
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueparameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelparameter_ );
	currentVersion_ integer;
	placeid_ bigint := __WCI_SCHEMA__.getplaceid( placename_ );
	valueParameterId_ integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	levelParameterId_ integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	dataId bigint;
BEGIN
	
	PERFORM verifyBlobSize(data, placename_);

	-- Determine dataversion
	SELECT 
		max(dataversion) INTO currentVersion_ 
	FROM 
		__WCI_SCHEMA__.gridvalue v
	WHERE
		myDataProviderId_ = dataproviderid AND
		v.referencetime = referencetime_ AND
		v.placeid = placeid_ AND
		v.valueparameterid = valueParameterId_ AND
		v.levelparameterid = levelParameterId_ AND
		v.levelFrom = levelFrom_ AND
		v.levelTo = levelTo_ AND
		v.validtimeFrom = validFrom_ AND
		v.validtimeTo = validTo_;
	RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
	IF currentVersion_ IS NULL THEN
		currentVersion_ := 0;
	ELSE
		currentVersion_ := currentVersion_ + 1;
	END IF;
	
	dataId := __WCI_SCHEMA__.reserve_file_id();

	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		dataId,
		mydataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0, -- Exact
		valueparameterid_,
		NULL, 
		NULL,
		levelparameterid_,
		NULL, 
		NULL, 
		levelfrom_,
		levelto_,
		0, -- Exact
		currentversion_,
		NULL,
		0, -- Default
		NULL,
		NULL,
		NULL
	);
	
	PERFORM __WCI_SCHEMA__.write_file(dataId, data);
END 
\$BODY$
LANGUAGE 'plpgsql';



-- Write GRID Value
-- All parameter specified
CREATE OR REPLACE FUNCTION 
wci.write(
	data bytea,
	dataproviderName_ text,
	placeName_ text,
	referencetime_ timestamp with time zone,
	validfrom_ timestamp with time zone,
	validto_ timestamp with time zone,
	valueParameterName_ text,
	levelParameterName_ text,
	levelfrom_ real,
	levelto_ real,
	dataVersion_ integer,
	setConfidenceCode_ integer
)
RETURNS void AS
\$BODY$
DECLARE
	dataProviderId_ 		  bigint := __WCI_SCHEMA__.idfromdataprovider( dataProviderName_ );
	placeid_ 				  bigint := __WCI_SCHEMA__.getplaceid( placeName_ );
	normalizedValueParameter_ text := __WCI_SCHEMA__.normalizeParameter( valueParameterName_ );
	valueParameterId_ 	   	  integer := __WCI_SCHEMA__.getvalueparameterid( normalizedValueParameter_ );
	normalizedLevelParameter_ text := __WCI_SCHEMA__.normalizeLevelParameter( levelParameterName_ );
	levelParameterId_      	  integer := __WCI_SCHEMA__.getlevelparameterid( normalizedLevelParameter_ );
	currentVersion_ 		  integer := dataVersion_;
	confidenceCode_			  integer := setConfidenceCode_;
	
	dataId bigint;
BEGIN
	PERFORM verifyBlobSize(data, placename_);
	
	-- Determine dataversion
	IF (currentVersion_ IS NULL OR currentVersion_ < 0) THEN
		SELECT 
			max(dataversion) INTO currentVersion_ 
		FROM 
			__WCI_SCHEMA__.gridvalue v
		WHERE
			v.dataproviderid = dataProviderId_ AND
			v.referencetime = referencetime_ AND
			v.placeid = placeid_ AND
			v.valueparameterid = valueParameterId_ AND
			v.levelparameterid = levelParameterId_ AND
			v.levelFrom = levelFrom_ AND
			v.levelTo = levelTo_ AND
			v.validtimeFrom = validFrom_ AND
			v.validtimeTo = validTo_;
		RAISE DEBUG 'WCI.WRITE.CurrentVersion: %', currentVersion_;  
		IF currentVersion_ IS NULL THEN
			currentVersion_ := 0;
		ELSE
			currentVersion_ := currentVersion_ + 1;
		END IF;
	END IF;
	IF confidenceCode_ IS NULL THEN
		confidenceCode_ := 0;
	END IF;

	dataId := __WCI_SCHEMA__.reserve_file_id();
	
	INSERT INTO __WCI_SCHEMA__.gridvalue 
	(
		value,
		dataproviderid,
		dataprovidername,
		placeid, 
		placegeometry,
		placeindeterminatecode,
		referencetime, 
		validtimefrom, 
		validtimeto,
		validtimeindeterminatecode,
		valueparameterid,
		valueparametername, 
		valueunitname,
		levelparameterid,
		levelparametername,
		levelunitname,
		levelFrom, 
		levelTo,
		levelindeterminatecode,
		dataversion,
		maxdataversion,
		confidencecode, 
		valuestoretime,
		valueid,
		valuetype
	) 
	VALUES
	(
		dataId,
		dataproviderid_,
		NULL,
		placeid_,
		NULL,
		NULL,
		referencetime_,
		validfrom_,
		validto_,
		0,   -- Exact
		valueparameterId_,
		NULL, NULL,
		levelparameterId_,
		NULL, NULL, 
		levelfrom_,
		levelto_,
		0, --Exact
		currentVersion_,
		NULL,
		confidenceCode_,
		NULL,
		NULL,
		NULL
	);

	PERFORM __WCI_SCHEMA__.write_file(dataId, data);
END 
\$BODY$
LANGUAGE 'plpgsql';

\i $WCI_PATH/api/wciAggregate.sql
\i $WCI_PATH/api/wciRemove.sql

-- Clean out a redundant function (removed in WDB 1.0.2)
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getPlaceQuery( spec TEXT, returnType INTEGER );

SELECT wci.setConfiguration( 'WDB', '__WDB_VERSION__', 2 );

EOF
