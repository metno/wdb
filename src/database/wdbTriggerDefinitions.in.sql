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

CREATE TYPE __WDB_SCHEMA__.core_getAllGridPointsReturnType AS (
    i int,
    j int,
    geo text
);

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.core_getAllGridPoints( gridDefinition __WDB_SCHEMA__.placespec )
RETURNS SETOF __WDB_SCHEMA__.core_getAllGridPointsReturnType AS
'__WDB_LIBDIR__/__WCI_LIB__', 'wciGetAllGridPoints'
LANGUAGE 'c' STRICT IMMUTABLE;

-- Populating placepoints table when creating a regular grid
CREATE FUNCTION __WDB_SCHEMA__.triggerfun_populateplacepoints() RETURNS "trigger" AS 
$$
DECLARE
        gridDefinition __WDB_SCHEMA__.placespec;
        point __WDB_SCHEMA__.core_getAllGridPointsReturnType;
BEGIN
        SELECT * INTO gridDefinition FROM __WDB_SCHEMA__.placespec WHERE placeid = NEW.placeid;

		INSERT INTO __WDB_SCHEMA__.placepoint ( placeid, i, j, location )
		SELECT NEW.placeid, grid.i, grid.j, geomfromtext( grid.geo, 4030 )
		FROM __WDB_SCHEMA__.core_getAllGridPoints( gridDefinition ) grid;

        RETURN NULL;
END;
$$
LANGUAGE 'plpgsql' STRICT;



CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.core_getAllGridPoints( place int )
RETURNS SETOF __WDB_SCHEMA__.core_getAllGridPointsReturnType AS
$BODY$
DECLARE
	gridDefinition __WDB_SCHEMA__.placespec;
	entry __WDB_SCHEMA__.core_getAllGridPointsReturnType;
BEGIN
	SELECT * INTO gridDefinition FROM __WDB_SCHEMA_.placespec WHERE placeid=place;

	FOR entry IN SELECT * FROM __WDB_SCHEMA_.core_getAllGridPoints(gridDefinition) LOOP
		RETURN NEXT entry;
	END LOOP;

	--RETURN QUERY SELECT * FROM __WDB_SCHEMA_.core_getAllGridPoints(gridDefinition);
END;
$BODY$
LANGUAGE plpgsql STRICT IMMUTABLE;

-- Updating of placedefinition when inserting a regular grid
create or replace function __WDB_SCHEMA__.createGeometryText(
	iNum integer, jNum integer,
	iIncrement float8, jIncrement float8,
	startLongitude float8, startLatitude float8,
	projDefinition text
)
returns text as
'__WDB_LIBDIR__/wdb.so', 'createGeometryText'
language C strict immutable;

create or replace function 
__WDB_SCHEMA__.getGeometry(grid __WDB_SCHEMA__.placeregulargrid)
returns geometry as
$body$
declare 
	geo text;
	projdef text;
begin

	SELECT proj4text INTO projDef FROM spatial_ref_sys WHERE srid=grid.originalsrid;

	geo := __WDB_SCHEMA__.createGeometryText(
		grid.inumber, 
		grid.jnumber, 
		grid.iincrement, 
		grid.jincrement, 
		grid.startlongitude, 
		grid.startlatitude, 
		projdef
	); 

	return geomfromtext(
		geo,
		4030
	);
end 
$body$
language plpgsql strict immutable;

create or replace function
__WDB_SCHEMA__.createPlaceDefinitionFromPlaceRegularGrid() 
returns trigger as
$body$
DECLARE
	geo geometry;
BEGIN
	geo := __WDB_SCHEMA__.getGeometry(NEW);

	insert into __WDB_SCHEMA__.placedefinition values (NEW.placeid, 0, 'Grid', 'now', geo);

	return NEW;
END 
$body$
language plpgsql;

create trigger trigger_createPlaceDefinitionFromPlaceRegularGrid
before insert 
on __WDB_SCHEMA__.placeregulargrid 
for each row 
execute procedure
__WDB_SCHEMA__.createPlaceDefinitionFromPlaceRegularGrid();



-- Deletion control:

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.deleteObsoleteOids() RETURNS trigger
	AS 
$BODY$
DECLARE
	noOfRows integer;
	status integer;
BEGIN

	SELECT count(*) INTO noOfRows FROM __WDB_SCHEMA__.oidvalue WHERE value=OLD.value;

	IF noOfRows = 0 THEN
		BEGIN
			status := lo_unlink(OLD.value);
		EXCEPTION
			WHEN OTHERS THEN 
				-- don't know proper name of exception. 
				-- We silently ignore attempts to do multiple deletes of the same large object.
				-- This happens when a single delete affects several rows with the same value. 
				RETURN NULL;
		END;
		IF status = -1 THEN
			RAISE WARNING 'Error when attempting to delete large object <%>', OLD.value;
		END IF;
	ELSE
		RAISE DEBUG 'Still % rows left which refers to oid <%>', noOfRows, OLD.value;
	END IF;

	RETURN NULL;
END;
$BODY$
LANGUAGE plpgsql;

CREATE TRIGGER trigger_deleteObsoleteOids
    AFTER DELETE ON __WDB_SCHEMA__.oidvalue
    FOR EACH ROW
    EXECUTE PROCEDURE __WDB_SCHEMA__.deleteObsoleteOids();


--
-- TOC entry 2509 (class 2620 OID 1391800)
-- Dependencies: 1793 448
CREATE TRIGGER trigger_populateplacepoints
    AFTER INSERT ON __WDB_SCHEMA__.placeregulargrid
    FOR EACH ROW
    EXECUTE PROCEDURE __WDB_SCHEMA__.triggerfun_populateplacepoints();







CREATE OR REPLACE FUNCTION updatedataprovider_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.dataprovider_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_updatedataprovider_mv
	AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.dataprovidername
	EXECUTE PROCEDURE updatedataprovider_mv();


CREATE OR REPLACE FUNCTION updateplacedefinition_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.placedefinition_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_updateplacedefinition_mv1
	AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.placedefinition
	EXECUTE PROCEDURE updateplacedefinition_mv();

CREATE TRIGGER trigger_updateplacedefinition_mv2
	AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.placename
	EXECUTE PROCEDURE updateplacedefinition_mv();


CREATE OR REPLACE FUNCTION updatevalueparameter_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.valueparameter_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_updatevalueparameter_mv
	AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.valuephysicalparameter
	EXECUTE PROCEDURE updatevalueparameter_mv();


CREATE OR REPLACE FUNCTION updatelevelparameter_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.levelparameter_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger_updatelevelparameter_mv
	AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.levelphysicalparameter
	EXECUTE PROCEDURE updatelevelparameter_mv();

