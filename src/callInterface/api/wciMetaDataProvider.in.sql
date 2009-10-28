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
	dataProviderComment_	text
)
RETURNS bigint AS
$BODY$
DECLARE
	dpid_		bigint;
	namespace_	int;
BEGIN
	-- Session
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Namespace
	namespace_ := 0;	
	-- Check 
	SELECT dataproviderid INTO dpid_ 
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = lower(dataProviderName_) AND
		  dataprovidernamespaceid = namespace_;
	-- Add dataprovider
	IF NOT FOUND THEN
		dpid_ := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);
		
		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		( dpid_, dataProviderType_, domainDelivery_, '1 day', 'now' );
	
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		( dpid_, dataProviderComment_, 'now' );
	
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
		( dpid_, namespace_, lower(dataProviderName_), 'today'::TIMESTAMP WITH TIME ZONE, 'infinity'::TIMESTAMP WITH TIME ZONE, dpid_, dpid_ );
	ELSE
		RAISE EXCEPTION 'Dataprovider already exists in WDB';
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
	namespace_	int;
	id 			bigint;
BEGIN
	-- WCI User Check
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Check for existing name
	SELECT dataproviderid INTO id 
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = newDataProviderName AND
		dataprovidertype = 'WCI User';
	IF NOT FOUND THEN
		id := nextval('__WDB_SCHEMA__.dataprovider_dataproviderid_seq'::regclass);
		namespace_ = 0;
		-- Insert into tables
		INSERT INTO __WDB_SCHEMA__.dataprovider VALUES
		(id, 'WCI User', 'Any', '1 day', 'now');
		INSERT INTO __WDB_SCHEMA__.dataprovidercomment VALUES
		(id, 'wci user', 'now');
		INSERT INTO __WDB_SCHEMA__.dataprovidername VALUES
		(id, namespace_, newDataProviderName, 'today', '2999-12-31', id, id);
	ELSE
		RAISE INFO 'User was already defined as a dataprovider';
	END IF;

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
RETURNS SETOF __WCI_SCHEMA__.dataprovider AS
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
-- add new data provider name in namespace
-- 
CREATE OR REPLACE FUNCTION
wci.setDataProviderName
(
	canonicalName_		text,
	dataProviderName_ 	text
)
RETURNS void AS
$BODY$
DECLARE	
	namespace_ 		int;
	dataProviderId_ int;
BEGIN
	-- Get namespace
	SELECT dataprovidernamespaceid INTO namespace_
	FROM __WCI_SCHEMA__.getSessionData();
	IF ( namespace_ = 0 ) THEN
		RETURN;
	END IF;	
	-- Get parameterid
	SELECT dataproviderid INTO dataProviderId_
	FROM __WCI_SCHEMA__.dataprovider
	WHERE dataprovidername = lower(canonicalName_) AND
		  dataprovidernamespaceid = 0;
	-- Delete old name if any exist
	DELETE FROM __WDB_SCHEMA__.dataprovidername
	WHERE dataprovidernamespaceid = namespace_ AND
		  dataproviderid = dataProviderId_;
	-- Insert new name
	INSERT INTO __WDB_SCHEMA__.dataprovidername
	VALUES ( dataProviderId_,
			 namespace_,
			 lower(dataProviderName_),
			  'today'::TIMESTAMP WITH TIME ZONE, 
			  'infinity'::TIMESTAMP WITH TIME ZONE, 
			  dataproviderid_, 
			  dataproviderid_ );
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
		      AND  ( d.dataprovidername = $1 OR $1 IS NULL ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;