BEGIN;

SELECT wci.begin('wcitestwriter', 999,999,999);

-- Grid. Many versions
SELECT wci.write('aaaabbbbccccdddd'::bytea, 'test grid, rotated', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);
SELECT wci.write('aaaabbbbccccdddd'::bytea, 'test grid, rotated', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);
SELECT wci.write('aaaabbbbccccdddd'::bytea, 'test grid, rotated', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);

-- Grid. Only one version
SELECT wci.write('aaaabbbbccccdddd'::bytea, 'test grid, rotated', 'yesterday', 'yesterday', 'yesterday', 'air temperature', 'height above ground', 2, 2);

-- Point. Many versions
SELECT wci.write(21.2, 'oslo', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);
SELECT wci.write(22.2, 'oslo', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);
SELECT wci.write(23.2, 'oslo', 'today', 'today', 'today', 'air temperature', 'height above ground', 2, 2);

SELECT wci.write(17.3, 'oslo', 'yesterday', 'yesterday', 'yesterday', 'air temperature', 'height above ground', 2, 2);


CREATE OR REPLACE FUNCTION
assert_result_count(returntype anyelement, expected int, message text)
RETURNS void AS
$BODY$
DECLARE
	rows int;
BEGIN
	SELECT count(*) INTO rows FROM wci.read(NULL,NULL, NULL,NULL, NULL,NULL, NULL,returntype);
	IF rows != expected THEN
		RAISE EXCEPTION 'Fond % rows, expected %: %', rows, expected, message;
	END IF;
END;
$BODY$
LANGUAGE plpgsql;


SAVEPOINT setup_complete;



-- Correct number of rows left in normal case - grids
SELECT clean.clean_dataversions();
SELECT assert_result_count(NULL::wci.returngid, 2, 'Wrong number of grid rows left after clean');
SELECT assert_result_count(NULL::wci.returnfloat, 2, 'Wrong number of float rows left after clean');
ROLLBACK TO SAVEPOINT setup_complete;

-- Correct number of rows left when dataversions in middle was deleted
SELECT wci.remove(NULL,NULL, NULL,NULL, NULL,NULL, '{-2}');
SELECT assert_result_count(NULL::wci.returngid, 3, 'grid test precondition error');
SELECT assert_result_count(NULL::wci.returnfloat, 3, 'grid test precondition error');
SELECT clean.clean_dataversions();
SELECT assert_result_count(NULL::wci.returngid, 2, 'Wrong number of grid rows left after clean - after a random row was manually removed');
SELECT assert_result_count(NULL::wci.returnfloat, 2, 'Wrong number of float rows left after clean - after a random row was manually removed');
ROLLBACK TO SAVEPOINT setup_complete;

 


ROLLBACK;
