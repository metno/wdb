-- Various parameter specifications:

SELECT wci.begin('wcitest',999,999,0);

-- Get all parameters related to pressure (ie. the name contains the word pressure)
SELECT * FROM wci.read(
	ARRAY['test wci 5'],			-- dataprovider
	NULL,--'hirlam 10 grid',		-- location
	'2009-11-13 00:00:00+00',		-- reference time
	'2009-11-13 18:00:00+00',		-- valid time
	'{"%pressure%"}',			-- parameter
	NULL,					-- level, in meter
	ARRAY[-1],				-- data version (-1 is latest)
	NULL::wci.returngid			-- return type
);


-- Get all accumulated values
SELECT * FROM wci.read(
	ARRAY['test wci 5'],			-- dataprovider
	NULL,--'hirlam 10 grid',		-- location
	'2009-11-13 00:00:00+00',		-- reference time
	'inside 2009-11-13 00:00:00+00 FOR 18 hours',	-- valid time
	'{"precipitation %"}',			-- parameter
	NULL,					-- level, in meter
	ARRAY[-1],				-- data version (-1 is latest)
	NULL::wci.returngid			-- return type
);

--SELECT wci.end();
