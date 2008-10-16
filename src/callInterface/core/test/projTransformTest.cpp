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


#include "projTransformTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( projTransformTest );

using namespace std;

projTransformTest::projTransformTest()
{}

projTransformTest::~projTransformTest()
{}

void projTransformTest::setUp()
{
	ps.startingLongitude_ = 5.75;
	ps.startingLatitude_ = -13.25;
	ps.xNumber_ = 248;
	ps.yNumber_ = 400;
	ps.xIncrement_ = 0.1;
	ps.yIncrement_ = 0.1;
	//ps.poleLongitude = -40;
	//ps.poleLatitude = -22;
	ps.projDefinition_ = "+proj=ob_tran +o_proj=latlong +lon_0=-40 +o_lat_p=22 +a=6367470.0 +b=6367470.0 +no_defs";

	tc[0].lon = -30.36;
	tc[0].lat = 54.57;
	tc[1].lon = 91.77;
	tc[1].lat = 83.14;
	tc[2].lon = 65.36;
	tc[2].lat = 62.1;
	tc[3].lon = 3.19;
	tc[3].lat = 44.06;
	tc[4].lon = -10;
	tc[4].lat = 55.01;
}

void projTransformTest::tearDown()
{
}

void projTransformTest::testTransform()
{
	// The values used here may be slightly wrong, with too small delta, or
	// values slightly off what's correct:

	const double delta = 0.02;

	lonlat ret = transform( 0, 0, & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[0].lon, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[0].lat, ret.lat, delta );

	ret = transform( 0, 399, & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[1].lon, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[1].lat, ret.lat, delta );

	ret = transform( 247, 399, & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[2].lon, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[2].lat, ret.lat, delta );

	ret = transform( 247, 0, & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[3].lon, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[3].lat, ret.lat, delta );

	ret = transform( 112, 42, & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[4].lon, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( tc[4].lat, ret.lat, delta );
}

void projTransformTest::testRTransform()
{
	// The values used here may be slightly wrong, with too small delta, or
	// values slightly off what's correct:

	const double delta = 0.04;

	lonlat ret = rTransform( tc[0], & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, ret.lat, delta );

	ret = rTransform( tc[1], & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 399, ret.lat, delta );

	ret = rTransform( tc[2], & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 247, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 399, ret.lat, delta );

	ret = rTransform( tc[3], & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 247, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, ret.lat, delta );

	ret = rTransform( tc[4], & ps );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 112, ret.lon, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 42, ret.lat, delta );
}
