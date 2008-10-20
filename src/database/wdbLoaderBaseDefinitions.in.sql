CREATE SCHEMA loaderBase;
REVOKE ALL ON SCHEMA loaderBase FROM PUBLIC;
GRANT ALL ON SCHEMA loaderBase TO wdb_admin;
GRANT USAGE ON SCHEMA loaderBase TO wdb_write;
GRANT USAGE ON SCHEMA loaderBase TO wdb_read;


--
-- Max SRID
--
CREATE OR REPLACE FUNCTION 
loaderBase.maxSrid( )
RETURNS integer AS
$BODY$
	SELECT max(srid) 
	FROM public.spatial_ref_sys
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';

--
-- SRID
--
CREATE OR REPLACE FUNCTION 
loaderBase.setSrid(
	srid_ integer,
	name_ text,
	proj_ text
)
RETURNS void AS
$BODY$
	INSERT INTO public.spatial_ref_sys (srid, auth_name, auth_srid, srtext, proj4text)
	VALUES ($1, $2, NULL, NULL, $3)
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';


--
-- PlaceDefinition XREF
--
CREATE VIEW loaderBase.placexref AS
SELECT
	def.placeid, 
	def.placegeometry, 
	grid.inumber, 
	grid.jnumber,
	grid.iincrement,
	grid.jincrement,
	grid.startlongitude,
	grid.startlatitude,
	grid.originalsrid
FROM
	__WDB_SCHEMA__.placedefinition def,
	__WDB_SCHEMA__.placeregulargrid grid 
WHERE
	def.placeid = grid.placeid;

REVOKE ALL ON loaderBase.placexref FROM public;
GRANT ALL ON loaderBase.placexref TO wdb_admin;
GRANT SELECT ON loaderBase.placexref TO wdb_felt;


--
-- Get Place Id
--
CREATE OR REPLACE FUNCTION 
loaderBase.getplaceid(
	placegeo_ 	text,
	placesrid_ 	integer,
	inumber_ 	integer,
	jnumber_ 	integer,
	iincrement_ real,
	jincrement_ real,
	startLon_ 	real,
	startLat_ 	real,
	origsrid_ 	integer
)
RETURNS SETOF bigint AS
$BODY$
	SELECT PlaceId::bigint
	FROM   loaderBase.placexref
	WHERE
		Equals(placegeometry, geomfromtext($1, $2)) AND
		iNumber = $3 AND jNumber = $4 AND
		round(iIncrement::numeric, 3) = round($5::numeric, 3) AND
		round(jIncrement::numeric, 3) = round($6::numeric, 3) AND
		round(startLongitude::numeric, 3) = round($7::numeric, 3) AND
		round(startLatitude::numeric, 3) = round($8::numeric, 3) AND
		originalsrid = $9;
	-- Note: There is no reason to work with more than 3 decimal points
	-- of precision, as the values cannot exceed that limit due to FELT
	-- file format limitations.
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';

--
-- Get SRID
--
CREATE OR REPLACE FUNCTION 
loaderBase.getSrid(
	srid_	text
)
RETURNS integer AS
$BODY$
	SELECT 	srid 
	FROM 	public.spatial_ref_sys
	WHERE 	proj4text = $1
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';

--
-- SRID
--
CREATE OR REPLACE FUNCTION 
loaderBase.setSrid(
	srid_	integer,
	name_	text,
	proj_	text
)
RETURNS void AS
$BODY$
	INSERT INTO public.spatial_ref_sys (srid, auth_name, auth_srid, srtext, proj4text)
	VALUES ($1, $2, NULL, NULL, $3)
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';

--
-- Unit and Unit Conversions
--
CREATE VIEW loaderbase.unitwithconversion AS
SELECT un.unitname, 
	   un.unittype, 
	   co.siunitconversioncoefficient,
	   co.siunitconversionterm
FROM  
	   __WDB_SCHEMA__.unit un
LEFT OUTER JOIN
	   __WDB_SCHEMA__.siunitconversion co
ON 	   (un.unitname = co.unitname);

REVOKE ALL ON loaderBase.unitwithconversion FROM public;
GRANT ALL ON loaderBase.unitwithconversion TO wdb_admin;
GRANT SELECT ON loaderBase.unitwithconversion TO wdb_felt;
GRANT SELECT ON loaderBase.unitwithconversion TO wdb_grib;


CREATE OR REPLACE FUNCTION 
loaderBase.unitconversion(
	name_	text
)
RETURNS SETOF loaderbase.unitwithconversion AS
$BODY$
	SELECT *
	FROM loaderbase.unitwithconversion
	WHERE $1 = unitname;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';
