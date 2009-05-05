CREATE TABLE file_blob (
	file_id bigserial PRIMARY KEY NOT NULL,
	active boolean NOT NULL
);
REVOKE ALL ON file_blob FROM PUBLIC;
GRANT SELECT ON file_blob TO PUBLIC;

CREATE OR REPLACE FUNCTION write_file(data bytea)
RETURNS bigint AS
$BODY$
DECLARE
	gridId bigint := nextval('file_blob_file_id_seq');
BEGIN
	PERFORM write_file_impl(gridId, data);
	INSERT INTO file_blob VALUES (gridId, true);
	RETURN gridId;
END;
$BODY$
LANGUAGE plpgsql VOLATILE
SECURITY DEFINER;

CREATE OR REPLACE FUNCTION drop_file(id bigint)
RETURNS void AS
$BODY$
	UPDATE file_blob SET active=false WHERE file_id=$1;
$BODY$
LANGUAGE sql VOLATILE;

CREATE OR REPLACE FUNCTION vacuum_file_blob()
RETURNS void AS
$BODY$
DECLARE
	entry file_blob;
BEGIN
	FOR entry IN SELECT * FROM file_blob WHERE active=false LOOP
		PERFORM drop_file_impl(entry.file_id);
	END LOOP;
	DELETE FROM file_blob WHERE active=false;
END;
$BODY$
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION read_file(id bigint)
RETURNS bytea AS
$BODY$
DECLARE
	fileExists bool := false;
BEGIN
	SELECT active INTO fileExists FROM file_blob WHERE file_id=id;
	IF NOT FOUND OR fileExists=false THEN
		RAISE EXCEPTION 'No such file: %', id;
	END IF;
	RETURN read_file_impl(id);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;

CREATE OR REPLACE FUNCTION read_float_from_file(id bigint, pos int)
RETURNS real AS
$BODY$
DECLARE
	fileExists bool := false;
BEGIN
	SELECT active INTO fileExists FROM file_blob WHERE file_id=id;
	IF NOT FOUND OR fileExists=false THEN
		RAISE EXCEPTION 'No such file: %', id;
	END IF;
	RETURN read_float_from_file_impl(id, pos);
END;
$BODY$
SECURITY DEFINER
LANGUAGE plpgsql VOLATILE;



CREATE OR REPLACE FUNCTION write_file_impl(id bigint, data bytea)
RETURNS void AS
'__WDB_LIBDIR__/__WCI_LIB__', 'write_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION drop_file_impl(id bigint)
RETURNS void AS
'__WDB_LIBDIR__/__WCI_LIB__', 'drop_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION remove_unreferenced_files()
RETURNS int AS
'__WDB_LIBDIR__/__WCI_LIB__'
LANGUAGE C VOLATILE;



CREATE OR REPLACE FUNCTION read_file_impl(id bigint)
RETURNS bytea AS
'__WDB_LIBDIR__/__WCI_LIB__', 'read_file'
LANGUAGE C VOLATILE;

CREATE OR REPLACE FUNCTION read_float_from_file_impl(id bigint, pos int)
RETURNS real AS
'__WDB_LIBDIR__/__WCI_LIB__', 'read_float_from_file'
LANGUAGE C VOLATILE;
