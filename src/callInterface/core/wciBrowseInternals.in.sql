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
__WCI_SCHEMA__.getBrowseDataProviderQuery(	dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataProviderQuery'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowsePlaceQuery( dataprovider 		text[],
								    location 			text,
								    referencetime 		text,
								    validtime 			text,
								    parameter 			text[],
								    level 				text,
								    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowsePlaceQuery'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseReferenceTimeQuery( dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseReferenceTimeQuery'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValidTimeQuery( dataprovider 		text[],
									    location 			text,
									    referencetime 		text,
									    validtime 			text,
									    parameter 			text[],
									    level 				text,
									    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValidTimeQuery'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseValueParameterQuery(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseValueParameterQuery'
LANGUAGE 'c' IMMUTABLE;


CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseLevelParameterQuery(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseLevelParameterQuery'
LANGUAGE 'c' IMMUTABLE;

CREATE OR REPLACE FUNCTION
__WCI_SCHEMA__.getBrowseDataVersionQuery(dataprovider 		text[],
										    location 			text,
										    referencetime 		text,
										    validtime 			text,
										    parameter 			text[],
										    level 				text,
										    dataversion 		integer[]
)
RETURNS text AS
'__WDB_LIBDIR__/__WCI_LIB__', 'getWciBrowseDataVersionQuery'
LANGUAGE 'c' IMMUTABLE;
