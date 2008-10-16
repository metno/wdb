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
 * Implementation of wci.end Install Tests
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <WciEndTest.h>
#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION( wciEndTest );

using namespace std;


// Lifecycle
//---------------------------------------------------------------------------

wciEndTest::wciEndTest()
{
	// NOOP
}

wciEndTest::~wciEndTest()
{
	// NOOP
}

void wciEndTest::setUp()
{
    AbstractWdbTestFixture::setUp();
    t->exec( "SELECT wci.begin( 'wcitest' )" );
}


// The Tests
//---------------------------------------------------------------------------

void wciEndTest::testE1_01_WciBeginHasBeenCalled()
{
	// We expect exception if error
	t->exec( statement_() );
}

void wciEndTest::testE1_02_WciBeginHasNotBeenCalled()
{
	t->exec( statement_() ); // undo previous wci.begin
	CPPUNIT_ASSERT_THROW( t->exec( statement_() ), pqxx::sql_error );
}

namespace
{
	string getUser( pqxx::work & t )
	{
		return t.exec( "SELECT user" ).front()[ 0 ].as<string>();
	}
}

void wciEndTest::testE2_01_PreviousWciBeginDidChangeUser()
{
	CPPUNIT_ASSERT_EQUAL( string( "wcitest" ), getUser( * t ) );
	t->exec( statement_() );
	CPPUNIT_ASSERT( string( "wcitest" ) != getUser( * t ) );
}

void wciEndTest::testE2_02_PreviousWciBeginDidNotChangeUser()
{
	t->exec( statement_() );
	const string user = getUser( * t );
	const string begin = "SELECT wci.begin( '" + user + "' )";
	t->exec( begin );
	t->exec( statement_() );
	CPPUNIT_ASSERT_EQUAL( user, getUser( * t ) );
}


// Support
//---------------------------------------------------------------------------

std::string wciEndTest::statement_() const
{
	return "SELECT wci.end()";
}
