-- Various parameter specifications:

SELECT wci.begin('wcitest');

-- Get all parameters related to pressure
SELECT * FROM wci.read(
	NULL,      					-- dataprovider
	'hirlam 10 grid',			-- location (hirlam 10 area)
	('today','today','exact'),	-- reference time
	('today', 'today', 'exact'),-- valid time
	'{"* pressure of *"}',		-- parameter
	NULL,						-- level
	NULL,						-- data version (Currently not in use)
	0::returnoid				-- return type
);


-- Get all instant parameters related to air (but not 'air (water content)' and such)
SELECT * FROM wci.read(
	NULL,      					-- dataprovider
	'hirlam 10 grid',			-- location (hirlam 10 area)
	('today','today','exact'),	-- reference time
	('today', 'today', 'exact'),-- valid time
	'{"instant * of air"}',		-- parameter
	NULL,						-- level
	NULL,						-- data version (Currently not in use)
	0::returnoid				-- return type
);


-- Get all precipitation
SELECT * FROM wci.read(
	NULL,      					-- dataprovider
	'hirlam 10 grid',			-- location (hirlam 10 area)
	('today','today','exact'),	-- reference time
	('today', 'today', 'exact'),-- valid time
	'{"accumulated surface density of air (precipitation)"}',	-- parameter
	NULL,						-- level
	NULL,						-- data version (Currently not in use)
	0::returnoid				-- return type
);



--SELECT wci.end();
