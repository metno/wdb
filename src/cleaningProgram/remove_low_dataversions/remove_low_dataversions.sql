BEGIN;

CREATE SCHEMA clean_dataversions;
REVOKE ALL ON SCHEMA clean_dataversions FROM PUBLIC;
GRANT ALL ON SCHEMA clean_dataversions TO wdb_admin;
GRANT ALL ON SCHEMA clean_dataversions TO wdb_clean;

CREATE OR REPLACE FUNCTION clean_dataversions.clean()
RETURNS void AS
$BODY$
DECLARE
	highest_dataversion int;
	current_version int := -1;
	versions_to_delete int[];
BEGIN
	SELECT max(dataversion) INTO highest_dataversion FROM wci.browse(NULL::wci.browsedataversion);

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