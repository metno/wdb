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
--- Create the wdb_admin role
--- Role is the default role of the wdb administrator
CREATE ROLE wdb_admin NOLOGIN SUPERUSER;

--- Create the wdb_write role
--- Role is required by all users that need to be able to insert values
--- into tables.
CREATE ROLE wdb_write NOLOGIN;

--- Create the wdb_clean role
--- Role is required by all users that need to be able to delete values
--- from tables.
CREATE ROLE wdb_clean NOLOGIN;

--- Create the wdb_read role
--- Role is required by all users that need to read data from the database
CREATE ROLE wdb_read NOLOGIN;

--- Create the wdb_grib role
--- Role is required by all users that need to read grib metadata from the database
CREATE ROLE wdb_grib NOLOGIN IN ROLE wdb_write;

--- Create the wdb_felt role
--- Role is required by all users that need to read felt metadata from the database
CREATE ROLE wdb_felt NOLOGIN IN ROLE wdb_write;

--- Create the wdb_kvalobs role
--- Role is required by all users that need to read kvalobs metadata from the database
CREATE ROLE wdb_kvalobs NOLOGIN IN ROLE wdb_write;

--- Create the wdb_xml role
--- Role is required by all users that need to read xml metadata from the database
CREATE ROLE wdb_xml NOLOGIN IN ROLE wdb_write;

--- Create the wdb_test role
--- Role is required by all users that need to read test views from the database
CREATE ROLE wdb_test NOLOGIN;

--- Create the gribLoad user
CREATE ROLE gribload INHERIT LOGIN IN ROLE wdb_grib;

--- Create the gribLoad user
CREATE ROLE feltload INHERIT LOGIN IN ROLE wdb_felt;

--- Create the kvalobsLoad user
CREATE ROLE kvalobsload INHERIT LOGIN IN ROLE wdb_kvalobs;

GRANT INSERT ON spatial_ref_sys TO wdb_write;
GRANT SELECT ON spatial_ref_sys TO wdb_read;

--- A test writer for wci
CREATE ROLE writetest INHERIT LOGIN IN ROLE wdb_read, wdb_write, wdb_clean, wdb_test;

--- Create the test user
CREATE ROLE cleantest INHERIT LOGIN IN ROLE wdb_clean, wdb_test;

--- Create the test user
CREATE ROLE gribtest INHERIT LOGIN IN ROLE wdb_read, wdb_write, wdb_clean, wdb_test;

--- A test user for wci
CREATE ROLE wcitest INHERIT LOGIN IN ROLE wdb_read, wdb_clean, wdb_test;

--- A test writer for wci
CREATE ROLE wcitestwriter INHERIT LOGIN IN ROLE wdb_read, wdb_write, wdb_clean, wdb_test;
