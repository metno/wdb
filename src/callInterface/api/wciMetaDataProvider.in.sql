-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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

--
-- add New Data Provider
-- 
CREATE OR REPLACE FUNCTION
wci.addDataProvider
(
	dataProviderName_		text,
	dataProviderType_ 		text,
	domainDelivery_			text,
	lifeCycle_				interval,
	dataProviderComment_	text
)
RETURNS bigint AS
$BODY$
DECLARE
	dpid_		bigint;
	gpid_		bigint;
	namespace_	int;
BEGIN
	-- Get namespace
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Check 
	SELECT dataproviderid INTO dpid_ 
	FROM __WCI_SCHEMA__.dataprovider_v
	WHERE dataprovidername = lower(dataProviderName_) AND
		  dataprovidernamespaceid = namespace_;
	-- Add dataprovider
	IF NOT FOUND THEN
		dpid_ := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);
		
		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		( dpid_, lower(dataProviderType_), lower(domainDelivery_), lifeCycle_, 'now' );
	
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		( dpid_, dataProviderComment_, 'now' );
	
		-- TODO: Need to verify that this is in fact secure (serial)
		SELECT max(dataprovidernamerightset) INTO gpid_ 
		FROM   __WCI_SCHEMA__.dataprovider_v;

		IF ( gpid_ IS NULL ) THEN
			gpid_ := 0;
		END IF;	
			  
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
			( dpid_, namespace_, lower(btrim(dataProviderName_)), 
			  'today'::TIMESTAMP WITH TIME ZONE, 
			  'infinity'::TIMESTAMP WITH TIME ZONE, 
			  gpid_ + 1, gpid_ + 2 );
	ELSE
		UPDATE __WDB_SCHEMA__.dataprovider SET
				dataprovidertype = lower(dataProviderType_),
				spatialdomaindelivery = lower(domainDelivery_),
				dataproviderlife = lifeCycle_,
				dataproviderstoretime = 'now'
		WHERE dataproviderid = dpid_;
	
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		( dpid_, dataProviderComment_, 'now' );

		-- Data Provider Name is unchanged
		
	END IF;
	-- Return dataproviderid
	RETURN dpid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



--
-- add New Data Provider
-- 
CREATE OR REPLACE FUNCTION
wci.addDataProviderToGroup
(
	dataProviderName_		text,
	dataProviderGroup_ 		text
)
RETURNS bigint AS
$BODY$
DECLARE
	dpid_		bigint;
	gpid_		bigint;
	gplft_		bigint;
	gprgt_		bigint;
	namespace_	int;
BEGIN
	-- Get namespace
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	-- Get DataProvider
	SELECT dataproviderid INTO dpid_
	FROM __WCI_SCHEMA__.dataprovider_v
	WHERE dataprovidername = lower(dataProviderName_) AND
		  dataprovidernamespaceid = namespace_;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify the dataprovider name in WDB';
	END IF;
	-- Get Group
	SELECT dataproviderid, dataprovidernameleftset, dataprovidernamerightset INTO gpid_, gplft_, gprgt_ 
	FROM __WCI_SCHEMA__.dataprovider_v
	WHERE dataprovidername = lower(dataProviderGroup_) AND
		  dataprovidernamespaceid = namespace_;
	IF NOT FOUND THEN
		RAISE EXCEPTION 'Could not identify the dataprovider group in WDB';
	END IF;
	-- Update Sets to the right
	UPDATE	__WDB_SCHEMA__.dataprovidername 
	SET 	dataprovidernameleftset = dataprovidernameleftset + 2,
			dataprovidernamerightset = dataprovidernamerightset + 2
	WHERE	dataprovidernamerightset > gprgt_;
	-- Update Group
	UPDATE	__WDB_SCHEMA__.dataprovidername 
	SET 	dataprovidernamerightset = dataprovidernamerightset + 2
	WHERE	dataprovidernamespaceid = namespace_
	  AND	dataproviderid = gpid_;
	-- Update Dataprovider
	UPDATE	__WDB_SCHEMA__.dataprovidername 
	SET 	dataprovidernameleftset = gprgt_
	WHERE	dataprovidernamespaceid = namespace_
	  AND	dataproviderid = dpid_;
	UPDATE	__WDB_SCHEMA__.dataprovidername 
	SET 	dataprovidernamerightset = gprgt_ + 1
	WHERE	dataprovidernamespaceid = namespace_
	  AND	dataproviderid = dpid_;
	-- Return dataproviderid
	RETURN dpid_;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;




CREATE OR REPLACE FUNCTION
wci.addwciuser
(
	newDataProviderName text
)
RETURNS bigint AS
$BODY$
DECLARE
	id 			bigint;
BEGIN
	SELECT wci.addDataProvider
		( newDataProviderName,
		  'wci user',
		  'any',
		  '1 day',
		  'WCI User' ) INTO id; 
	RETURN id;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql STRICT VOLATILE;



--
-- Get DataProviders
--
CREATE OR REPLACE FUNCTION 
wci.getDataProvider( dataprovider 		text )	
RETURNS SETOF __WCI_SCHEMA__.dataprovider_v AS
$BODY$
	SELECT 	d.dataproviderid,
			d.dataprovidertype,
			d.spatialdomaindelivery,
			d.dataprovidernamespaceid,
			d.dataprovidername,
			d.dataprovidernameleftset,
			d.dataprovidernamerightset,
			d.dataproviderstoretime,
			d.dataprovidercomment
	FROM   __WCI_SCHEMA__.dataprovider_mv  d,
		   __WCI_SCHEMA__.getSessionData() s
	WHERE  d.dataprovidernamespaceid = s.dataprovidernamespaceid
	  AND  ( $1 IS NULL OR d.dataprovidername LIKE lower($1) );
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;


--
-- Get a list of all sub-dataproviders for the given data provider name.
--
CREATE OR REPLACE FUNCTION
wci.getdataprovidersingroup(dataprovidername text)
RETURNS SETOF text AS
$BODY$
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
		pb.dataprovidername=$1
	ORDER BY 
		p.dataproviderid;
$BODY$
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
$BODY$
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
	FROM __WCI_SCHEMA__.dataprovider_v
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
	FROM   __WCI_SCHEMA__.dataprovider_v
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
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



--
-- Get DataProviderName
--
CREATE OR REPLACE FUNCTION 
wci.getDataProviderName(
	name_	text
)
RETURNS SETOF __WCI_SCHEMA__.dataprovidername_v AS
$BODY$
	SELECT
		*
	FROM
		__WCI_SCHEMA__.dataprovidername_v
	WHERE dataproviderid = SOME 
		  ( SELECT dataproviderid 
		    FROM   __WCI_SCHEMA__.dataprovidername_v d,
		    	   __WCI_SCHEMA__.getSessionData() s
		    WHERE  d.dataprovidernamespaceid = s.dataprovidernamespaceid
		      AND  ( d.dataprovidername = lower($1) OR $1 IS NULL ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;