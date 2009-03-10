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
-- Input / Output
CREATE FUNCTION __WCI_SCHEMA__.interpolationType_in( cstring )
    RETURNS __WCI_SCHEMA__.interpolationType
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'interpolationType_in'
    LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION __WCI_SCHEMA__.interpolationType_out( __WCI_SCHEMA__.interpolationType )
    RETURNS cstring
    AS '__WDB_LIBDIR__/__WCI_LIB__', 'interpolationType_out'
    LANGUAGE C IMMUTABLE STRICT;

-- The interpolation type for a single point.
--
-- Possible values are currently "exact", "nearest" and "surround".
-- "exact" Means that we are interested in _exactly_ the given point only. No
-- interpolation will be performed.
-- "nearest" return the nearest point for which we have data.
-- "surround" return the four nearest points.
CREATE TYPE __WCI_SCHEMA__.interpolationType (
   internallength = 4,
   input = __WCI_SCHEMA__.interpolationType_in,
   output = __WCI_SCHEMA__.interpolationType_out,
   -- receive = int4recv,
   -- send = int4send, --does not work
   alignment = int,
   PASSEDBYVALUE
);

CREATE CAST ( __WCI_SCHEMA__.interpolationType AS int ) WITHOUT FUNCTION AS IMPLICIT;
CREATE CAST ( int AS __WCI_SCHEMA__.interpolationType ) WITHOUT FUNCTION;
