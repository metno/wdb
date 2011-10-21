/*
    wdb - weather and water data storage

    Copyright (C) 2011 met.no

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
 * Implementation of Data Version WCI Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <AggregateTest.h>

CPPUNIT_TEST_SUITE_REGISTRATION( AggregateTest );

using namespace std;
using namespace pqxx;

AggregateTest::AggregateTest()
{
	// NOOP
}

AggregateTest::~AggregateTest()
{
	// NOOP
}

void AggregateTest::setUp()
{
	//setUser("wcitestwriter");
	AbstractWciTestFixture::setUp();
}

void AggregateTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
	//resetUser();
}

void
AggregateTest::testA1_01_NullMedian()
{
	result r = t->exec( statement_( 66 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT( r.front() [ "median" ].is_null() );
}

void
AggregateTest::testA1_02_SimpleMedian()
{
	result r = t->exec( statement_( 11 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT_EQUAL( 2, r.front() [ "median" ].as<int>() );
}

void
AggregateTest::testA1_03_CalculatedMedian()
{
	result r = t->exec( statement_( 10 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.9, r.front() [ "median" ].as<double>(), 0.01 );
}

void
AggregateTest::testA2_01_DuplicateIsMedian()
{
	result r = t->exec( statement_( 1 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT_EQUAL( 3, r.front() [ "median" ].as<int>() );
}

void
AggregateTest::testA2_02_DuplicateBeforeMedian()
{
	result r = t->exec( statement_( 15 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT_EQUAL( 3, r.front() [ "median" ].as<int>() );
}

void
AggregateTest::testA2_03_DuplicateAfterMedian()
{
	result r = t->exec( statement_( 16 ) );
	CPPUNIT_ASSERT_EQUAL( result::size_type( 1 ), r.size() );
    CPPUNIT_ASSERT_EQUAL( 5, r.front() [ "median" ].as<int>() );
}



//
// Support Functions
//

string AggregateTest::statement_( int paramid ) const
{
    ostringstream st;
    st << "SELECT wci.median(value) AS median FROM wci.read( ARRAY['test wci 4'], "
	   << "'test point 0', "
       << "'2005-01-01 06:00:00+00', NULL, "
       << "ARRAY['" << specFromParamNumber_.find( paramid )->second << "'], "
       << "NULL, NULL, NULL::wci.returnFloat )";

    return st.str();
}


namespace
{

map<int, string> getSpecFromParamNumber()
{
    map<int, string> ret;

    ret[ 1 ] = "air pressure";
    ret[ 3 ] = "tendency of surface air pressure";
    ret[ 10 ] = "snowfall amount";
    ret[ 11 ] = "air temperature";
    ret[ 15 ] = "max air temperature";
    ret[ 16 ] = "min air temperature";
    ret[ 32 ] = "altitude";
    ret[ 33 ] = "x wind";
    ret[ 34 ] = "y wind";
    ret[ 66 ] = "surface roughness length";

    return ret;
}

}

const map<int, string> AggregateTest::specFromParamNumber_ = getSpecFromParamNumber();
