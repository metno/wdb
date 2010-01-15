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
__WCI_SCHEMA__.setupSession("user" text,
				          dataprovidernamespaceid integer,
				          placenamespaceid integer,
				          parameternamespaceid integer
)
RETURNS VOID AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_initialize'
LANGUAGE 'c' VOLATILE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.tearDownSession
()
RETURNS VOID AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_reset'
LANGUAGE 'c' VOLATILE;


-- correctly defined in wciSession.sql
-- Get session info - the __WCI_SCHEMA__.sessionData object associated with the
-- current session.
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.getSessionData
()
RETURNS __WCI_SCHEMA__.sessionData AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_get'
LANGUAGE 'c' STABLE;


-- Has wci.begin() been called?
CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.sessionIsInitialized
()
RETURNS boolean AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_initialized'
LANGUAGE 'c' STABLE;

-- Call 'SET ROLE newRole' in a correct way.
CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.setRole
(
	newRole text
)
RETURNS VOID AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciSession_setRole'
LANGUAGE 'c' VOLATILE;

CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.readCachePurge()
RETURNS void AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciReadCachePurge'
LANGUAGE 'c' STABLE;
