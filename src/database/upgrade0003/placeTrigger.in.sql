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

DROP FUNCTION wdb_int.updateplacedefinition_mv() CASCADE;
DROP FUNCTION wdb_int.updateplacespec_mv() CASCADE;
DROP FUNCTION wdb_int.updateregulargrid_mv() CASCADE;


CREATE OR REPLACE FUNCTION wdb_int.updateplacespec() 
RETURNS TRIGGER
AS 
$BODY$
BEGIN
	DELETE FROM wci_int.placespec WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placespec (SELECT * FROM wci_int.placespec_v WHERE placeid=NEW.placeid);
	DELETE FROM wci_int.placedefinition_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placedefinition_mv (SELECT * FROM wci_int.placedefinition WHERE placeid=NEW.placeid);
	DELETE FROM wci_int.placeregulargrid_mv WHERE placeid=NEW.placeid;
	INSERT INTO wci_int.placeregulargrid_mv (SELECT * FROM wci_int.placeregulargrid WHERE placeid=NEW.placeid);
	RETURN NULL;
END
$BODY$
SECURITY DEFINER LANGUAGE 'plpgsql';
CREATE OR REPLACE FUNCTION wdb_int.updateplacespec_delete() 
RETURNS TRIGGER
AS 
$BODY$
BEGIN
	DELETE FROM wci_int.placespec WHERE placeid=OLD.placeid;
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
	PERFORM wdb_int.refreshMV('wci_int.placespec');
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
