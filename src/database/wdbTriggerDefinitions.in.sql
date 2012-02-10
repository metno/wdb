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

	SELECT proj4text INTO projDef FROM spatial_ref_sys WHERE srid=grid.originalsrid;

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
		--EXCEPTION
		--	WHEN OTHERS THEN 
				-- don't know proper name of exception. 
				-- We silently ignore attempts to do multiple deletes of the same large object.
				-- This happens when a single delete affects several rows with the same value. 
		--		RETURN NULL;
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

CREATE TRIGGER trigger___WDB_SCHEMA___deleteObsoleteGrids
    AFTER DELETE ON __WDB_SCHEMA__.gridvalue
    FOR EACH ROW
    EXECUTE PROCEDURE __WDB_SCHEMA__.deleteObsoleteGrids();




CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.updatedataprovider_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.dataprovider_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger___WDB_SCHEMA___updatedataprovider_mv1
	AFTER INSERT OR UPDATE OR DELETE ON __WDB_SCHEMA__.dataprovidername
	EXECUTE PROCEDURE __WDB_SCHEMA__.updatedataprovider_mv();

CREATE TRIGGER trigger___WDB_SCHEMA___updatedataprovider_mv2
	AFTER INSERT OR UPDATE OR DELETE ON __WDB_SCHEMA__.dataprovidercomment
	EXECUTE PROCEDURE __WDB_SCHEMA__.updatedataprovider_mv();


	
	
	
CREATE FUNCTION __WDB_SCHEMA__.updateplacespec_delete() 
RETURNS TRIGGER
AS 
$$
BEGIN
	DELETE FROM wci_int.placespec WHERE placeid=OLD.placeid;
	DELETE FROM wci_int.placedefinition_mv WHERE placeid=OLD.placeid;
	DELETE FROM wci_int.placeregulargrid_mv WHERE placeid=OLD.placeid;
	RETURN NULL;
END
$$
SECURITY DEFINER LANGUAGE 'plpgsql';
CREATE FUNCTION __WDB_SCHEMA__.updateplacespec() 
RETURNS TRIGGER
AS 
$$
BEGIN
	DELETE FROM wci_int.placespec WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placespec (SELECT * FROM wci_int.placespec_v WHERE placeid=NEW.placeid);
	DELETE FROM wci_int.placedefinition_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placedefinition_mv (SELECT * FROM wci_int.placedefinition WHERE placeid=NEW.placeid);
	DELETE FROM wci_int.placeregulargrid_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placeregulargrid_mv (SELECT * FROM wci_int.placeregulargrid WHERE placeid=NEW.placeid);
	RETURN NULL;
END
$$
SECURITY DEFINER LANGUAGE 'plpgsql';

CREATE FUNCTION __WDB_SCHEMA__.refreshplacespec() 
RETURNS TRIGGER
AS 
$$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('wci_int.placespec');
	RETURN NULL;
END
$$
SECURITY DEFINER LANGUAGE 'plpgsql';

CREATE TRIGGER __WDB_SCHEMA___updateplacespec_a AFTER INSERT OR UPDATE ON __WDB_SCHEMA__.placename
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec(); 
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_b AFTER UPDATE ON __WDB_SCHEMA__.placedefinition
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_c AFTER UPDATE ON __WDB_SCHEMA__.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec(); 
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_d AFTER UPDATE ON spatial_ref_sys
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.refreshplacespec();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_e AFTER UPDATE ON wdb_int.placeindeterminatetype
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.refreshplacespec();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_f AFTER UPDATE ON __WDB_SCHEMA__.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.refreshplacespec();
  
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_ad AFTER DELETE ON __WDB_SCHEMA__.placename
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete(); 
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_bd AFTER DELETE ON __WDB_SCHEMA__.placedefinition
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_cd AFTER DELETE ON __WDB_SCHEMA__.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete(); 
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_dd AFTER DELETE ON spatial_ref_sys
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_ed AFTER DELETE ON wdb_int.placeindeterminatetype
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete();
CREATE TRIGGER __WDB_SCHEMA___updateplacespec_ed AFTER DELETE ON __WDB_SCHEMA__.placeregulargrid
  FOR EACH ROW EXECUTE PROCEDURE __WDB_SCHEMA__.updateplacespec_delete();



	
CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.updateparameter_mv() RETURNS "trigger"
	AS $$
BEGIN
	PERFORM __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.parameter_mv');
	RETURN NULL;
END;
$$ LANGUAGE 'plpgsql';

CREATE TRIGGER trigger___WDB_SCHEMA___updateparameter_mv1
	AFTER INSERT OR UPDATE OR DELETE ON __WDB_SCHEMA__.parameter
	EXECUTE PROCEDURE __WDB_SCHEMA__.updateparameter_mv();

CREATE TRIGGER trigger___WDB_SCHEMA___updatevalueparameter_mv2
	AFTER INSERT OR UPDATE OR DELETE ON __WDB_SCHEMA__.parametername
	EXECUTE PROCEDURE __WDB_SCHEMA__.updateparameter_mv();
	
