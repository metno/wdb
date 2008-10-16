-- This is an outline of how to create a station list from the kvalobs database

-- WARNING
-- There is a serious problem in this procedure, as kvalobs does not specify a 
-- spatial_ref_sys for its coordinates.  

-- In a kvalobs database:

CREATE TEMP TABLE t AS (
SELECT 
	(stationid*10) + 100000000 AS placeid,
	0 AS placeindeterminatecode,
	1 AS datumid,
	'Point' as placegeometrytype,
	'Y' as areareferenceflag,
	'now' as placestoretime,
	'Observation site' as placedefinitioncomment,
	'SRID=4030;POINT(' || lat || ' ' || lon || ')' as placegeo 
FROM 
	station
);

COPY t TO '/home/vegardb/tmp/stations.csv' WITH CSV HEADER;



-- In wdb:

COPY wdb.placedefinition FROM '/home/vegardb/tmp/stations.csv' CSV HEADER;