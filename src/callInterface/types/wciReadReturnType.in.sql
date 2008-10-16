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
	dataProviderName varchar( 255 ),
	placeName varchar( 255 ),
	placeGeometry GEOMETRY,
	referencetime timestamp with time zone,
	validFrom timestamp with time zone,
	validTo timestamp with time zone,
	valueParameterName varchar ( 255 ),
	valueParameterUnit varchar( 80 ),
	levelParameterName varchar( 255 ),
	levelUnitName varchar( 80 ),
	levelFrom real,
	levelTo real,
	dataVersion integer,
	confidenceCode integer,
	storetime timestamp with time zone,
	valueid bigint,
	valuetype varchar( 80 )
);


CREATE TYPE wci.returnOid AS (
	value oid,
	dataProviderName varchar( 255 ),
	placeName varchar( 255 ),
	placeGeometry GEOMETRY,
	referencetime timestamp with time zone,
	validFrom timestamp with time zone,
	validTo timestamp with time zone,
	valueParameterName varchar ( 255 ),
	valueParameterUnit varchar( 80 ),
	levelParameterName varchar( 255 ),
	levelUnitName varchar( 80 ),
	levelFrom real,
	levelTo real,
	dataVersion integer,
	confidenceCode integer,
	storetime timestamp with time zone,
	valueid bigint,
	valuetype varchar( 80 )
);

CREATE TYPE wci.readReturnTypeOid AS (
	value oid,
	dataProviderId integer,
	referencetime timestamp with time zone,
	placeId integer,
	position GEOMETRY,
	dataVersion integer,
	parameterUnit varchar( 80 ),
	parameter text,
	levelFrom integer,
	levelTo integer,
	levelUnitName varchar( 80 ),
	levelParameterName varchar( 80 ),
	validFrom timestamp with time zone,
	validTo timestamp with time zone,
	quality integer,
	storetime timestamp with time zone,
	valueid bigint
);

CREATE TYPE wci.readReturnTypeFloat AS (
	value real,
	dataProviderId integer,
	referencetime timestamp with time zone,
	placeId integer,
	position GEOMETRY,
	dataVersion integer,
	parameterUnit varchar( 80 ),
	parameter text,
	levelFrom integer,
	levelTo integer,
	levelUnitName varchar( 80 ),
	levelParameterName varchar( 80 ),
	validFrom timestamp with time zone,
	validTo timestamp with time zone,
	quality integer,
	storetime timestamp with time zone,
	valueid bigint
);
