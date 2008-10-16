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



#include "locationTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( LocationTest ); 

LocationTest::LocationTest()
{
}

LocationTest::~LocationTest()
{
}

void LocationTest::setUp()
{
}

void LocationTest::tearDown()
{
}

void LocationTest::testExactInterpolation()
{
	Location l("exact foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("exact"), l.interpolation());
	CPPUNIT_ASSERT(! l.isGeometry());
}

void LocationTest::testNearestInterpolation()
{
	Location l("nearest foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( ! l.isGeometry() );
}

void LocationTest::testSurroundInterpolation()
{
	Location l("surround foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("surround"), l.interpolation());
	CPPUNIT_ASSERT( ! l.isGeometry() );
}

void LocationTest::testImplicitInterpolation()
{
	Location l("foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.location());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.isGeometry() );	
}

void LocationTest::testInvalidInterpolationPlaceLocation()
{
	// There is no way we can pick up this error, so we don't even try
	// In effect we treat "omg" not as interpolation type, but as part of the 
	// identifier name
	
	Location l("omg foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("omg foo"), l.location());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.isGeometry() );
}

void LocationTest::testInvalidInterpolationGeometryLocation()
{
	// As opposed to testInvalidInterpolationPlaceLocation, we can pick up 
	// this error, since the format of geometies is more strict 
	
	Location l("omg POINT(23 14)");
}

void LocationTest::testPoint()
{
	Location l("nearest POINT(68.1 -1.4)");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POINT(68.1 -1.4)"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( l.isGeometry() );		
}

void LocationTest::testPointImplicitInterpolation()
{
	Location l("POINT(-68.21 1.034)");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POINT(-68.21 1.034)"), l.location());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( l.isGeometry() );		
}

void LocationTest::testPolygon()
{
	Location l("nearest POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( l.isGeometry() );		
}

void LocationTest::testPolygonImplicitInterpolation()
{
	Location l("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))"), l.location());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( l.isGeometry() );		
}

void LocationTest::testFreeTextPlace()
{
	Location l("surround foo Bar");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo Bar"), l.location());
	CPPUNIT_ASSERT_EQUAL(std::string("surround"), l.interpolation());
	CPPUNIT_ASSERT( ! l.isGeometry() );
}

void LocationTest::testFreeTextPlaceImplicitInterpolation()
{
	Location l("foo Bar");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo Bar"), l.location());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.isGeometry() );
}
