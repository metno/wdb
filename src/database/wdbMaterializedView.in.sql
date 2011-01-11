-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2007 met.no
--
--  Contact information:
--  Norwegian Meteorological Institute
--  Box 43 Blindern
--  0313 OSLO
--  NORWAY
--  E-mail: wdb@met.no
--
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


-- The materializedView table contains a list of the materialized views
CREATE TABLE __WDB_SCHEMA__.materializedView (
	materializedViewName NAME NOT NULL PRIMARY KEY,
	viewName NAME NOT NULL,
	lastRefreshTime TIMESTAMP WITH TIME ZONE
);


CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.createMV(NAME, NAME)
RETURNS VOID 
SECURITY DEFINER
AS 
$BODY$
DECLARE
    matview ALIAS FOR $1;
    view_name ALIAS FOR $2;
    entry __WDB_SCHEMA__.materializedView%ROWTYPE;
BEGIN
    SELECT * INTO entry FROM __WDB_SCHEMA__.materializedView WHERE materializedViewName = matview;

    IF FOUND THEN
        RAISE EXCEPTION 'Materialized view % already exists.', matview;
    END IF;

    EXECUTE 'REVOKE ALL ON ' || view_name || ' FROM PUBLIC'; 

    EXECUTE 'GRANT SELECT ON ' || view_name || ' TO PUBLIC';

    EXECUTE 'CREATE TABLE ' || matview || ' AS SELECT * FROM ' || view_name;

    EXECUTE 'REVOKE ALL ON ' || matview || ' FROM PUBLIC';

    EXECUTE 'GRANT SELECT ON ' || matview || ' TO PUBLIC';

    INSERT INTO __WDB_SCHEMA__.materializedView (materializedViewName, viewName, lastRefreshTime)
      VALUES (matview, view_name, CURRENT_TIMESTAMP); 
    
    RETURN;
END
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.dropMV(NAME)
RETURNS VOID 
SECURITY DEFINER
AS 
$BODY$
DECLARE
    matview ALIAS FOR $1;
    entry __WDB_SCHEMA__.materializedView%ROWTYPE;
BEGIN

    SELECT * INTO entry FROM __WDB_SCHEMA__.materializedView WHERE materializedViewName = matview;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Materialized view % does not exist.', matview;
    END IF;

    EXECUTE 'DROP TABLE ' || matview;
    DELETE FROM __WDB_SCHEMA__.materializedView WHERE materializedViewName=matview;

    RETURN;
END
$BODY$
LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION __WDB_SCHEMA__.refreshMV(name)
RETURNS VOID 
SECURITY DEFINER
AS 
$BODY$
DECLARE 
    matview ALIAS FOR $1;
    entry __WDB_SCHEMA__.materializedView%ROWTYPE;
BEGIN

    SELECT * INTO entry FROM __WDB_SCHEMA__.materializedView WHERE materializedViewName = matview;

    IF NOT FOUND THEN
        RAISE EXCEPTION 'Materialized view % does not exist.', matview;
    END IF;

    EXECUTE 'DELETE FROM ' || matview;
    EXECUTE 'INSERT INTO ' || matview
        || ' SELECT * FROM ' || entry.viewName;

    UPDATE __WDB_SCHEMA__.materializedView
        SET lastRefreshTime=CURRENT_TIMESTAMP
        WHERE materializedViewName=matview;

    RETURN;
END
$BODY$
LANGUAGE plpgsql;


