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
\pset format unaligned
set time zone 'UTC';
SELECT 
	dataproviderid, 
	placeid, 
	referencetime, 
	validtimefrom, 
	validtimeto, 
	validtimeindeterminatecode, 
	valueparametername, 
	valueunitname,
	levelparametername, 
	levelunitname, 
	levelfrom, 
	levelto, 
	levelindeterminatecode, 
	dataversion, 
	confidencecode
FROM test.oidvalue 
WHERE dataproviderid=1096 AND
      (referenceTime='2006-12-05T12:00:00+00' OR
	  referenceTime='1919-01-01T01:01:00 UTC')
ORDER BY 
		dataproviderid, 
		placeid, 
		referencetime, 
		validtimefrom, 
		validtimeto, 
		valueparametername, 
		valueunitname, 
		levelparametername, 
		levelunitname, 
		levelfrom, 
		levelto,
		validtimeindeterminatecode, 
		dataversion, 
		confidencecode;
--- valueid, valuedomain, spatialdomain, value, valuestoretime are ignored as they may vary.
