-- Get pressure between 0 and 0 metre above mean sea level at midnight today, as generated at midnight today.

SELECT wci.begin('wcitest');

-- Get the oid for a specific field
SELECT * FROM wci.read(
	ARRAY['hirlam'],      					-- dataprovider
	'hirlam 10 grid',					-- location
	('today', 'today', 'exact'),			-- reference time
	('today', 'today', 'exact'),			-- valid time
	'{"instant pressure of air"}',			-- parameter
	(0,0,'above mean sea level','exact'),	-- level
	NULL,									-- data version (Currently not in use)
	0::returnoid							-- return type
);

-- Get value for for Bodø (14.38, 67.28).
SELECT * FROM wci.read(
	ARRAY['hirlam'],      					-- dataprovider
	'near POINT(14.38 67.28)',				-- location (note: long lat)
	('today','today','exact'),				-- reference time
	('today', 'today', 'exact'),			-- valid time
	'{"instant pressure of air"}',			-- parameter
	(0,0,'above mean sea level','exact'),	-- level
	NULL,									-- data version (Currently not in use)
	0::returnfloat								-- return type
);

--SELECT wci.end();
