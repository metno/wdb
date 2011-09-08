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
#include <boost/scoped_ptr.hpp>

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

void LocationTest::testEmptyLocation()
{
	CPPUNIT_ASSERT_THROW(Location l(""), Location::InvalidSpecification);
}

void LocationTest::testExactInterpolation()
{
	Location l("Exact foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.placeName());
	CPPUNIT_ASSERT_EQUAL(std::string("exact"), l.interpolation());
	CPPUNIT_ASSERT(! l.hasGeometry());
}

void LocationTest::testNearestInterpolation()
{
	Location l("nearest foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.placeName());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testSurroundInterpolation()
{
	Location l("surround foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.placeName());
	CPPUNIT_ASSERT_EQUAL(std::string("surround"), l.interpolation());
	CPPUNIT_ASSERT_EQUAL(1, l.interpolationParameter());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testSurroundInterpolationWithExplicitParameter()
{
	Location l("surround(4) foo");

	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.placeName());
	CPPUNIT_ASSERT_EQUAL(std::string("surround(4)"), l.interpolation());
	CPPUNIT_ASSERT_EQUAL(4, l.interpolationParameter());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testImplicitInterpolation()
{
	Location l("foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo"), l.placeName());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.hasGeometry() );	
}

void LocationTest::testInvalidInterpolationPlaceLocation()
{
	// There is no way we can pick up this error, so we don't even try
	// In effect we treat "omg" not as interpolation type, but as part of the 
	// identifier name
	
	Location l("omg foo");
	
	CPPUNIT_ASSERT_EQUAL(std::string("omg foo"), l.placeName());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
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
	
	CPPUNIT_ASSERT_EQUAL(std::string("POINT(68.1 -1.4)"), l.geometry());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( l.hasGeometry() );		
}

void LocationTest::testPointImplicitInterpolation()
{
	Location l("POINT(-68.21 1.034)");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POINT(-68.21 1.034)"), l.geometry());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( l.hasGeometry() );		
}

void LocationTest::testPolygon()
{
	Location l("nearest POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))"), l.geometry());
	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
	CPPUNIT_ASSERT( l.hasGeometry() );		
}

void LocationTest::testPolygonImplicitInterpolation()
{
	Location l("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))");
	
	CPPUNIT_ASSERT_EQUAL(std::string("POLYGON((68.1 -1.4, 67.3 14, 61 0, 68.1 -1.4))"), l.geometry());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( l.hasGeometry() );		
}

void LocationTest::testFreeTextPlace()
{
	Location l("surround foo Bar");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo bar"), l.placeName());
	CPPUNIT_ASSERT_EQUAL(std::string("surround"), l.interpolation());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testFreeTextPlaceImplicitInterpolation()
{
	Location l("foo Bar");
	
	CPPUNIT_ASSERT_EQUAL(std::string("foo bar"), l.placeName());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testSpecialCharacterInPlaceName()
{
	// This should not throw
	Location l("sært sted i østfåld");

	CPPUNIT_ASSERT_EQUAL(std::string("sært sted i østfåld"), l.placeName());
	CPPUNIT_ASSERT(l.interpolation().empty());
	CPPUNIT_ASSERT( ! l.hasGeometry() );
}

void LocationTest::testRejectMultiPoint()
{
	CPPUNIT_ASSERT_THROW(
			Location("MULTIPOINT(1 2, 4 2)"),
			Location::InvalidSpecification
	);
}

void LocationTest::testRejectMultiPolygon()
{
	CPPUNIT_ASSERT_THROW(
			Location("MULTIPOLYGON((1 2, 2 2, 2 1, 1 2) (3 2, 7 2, 7 1, 3 2))"),
			Location::InvalidSpecification
	);
}

void LocationTest::testSpaceInWkt()
{
	Location * l;
	CPPUNIT_ASSERT_NO_THROW(l = new Location("POINT ( 15.4161    68.6931 )")); // postgis cannot handle space at start of string

	boost::scoped_ptr<Location> loc(l);

	CPPUNIT_ASSERT_EQUAL(std::string("POINT ( 15.4161    68.6931 )"), loc->geometry());
	CPPUNIT_ASSERT(loc->interpolation().empty());
	CPPUNIT_ASSERT( loc->hasGeometry() );
}

//void LocationTest::testPointAndName()
//{
//	Location l("nearest POINT(68.1 -1.4) hirlam 8 grid");
//
//	CPPUNIT_ASSERT_EQUAL(std::string("nearest"), l.interpolation());
//	CPPUNIT_ASSERT_EQUAL(std::string("POINT(68.1 -1.4)"), l.geometry());
//	CPPUNIT_ASSERT_EQUAL(std::string("hirlam 8 grid"), l.placeName());
//	CPPUNIT_ASSERT( l.hasGeometry() );
//}
