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

-- Updating of placedefinition when inserting a regular grid
create or replace function __WDB_SCHEMA__.createGeometryText(
	xNum 		integer, 
	yNum 		integer,
	xIncrement 	float8, 
	yIncrement 	float8,
	startX 		float8, 
	startY 		float8,
	projDefinition text
)
returns text as
'__WDB_LIBDIR__/__WDB_LIB__', 'createGeometryText'
language C strict immutable;



create or replace function 
__WDB_SCHEMA__.getGeometry(grid __WDB_SCHEMA__.placeregulargrid)
returns geometry as
$body$
declare 
	geo text;
	projdef text;
begin
	-- Get ProjDef
	SELECT proj4text INTO projDef FROM spatial_ref_sys WHERE srid=grid.originalsrid;
	-- Create the geometry
	geo := __WDB_SCHEMA__.createGeometryText(
		grid.numberX, 
		grid.numberY, 
		grid.incrementX, 
		grid.incrementY, 
		grid.startX, 
		grid.startY, 
		projdef
	); 
	RAISE DEBUG 'Geo is %', geo;
	-- Return geometry object
	return st_geomfromtext(
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
	insert into __WDB_SCHEMA__.placedefinition values (NEW.placeid, 0, 'grid', 'now', geo);
	return NEW;
END 
$body$
language plpgsql;



create trigger trigger___WDB_SCHEMA___createPlaceDefinitionFromPlaceRegularGrid
before insert 
on __WDB_SCHEMA__.placeregulargrid 
for each row 
execute procedure
__WDB_SCHEMA__.createPlaceDefinitionFromPlaceRegularGrid();



-- Deletion control:
CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.deleteObsoleteGrids() RETURNS trigger
	AS 
$BODY$
DECLARE
	noOfRows integer;
	status integer;
BEGIN
	SELECT count(*) INTO noOfRows FROM __WDB_SCHEMA__.gridvalue WHERE value=OLD.value;
	IF noOfRows = 0 THEN
		BEGIN
			PERFORM __WDB_SCHEMA__.drop_file(OLD.value);
			-- No reaction to Exceptions
		END;
		IF status = -1 THEN
			RAISE WARNING 'Error when attempting to delete large object <%>', OLD.value;
		END IF;
	ELSE
		RAISE DEBUG 'Still % rows left which refers to Grid <%>', noOfRows, OLD.value;
	END IF;
	RETURN NULL;
END;
$BODY$
LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION wdb_int.updateplacespec() 
RETURNS TRIGGER
AS 
$BODY$
BEGIN
	DELETE FROM wci_int.placedefinition_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placedefinition_mv (SELECT * FROM wci_int.placedefinition_v WHERE placeid=NEW.placeid);
	DELETE FROM wci_int.placeregulargrid_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placeregulargrid_mv (SELECT * FROM wci_int.placeregulargrid_v WHERE placeid=NEW.placeid);
	RETURN NULL;
END
$BODY$
SECURITY DEFINER LANGUAGE 'plpgsql';



CREATE OR REPLACE FUNCTION wdb_int.updateplacespec_delete() 
RETURNS TRIGGER
AS 
$BODY$
BEGIN
	DELETE FROM wci_int.placedefinition_mv WHERE placeid=OLD.placeid;
	DELETE FROM wci_int.placeregulargrid_mv WHERE placeid=OLD.placeid;
	RETURN NULL;
END
$BODY$
SECURITY DEFINER LANGUAGE 'plpgsql';



CREATE FUNCTION wdb_int.refreshplacespec() 
RETURNS TRIGGER
AS 
$BODY$
BEGIN
	PERFORM wdb_int.refreshMV('wci_int.placedefinition_mv');
	PERFORM wdb_int.refreshMV('wci_int.placeregulargrid_mv');
	RETURN NULL;
END
$BODY$
SECURITY DEFINER LANGUAGE 'plpgsql';



CREATE TRIGGER wdb_int_updateplacespec_a AFTER INSERT OR UPDATE ON wdb_int.placename
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec(); 

CREATE TRIGGER wdb_int_updateplacespec_b AFTER UPDATE ON wdb_int.placedefinition
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec();

CREATE TRIGGER wdb_int_updateplacespec_c AFTER UPDATE ON wdb_int.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec(); 

CREATE TRIGGER wdb_int_updateplacespec_d AFTER UPDATE ON spatial_ref_sys
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.refreshplacespec();

CREATE TRIGGER wdb_int_updateplacespec_e AFTER UPDATE ON wdb_int.placeindeterminatetype
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.refreshplacespec();

CREATE TRIGGER wdb_int_updateplacespec_f AFTER UPDATE ON wdb_int.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.refreshplacespec();


  
CREATE TRIGGER wdb_int_updateplacespec_ad AFTER DELETE ON wdb_int.placename
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete(); 

CREATE TRIGGER wdb_int_updateplacespec_bd AFTER DELETE ON wdb_int.placedefinition
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete();

CREATE TRIGGER wdb_int_updateplacespec_cd AFTER DELETE ON wdb_int.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete(); 

CREATE TRIGGER wdb_int_updateplacespec_dd AFTER DELETE ON spatial_ref_sys
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete();

CREATE TRIGGER wdb_int_updateplacespec_ed AFTER DELETE ON wdb_int.placeindeterminatetype
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete();

CREATE TRIGGER wdb_int_updateplacespec_ed AFTER DELETE ON wdb_int.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE wdb_int.updateplacespec_delete();
    