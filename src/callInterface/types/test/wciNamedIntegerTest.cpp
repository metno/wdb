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


#include "wciNamedIntegerTest.h"
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( wciNamedIntegerTest );


using namespace std;


wciNamedIntegerTest::wciNamedIntegerTest()
{}

wciNamedIntegerTest::~wciNamedIntegerTest()
{}

void wciNamedIntegerTest::setUp()
{}

void wciNamedIntegerTest::tearDown()
{}

void wciNamedIntegerTest::testDefault()
{
	const string data[3] = { string( "foo" ), string( "bar" ), string( "baz" ) }; 

	wciNamedInteger ni( data, data +3 );
	
	for ( int i = 0; i < 3; ++ i )
	{
		CPPUNIT_ASSERT_EQUAL( ni[i], data[ i ] );
		CPPUNIT_ASSERT_EQUAL( i, ni[ data[i].c_str() ] );
	}
}

void wciNamedIntegerTest::testNoInput()
{
	char ** cp = 0;
	CPPUNIT_ASSERT_THROW( wciNamedInteger ni( cp, cp ), logic_error );
}


void wciNamedIntegerTest::testOneElement()
{
	const string data[1] = { string( "foo" ) }; 

	wciNamedInteger ni( data, data +1 );
	
	CPPUNIT_ASSERT_EQUAL( ni[0], data[ 0 ] );
	CPPUNIT_ASSERT_EQUAL( 0, ni[ data[0].c_str() ] );
}


void wciNamedIntegerTest::testMany()
{
	const size_t size = 26*26*26; // 26 letters in alphabet
	
	string data[ size ];
	string * it = data;
	for ( char a = 'a'; a <= 'z'; ++ a )
		for ( char b = 'a'; b <= 'z'; ++ b )
			for ( char c = 'a'; c <= 'z'; ++ c )
			{
				char buf[4] = {a,b,c,'\0'};
				* it = buf;
				++ it;
			}
	wciNamedInteger ni( data, data + size );
	
	for ( int i = 0; i < size; ++ i )
	{
		CPPUNIT_ASSERT_EQUAL( ni[i], data[ i ] );
		CPPUNIT_ASSERT_EQUAL( i, ni[ data[i].c_str() ] );
	}
}
