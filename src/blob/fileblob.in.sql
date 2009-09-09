CREATE TABLE __WDB_SCHEMA__.file_blob (
	file_id bigserial PRIMARY KEY NOT NULL,
	active boolean NOT NULL
);
REVOKE ALL ON __WDB_SCHEMA__.file_blob FROM PUBLIC;
GRANT ALL ON __WDB_SCHEMA__.file_blob TO wdb_admin;
GRANT SELECT, UPDATE, DELETE ON __WDB_SCHEMA__.file_blob TO wdb_clean;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.reserve_file_id()
RETURNS bigint AS
$BODY$
	SELECT nextval('__WDB_SCHEMA__.file_blob_file_id_seq');
$BODY$
LANGUAGE SQL
SECURITY DEFINER;

CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.write_file(gridId bigint, data bytea)
RETURNS void AS
$BODY$
BEGIN
	-- Multiple attempts to write data with the same id as handled by the 
	-- primary key of __WDB_SCHEMA__.file_blob. (It will fail.)
	 
	INSERT INTO __WDB_SCHEMA__.file_blob VALUES (gridId, true);	
	PERFORM __WDB_SCHEMA__.write_file_impl(gridId, data);
END;
$BODY$
LANGUAGE plpgsql VOLATILE
SECURITY DEFINER;


-- deprecated
CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.write_file__(data bytea)
RETURNS bigint AS
$BODY$
DECLARE
	gridId bigint := nextval('__WDB_SCHEMA__.file_blob_file_id_seq');
BEGIN
	INSERT INTO __WDB_SCHEMA__.file_blob VALUES (gridId, true);
	PERFORM __WDB_SCHEMA__.write_file_impl(gridId, data);
	RETURN gridId;
END;
$BODY$
LANGUAGE plpgsql VOLATILE
SECURITY DEFINER;


CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.drop_file( id bigint )
RETURNS void AS
$BODY$
	UPDATE __WDB_SCHEMA__.file_blob SET active=false WHERE file_id=$1;
$BODY$
LANGUAGE sql VOLATILE;


CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.vacuum_file_blob()
RETURNS void AS
$BODY$
DECLARE
	entry __WDB_SCHEMA__.file_blob;
BEGIN
	FOR entry IN SELECT * FROM __WDB_SCHEMA__.file_blob WHERE active=false LOOP
		PERFORM __WDB_SCHEMA__.drop_file_impl(entry.file_id);
	END LOOP;
	DELETE FROM __WDB_SCHEMA__.file_blob WHERE active=false;
END;
$BODY$
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.read_file(id bigint)
RETURNS bytea AS
$BODY$
DECLARE
	fileExists bool := false;
BEGIN
	SELECT active INTO fileExists FROM __WDB_SCHEMA__.file_blob WHERE file_id=id;
	IF NOT FOUND OR fileExists=false THEN
		RAISE EXCEPTION 'No such file: %', id;
	END IF;
	RETURN __WDB_SCHEMA__.read_file_impl(id);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.read_float_from_file(id bigint, pos int)
RETURNS real AS
$BODY$
DECLARE
	fileExists bool := false;
BEGIN
	SELECT active INTO fileExists FROM __WDB_SCHEMA__.file_blob WHERE file_id=id;
	IF NOT FOUND OR fileExists=false THEN
		RAISE EXCEPTION 'No such file: %', id;
	END IF;
	RETURN __WDB_SCHEMA__.read_float_from_file_impl(id, pos);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;


CREATE OR REPLACE FUNCTION __WCI_SCHEMA__.exists_file(id bigint)
RETURNS bool AS
$BODY$
DECLARE
	data bytea;
BEGIN
	SELECT __WDB_SCHEMA__.read_file_impl(id) INTO data;
	RETURN true;
EXCEPTION
	WHEN raise_exception THEN
		RETURN FALSE;
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.write_file_impl(id bigint, data bytea)
RETURNS void AS
'__WDB_LIBDIR__/__WCI_LIB__', 'write_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.drop_file_impl(id bigint)
RETURNS void AS
'__WDB_LIBDIR__/__WCI_LIB__', 'drop_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.remove_unreferenced_files()
RETURNS int AS
'__WDB_LIBDIR__/__WCI_LIB__'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.read_file_impl(id bigint)
RETURNS bytea AS
'__WDB_LIBDIR__/__WCI_LIB__', 'read_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.read_float_from_file_impl(id bigint, pos int)
RETURNS real AS
'__WDB_LIBDIR__/__WCI_LIB__', 'read_float_from_file'
LANGUAGE C VOLATILE;
	