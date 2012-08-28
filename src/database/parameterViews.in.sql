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


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getCanonicalParameterName( 
				  cfstandardname_		text,
				  cfsurface_ 			text,
				  cfcomponent_			text,
				  cfmedium_				text,
				  cfprocess_			text,
				  cfcondition_			text,
				  cfmethods_			text )	
RETURNS text AS
$BODY$
DECLARE
	parametername 		text;
BEGIN
	parametername := cfstandardname_;	
	IF cfcomponent_ IS NOT NULL then
		parametername := cfcomponent_ || ' ' || parametername;  
	END IF;
	IF cfsurface_ IS NOT NULL then
		IF ( position( ' ' IN cfsurface_ ) < 1 ) THEN
			parametername := cfsurface_ || ' ' || parametername;
		ELSE
			parametername := parametername || ' at ' || cfsurface_;
		END IF;			
	END IF;
	IF cfmedium_ IS NOT NULL then
		parametername := parametername || ' in ' || cfmedium_;  
	END IF;
	IF cfprocess_ IS NOT NULL then
		parametername := parametername || ' due to ' || cfprocess_;  
	END IF;
	IF cfcondition_ IS NOT NULL then
		parametername := parametername || ' assuming ' || cfcondition_;  
	END IF;
	IF cfmethods_ IS NOT NULL then
		parametername := parametername || ' [' || cfmethods_ || ']';  
	END IF;
	RETURN lower(parametername);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getParameterName( 
				  cfstandardname_		text,
				  cfsurface_ 			text,
				  cfcomponent_			text,
				  cfmedium_				text,
				  cfprocess_			text,
				  cfcondition_			text,
				  cfmethods_			text )	
RETURNS text AS
$BODY$
DECLARE
	parametername 		text;
	functionname 		text;
BEGIN
	parametername := cfstandardname_;	
	IF cfcomponent_ IS NOT NULL then
		parametername := cfcomponent_ || ' ' || parametername;  
	END IF;
	IF cfsurface_ IS NOT NULL then
		IF ( position( ' ' IN cfsurface_ ) < 1 ) THEN
			parametername := cfsurface_ || ' ' || parametername;
		ELSE
			parametername := parametername || ' at ' || cfsurface_;
		END IF;			
	END IF;
	IF cfmethods_ IS NOT NULL then
		SELECT cfmethodsname INTO functionname
		FROM   __WCI_SCHEMA__.cfmethods_v
		WHERE  cfmethods = cfmethods_;
		IF NOT FOUND THEN
			RAISE EXCEPTION 'Could not identify the cfmethods description %', cfmethods_;
		END IF;
		parametername := functionname || ' ' || parametername;  
	END IF;
	IF cfmedium_ IS NOT NULL then
		parametername := parametername || ' in ' || cfmedium_;  
	END IF;
	IF cfprocess_ IS NOT NULL then
		parametername := parametername || ' due to ' || cfprocess_;  
	END IF;
	IF cfcondition_ IS NOT NULL then
		parametername := parametername || ' assuming ' || cfcondition_;  
	END IF;
	RETURN lower(parametername);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



CREATE VIEW __WCI_SCHEMA__.parameter_v AS
SELECT
	v.parameterid,
  	v.cfstandardname,
 	v.cfsurface,
    v.cfcomponent,
    v.cfmedium,
    v.cfprocess,
    v.cfcondition,
    v.cfmethods,
    v.unitname,
	n.parameternamespaceid,
	n.parametername
FROM
	__WDB_SCHEMA__.parameter AS v, 
	__WDB_SCHEMA__.parametername AS n
WHERE 	v.parameterid = n.parameterid
UNION ALL
SELECT
	v.parameterid,
  	v.cfstandardname,
 	v.cfsurface,
    v.cfcomponent,
    v.cfmedium,
    v.cfprocess,
    v.cfcondition,
    v.cfmethods,
    v.unitname,
	0 AS parameternamespaceid,
	__WCI_SCHEMA__.getCanonicalParameterName( v.cfstandardname, v.cfsurface, v.cfcomponent, 
			v.cfmedium, v.cfprocess, v.cfcondition, v.cfmethods ) AS parametername
FROM
	__WDB_SCHEMA__.parameter AS v;
  
REVOKE ALL ON __WCI_SCHEMA__.parameter_v FROM PUBLIC;
GRANT ALL ON __WCI_SCHEMA__.parameter_v TO wdb_admin;
GRANT SELECT ON __WCI_SCHEMA__.parameter_v TO wdb_read, wdb_write;



-- Materialized View
SELECT __WDB_SCHEMA__.createMV('__WCI_SCHEMA__.parameter_mv', '__WCI_SCHEMA__.parameter_v');
SELECT __WDB_SCHEMA__.refreshMV('__WCI_SCHEMA__.parameter_mv');

REVOKE ALL ON TABLE __WCI_SCHEMA__.parameter_mv FROM PUBLIC;
GRANT ALL ON TABLE __WCI_SCHEMA__.parameter_mv TO wdb_admin;
GRANT SELECT ON TABLE __WCI_SCHEMA__.parameter_mv TO wdb_read, wdb_write;

CREATE INDEX XIE0wci_parameter_mv ON __WCI_SCHEMA__.parameter_mv
(
    parameterid,
	parametername,
	parameternamespaceid
);
