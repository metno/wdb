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


#ifndef LOCATIONTEST_H_
#define LOCATIONTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <location.h>

class LocationTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LocationTest );
	CPPUNIT_TEST( testExactInterpolation );
	CPPUNIT_TEST( testNearestInterpolation );
	CPPUNIT_TEST( testSurroundInterpolation );
	CPPUNIT_TEST( testImplicitInterpolation );
	CPPUNIT_TEST( testInvalidInterpolationPlaceLocation );
	CPPUNIT_TEST_EXCEPTION( testInvalidInterpolationGeometryLocation, Location::InvalidSpecification );
	CPPUNIT_TEST( testPoint );
	CPPUNIT_TEST( testPointImplicitInterpolation );
	CPPUNIT_TEST( testPolygon );
	CPPUNIT_TEST( testPolygonImplicitInterpolation );
	CPPUNIT_TEST( testFreeTextPlace );
	CPPUNIT_TEST( testFreeTextPlaceImplicitInterpolation );
	CPPUNIT_TEST_SUITE_END();
	
public:
	LocationTest();
	virtual ~LocationTest();
	
	virtual void setUp();
	virtual void tearDown();

	void testExactInterpolation();
	void testNearestInterpolation();
	void testSurroundInterpolation();
	void testImplicitInterpolation();
	void testInvalidInterpolationPlaceLocation();
	void testInvalidInterpolationGeometryLocation();
	
	void testPoint();
	void testPointImplicitInterpolation();
	void testPolygon();
	void testPolygonImplicitInterpolation();
	void testFreeTextPlace();
	void testFreeTextPlaceImplicitInterpolation();
};

#endif /*LOCATIONTEST_H_*/
