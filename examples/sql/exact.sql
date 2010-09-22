-- Get pressure between 0 and 0 metre above ground level

SELECT wci.begin('wcitest', 999, 999, 0);

-- Get the oid for a specific field
SELECT * FROM wci.read(
	ARRAY['test wci 5'],			-- dataprovider
	NULL,--'hirlam 10 grid',		-- location
	'2009-11-13 00:00:00+00',		-- reference time
	'2009-11-13 18:00:00+00',		-- valid time
	'{"air pressure"}',			-- parameter
	'0 height above ground distance',	-- level, in meter
	ARRAY[-1],				-- data version (-1 is latest)
	NULL::wci.returngid			-- return type
);

-- Get value for for the town of Bodø (14.405, 67.28).
SELECT * FROM wci.read(
	ARRAY['test wci 5'],			-- dataprovider
	'POINT(14.405 67.28)',			-- location (note: long lat)
	'2009-11-13 00:00:00+00',		-- reference time
	'2009-11-13 18:00:00+00',		-- valid time
	'{"air pressure"}',			-- parameter
	'0 height above ground distance',	-- level, in meter
	ARRAY[-1],				-- data version (-1 is latest)
	NULL::wci.returnfloat			-- return type
);

--SELECT wci.end();
