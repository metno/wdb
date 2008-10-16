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

CREATE TYPE wci.browsedataprovider AS 
(
	dataprovidername	text,
	referencetimefrom 	timestamp with time zone,
	referencetimeto 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browseplace AS 
(
	placename			text,
	referencetimefrom 	timestamp with time zone,
	referencetimeto 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsereferencetime AS 
(
	referencetime	 	timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsevalidtime AS 
(
	validtimefrom 		timestamp with time zone,
	validtimeto	 		timestamp with time zone,
	numberoftuples 		integer
);

CREATE TYPE wci.browsevalueparameter AS 
(
	valueparametername	text,
	valueunitname		text,
	numberoftuples 		integer
);

CREATE TYPE wci.browselevelparameter AS 
(
	levelparametername	text,
	levelunitname		text,
	levelfrom		 	real,
	levelto			 	real,
	numberoftuples 		integer
);
