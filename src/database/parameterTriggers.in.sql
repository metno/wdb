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
