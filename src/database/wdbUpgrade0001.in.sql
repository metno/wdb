-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2011 met.no
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
-- This is the update file for WDB from schema version 0 to schema version 1
--
SET SESSION client_min_messages TO 'warning';

-- Fix dataprovidername to correctly handle similar names in different namespaces
-- 
ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    DROP CONSTRAINT dataprovidername_skey;

ALTER TABLE ONLY __WDB_SCHEMA__.dataprovidername
    ADD CONSTRAINT dataprovidername_skey UNIQUE (dataprovidernamespaceid, dataprovidername);


    
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
	FROM __WCI_SCHEMA__.dataprovider
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
		FROM   __WCI_SCHEMA__.dataprovider;

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
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



CREATE OR REPLACE FUNCTION
wci.setConfiguration( name_				varchar(80),
				  	  code_				varchar(80),
				  	  versionNumber_	integer )
RETURNS VOID AS
$BODY$
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
		END IF;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;
