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

CREATE TYPE wci.returnFloat AS (
	value float,
	dataprovidername varchar( 255 ),
	placename text,
	placegeometry text,
	referencetime timestamp with time zone,
	validtimefrom timestamp with time zone,
	validtimeto timestamp with time zone,
	validtimeindeterminatecode integer,
	valueparametername varchar ( 255 ),
	valueparameterunit varchar( 80 ),
	levelparametername varchar( 255 ),
	levelunitname varchar( 80 ),
	levelfrom real,
	levelto real,
	levelindeterminatecode integer,
	dataversion integer,
	confidencecode integer,
	storetime timestamp with time zone,
	valueid bigint,
	valuetype varchar( 80 )
);

CREATE TYPE wci.returnGid AS (
	value bigint,
	dataprovidername varchar( 255 ),
	placename text,
	placegeometry text,
	referencetime timestamp with time zone,
	validtimefrom timestamp with time zone,
	validtimeto timestamp with time zone,
	validtimeindeterminatecode integer,
	valueparametername varchar ( 255 ),
	valueparameterunit varchar( 80 ),
	levelparametername varchar( 255 ),
	levelunitname varchar( 80 ),
	levelfrom real,
	levelto real,
	levelindeterminatecode integer,
	dataversion integer,
	confidencecode integer,
	storetime timestamp with time zone,
	valueid bigint,
	valuetype varchar( 80 )
);
