-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2012 met.no
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
-- Update Schema to use Place Definition with Valid Times
-- 

SET SESSION client_min_messages TO 'warning';

--
-- Drop Dependent Views and Functions
--
DROP view wci_int.floatgridvalue;
DROP function wci_int.getlevelparametername(wci_int.gridvalue);
DROP function wci_int.lp_matches(wci_int.gridvalue,text[]);
DROP function wci_int.getparametername(wci_int.gridvalue);
DROP view wci_int.gridvalue;
DROP view wci_int.floatvalue;
SELECT wdb_int.dropMV('wci_int.placedefinition_mv');
DROP VIEW wci_int.placedefinition CASCADE;

--
-- Replace PlaceDefinition View
--
CREATE VIEW wci_int.placedefinition AS
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pn.placenamevalidfrom,
	pn.placenamevalidto,
	pg.originalsrid,
	pd.placestoretime
FROM 
	wdb_int.placedefinition pd, 
	wdb_int.placeregulargrid pg,
	wdb_int.placename pn
WHERE
	pd.placeid = pg.placeid
	AND pd.placeid = pn.placeid
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	pn.placenamespaceid,
	pn.placename,
	pn.placenamevalidfrom,
	pn.placenamevalidto,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 
	wdb_int.placedefinition pd, 
	wdb_int.placename pn
WHERE
	pd.placeid = pn.placeid
	AND pd.placegeometrytype <> 'grid' 
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	'-infinity' AS placenamevalidfrom,
	'infinity'  AS placenamevalidto,
	pg.originalsrid,
	pd.placestoretime	
FROM 	
	wdb_int.placedefinition pd, 
	wdb_int.placeregulargrid pg
WHERE
	pd.placeid = pg.placeid
UNION ALL
SELECT  
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pd.placeindeterminatecode,
	0 AS placenamespaceid,
	lower(st_astext( pd.placegeometry )) as placename,
	'-infinity' AS placenamevalidfrom,
	'infinity'  AS placenamevalidto,
	__WDB_SRID__ as originalsrid,
	pd.placestoretime	
FROM 	
	wdb_int.placedefinition pd
WHERE
	pd.placegeometrytype != 'grid';
		
REVOKE ALL ON wci_int.placedefinition FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition TO wdb_read, wdb_write;

--
-- Recreate Materialized view
--
SELECT wdb_int.createMV('wci_int.placedefinition_mv', 'wci_int.placedefinition');
SELECT wdb_int.refreshMV('wci_int.placedefinition_mv');

REVOKE ALL ON wci_int.placedefinition_mv FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition_mv TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition_mv TO wdb_read, wdb_write;

--
-- Index Materialized View
--
CREATE INDEX XIE0wci_placedefinition_mv ON wci_int.placedefinition_mv
USING GIST
(
    placeGeometry
);

CREATE INDEX XIE1wci_placedefinition_mv ON wci_int.placedefinition_mv
(
    placeNameValidFrom,
    placeNameValidTo,
	placeid
);

CREATE INDEX XIE2wci_placedefinition_mv ON wci_int.placedefinition_mv
(
	placeName,
	placeNameValidFrom,
	placeNameValidTo,
	placeNameSpaceId,
	placeId
);


--
-- New/Replacement WCI Functions
--
CREATE OR REPLACE FUNCTION 
wci.getPlaceDefinition( location 			text,	
						valid 				timestamp with time zone )
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	p.placeid,
			p.placegeometrytype,
			p.placegeometry,
			p.placeindeterminatecode,
			p.placenamespaceid,
			p.placename,
			p.placenamevalidfrom,
			p.placenamevalidto,
			p.originalsrid,
			p.placestoretime
	FROM	wci_int.placeDefinition_mv p,
			wci_int.getSessionData() s
	WHERE	p.placenamespaceid = s.placenamespaceid
	  AND	( $1 IS NULL OR placename LIKE lower($1) )
	  AND	( $2 IS NULL OR ( placenamevalidfrom <= $2 AND placenamevalidto >= $2 ) );
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

CREATE OR REPLACE FUNCTION 
wci.getPlaceDefinition( location 			text )	
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	*
	FROM	wci.getPlaceDefinition($1, 'now');
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

-- Place Point Info
CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text,
				   valid 				timestamp with time zone )
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	p.placeid,
			p.placegeometrytype,
			p.placegeometry,
			p.placeindeterminatecode,
			p.placenamespaceid,
			p.placename,
			p.placenamevalidfrom,
			p.placenamevalidto,
			p.originalsrid,
			p.placestoretime
	FROM	wci_int.placeDefinition_mv p,
			wci_int.getSessionData() s
	WHERE	p.placenamespaceid = s.placenamespaceid
	  AND	( $1 IS NULL OR placename LIKE lower($1) )
	  AND	( $2 IS NULL OR ( placenamevalidfrom <= $2 AND placenamevalidto >= $2 ) )
	  AND	p.placegeometrytype = 'point';
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;

CREATE OR REPLACE FUNCTION 
wci.getPlacePoint( location 			text )	
RETURNS SETOF wci_int.placedefinition AS
$BODY$
	SELECT 	*
	FROM	wci.getPlacePoint($1, 'now');
$BODY$
SECURITY DEFINER
LANGUAGE sql STABLE;




-- Replace view wci_int.floatvalue;
-- Replace view wci_int.gridvalue;
-- Replace function wci_int.getparametername(wci_int.gridvalue);
-- Replace function wci_int.lp_matches(wci_int.gridvalue,text[]);
-- Replace function wci_int.getlevelparametername(wci_int.gridvalue);
-- Replace view wci_int.floatgridvalue;
