BEGIN;

CREATE FUNCTION add_to_referencetime_lifetime_table(dataprovider text, timetolive interval)
RETURNS void AS
$BODY$
	INSERT INTO clean.referencetime_lifetime (dataprovidername, oldest_lifetime) 
	SELECT $1, $2 WHERE NOT EXISTS 
	(SELECT dataprovidername FROM clean.referencetime_lifetime WHERE dataprovidername=$1);
$BODY$
LANGUAGE sql;

INSERT INTO clean.referencetime_lifetime (dataprovidername, oldest_lifetime) 
SELECT NULL, '1 day' WHERE NOT EXISTS 
(SELECT dataprovidername FROM clean.referencetime_lifetime WHERE dataprovidername IS NULL);

SELECT add_to_referencetime_lifetime_table('statkart.no', '1000 years');
SELECT add_to_referencetime_lifetime_table('norsk_polarinstitutt', '1000 years');

INSERT INTO clean.strategies (function) 
SELECT 'clean_referencetimes' WHERE NOT EXISTS
(SELECT function FROM clean.strategies WHERE function='clean_referencetimes'); 


DROP FUNCTION add_to_referencetime_lifetime_table(text, interval);


END;
