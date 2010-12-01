-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2010 met.no
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


CREATE OR REPLACE FUNCTION
wci.addNameSpace( id_			integer,
				  name_			varchar(80),
				  desc_			varchar(255),
				  usage_		varchar(80),
				  owner_		varchar(80),
				  contact_		varchar(80),
				  validfrom_	date )
RETURNS VOID AS
$BODY$
DECLARE
	oid_		int;
	cid_		int;
BEGIN
	-- Check Session ID
	PERFORM __WCI_SCHEMA__.getSessionData();
	-- Get OwnerID
	SELECT partyid INTO oid_
	FROM __WCI_SCHEMA__.organization
	WHERE organizationalias = owner_;
	-- Get ContactID
	SELECT partyid INTO cid_
	FROM __WCI_SCHEMA__.person
	WHERE initials = contact_;
	-- Check
	PERFORM * 
	FROM __WDB_SCHEMA__.namespace
	WHERE namespaceid = id_;
	-- Update or INSERT
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.namespace
		VALUES ( id_,
				 name_,
				 desc_,
				 usage_,
				 oid_,
				 cid_,
				 validfrom_ );
	ELSE
		UPDATE  __WDB_SCHEMA__.namespace 
		SET 	namespacename = name_,
				namespacedescription = desc_,
				namespacefieldofapplication = usage_,
				namespaceownerid = oid_,
				namespacecontactid = cid_,
		    	namespacevalidfrom = validfrom_
		WHERE   namespaceid = id_;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;

REVOKE ALL ON FUNCTION wci.addNameSpace( id_		integer,
									     name_		varchar(80),
									     desc_		varchar(255),
									     usage_		varchar(80),
									     owner_		varchar(80),
									     contact_	varchar(80),
									     validfrom_	date )
									  FROM public;
GRANT ALL ON FUNCTION wci.addNameSpace( id_			integer,
				  						name_		varchar(80),
				  						desc_		varchar(255),
									  	usage_		varchar(80),
									  	owner_		varchar(80),
									  	contact_	varchar(80),
									  	validfrom_	date )
 									  TO wdb_admin;
          				 
-- Set the Default Namespace of a User
-- This function sets the default namespace that is used if wci.begin is
-- called without additional parameters.
-- UserName '' sets the default namespace usage.
CREATE OR REPLACE FUNCTION
wci.setDefaultNameSpace( userName_ NAME, 
          				 dataproviderNameSpaceId_ integer,
          				 placeNameSpaceId_ integer,
          				 parameterNameSpaceId_ integer )
RETURNS VOID AS
$BODY$
BEGIN
	-- Check for user
	PERFORM *
	FROM   __WDB_SCHEMA__.defaultnamespace
	WHERE  rolname = userName_;
	-- Update or INSERT
	IF NOT FOUND THEN
		INSERT INTO __WDB_SCHEMA__.defaultnamespace VALUES ( userName_, 
															 dataprovidernamespaceid_,
															 placenamespaceid_,
															 parameternamespaceid_ );	
	ELSE
		UPDATE  __WDB_SCHEMA__.defaultnamespace 
		SET 	dataprovidernamespaceid = dataprovidernamespaceid_,
		    	placenamespaceid = placenamespaceid_,
		    	parameternamespaceid = parameternamespaceid_
		WHERE   rolName = userName_;
	END IF;
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;

REVOKE ALL ON FUNCTION wci.setDefaultNameSpace( userName_ NAME, 
						dataproviderNameSpaceId_ integer,
          				 placeNameSpaceId_ integer,
          				 parameterNameSpaceId_ integer ) FROM public;
GRANT ALL ON FUNCTION wci.setDefaultNameSpace( userName_ NAME, 
          				 dataproviderNameSpaceId_ integer,
          				 placeNameSpaceId_ integer,
          				 parameterNameSpaceId_ integer ) TO wdb_admin;

          				 
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
			VALUES ( sid_, 'software', 'now',  '2999-12-31' );
	
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

REVOKE ALL ON FUNCTION wci.addNameSpace( id_		integer,
									     name_		varchar(80),
									     desc_		varchar(255),
									     usage_		varchar(80),
									     owner_		varchar(80),
									     contact_	varchar(80),
									     validfrom_	date )
									  FROM public;
GRANT ALL ON FUNCTION wci.addNameSpace( id_			integer,
				  						name_		varchar(80),
				  						desc_		varchar(255),
									  	usage_		varchar(80),
									  	owner_		varchar(80),
									  	contact_	varchar(80),
									  	validfrom_	date )
 									  TO wdb_admin;          				 