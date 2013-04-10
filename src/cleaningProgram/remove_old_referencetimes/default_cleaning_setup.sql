BEGIN;

CREATE FUNCTION
setup_referencetime_cleanig()
RETURNS void AS
$BODY$
DECLARE
	rows int;
BEGIN
	SELECT count(*) INTO rows FROM clean.referencetime_lifetime;
	IF rows = 0 THEN
		INSERT INTO clean.referencetime_lifetime VALUES (NULL, '1 day');
		INSERT INTO clean.referencetime_lifetime VALUES ('statkart.no', '1000 years');
	END IF;
END;
$BODY$
LANGUAGE plpgsql;
SELECT setup_referencetime_cleanig();

DROP FUNCTION setup_referencetime_cleanig();

INSERT INTO clean.strategies VALUES(DEFAULT, 'clean_referencetimes');

END;
