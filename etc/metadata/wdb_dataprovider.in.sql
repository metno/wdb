--- WDB Specific
select wci.begin( 'wdb', 0, 0, 0 );
select wci.adddataprovider( 'wdb', 'data provider group', 'any', '1 day', 'Data received from WDB components (unidentified)' );
select wci.adddataprovider( 'gribload', 'computer system', 'grid', '1 day', 'Data received from WDB GribLoad (unidentified)' );
select wci.adddataprovidertogroup( 'gribload', 'wdb');
select wci.adddataprovider( 'feltload', 'computer system', 'grid', '1 day', 'Data received from WDB FeltLoad (unidentified)' );
select wci.adddataprovidertogroup( 'feltload', 'wdb');

-- Test Data
select wci.begin( 'wdb', 999, 999, 999 );
select wci.adddataprovider( 'test invalid', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests for invalid data' );
select wci.adddataprovider( 'test group',   'data provider group', 'any', '1 day', 'Test group' );
select wci.adddataprovider( 'test writer', 'computer system', 'grid', '1 day', 'Test writer for WDB tests' );
select wci.adddataprovidertogroup( 'test writer', 'test group');
select wci.adddataprovider( 'test wci 0', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 0', 'test group');
select wci.adddataprovider( 'test wci 1', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 1', 'test group');
select wci.adddataprovider( 'test wci 2', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 2', 'test group');
select wci.adddataprovider( 'test wci 3', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 3', 'test group');
select wci.adddataprovider( 'test wci 4', 'computer system', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 4', 'test group');
select wci.adddataprovider( 'test wci 5', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 5', 'test group');
select wci.adddataprovider( 'test wci 6', 'computer system', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 6', 'test group');
select wci.adddataprovider( 'test wci 7', 'computer system', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test wci 7', 'test group');
select wci.adddataprovider( 'test cleandb', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test cleandb', 'test group');
select wci.adddataprovider( 'test gribload', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test gribload', 'test group');
select wci.adddataprovider( 'test feltload', 'computer system', 'grid', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test feltload', 'test group');
select wci.adddataprovider( 'test moxload', 'computer system', 'point', '1 day', 'Test data provider for WDB tests' );
select wci.adddataprovidertogroup( 'test moxload', 'test group');

--- WCI users
select wci.addwciuser('wcitestwriter');
