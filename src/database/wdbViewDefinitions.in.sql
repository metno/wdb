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


--
-- Convenience Views
--

CREATE VIEW __WDB_SCHEMA__.unitwithabbreviation AS
       SELECT un.unitname, 
			  un.unittype, 
			  un.measure, 
              ua.unitabbreviation
       FROM __WDB_SCHEMA__.unit un, 
			__WDB_SCHEMA__.unitabbreviation ua
       WHERE un.unitname = ua.unitname;

REVOKE ALL ON __WDB_SCHEMA__.unitwithabbreviation FROM public;
GRANT ALL ON __WDB_SCHEMA__.unitwithabbreviation TO wdb_admin;


CREATE VIEW __WDB_SCHEMA__.placespec AS SELECT 
	pd.placeid,
	pd.placegeometry, 
	pg.inumber, 
	pg.jnumber, 
	pg.iincrement, 
	pg.jincrement, 
	pg.startlongitude, 
	pg.startlatitude, 
	pg.originalsrid,
	srs.proj4text AS projdefinition 
FROM 
	__WDB_SCHEMA__.placedefinition pd,
	spatial_ref_sys srs, 
	__WDB_SCHEMA__.placeregulargrid pg
WHERE 
	((srs.srid = pg.originalsrid)
	AND (pd.placeid = pg.placeid));


