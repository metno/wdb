select wci.begin( 'wdb', 999, 999, 999 );
select wci.adddataprovider( 'test invalid', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests for invalid data' );
select wci.adddataprovider( 'test group',   'Data Provider Group', 'any', '1 day', 'Test group' );
select wci.adddataprovider( 'test writer', 'Computer System', 'grid', '1 day', 'Test writer for WDB tests' );
select wci.adddataprovidertogroup( 'test writer', 'test group');
select wci.adddataprovider( 'test wci 0', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 0', 'test group');
select wci.adddataprovider( 'test wci 1', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 1', 'test group');
select wci.adddataprovider( 'test wci 2', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 2', 'test group');
select wci.adddataprovider( 'test wci 3', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 3', 'test group');
select wci.adddataprovider( 'test wci 4', 'Computer System', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 4', 'test group');
select wci.adddataprovider( 'test wci 5', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 5', 'test group');
select wci.adddataprovider( 'test wci 6', 'Computer System', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 6', 'test group');
select wci.adddataprovider( 'test wci 7', 'Computer System', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 7', 'test group');
select wci.adddataprovider( 'test cleandb', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test cleandb', 'test group');
select wci.adddataprovider( 'test gribload', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test gribload', 'test group');
select wci.adddataprovider( 'test feltload', 'Computer System', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test feltload', 'test group');
select wci.adddataprovider( 'test xmlload', 'Computer System', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test xmlload', 'test group');
--- WDB Specific
select wci.begin( 'wdb', 0, 0, 0 );
select wci.adddataprovider( 'wdb', 'Data Provider Group', 'any', '1 day', 'Data received from WDB components (unidentified)' );
select wci.adddataprovider( 'gribload', 'Computer System', 'grid', '1 day', 'Data received from WDB GribLoad (unidentified)' );
select wci.adddataprovidertogroup( 'gribload', 'wdb');
select wci.adddataprovider( 'feltload', 'Computer System', 'grid', '1 day', 'Data received from WDB FeltLoad (unidentified)' );
select wci.adddataprovidertogroup( 'feltload', 'wdb');
--- PROFF Data
select wci.begin( 'wdb', 88, 88, 88 );
select wci.adddataprovider( 'proff',   'Data Provider Group', 'any', '1 day', 'Field data produced in the PROFF system' );
select wci.adddataprovider( 'proff.raw', 'Computer System', 'grid', '1 day', 'proff.raw' );
select wci.adddataprovidertogroup( 'proff.raw', 'proff');
select wci.adddataprovider( 'proff.h12', 'Computer System', 'grid', '1 day', 'proff.h12' );
select wci.adddataprovidertogroup( 'proff.h12', 'proff');
select wci.adddataprovider( 'proff.h12h8', 'Computer System', 'grid', '1 day', 'proff.h12h8' );
select wci.adddataprovidertogroup( 'proff.h12h8', 'proff');
select wci.adddataprovider( 'proff.h12h8h4', 'Computer System', 'grid', '1 day', 'proff.h12h8h4' );
select wci.adddataprovidertogroup( 'proff.h12h8h4', 'proff');
select wci.adddataprovider( 'proff.h12h4', 'Computer System', 'grid', '1 day', 'proff.h12h4' );
select wci.adddataprovidertogroup( 'proff.h12h4', 'proff');
select wci.adddataprovider( 'proff.h12um4', 'Computer System', 'grid', '1 day', 'proff.h12um4' );
select wci.adddataprovidertogroup( 'proff.h12um4', 'proff');
select wci.adddataprovider( 'proff.h12h8h4vind', 'Computer System', 'grid', '1 day', 'proff.h12h8h4vind' );
select wci.adddataprovidertogroup( 'proff.h12h8h4vind', 'proff');
select wci.adddataprovider( 'proff.default', 'Computer System', 'grid', '1 day', 'proff.default' );
select wci.adddataprovidertogroup( 'proff.default', 'proff');
select wci.adddataprovider( 'proff.approved', 'Computer System', 'grid', '1 day', 'proff.approved' );
select wci.adddataprovidertogroup( 'proff.approved', 'proff');
--- ECMWF Data
select wci.adddataprovider( 'ecmwf atmospheric', 'Data Provider Group', 'grid', '1 day', 'The ECMWF atmospheric' );
select wci.adddataprovider( 'ecmwf atmospheric 35r1', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 35r1' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 35r1', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 35r2', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 35r2' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 35r2', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 35r3', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 35r3' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 35r3', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 36r1', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 36r1' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 36r1', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 36r2', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 36r2' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 36r2', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 36r3', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 36r3' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 36r3', 'ecmwf atmospheric');
select wci.adddataprovider( 'ecmwf atmospheric 36r4', 'Computer System', 'grid', '1 day', 'ECMWF atmospheric development cycle 36r4' );
select wci.adddataprovidertogroup( 'ecmwf atmospheric 36r4', 'ecmwf atmospheric');
-- Norwegian EC Data
select wci.adddataprovider( 'ecmwf felt', 'Data Provider Group', 'grid', '1 day', 'ECMWF fields packaged as FELT files' );
select wci.adddataprovider( 'ecmwf felt 103', 'Computer System', 'grid', '1 day', 'ECMWF field packaged as FELT files' );
select wci.adddataprovidertogroup( 'ecmwf felt 103', 'ecmwf felt');
select wci.adddataprovider( 'ecmwf felt 104', 'Computer System', 'grid', '1 day', 'ECMWF field packaged as FELT files' );
select wci.adddataprovidertogroup( 'ecmwf felt 104', 'ecmwf felt');
select wci.adddataprovider( 'ecmwf felt 105', 'Computer System', 'grid', '1 day', 'ECMWF field packaged as FELT files' );
select wci.adddataprovidertogroup( 'ecmwf felt 105', 'ecmwf felt');
select wci.adddataprovider( 'ecmwf felt 106', 'Computer System', 'grid', '1 day', 'ECMWF field packaged as FELT files' );
select wci.adddataprovidertogroup( 'ecmwf felt 106', 'ecmwf felt');
-- Norwegian Forecast
select wci.adddataprovider( 'hirlam norway', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'wam norway', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'pgen_probability yr', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'pgen_percentile yr', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'trheimsleia', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'vestland', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'arctic', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'north sea', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'ss eps', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'north sea skagen', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'barents', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'bodoloppa', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'oslo fjord', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'nordic', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'senorge', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
select wci.adddataprovider( 'yr corrected', 'Computer System', 'grid', '1 day', 'Local norwegian field data' );
---98198|0|local eceps modification|2010-01-01T00:00:00Z|infinity|98198|98198
---10300|0|proff.forecaster|2000-01-01T00:00:00Z|2000-01-01T00:00:00Z|10300|10301
---88150|0|proff eps|2000-01-01T00:00:00Z|2000-01-01T00:00:00Z|88150|88151
---88960|0|wam|2000-01-01T00:00:00Z|2000-01-01T00:00:00Z|88960|88961

--- WCI users
select wci.begin( 'wdb', 999, 999, 999 );
select wci.addwciuser('wcitestwriter');
