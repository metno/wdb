-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- 
-- wdb - weather and water data storage
--
-- Copyright (C) 2009 met.no
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


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataProviderQueryGrid(	dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataProviderQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowsePlaceQueryGrid( dataprovider 		text[],
								    location 			text,
								    referencetime 		text,
								    validtime 			text,
								    parameter 			text[],
								    level 				text,
								    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowsePlaceQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseReferenceTimeQueryGrid( dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseReferenceTimeQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValidTimeQueryGrid( dataprovider 		text[],
									    location 			text,
									    referencetime 		text,
									    validtime 			text,
									    parameter 			text[],
									    level 				text,
									    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValidTimeQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValueParameterQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValueParameterQueryGrid'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseLevelParameterQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseLevelParameterQueryGrid'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQueryGrid(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQueryGrid'
LANGUAGE 'c' IMMUTABLE;





CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataProviderQueryFloat(	dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataProviderQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowsePlaceQueryFloat( dataprovider 		text[],
								    location 			text,
								    referencetime 		text,
								    validtime 			text,
								    parameter 			text[],
								    level 				text,
								    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowsePlaceQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseReferenceTimeQueryFloat( dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseReferenceTimeQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValidTimeQueryFloat( dataprovider 		text[],
									    location 			text,
									    referencetime 		text,
									    validtime 			text,
									    parameter 			text[],
									    level 				text,
									    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValidTimeQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValueParameterQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValueParameterQueryFloat'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseLevelParameterQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseLevelParameterQueryFloat'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQueryFloat(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQueryFloat'
LANGUAGE 'c' IMMUTABLE;
