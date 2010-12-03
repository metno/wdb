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


--
-- add New Organization
-- 
CREATE OR REPLACE FUNCTION
wci.addOrganization
(
	name_				text,
	alias_				text,
	type_ 				text,
	validFrom_			date,
	validTo_			date,
	comment_			text
)
RETURNS bigint AS
AS $$
DECLARE
	ppid_		int;
BEGIN
	-- Check Session ID
	PERFORM wci_int.getSessionData();
	-- Check 
	SELECT partyid INTO ppid_ 
	FROM wci_int.organization
	WHERE organizationalias = alias_;
	-- Add Organization
	IF NOT FOUND THEN
		ppid_ := nextval('wdb_int.party_partyid_seq'::regclass);
		
		INSERT INTO wdb_int.party( PartyId, PartyType, PartyValidFrom, PartyValidTo ) 
			VALUES ( ppid_, 'organization', validfrom_,  validto_ );
	
		INSERT INTO wdb_int.organization VALUES
		( ppid_, name_, alias_, type_ );
		
		IF ( comment_ IS NOT NULL ) THEN
			INSERT INTO wdb_int.partycomment VALUES
			( ppid_, comment_, 'now' );
		END IF;
	
	ELSE
		UPDATE wdb_int.party SET
				partyvalidfrom = validfrom_,
				partyvalidto = validto_
		WHERE partyid = ppid_;
	
		UPDATE wdb_int.organization SET
				organizationname = name_,
				organizationalias = alias_,
				organizationtype = type_
		WHERE partyid = ppid_;
		
		IF ( comment_ IS NOT NULL ) THEN
			PERFORM *  
			FROM wdb_int.partycomment
			WHERE partyid = ppid_;
			
			IF NOT FOUND THEN
				INSERT INTO wdb_int.partycomment VALUES
				( ppid_, comment_, 'now' );
			ELSE
				UPDATE wdb_int.partycomment SET
					partycomment = comment_,
					partycommentstoretime = 'now'
				WHERE partyid = ppid_;
			END IF;
		END IF;
		

	END IF;
	-- Return partyid
	RETURN Ppid_;
END;
$$
LANGUAGE plpgsql SECURITY DEFINER;



--
-- add New Person
-- 
CREATE OR REPLACE FUNCTION
wci.addPerson
(
	firstname_				text,
	lastName_				text,
	title_	 				text,
	salutation_				text,
	initials_				text,
	signum_					text,
	gender_					text,
	suffix_					text,
	marital_				text,
	validFrom_				date,
	validTo_				date,
	comment_				text
)
		( ppid_, firstName_, lastName_, title_, salutation_, initials_, signum_, gender_, suffix_, marital_ );
RETURNS bigint AS
AS $$
DECLARE
	ppid_		int;
BEGIN
	-- Check Session ID
	PERFORM wci_int.getSessionData();
	-- Check 
	SELECT partyid INTO ppid_ 
	FROM wci_int.person
	WHERE initials = initials_;
	-- Add dataprovider
	IF NOT FOUND THEN
		ppid_ := nextval('wdb_int.party_partyid_seq'::regclass);
		
		INSERT INTO wdb_int.party( PartyId, PartyType, PartyValidFrom, PartyValidTo ) 
			VALUES ( ppid_, 'person', validfrom_,  validto_ );
	
		INSERT INTO wdb_int.person VALUES
		( ppid_, firstName_, lastName_, title_, salutation_, initials_, signum_, gender_, suffix_, marital_ );
		
		IF ( comment_ IS NOT NULL ) THEN
			INSERT INTO wdb_int.partycomment VALUES
			( ppid_, comment_, 'now' );
		END IF;
	
	ELSE
		UPDATE wdb_int.party SET
				partyvalidfrom = validfrom_,
				partyvalidto = validto_
		WHERE partyid = ppid_;
	
		UPDATE wdb_int.person SET
				firstName = firstName_,
				lastName = lastName_,
				title = title_,
				salutation = salutation_,
				initials = initials_,
				signum = signum_, 
				gender = gender_,
				namesuffix = suffix_,
				maritalstatus = marital_
		WHERE partyid = ppid_;
		
		IF ( comment_ IS NOT NULL ) THEN
			PERFORM *  
			FROM wdb_int.partycomment
			WHERE partyid = ppid_;
			
			IF NOT FOUND THEN
				INSERT INTO wdb_int.partycomment VALUES
				( ppid_, comment_, 'now' );
			ELSE
				UPDATE wdb_int.partycomment SET
					partycomment = comment_,
					partycommentstoretime = 'now'
				WHERE partyid = ppid_;
			END IF;
		END IF;
		

	END IF;
	-- Return partyid
	RETURN ppid_;
END;
$$
LANGUAGE plpgsql SECURITY DEFINER;



--
-- add New Software Version
-- 
CREATE OR REPLACE FUNCTION
wci.addSoftwareVersion
(
	name_				text,
	version_			text,
	validFrom_			date,
	validTo_			date,
	comment_			text
)
RETURNS bigint AS
AS $$
DECLARE
	ppid_		int;
BEGIN
	-- Check Session ID
	PERFORM wci_int.getSessionData();
	-- Check 
	SELECT partyid INTO ppid_ 
	FROM wci_int.softwareversion
	WHERE softwarename = name_ AND
	      softwareversioncode = version_;
	-- Add Organization
	IF NOT FOUND THEN
		ppid_ := nextval('wdb_int.party_partyid_seq'::regclass);
		
		INSERT INTO wdb_int.party( PartyId, PartyType, PartyValidFrom, PartyValidTo ) 
			VALUES ( ppid_, 'software', validfrom_,  validto_ );
	
		INSERT INTO wdb_int.softwareversion VALUES
		( ppid_, name_, version_ );
		
		IF ( comment_ IS NOT NULL ) THEN
			INSERT INTO wdb_int.partycomment VALUES
			( ppid_, comment_, 'now' );
		END IF;
	
	ELSE
		UPDATE wdb_int.party SET
				partyvalidfrom = validfrom_,
				partyvalidto = validto_
		WHERE partyid = ppid_;
	
		UPDATE wdb_int.softwareversion SET
				softwarename = name_,
				softwareversioncode = version_
		WHERE partyid = ppid_;
		
		IF ( comment_ IS NOT NULL ) THEN
			PERFORM *  
			FROM wdb_int.partycomment
			WHERE partyid = ppid_;
			
			IF NOT FOUND THEN
				INSERT INTO wdb_int.partycomment VALUES
				( ppid_, comment_, 'now' );
			ELSE
				UPDATE wdb_int.partycomment SET
					partycomment = comment_,
					partycommentstoretime = 'now'
				WHERE partyid = ppid_;
			END IF;
		END IF;
		

	END IF;
	-- Return partyid
	RETURN ppid_;
END;
$$
    LANGUAGE plpgsql SECURITY DEFINER;
