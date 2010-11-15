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
