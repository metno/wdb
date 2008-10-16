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
-- List the Units available in the database
--
-- @param unit		A wildcard string to use in the search. If NULL is
--                     	given, all entries are returned
--
-- @return A set of unit information matching the given wildcard
CREATE OR REPLACE FUNCTION
wci.listUnit
(
          unit character varying(80)
)
RETURNS SETOF __WCI_SCHEMA__.unit AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.unit
	WHERE 
		$1 IS NULL OR
		unitname ILIKE $1 ;
$BODY$
LANGUAGE 'sql';


-- List all of the units available in the database
-- Equivalent to wci.listUnit ( NULL )
CREATE OR REPLACE FUNCTION
wci.listUnit
(
)
RETURNS SETOF __WCI_SCHEMA__.unit AS
$BODY$
	SELECT wci.listUnit ( NULL ); 
$BODY$
LANGUAGE 'sql';


-- List the Statistics Types usable to describe values
--
-- @param stattype	A wildcard string to use in the search. If NULL is
--			given, all entries are returned.
--
-- @return A set of statistics types with description matching the wildcard
CREATE OR REPLACE FUNCTION
wci.listStatisticsType
(
          stattype character varying(80)
)
RETURNS SETOF __WCI_SCHEMA__.statisticstype AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.statisticstype
	WHERE 
		$1 IS NULL OR
		statisticstype ILIKE $1 ;
$BODY$
LANGUAGE 'sql';


-- List all of the Statistics Types available in the database
-- Equivalent to wci.listStatisticsType ( NULL )
CREATE OR REPLACE FUNCTION
wci.listStatisticsType
(
)
RETURNS SETOF __WCI_SCHEMA__.statisticstype AS
$BODY$
	SELECT  wci.listStatisticsType ( NULL );
$BODY$
LANGUAGE 'sql';


-- List the physical phenomena that can be used to describe values
--
-- @param pp		A wilcard string to use in the search. If NULL is
--			given, all entries are returned
--
-- @return A set of physical phenomena matching the wildcard
CREATE OR REPLACE FUNCTION
wci.listPhysicalPhenomena
(
          pp character varying(80)
)
RETURNS SETOF __WCI_SCHEMA__.physicalphenomenon AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.physicalphenomenon
	WHERE 
		$1 IS NULL OR
		physicalphenomenon ILIKE $1 ;
$BODY$
LANGUAGE 'sql';


-- List all of the physical phenomena available in the database
-- Equivalent to wci.listPhysicalPhenomena ( NULL )
CREATE OR REPLACE FUNCTION
wci.listPhysicalPhenomena
(
)
RETURNS SETOF __WCI_SCHEMA__.physicalphenomenon AS
$BODY$
	SELECT  wci.listPhysicalPhenomena ( NULL );
$BODY$
LANGUAGE 'sql';


-- List the Value domains in the database matching the current value domain
-- codespaceid.
--
-- @param valdom	A wildcard string to use in the search. If NULL is
--			given, all entries are returned.
--
-- @return A set of value domains matching the wildcard
CREATE OR REPLACE FUNCTION
wci.listValueDomain
(
          valdom character varying(80)
)
RETURNS SETOF __WCI_SCHEMA__.valuedomain AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.valuedomain
	WHERE 
		$1 IS NULL OR
		valuedomain ILIKE $1 ;
$BODY$
LANGUAGE 'sql';


-- List all of the value domains in the database matching the current value
-- domain codespaceid.
-- Equivalent to wci.listValueDomain ( NULL )
CREATE OR REPLACE FUNCTION
wci.listValueDomain
(
)
RETURNS SETOF __WCI_SCHEMA__.valuedomain AS
$BODY$
	SELECT  wci.listValueDomain ( NULL );
$BODY$
LANGUAGE 'sql';


-- List the level domains in the database matching the current level domain
-- codespaceid.
--
-- @param levdom	A wildcard string to use in the search. If NULL is
--			given, all entries are returned.
--
-- @return A set of level domains matching the wildcard
CREATE OR REPLACE FUNCTION
wci.listLevelDomain
(
          levdom character varying(80)
)
RETURNS SETOF __WCI_SCHEMA__.leveldomain AS
$BODY$
	SELECT 
		* 
	FROM 
		__WCI_SCHEMA__.leveldomain
	WHERE 
		$1 IS NULL OR
		leveldomain ILIKE  $1 ;
$BODY$
LANGUAGE 'sql';


-- List all of the level domains in the database matching the current level 
-- domain codespaceid.
-- Equivalent to wci.listLevelDomain ( NULL )
CREATE OR REPLACE FUNCTION
wci.listLevelDomain
(
)
RETURNS SETOF __WCI_SCHEMA__.leveldomain AS
$BODY$
	SELECT  wci.listLevelDomain ( NULL );
$BODY$
LANGUAGE 'sql';
