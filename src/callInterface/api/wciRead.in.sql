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
	placegeometry GEOMETRY,
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
	placegeometry GEOMETRY,
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


-- Output Large Objects
CREATE OR REPLACE FUNCTION 
wci.read( dataprovider 		text[],
		  location 			text,
		  referencetime 	text,
		  validtime 		text,
		  parameter 		text[],
		  level 			text,
		  dataversion 		integer[],
		  returntype 		wci.returnGid
)	
RETURNS SETOF wci.returngid AS
$BODY$
DECLARE
	readQ	 		text;
	entry 			__WCI_SCHEMA__.gridValue;
	returnObject 	wci.returngid;
BEGIN
	readQ := 'SELECT * ' ||
		-- Create Query to Run
			  __WCI_SCHEMA__.readQuery( 0,
										dataprovider,
										__WCI_SCHEMA__.getPlaceQuery(location, 0),
										__WCI_SCHEMA__.getTimeSpec(referencetime),
										__WCI_SCHEMA__.getTimeSpec(validtime),
										parameter,
										__WCI_SCHEMA__.getLevelSpec(level),
										dataversion );
	RAISE DEBUG 'WCI.READ.Query: %', readQ;

	<<main_select>>
	FOR entry IN
		EXECUTE readQ
	LOOP
			returnObject := (
					entry.value,
					entry.dataprovidername,
					entry.placename,
					entry.placegeometry,
					entry.referencetime,
					entry.validtimefrom,
					entry.validtimeto,
					entry.validtimeindeterminatecode,
					entry.valueparametername,
					entry.valueunitname,
					entry.levelparametername,
					entry.levelunitname,
					entry.levelFrom,
					entry.levelTo,
					entry.levelindeterminatecode,
					entry.dataversion,
					entry.confidencecode, 
					entry.valuestoretime,
					entry.valueid,
					entry.valuetype );
		RETURN NEXT returnObject;
	END LOOP main_select;
END;
$BODY$
LANGUAGE 'plpgsql' STABLE;



-- Output Floating Points/Integers
CREATE OR REPLACE FUNCTION 
wci.read( dataprovider 		text[],
		  location 			text,
		  referencetime 	text,
		  validtime 		text,
		  parameter 		text[],
		  level 			text,
		  dataversion 		integer[],
		  returntype 		wci.returnFloat
)
RETURNS SETOF wci.returnFloat AS
$BODY$
DECLARE
	-- Values
	entryF 			__WCI_SCHEMA__.floatValue;
	entryO 			__WCI_SCHEMA__.gridValue;
	pointData 		__WCI_SCHEMA__.extractGridDataReturnType;
	returnObject 	wci.returnFloat;
	-- Geometry
	loc 			__WCI_SCHEMA__.location;
	gLocation 		GEOMETRY;
	interpolation 	__WCI_SCHEMA__.interpolationType;

	readQ	 		text;
	returnPlaceName	text;
BEGIN
	-- Location
	IF location IS NOT NULL THEN
		-- Decode Location Information
		loc := __WCI_SCHEMA__.getLocation(location);
		gLocation := __WCI_SCHEMA__.getGeometry(loc);
		-- Bail out if we cannot handle this geometry type
		PERFORM __WCI_SCHEMA__.verifyGeometry(gLocation);
		-- IF char_length(loc.interpolation) <= 1 THEN
		IF loc.interpolation = '' THEN
			interpolation := 'nearest';
		ELSE
			interpolation := loc.interpolation;
		END IF;
		RAISE DEBUG 'WCI.READ.Location: %, %, %, %', 
			loc.interpolation, loc.location, loc.isgeometry, astext(gLocation);
	ELSE
		gLocation := NULL;
	END IF;
	--
	-- Return Point Values
	--
	IF __WCI_SCHEMA__.hasPoint( dataprovider ) = TRUE THEN
		-- Create Query to Run
		readQ := 'SELECT * ' ||
				 __WCI_SCHEMA__.readQuery( 1, 
										   dataprovider,
										   __WCI_SCHEMA__.getPlaceQuery(location, 1),
										   __WCI_SCHEMA__.getTimeSpec(referencetime),
										   __WCI_SCHEMA__.getTimeSpec(validtime),
										   parameter,
										   __WCI_SCHEMA__.getLevelSpec(level),
										   dataversion );
		RAISE DEBUG 'WCI.READ.Query: %', readQ;
		<<float_select>>
		FOR entryF IN
			EXECUTE readQ
		LOOP
			returnObject := (
					entryF.value,
					entryF.dataprovidername,
					entryF.placename,
					entryF.placegeometry,
					entryF.referencetime,
					entryF.validtimefrom,
					entryF.validtimeto,
					entryF.validtimeindeterminatecode,
					entryF.valueparametername,
					entryF.valueunitname,
					entryF.levelparametername,
					entryF.levelunitname,
					entryF.levelFrom,
					entryF.levelTo,
					entryF.levelindeterminatecode,
					entryF.dataversion,
					entryF.confidencecode, 
					entryF.valuestoretime,
					entryF.valueid,
					entryF.valuetype );
			RETURN NEXT returnObject;
		END LOOP float_select;
	END IF;

	--
	-- Return Grid Values
	--
	IF ( ( __WCI_SCHEMA__.hasGrid( dataprovider ) = TRUE ) AND
	   ( location IS NOT NULL ) ) THEN
		-- Create Query to Run
		readQ := 'SELECT * ' ||
				 __WCI_SCHEMA__.readQuery( 0, 
										   dataprovider,
										   __WCI_SCHEMA__.getPlaceQuery(location, 2),
										   __WCI_SCHEMA__.getTimeSpec(referencetime),
										   __WCI_SCHEMA__.getTimeSpec(validtime),
										   parameter,
										   __WCI_SCHEMA__.getLevelSpec(level),
										   dataversion );
		RAISE DEBUG 'WCI.READ.Query: %', readQ;
		<<grid_select>>
		FOR entryO IN
			EXECUTE readQ
		LOOP
			--RAISE DEBUG 'WCI.READ.FloatResult: %, %, %, %', astext(gLocation), interpolation, entryO.placeid, entryO.valueid;
			<<extract_points>>
			FOR pointData IN SELECT * FROM __WCI_SCHEMA__.extractGridData( entryO.placeid, gLocation, interpolation, entryO.value ) LOOP
				returnPlaceName := loc.location || ' ' || entryO.placename; 
				returnObject := ( 
						pointData.value, 
						entryO.dataprovidername, 
						returnPlaceName,
						pointData.location,
						entryO.referencetime,
						entryO.validtimefrom,
						entryO.validtimeto, 
						entryO.validtimeindeterminatecode, 
						entryO.valueparametername,
						entryO.valueunitname, 
						entryO.levelparametername,
						entryO.levelunitname,
						entryO.levelFrom, 
						entryO.levelTo,
						entryO.levelindeterminatecode, 
						entryO.dataversion,
						entryO.confidencecode, 
						entryO.valuestoretime, 
						entryO.valueid,
						entryO.valuetype );
				RETURN NEXT returnObject;
			END LOOP extract_points;
		END LOOP grid_select;
	END IF;

END;
$BODY$
LANGUAGE 'plpgsql' STABLE;
