/*
    wdb - weather and water data storage

    Copyright (C) 2007 met.no

    Contact information:
    Norwegian Meteorological Institute
    Box 43 Blindern
    0313 OSLO
    NORWAY
    E-mail: wdb@met.no

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor1, Boston,
    MA  02110-1301, USA
*/


/**
 * @file
 * Implementation of Data Provider WCI Test Cases
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <DataProviderTest.h>
#include <string>
#include <sstream>
#include <set>
#include <boost/lexical_cast.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION( DataProviderTest );

using namespace std;
using namespace pqxx;


// Lifecycle
//---------------------------------------------------------------------------

DataProviderTest::DataProviderTest()
{
	// NOOP
}

DataProviderTest::~DataProviderTest()
{
	// NOOP
}

void DataProviderTest::setUp()
{
    AbstractWciTestFixture::setUp();
}

void DataProviderTest::tearDown()
{
    AbstractWciTestFixture::tearDown();
}


// The Tests
//---------------------------------------------------------------------------

void DataProviderTest::testD1_01A_NoDataProvider()
{
	CPPUNIT_ASSERT_THROW(  t->exec( statementGrid_( "'{}'" ) ), pqxx::data_exception );
}

void DataProviderTest::testD1_01B_NoDataProvider()
{
    CPPUNIT_ASSERT_THROW(  t->exec( statementFloat_( "'{}'" ) ), pqxx::sql_error );
}

void DataProviderTest::testD1_02A_OneDataProvider()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ), size_t( r1.size() ) );
}

void DataProviderTest::testD1_02B_OneDataProvider()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 0']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ), size_t( r2.size() ) );
}

void DataProviderTest::testD1_02C_OneDataProvider()
{
    result r3 = t->exec( statementFloat_( "ARRAY['test xml']" ) );

    CPPUNIT_ASSERT( count_val( r3, "dataprovidername", "test xml" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r3, "dataprovidername", "test xml" ), size_t( r3.size() ) );
}

void DataProviderTest::testD1_03A_MultipleDataProviders()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 2', 'test wci 4', 'test xml']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 4" ), size_t( 0 ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test xml" ), size_t( 0 ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );
}

void DataProviderTest::testD1_03B_MultipleDataProviders()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 1', 'test wci 2', 'test wci 4', 'test xml']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test xml" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) +
						  count_val( r2, "dataprovidername", "test wci 1" ) +
						  count_val( r2, "dataprovidername", "test wci 2" ) +
						  count_val( r2, "dataprovidername", "test wci 4" ) +
						  count_val( r2, "dataprovidername", "test xml" ),
						  size_t( r2.size() ) );
}


void DataProviderTest::testD1_04A_MoreThan255DataProviders()
{
    const int noProvidersQueried = 260;
    ostringstream ss;
    ss << "ARRAY[";
    for ( int i = 0; i < noProvidersQueried; i++ )
        ss << "'test wci " << i << "',";
    ss << " 'test wci 999']";

    result r1 = t->exec( statementGrid_( ss.str() ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );
}

void DataProviderTest::testD1_04B_MoreThan255DataProviders()
{
    const int noProvidersQueried = 260;
    ostringstream ss;
    ss << "ARRAY[";
    for ( int i = 0; i < noProvidersQueried; i++ )
        ss << "'test wci " << i << "',";
    ss << " 'test wci 999']";

    result r2 = t->exec( statementFloat_( ss.str() ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) +
						  count_val( r2, "dataprovidername", "test wci 1" ) +
						  count_val( r2, "dataprovidername", "test wci 2" ) +
						  count_val( r2, "dataprovidername", "test wci 4" ),
						  size_t( r2.size() ) );
}

void DataProviderTest::testD1_05A_NullDataProvider()
{
    result r1 = t->exec( statementGrid_( "NULL" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );
}

void DataProviderTest::testD1_05B_NullDataProvider()
{
    result r2 = t->exec( statementFloat_( "NULL" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test xml" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) +
						  count_val( r2, "dataprovidername", "test wci 1" ) +
						  count_val( r2, "dataprovidername", "test wci 2" ) +
						  count_val( r2, "dataprovidername", "test wci 4" ) +
						  count_val( r2, "dataprovidername", "test xml" ),
						  size_t( r2.size() ) );
}

void DataProviderTest::testD2_01A_LowerCase()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ), size_t( r1.size() ) );
}

void DataProviderTest::testD2_01B_LowerCase()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 4']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 4" ), size_t( r2.size() ) );
}

void DataProviderTest::testD2_02A_UpperCase()
{
    result r1 = t->exec( statementGrid_( "ARRAY['TEST WCI 0']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ), size_t( r1.size() ) );
}

void DataProviderTest::testD2_02B_UpperCase()
{
    result r2 = t->exec( statementFloat_( "ARRAY['TEST WCI 4']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 4" ), size_t( r2.size() ) );
}

void DataProviderTest::testD2_03A_MixedCase()
{
    result r1 = t->exec( statementGrid_( "ARRAY['teST wcI 0']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ), size_t( r1.size() ) );
}

void DataProviderTest::testD2_03B_MixedCase()
{
    result r2 = t->exec( statementFloat_( "ARRAY['teST wcI 4']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 4" ), size_t( r2.size() ) );
}

void DataProviderTest::testD3_01_AllRequestedProvidersExist()   //ok
{
    // No point in testing this, as testMultipleDataProviders() does the same job.
}

void DataProviderTest::testD3_02A_NoRequestedProvidersExist()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci -', 'test wci 12345', 'test wci XYZ']" ) );
    CPPUNIT_ASSERT( r1.empty() );
}

void DataProviderTest::testD3_02B_NoRequestedProvidersExist()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci -', 'test wci 12345', 'test wci XYZ']" ) );
    CPPUNIT_ASSERT( r2.empty() );
}

void DataProviderTest::testD3_03A_SomeRequestedProvidersExist()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 12345', 'test wci XYZ']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ), size_t( r1.size() ) );
}

void DataProviderTest::testD3_03B_SomeRequestedProvidersExist()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 4', 'test wci 12345', 'test wci XYZ']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) + count_val( r2, "dataprovidername", "test wci 4" ), size_t( r2.size() ) );
}

void DataProviderTest::testD4_01_NoDuplicates()   //ok
{
    // No point in testing this, as testMultipleDataProviders() does the same job.
}


void DataProviderTest::testD4_02A_OneDuplicate()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 2', 'test wci 1']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );

    result reference = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 2']" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 0" ), count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 1" ), count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 2" ), count_val( r1, "dataprovidername", "test wci 2") );
}

void DataProviderTest::testD4_02B_OneDuplicate()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 4', 'test xml', 'test wci 4']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test xml" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) + count_val( r2, "dataprovidername", "test wci 4" ) + count_val( r2, "dataprovidername", "test xml" ), size_t( r2.size() ) );

    result reference2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 4', 'test xml']" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test wci 0" ), count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test wci 4" ), count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test xml" ), count_val( r2, "dataprovidername", "test xml") );
}

void DataProviderTest::testD4_03A_SeveralDuplicates()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 1', 'test wci 2', 'test wci 2', 'test wci 1']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );

    result reference = t->exec( statementGrid_( "ARRAY['test wci 0', 'test wci 1', 'test wci 2']" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 0" ), count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 1" ), count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference, "dataprovidername", "test wci 2" ), count_val( r1, "dataprovidername", "test wci 2" ) );
}

void DataProviderTest::testD4_03B_SeveralDuplicates()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 1', 'test wci 1', 'test wci 4', 'test wci 4', 'test wci 1']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) + count_val( r2, "dataprovidername", "test wci 1" ) + count_val( r2, "dataprovidername", "test wci 4" ), size_t( r2.size() ) );

    result reference2 = t->exec( statementFloat_( "ARRAY['test wci 0', 'test wci 1', 'test wci 4']" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test wci 0" ), count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test wci 1" ), count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT_EQUAL( count_val( reference2, "dataprovidername", "test wci 4" ), count_val( r2, "dataprovidername", "test wci 4" ) );
}

void DataProviderTest::testD5_01A_GroupData()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test group']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );
}

void DataProviderTest::testD5_01B_GroupData()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test group']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test xml" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) + count_val( r2, "dataprovidername", "test wci 1" ) + count_val( r2, "dataprovidername", "test wci 2" ) + count_val( r2, "dataprovidername", "test wci 4" ) + count_val( r2, "dataprovidername", "test xml" ), size_t( r2.size() ) );
}

void DataProviderTest::testD5_02A_GroupNoData()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test group empty']" ) );

    CPPUNIT_ASSERT( r1.empty() );
}

void DataProviderTest::testD5_02B_GroupNoData()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test group empty']" ) );

    CPPUNIT_ASSERT( r2.empty() );
}

void DataProviderTest::testD5_03A_SeveralGroups()
{
    result r1 = t->exec( statementGrid_( "ARRAY['test group', 'test group empty']" ) );

    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r1, "dataprovidername", "test wci 2" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r1, "dataprovidername", "test wci 0" ) + count_val( r1, "dataprovidername", "test wci 1" ) + count_val( r1, "dataprovidername", "test wci 2" ), size_t( r1.size() ) );
}

void DataProviderTest::testD5_03B_SeveralGroups()
{
    result r2 = t->exec( statementFloat_( "ARRAY['test group', 'test group empty']" ) );

    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 2" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test wci 4" ) );
    CPPUNIT_ASSERT( count_val( r2, "dataprovidername", "test xml" ) );

    CPPUNIT_ASSERT_EQUAL( count_val( r2, "dataprovidername", "test wci 0" ) + count_val( r2, "dataprovidername", "test wci 1" ) + count_val( r2, "dataprovidername", "test wci 2" ) + count_val( r2, "dataprovidername", "test wci 4" ) + count_val( r2, "dataprovidername", "test xml" ), size_t( r2.size() ) );
}

void DataProviderTest::testD6_01_AddDataProvider()
{
	// Add Meta
    result rId = t->exec( "SELECT wci.addDataProvider(\'InstallTest 06-01\',"
													  "\'Computer System\',"
													  "\'Grid\',"
													  "\'Data Provider inserted by the WDB install tests\')" );
    // Check for meta
    result rC = t->exec( "SELECT * FROM wci.info( \'installTest 06-01\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC.size() > 0 );

}

void DataProviderTest::testD6_02_SetDataProviderName()
{
	// Set namespace to 1
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 0, 0 )" );
	// Add Meta
    result rId = t->exec( "SELECT wci.addDataProvider(\'InstallTest 06-02\',"
													  "\'Computer System\',"
													  "\'Grid\',"
													  "\'Data Provider inserted by the WDB install tests\')" );
    // Check for meta
    result rC = t->exec( "SELECT * FROM wci.info( \'Installtest 06-02\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC.size() > 0 );
	// Set namespace to 1
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 999, 0, 0 )" );
    // Set Name
    result rN = t->exec( "SELECT wci.SetDataProviderName(\'InstallTest 06-02\',\'InstallationsTest 06-02\')" );
    // Check for meta
    result rC1 = t->exec( "SELECT * FROM wci.info( \'Installtest 06-02\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC1.size() == 0 );
    result rC2 = t->exec( "SELECT * FROM wci.info( \'Installationstest 06-02\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC2.size() > 0 );
    // Return
    t->exec( "SELECT wci.begin('" + currentUser_ + "', 0, 999, 0 )" );
}

void DataProviderTest::testD6_03_SetDataProviderNameFail()
{
	// Add Meta
    result rId = t->exec( "SELECT wci.addDataProvider(\'InstallTest 06-03\',"
													  "\'Computer System\',"
													  "\'Grid\',"
													  "\'Data Provider inserted by the WDB install tests\')" );
    // Set Name
    result rN = t->exec( "SELECT wci.setDataProviderName(\'InstallTest 06-03\',\'InstallationsTest 06-03\')" );
    // Check for meta
    result rC1 = t->exec( "SELECT * FROM wci.info( \'Installtest 06-03\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC1.size() > 0 );
    result rC2 = t->exec( "SELECT * FROM wci.info( \'Installationstest 06-03\', NULL::wci.infodataprovider )" );
    CPPUNIT_ASSERT( rC2.size() == 0 );
}

// Support Functions
//---------------------------------------------------------------------------

std::string DataProviderTest::statementGrid_( const std::string & provider ) const
{
    ostringstream st;
    st << "SELECT dataprovidername FROM wci.read( ";
    if ( "NULL" == provider )
    	st << "NULL";
	else
		st << provider;
    st << ", NULL, '2004-12-24 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
    st << "NULL, NULL, NULL::wci.returngid )";

    return st.str();
}

std::string DataProviderTest::statementFloat_( const std::string & provider ) const
{
    ostringstream st;
    st << "SELECT dataprovidername FROM wci.read( ";
    if ( "NULL" == provider )
    	st << "NULL";
	else
		st << provider;
    st << ", 'POINT(-40 68.1332)', '2004-12-24 06:00:00+00', NULL, ";
	st << "'{\"" << defaultParameter << "\"}', ";
    st << "NULL, NULL, NULL::wci.returnfloat )";

    return st.str();
}
