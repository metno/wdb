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


#include <gribPointTest.h>
#include <gribPoint.h>
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( GribPointTest );


GribPointTest::GribPointTest()
{}

GribPointTest::~GribPointTest()
{}

void GribPointTest::setUp()
{}

void GribPointTest::tearDown()
{}

void GribPointTest::testValid()
{
	GribPoint p( "23,1=12.1" );
	
	CPPUNIT_ASSERT_EQUAL( 23, p.x() );
	CPPUNIT_ASSERT_EQUAL( 1, p.y() );
	CPPUNIT_ASSERT_EQUAL( 12.1, p.val() );
}

void GribPointTest::testImplicitValIsOne()
{
	GribPoint p( "0,3" );
	
	CPPUNIT_ASSERT_EQUAL( 0, p.x() );
	CPPUNIT_ASSERT_EQUAL( 3, p.y() );
	CPPUNIT_ASSERT_EQUAL( 1.0, p.val() );
}

void GribPointTest::testInvalidSyntax()
{
	CPPUNIT_ASSERT_THROW( GribPoint( "hallo?" ), std::logic_error );
	CPPUNIT_ASSERT_THROW( GribPoint( " 0, 3 = 4" ), std::logic_error );
	CPPUNIT_ASSERT_THROW( GribPoint( "0,9=a" ), std::logic_error );
	CPPUNIT_ASSERT_THROW( GribPoint( "0,9a=3" ), std::logic_error );
}

void GribPointTest::testStreams()
{
	const std::string data = "23,1=12.1";
	GribPoint p;
	std::istringstream is( data );
	is >> p;
	CPPUNIT_ASSERT( is );
	CPPUNIT_ASSERT_EQUAL( 23, p.x() );
	CPPUNIT_ASSERT_EQUAL( 1, p.y() );
	CPPUNIT_ASSERT_EQUAL( 12.1, p.val() );
	
	std::ostringstream os;
	os << p;
	CPPUNIT_ASSERT( os );
	CPPUNIT_ASSERT_EQUAL( data, os.str() );

	std::istringstream is2( "error," );
	is2 >> p;
	CPPUNIT_ASSERT( ! is2 );	
}
