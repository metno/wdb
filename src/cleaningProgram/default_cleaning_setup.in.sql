BEGIN;

INSERT INTO clean.strategies VALUES(1, 'clean_referencetimes');
INSERT INTO clean.referencetime_lifetime VALUES (NULL, '1 day');
INSERT INTO clean.referencetime_lifetime VALUES ('statkart.no', '10000 years');

END;