-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2011 met.no
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
--
-- This is the update file for WDB from schema version 1 to schema version 2
--
SET SESSION client_min_messages TO 'warning';

-- Clean out a redundant function (removed in WDB 1.0.2)
DROP FUNCTION IF EXISTS __WCI_SCHEMA__.getPlaceQuery( spec TEXT, returnType INTEGER );

NOTE: This code is not compiled yet. The above DROP should be included when we advance
the schema to version 2 in a future release.
