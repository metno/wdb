BEGIN;

CREATE OR REPLACE FUNCTION clean.clean_dataversions()
RETURNS void AS
$BODY$
DECLARE
	highest_dataversion int := 0;
	current_version int := -1;
	versions_to_delete int[];
BEGIN
	SELECT max(dataversion) INTO highest_dataversion FROM wci.browse(NULL::wci.browsedataversion);

	IF highest_dataversion IS NULL THEN
		RETURN;
	END IF;
	
	LOOP
		IF current_version < -highest_dataversion THEN
			EXIT;
		END IF;
		current_version = current_version -1;
		versions_to_delete = versions_to_delete || current_version;
	END LOOP;
	
	PERFORM wci.remove(NULL,NULL, NULL,NULL, NULL,NULL, versions_to_delete);
END;
$BODY$
LANGUAGE plpgsql;



COMMIT;