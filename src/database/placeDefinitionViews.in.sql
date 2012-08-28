-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 - 2012 met.no
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
SET SESSION client_min_messages TO 'warning';


CREATE VIEW __WCI_SCHEMA__.placename_v AS
SELECT
    p.placeid,
    p.placenamespaceid,
    p.placename
FROM
	__WDB_SCHEMA__.placename p;

REVOKE ALL ON __WCI_SCHEMA__.placename_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.placename_valid_v AS
SELECT
    p.placeid,
    p.placename,
    p.placenamevalidfrom,
    p.placenamevalidto
FROM
	__WDB_SCHEMA__.placename p,
	__WCI_SCHEMA__.getSessionData() s
WHERE
	p.placenamespaceid = s.placenamespaceid;

REVOKE ALL ON __WCI_SCHEMA__.placename_valid_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.placename_valid_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.placename_valid_V TO wdb_read, wdb_write;



CREATE VIEW wci_int.placedefinition_v AS
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
		
REVOKE ALL ON wci_int.placedefinition_v FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition_v TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition_v TO wdb_read, wdb_write;



CREATE VIEW __WCI_SCHEMA__.placeregulargrid_v AS 
SELECT 
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry,
	pit.placeindeterminatetype,
	pn.placenamespaceid,
	pn.placename,	
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	btrim(srs.proj4text) AS projdefinition,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	__WDB_SCHEMA__.placename pn,
	__WDB_SCHEMA__.placeregulargrid pg,
	spatial_ref_sys srs,
	__WDB_SCHEMA__.placeindeterminatetype pit
WHERE
	pd.placeid = pn.placeid 
	AND pd.placeid = pg.placeid
	AND srs.srid = pg.originalsrid
	AND pit.placeindeterminatecode = pd.placeindeterminatecode
UNION ALL
SELECT
	pd.placeid,
	pd.placegeometrytype,
	pd.placegeometry, 
	pit.placeindeterminatetype,
	0 AS placenamespaceid,
	'grid(' || pg.startX || ' ' || pg.startY || ', ' || pg.incrementX || ' ' || pg.incrementY || ', ' || pg.numberX || ' ' || pg.numberY || ', ' || pg.originalsrid || ')' AS placename,
	pg.numberX, 
	pg.numberY, 
	pg.incrementX, 
	pg.incrementY, 
	pg.startX, 
	pg.startY, 
	pg.originalsrid,
	btrim(srs.proj4text) AS projdefinition,
	pd.placestoretime
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	__WDB_SCHEMA__.placeregulargrid pg,
	spatial_ref_sys srs,
	__WDB_SCHEMA__.placeindeterminatetype pit
WHERE
	pd.placeid = pg.placeid
	AND srs.srid = pg.originalsrid
	AND pit.placeindeterminatecode = pd.placeindeterminatecode;
	
REVOKE ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_v FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_v TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placeregulargrid_v TO wdb_read, wdb_write;

SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.placeregulargrid_mv', '__WCI_SCHEMA__.placeregulargrid_v');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placeregulargrid_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.placeregulargrid_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.placeregulargrid_mv TO wdb_read, wdb_write;



-- Recreate Materialized view
SELECT wdb_int.createMV('wci_int.placedefinition_mv', 'wci_int.placedefinition_v');
SELECT wdb_int.refreshMV('wci_int.placedefinition_mv');

REVOKE ALL ON wci_int.placedefinition_mv FROM PUBLIC;
GRANT ALL ON wci_int.placedefinition_mv TO wdb_admin;
GRANT SELECT ON wci_int.placedefinition_mv TO wdb_read, wdb_write;

-- Index Materialized View
CREATE INDEX XIE0Wci_placedefinition_mv ON wci_int.placedefinition_mv
USING GIST
(
    placeGeometry
);

CREATE INDEX XIE1Wci_placedefinition_mv ON wci_int.placedefinition_mv
(
	placeid
);

CREATE INDEX XIE2Wci_placedefinition_mv ON wci_int.placedefinition_mv
(
	placeNameValidFrom,
	placeNameValidTo
);

CREATE INDEX XIE3Wci_placedefinition_mv ON wci_int.placedefinition_mv
(
	placeName,
	placeNameSpaceId
);
