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
select wci.begin('wcitest');
-- Query 1
select * from wci.Read 
(  
ARRAY[1], 
('1980-01-01 00:00:00', '1980-01-31 00:00:00', 'exact')::wci.TimeSpec, 
'POLYGON((10 58.5,11 58.5,11 59.5,10 58.5,10 58.5))', 
0::wci.interpolationtype, 
ARRAY[0], 
ARRAY['* temperature of air'],
(0,1000,'of isobaric surface','any')::wci.LevelSpec, 
('1980-01-01 00:00:00', '1980-01-1 00:00:00', 'any')::wci.TimeSpec, 
0::float
);
-- Query 2
select * from wci.Read 
(  
ARRAY[1], 
('1980-01-01 00:00:00', '1980-01-31 00:00:00', 'exact')::wci.TimeSpec, 
'POLYGON((10 60,11 60,11 61,10 61,10 60))', 
0::wci.interpolationtype, 
ARRAY[0], 
ARRAY['* temperature of air'],
(0,1000,'of isobaric surface','any')::wci.LevelSpec, 
('1980-01-01 00:00:00', '1980-01-1 00:00:00', 'any')::wci.TimeSpec, 
0::float
);
-- Query 3
select * from wci.Read 
(  
ARRAY[1], 
('1980-01-01 00:00:00', '1980-01-31 00:00:00', 'exact')::wci.TimeSpec, 
'POLYGON((13 60,14 60,14 61,13 61,13 60))', 
0::wci.interpolationtype, 
ARRAY[0], 
ARRAY['* temperature of air'],
(0,1000,'of isobaric surface','any')::wci.LevelSpec, 
('1980-01-01 00:00:00', '1980-01-1 00:00:00', 'any')::wci.TimeSpec, 
0::float
);
-- Query 4
select * from wci.Read 
(  
ARRAY[1], 
('1980-01-01 00:00:00', '1980-01-31 00:00:00', 'exact')::wci.TimeSpec, 
'POLYGON((10 60,11 60,11 61,10 61,10 60))', 
0::wci.interpolationtype, 
ARRAY[0], 
ARRAY['* temperature of air (potential)'],
(0,1000,'of isobaric surface','any')::wci.LevelSpec, 
('1980-01-01 00:00:00', '1980-01-1 00:00:00', 'any')::wci.TimeSpec, 
0::float
);
-- Query 5
select * from wci.Read 
(  
ARRAY[1], 
('1980-01-01 00:00:00', '1980-01-31 00:00:00', 'exact')::wci.TimeSpec, 
'POLYGON((10 60,11 60,11 61,10 61,10 60))', 
0::wci.interpolationtype, 
ARRAY[0], 
ARRAY['* pressure of air'],
(0,1000,'of isobaric surface','any')::wci.LevelSpec, 
('1980-01-01 00:00:00', '1980-01-1 00:00:00', 'any')::wci.TimeSpec, 
0::float
);
-- Close down
select wci.end();
