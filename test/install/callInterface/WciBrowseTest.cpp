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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301, USA
*/



/**
 * @file
 * Implementation of wci.browse Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <WciBrowseTest.h>
#include <sstream>
#include <string>

CPPUNIT_TEST_SUITE_REGISTRATION(WciBrowseTest);

using namespace std;
using namespace pqxx;

WciBrowseTest::WciBrowseTest()
{
	// NOOP
}

WciBrowseTest::~WciBrowseTest()
{
	// NOOP
}

void WciBrowseTest::setUp()
{
	AbstractWciTestFixture::setUp();
}

void WciBrowseTest::tearDown()
{
	AbstractWciTestFixture::tearDown();
}


// Todo: Reestablish Info Tests

void WciBrowseTest::testBrowseDataProvider()
{
	const string select0 = "SELECT * "
						   "FROM wci.browse( ARRAY['test group'],"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL::wci.browsedataprovider )";
	result rS = t->exec(select0);

	// There is at least one browse row
	CPPUNIT_ASSERT( rS.size() );
	// One of each test wci provider
    CPPUNIT_ASSERT( count_val( rS, "dataprovidername", "test wci 0" ) );
    CPPUNIT_ASSERT( count_val( rS, "dataprovidername", "test wci 1" ) );
    CPPUNIT_ASSERT( count_val( rS, "dataprovidername", "test wci 2" ) );

}

void WciBrowseTest::testBrowsePlace()
{
	const string select0 = "SELECT * "
						   "FROM wci.browse( ARRAY['test group'],"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL::wci.browseplace )";
	result rS = t->exec( select0 );

	// There is at least one browse row
	CPPUNIT_ASSERT( rS.size() );
	// Check various places used
    CPPUNIT_ASSERT( count_val( rS, "placename", "test grid, rotated" ) );
    CPPUNIT_ASSERT( count_val( rS, "placename", "hirlam 10" ) );
    CPPUNIT_ASSERT( count_val( rS, "placename", "hirlam 20" ) );

}

void WciBrowseTest::testBrowseReferenceTime()
{
	const string select0 = "SELECT * "
						   "FROM wci.browse( ARRAY['test group'],"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL::wci.browsereferencetime ) "
						   "WHERE referencetime = '2004-12-24 06:00:00+00'";
	result rS = t->exec( select0 );

	// There is at least one browse row
	CPPUNIT_ASSERT( rS.size() );

}

void WciBrowseTest::testBrowseValueParameter()
{
	const string select0 = "SELECT * "
						   "FROM wci.browse( ARRAY['test group'],"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL::wci.browsevalueparameter )";
	result rS = t->exec( select0 );

	// There is at least one browse row
	CPPUNIT_ASSERT( rS.size() );
	// Check base valparam
    //CPPUNIT_ASSERT( count_val( rS, "valueparametername", "instant pressure of air" ) );
    //CPPUNIT_ASSERT( count_val( rS, "valueparametername", "instant temperature of air, anomaly" ) );
    //CPPUNIT_ASSERT( count_val( rS, "valueparametername", "instant temperature of air" ) );
}

void WciBrowseTest::testBrowseLevelParameter()
{
	const string select0 = "SELECT * "
						   "FROM wci.browse( ARRAY['test group'],"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL,"
										    "NULL::wci.browselevelparameter )";
	result rS = t->exec( select0 );

	// There is at least one browse row
	CPPUNIT_ASSERT( rS.size() );
	// Check base valparam
    CPPUNIT_ASSERT( count_val( rS, "levelparametername", "height above mean sea level distance" ) );
}

