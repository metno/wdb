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
 * Implementation of wci.begin Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <WciBeginTest.h>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( wciBeginTest );

using namespace std;
using namespace pqxx;


// Lifecycle
//---------------------------------------------------------------------------

wciBeginTest::wciBeginTest()
{
	// NOOP
}

wciBeginTest::~wciBeginTest()
{
	// NOOP
}

void wciBeginTest::tearDown()
{
	try
	{
		t->exec( "SELECT wci.end()" );
	}
	catch ( ... )
	{
		// NOOP
	}
	AbstractWdbTestFixture::tearDown();
}


// The Tests
//---------------------------------------------------------------------------

void wciBeginTest::testB1_01_ExistingUser()
{
	// We expect exception on error
	t->exec( statement_( "wcitest" ) );
	t->exec( statement_( "wcitest", 0, 0, 0 ) );
}

void wciBeginTest::testB1_02_NonExistingUser()
{
	// We expect exception on error
	CPPUNIT_ASSERT_THROW( t->exec( statement_( "noSuchUser" ) ), sql_error );
	startNewTransaction(); // Previous one has been aborted by error above
	CPPUNIT_ASSERT_THROW( t->exec( statement_( "noSuchUser", 0, 0, 0 ) ), sql_error );
}

void wciBeginTest::testB1_03_EmptyUser()
{
	// We expect exception on error
	CPPUNIT_ASSERT_THROW( t->exec( statement_( "" ) ), sql_error );
	startNewTransaction(); // Previous one has been aborted by error above
	CPPUNIT_ASSERT_THROW( t->exec( statement_( "", 0, 0, 0 ) ), sql_error );
}

void wciBeginTest::testB2_01_AutomaticNamespaceSelection()
{
	t->exec( statement_( "wcitest" ) );
	result r = t->exec( "SELECT * FROM " + std::string(WCI_SCHEMA) + ".getSessionData()" );
	CPPUNIT_ASSERT_EQUAL( 0, r.front()[ 0 ].as<int>() );
	CPPUNIT_ASSERT_EQUAL( 0, r.front()[ 1 ].as<int>() );
	CPPUNIT_ASSERT_EQUAL( 0, r.front()[ 2 ].as<int>() );
}

void wciBeginTest::testB2_02_ManualNamespaceSelection()
{
	t->exec( statement_( "wcitest", 1, 1, 1 ) );
	result r = t->exec( "SELECT * FROM " + std::string(WCI_SCHEMA) + ".getSessionData()" );
	CPPUNIT_ASSERT_EQUAL( 1, r.front()[ 0 ].as<int>() );
	CPPUNIT_ASSERT_EQUAL( 1, r.front()[ 1 ].as<int>() );
	CPPUNIT_ASSERT_EQUAL( 1, r.front()[ 2 ].as<int>() );
}

void wciBeginTest::testB3_01_NamespaceWithData()
{
	t->exec( statement_( "wcitest" ) );
	result r = t->exec( "SELECT * FROM wci.read( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.returngid )" );
	CPPUNIT_ASSERT( not r.empty() );
}

void wciBeginTest::testB3_02_NamespaceWithoutData()
{
	// We assume that noone uses namespace 99999
	t->exec( statement_( "wcitest", 99999, 99999, 99999 ) );
	result r = t->exec( "SELECT * FROM wci.read( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.returngid )" );
	CPPUNIT_ASSERT( r.empty() );
}

void wciBeginTest::testB3_03_NamespacePartialMatch()
{
	t->exec( statement_( "wcitest",998,0,0 ) );
	result r = t->exec( "SELECT * FROM wci.read( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.returngid )" );
	CPPUNIT_ASSERT( r.empty() );
	t->exec( "SELECT wci.end()" );

	t->exec( statement_( "wcitest",0,998,0 ) );
	r = t->exec( "SELECT * FROM wci.read( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.returngid )" );
	CPPUNIT_ASSERT( r.empty() );
	t->exec( "SELECT wci.end()" );

	t->exec( statement_( "wcitest",0,0,998 ) );
	r = t->exec( "SELECT * FROM wci.read( NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL::wci.returngid )" );
	CPPUNIT_ASSERT( r.empty() );
	t->exec( "SELECT wci.end()" );
}

void wciBeginTest::testB4_01_CallChangesUser()
{
	t->exec( statement_( "wcitest" ) );
	result r = t->exec( "select user" );
	if ( r.size() != 1 )
		CPPUNIT_FAIL( "Query should have returned 1 row" );

	CPPUNIT_ASSERT_EQUAL( string("wcitest"), r.front()[0].as<string>() );
}

void wciBeginTest::testB4_02_CallDoesNotChangeUser()
{
	NOT_FINISHED();
}

void wciBeginTest::testB4_03_CallDoesNotChangeUser()
{
	result ur = t->exec( "select user" );
	const string user = ur.front()[0].as<string>();

	CPPUNIT_ASSERT( user != "wcitest" );

	t->exec( statement_( user ) );
	result r = t->exec( "select user" );
	CPPUNIT_ASSERT_EQUAL( user, r.front()[0].as<string>() );
	NOT_FINISHED();
}

void wciBeginTest::testB5_01_NoSqlCodeAllowed()
{
	//CPPUNIT_ASSERT_THROW( t->exec( statement_( "wcitest; select user;" ) ), sql_error );
	// The above should not work, as it would allow all kinds of nasty attacks and manipulation of the DB
	NOT_FINISHED();
}


void wciBeginTest::testU1_01_ValidUser()
{
	// Same as B1_01
}

void wciBeginTest::testU1_02_InvalidUser()
{
	// Same as B1_02
}

void wciBeginTest::testU2_01_StringUser()
{
	// Tested by B1_01
}

void wciBeginTest::testU2_02_NotStringUser()
{
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.begin( 1234 )" ), sql_error );
	startNewTransaction(); // Previous one has been aborted by error above
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.begin( 1234, 5, 6, 7 )" ), sql_error );
}

void wciBeginTest::testU2_03_NullUser()
{
	// A server crash would give pqxx::broken_connection
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.begin(NULL)" ), data_exception );
	startNewTransaction(); // Previous one has been aborted by error above
	CPPUNIT_ASSERT_THROW( t->exec( "SELECT wci.begin(NULL, 0, 0, 0)" ), data_exception );
}


// Support Functions
//---------------------------------------------------------------------------

string wciBeginTest::statement_( const std::string & user ) const
{
	ostringstream ss;
	ss << "SELECT wci.begin( '" << user << "' )";
	return ss.str();
}

std::string wciBeginTest::statement_( const std::string & user, int cs1, int cs2, int cs3 ) const
{
	ostringstream ss;
	ss << "SELECT wci.begin( '" << user << "', " << cs1 << ", " << cs2 << ", " << cs3 << " )";
	return ss.str();
}
