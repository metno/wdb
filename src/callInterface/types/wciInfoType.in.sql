-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2008 met.no
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

CREATE TYPE wci.infodataprovider AS 
(
	dataprovidername		text,
	dataprovidertype		character varying(80),
	spatialdomaindelivery	character varying(80),
	dataprovidercomment		character varying(254),
	dataproviderstoretime 	timestamp with time zone
);

CREATE TYPE wci.infoplace AS 
(
	placename				text,
	placegeometry			text,
	placeindeterminatetype	character varying(80),
	placegeometrytype		character varying(80),
	placestoretime			timestamp with time zone
);

CREATE TYPE wci.inforegulargrid AS
(
	placename text,
	iNumber int,
	jNumber int,
	iIncrement real,
	jIncrement real,
	startLatitude real,
	startLongitude real,
	projDefinition text
);

CREATE TYPE wci.infovalueparameter AS 
(
	valueparametername	text,
	valueunitname		text
);

CREATE TYPE wci.infolevelparameter AS 
(
	levelparametername	text,
	levelunitname		text
);

