--BEGIN;
DROP TYPE IF EXISTS remove_stats CASCADE;
CREATE TYPE remove_stats AS (grids_removed integer, floats_removed integer);

CREATE OR REPLACE FUNCTION 
wci.remove( dataprovider_ 	text[],
		  location_ 		text,
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
		wdb_int.floatvalueitem 
	WHERE
		valuegroupid in (
			SELECT distinct valueid 
			FROM wci.read(
				dataprovider_, location_, 
				referencetime_, validtime_, 
				parameter_, level_, 
				dataversion_, NULL::wci.returnfloat)
		) AND
		referencetime = referencetime_::timestamp with time zone;
	GET DIAGNOSTICS ret.floats_removed = ROW_COUNT;
	ret.floats_removed = 0;
		
	RETURN ret;
END;
$BODY$
LANGUAGE plpgsql VOLATILE
SECURITY INVOKER;
