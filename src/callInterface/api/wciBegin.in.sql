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
CREATE OR REPLACE FUNCTION
wci.begin( newUser NAME, 
          dataproviderNameSpaceId integer,
          placeNameSpaceId integer,
          parameterNameSpaceId integer)
RETURNS VOID AS
$BODY$
DECLARE
BEGIN
	RAISE DEBUG 'Setting new role: %', newUser;
	PERFORM __WCI_SCHEMA__.setRole( newUser );
	PERFORM __WCI_SCHEMA__.setupSession(newUser,dataproviderNameSpaceId,placeNameSpaceId,parameterNameSpaceId );
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;


CREATE OR REPLACE FUNCTION
wci.begin( newUser NAME )
RETURNS VOID AS
$BODY$
DECLARE
	dataproviderNameSpaceId integer;
	placeNameSpaceId integer;
	parameterNameSpaceId integer;
BEGIN

	PERFORM __WCI_SCHEMA__.setRole( newUser );
	
	SELECT * INTO dataproviderNameSpaceId, placeNameSpaceId, parameterNameSpaceId
		FROM __WCI_SCHEMA__.sessiondata;

	PERFORM __WCI_SCHEMA__.setupSession(newUser,dataproviderNameSpaceId,placeNameSpaceId,parameterNameSpaceId );
END;
$BODY$
LANGUAGE 'plpgsql' VOLATILE;
