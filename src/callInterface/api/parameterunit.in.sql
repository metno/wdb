-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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

CREATE TYPE wci.infoparameterunit AS 
(
	unitname						text,
	unittype						text,
	siunitconversioncoefficient		float,
	siunitconversionterm			float
);

CREATE OR REPLACE FUNCTION 
wci.info(
	parameterunit	text,
	returnType		wci.infoparameterunit	
)
RETURNS SETOF wci.infoparameterunit AS
$BODY$
	SELECT *
	FROM __WCI_SCHEMA__.unitwithconversion
	WHERE $1 = unitname;
$BODY$
SECURITY DEFINER
LANGUAGE 'sql';
