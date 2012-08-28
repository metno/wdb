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


-- Unit and Unit Conversions
CREATE VIEW __WCI_SCHEMA__.unitwithconversion_v AS
SELECT 	un.unitname, 
	   	un.unittype,
	   	un.description,
	   	co.baseunitname,
	   	co.baseunitconversioncoefficient,
	   	co.baseunitconversionterm
FROM	__WDB_SCHEMA__.unit un,
		__WDB_SCHEMA__.baseunitconversion co
WHERE	un.unitname = co.unitname
UNION ALL
SELECT 	un.unitname, 
	   	un.unittype,
	   	un.description,
	   	un.unitname AS baseunitname,
	   	1.0 AS baseunitconversioncoefficient,
	   	0.0 AS baseunitconversionterm
FROM  	__WDB_SCHEMA__.unit un
WHERE 	NOT EXISTS ( SELECT * 
					 FROM 	__WDB_SCHEMA__.baseunitconversion co
					 WHERE	un.unitname = co.unitname );

REVOKE ALL ON __WCI_SCHEMA__.unitwithconversion_v FROM public;
GRANT ALL ON __WCI_SCHEMA__.unitwithconversion_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.unitwithconversion_v TO wdb_felt;
GRANT SELECT ON __WCI_SCHEMA__.unitwithconversion_v TO wdb_grib;



CREATE VIEW __WCI_SCHEMA__.unit_v AS
	SELECT  
		unitname,
		unittype,
		description
	FROM 	
		__WDB_SCHEMA__.unit;
		
REVOKE ALL ON __WCI_SCHEMA__.unit_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.unit_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.unit_v TO wdb_read;
GRANT SELECT ON __WCI_SCHEMA__.unit_v TO wdb_write;
