SELECT wci.begin('wcitest');

SELECT * FROM wci.read(
	NULL,      		-- dataprovider
	NULL, 	   		-- location
	NULL,      		-- reference time
	NULL,      		-- valid time
	NULL,      		-- parameter
	NULL,      		-- level
	NULL,      		-- data version
	0::returnoid  	-- return type
);

-- Do NOT do this (and preferably not the above either):
--SELECT * FROM wci.read(
--	NULL,      		-- dataprovider
--	NULL,			-- location
--	NULL,      		-- reference time
--	NULL,      		-- valid time
--	NULL,      		-- parameter
--	NULL,      		-- level
--	NULL,      		-- data version
--	0::returnfloat  -- return type
--);


--SELECT wci.end();
