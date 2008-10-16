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
__WCI_SCHEMA__.dataProviderWhereClause
(
	dataprovider integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_dataProviderWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.locationWhereClause
(
	location text
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_locationWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.placeIdWhereClause
(
	placeid integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_placeIdWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.referenceTimeWhereClause
(
	referencetime wci.timeSpec
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_referenceTimeWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.validTimeWhereClause
(
	validtime wci.timeSpec
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_validTimeWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.parameterWhereClause
(
	parameter text[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_parameterWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.levelWhereClause
(
	level wci.levelSpec
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_levelWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;

CREATE OR REPLACE FUNCTION 
__WCI_SCHEMA__.dataVersionWhereClause
(
	dataversion integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wci_dataVersionWhereClause'
LANGUAGE 'c' STRICT IMMUTABLE;
