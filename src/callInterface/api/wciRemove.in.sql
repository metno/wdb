--BEGIN;

CREATE TYPE remove_stats AS (grids_removed integer, floats_removed integer);

CREATE OR REPLACE FUNCTION 
wci.remove( dataprovider_ 		text[],
		  location_ 			text,
		  referencetime_ 	text,
		  validtime_ 		text,
		  parameter_ 		text[],
		  level_ 			text,
		  dataversion_ 		integer[]
)	
RETURNS remove_stats AS
$BODY$
DECLARE
	ret remove_stats;
BEGIN
	DELETE FROM 
		wdb_int.gridvalue 
	WHERE
		valueid in (
			SELECT valueid FROM wci.read(
				dataprovider_, location_, 
				referencetime_, validtime_, 
				parameter_, level_, 
				dataversion_, NULL::wci.returngid)
		);
	GET DIAGNOSTICS ret.grids_removed = ROW_COUNT;
	
	DELETE FROM 
		wdb_int.floatvalue 
	WHERE
		valueid in (
			SELECT valueid FROM wci.read(
				dataprovider_, location_, 
				referencetime_, validtime_, 
				parameter_, level_, 
				dataversion_, NULL::wci.returnfloat)
		);
	GET DIAGNOSTICS ret.floats_removed = ROW_COUNT;
		
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql VOLATILE
SECURITY INVOKER;



--CREATE OR REPLACE FUNCTION
--remove_blobs()
--RETURNS trigger AS
--$BODY$
--DECLARE
--	noOfRows integer;	
--BEGIN
--	SELECT count(*) INTO noOfRows FROM wdb_int.gridvalue WHERE value=OLD.value;
--	IF noOfRows = 0 THEN
--		BEGIN
--			PERFORM wdb_int.drop_file(OLD.value);
--			--RAISE INFO 'Dropped %', OLD.value;
--		END;
--	END IF;
--	RETURN NULL;
--END;
--$BODY$
--LANGUAGE plpgsql;
--
--CREATE CONSTRAINT TRIGGER remove_file_on_delete
--AFTER DELETE ON wdb_int.gridvalue
--DEFERRABLE INITIALLY DEFERRED
--FOR EACH ROW
--EXECUTE PROCEDURE remove_blobs();

--COMMIT;
