/*
 wdb

 Copyright (C) 2009 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: mail@met.no

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

#include "PolygonGridExtractTest.h"
#include <PolygonGridExtract.h>
#include <WdbProjection.h>
//#include <vector>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( PolygonGridExtractTest );

PolygonGridExtractTest::PolygonGridExtractTest( )
{
	// NOOP
}

PolygonGridExtractTest::~PolygonGridExtractTest( )
{
	// NOOP
}

void PolygonGridExtractTest::setUp()
{
	ps.startX_ = 5.7;
	ps.startY_ = -13.2;
	ps.xNumber_ = 248;
	ps.yNumber_ = 400;
	ps.xIncrement_ = 0.1;
	ps.yIncrement_ = 0.1;
	//ps.poleLongitude = -40;
	//ps.poleLatitude = -22;
	ps.projDefinition_ = const_cast<char*>("+proj=ob_tran +o_proj=latlong +lon_0=-40 +o_lat_p=22 +a=6367470.0 +b=6367470.0 +no_defs");
}

void PolygonGridExtractTest::tearDown()
{
	// NOOP
}

void PolygonGridExtractTest::testLineSegmentIntersect()
{
	PolygonGridExtract grid( ps );
	float x = 0;
	float y = 0;
	CPPUNIT_ASSERT( grid.lineSegmentIntersection( &x, &y, 2, 0, 14, 0, 7, 5, 15, -3 ) );
	CPPUNIT_ASSERT_EQUAL( static_cast<float>(12), x );
	CPPUNIT_ASSERT_EQUAL( static_cast<float>(0), y );
	// Extra checks
	float x1 = 0;
	float x2 = 0;
	float y1 = 0;
	float y2 = 0;
	float delta = 0.00001;
	CPPUNIT_ASSERT( grid.lineSegmentIntersection( &x1, &y1, 2, 0, 14, 0, 7, 5, 7, -3 ) );
	CPPUNIT_ASSERT( grid.lineSegmentIntersection( &x2, &y2, 2, 0, 14, 0, 7, -3, 7, 5 ) );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( x1, x2, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( y1, y2, delta );
}

void PolygonGridExtractTest::testLineSegmentNotIntersect()
{
	PolygonGridExtract grid( ps );
	float x = 0;
	float y = 0;

	CPPUNIT_ASSERT( ! grid.lineSegmentIntersection( &x, &y, 2, 0, 11, 0, 7, 5, 15, -3 ) );
	CPPUNIT_ASSERT_EQUAL( static_cast<float>(0), x );
	CPPUNIT_ASSERT_EQUAL( static_cast<float>(0), y );
}

void PolygonGridExtractTest::testLineInPolygon()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 7.5; p.y = 15.5;
	polygon.push_back( p );
	p.x = 8.1; p.y = 15.9;
	polygon.push_back( p );
	p.x = 8.2; p.y = 15.6;
	polygon.push_back( p );
	p.x = 8.4; p.y = 15.8;
	polygon.push_back( p );
	p.x = 8.2; p.y = 15.0;
	polygon.push_back( p );
	p.x = 7.8; p.y = 15.2;
	polygon.push_back( p );
	p.x = 7.5; p.y = 15.1;
	polygon.push_back( p );
	float aX = 5.7;
	float aY = 15.7;
	float bX = 30.0;
	float bY = 15.7;
	const double delta = 0.001;
	CPPUNIT_ASSERT( grid.linePointsInPolygon( points,
											  aX, aY, bX, bY,
											  289,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(5), points.size() );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 21 , points[0].x, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 22 , points[1].x, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 23 , points[2].x, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 24 , points[3].x, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 26 , points[4].x, delta );
}

void PolygonGridExtractTest::testPointsInPolygon()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 7.5; p.y = 15.5;
	polygon.push_back( p );
	p.x = 8.1; p.y = 15.9;
	polygon.push_back( p );
	p.x = 8.2; p.y = 15.6;
	polygon.push_back( p );
	p.x = 8.4; p.y = 15.8;
	polygon.push_back( p );
	p.x = 8.2; p.y = 15.0;
	polygon.push_back( p );
	p.x = 7.8; p.y = 15.2;
	polygon.push_back( p );
	p.x = 7.5; p.y = 15.1;
	polygon.push_back( p );
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	/*std::cout << points.size() << " ";
	std::cout << polygon.size() <<  std::endl;
	for (int i=0; i<points.size(); i++)
		std::cout << points[i]->x << ", " << points[i]->y << std::endl;
	*/
	const double delta = 0.001;
	CPPUNIT_ASSERT_EQUAL( size_t(53), points.size() );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 25 , points[0].x, delta );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 282 , points[0].y, delta );
}

void PolygonGridExtractTest::testPolygonInsideGrid()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 7.55; p.y = 15.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = 15.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = 16.15;
	polygon.push_back( p );
	p.x = 7.55; p.y = 16.15;
	polygon.push_back( p );
	p.x = 7.55; p.y = 15.55;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(36), points.size() );
};

void PolygonGridExtractTest::testPolygonOutsideGrid_Above()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 7.55; p.y = 28.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = 28.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = 29.15;
	polygon.push_back( p );
	p.x = 7.55; p.y = 29.15;
	polygon.push_back( p );
	p.x = 7.55; p.y = 28.55;
	CPPUNIT_ASSERT( ! grid.gridPointsInPolygon( points,
						  					    polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), points.size() );
};

void PolygonGridExtractTest::testPolygonOutsideGrid_Below()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 7.55; p.y = -15.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = -15.55;
	polygon.push_back( p );
	p.x = 8.15; p.y = -14.85;
	polygon.push_back( p );
	p.x = 7.55; p.y = -14.85;
	polygon.push_back( p );
	p.x = 7.55; p.y = -15.55;
	CPPUNIT_ASSERT( ! grid.gridPointsInPolygon( points,
						  					    polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), points.size() );
};

void PolygonGridExtractTest::testPolygonOutsideGrid_Left()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 3.55; p.y = 15.55;
	polygon.push_back( p );
	p.x = 4.15; p.y = 15.55;
	polygon.push_back( p );
	p.x = 4.15; p.y = 16.15;
	polygon.push_back( p );
	p.x = 3.55; p.y = 16.15;
	polygon.push_back( p );
	p.x = 3.55; p.y = 15.55;
	CPPUNIT_ASSERT( ! grid.gridPointsInPolygon( points,
											    polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), points.size() );
};

void PolygonGridExtractTest::testPolygonOutsideGrid_Right()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 30.55; p.y = 15.55;
	polygon.push_back( p );
	p.x = 31.15; p.y = 15.55;
	polygon.push_back( p );
	p.x = 31.15; p.y = 16.15;
	polygon.push_back( p );
	p.x = 30.55; p.y = 16.15;
	polygon.push_back( p );
	p.x = 30.55; p.y = 15.55;
	CPPUNIT_ASSERT( ! grid.gridPointsInPolygon( points,
											    polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(0), points.size() );
};

void PolygonGridExtractTest::testPolygonContainsGrid()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 5.5; p.y = -14.0;
	polygon.push_back( p );
	p.x = 32.0; p.y = -14.0;
	polygon.push_back( p );
	p.x = 32.0; p.y = 29.0;
	polygon.push_back( p );
	p.x = 5.5; p.y = 29.0;
	polygon.push_back( p );
	p.x = 5.5; p.y = -14.0;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t( ps.xNumber_ * ps.yNumber_ ), points.size() );
};

void PolygonGridExtractTest::testPolygonPreciselyOnGrid()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 5.7; p.y = -13.2;
	polygon.push_back( p );
	p.x = 5.7 + (247 * 0.1); p.y = -13.2;
	polygon.push_back( p );
	p.x = 5.7 + (247 * 0.1); p.y = -13.2 + (399 * 0.1);
	polygon.push_back( p );
	p.x = 5.7; p.y = -13.2 + (399 * 0.1);
	polygon.push_back( p );
	p.x = 5.7; p.y = -13.2;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( size_t( ps.xNumber_ * ps.yNumber_ ), points.size(), 1000.0 );
};

void PolygonGridExtractTest::testPolygonPartlyOutsideGrid_Top()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 8.55; p.y = 26.35;
	polygon.push_back( p );
	p.x = 9.25; p.y = 26.35;
	polygon.push_back( p );
	p.x = 9.25; p.y = 27.15;
	polygon.push_back( p );
	p.x = 8.55; p.y = 27.15;
	polygon.push_back( p );
	p.x = 8.55; p.y = 26.35;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t( 4 * 7 ), points.size() );
};

void PolygonGridExtractTest::testPolygonPartlyOutsideGrid_Bottom()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 8.55; p.y = -12.85;
	polygon.push_back( p );
	p.x = 9.25; p.y = -12.85;
	polygon.push_back( p );
	p.x = 9.25; p.y = -13.45;
	polygon.push_back( p );
	p.x = 8.55; p.y = -13.45;
	polygon.push_back( p );
	p.x = 8.55; p.y = -12.85;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t( 4 * 7 ), points.size() );
};

void PolygonGridExtractTest::testPolygonPartlyOutsideGrid_Left()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 5.15; p.y = 12.35;
	polygon.push_back( p );
	p.x = 6.05; p.y = 12.35;
	polygon.push_back( p );
	p.x = 6.05; p.y = 13.15;
	polygon.push_back( p );
	p.x = 5.15; p.y = 13.15;
	polygon.push_back( p );
	p.x = 5.15; p.y = 12.35;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t( 4 * 8 ), points.size() );
};

void PolygonGridExtractTest::testPolygonPartlyOutsideGrid_Right()
{
	PolygonGridExtract grid( ps );
	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;
	GridPointData p;
	p.x = 29.95; p.y = 12.35;
	polygon.push_back( p );
	p.x = 32.05; p.y = 12.35;
	polygon.push_back( p );
	p.x = 32.05; p.y = 13.15;
	polygon.push_back( p );
	p.x = 29.95; p.y = 13.15;
	polygon.push_back( p );
	p.x = 29.95; p.y = 12.35;
	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points,
											  polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t( 5 * 8 ), points.size() );
};

void PolygonGridExtractTest::testMetricGrid()
{
	ps.projDefinition_ = "+proj=utm +zone=32 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";

	CPPUNIT_ASSERT(isMetric(ps.projDefinition_)); // Precondition for test

	PolygonGridExtract grid( ps );

	std::vector<GridPointData> points;
	std::vector<GridPointData> polygon;

	GridPointData p;

	p.x = 7.55; p.y = 15.55;
	polygon.push_back( p );

	p.x = 8.15; p.y = 15.55;
	polygon.push_back( p );

	p.x = 8.15; p.y = 16.15;
	polygon.push_back( p );

	p.x = 7.55; p.y = 16.15;
	polygon.push_back( p );

	p.x = 7.55; p.y = 15.55;

	CPPUNIT_ASSERT( grid.gridPointsInPolygon( points, polygon ) );
	CPPUNIT_ASSERT_EQUAL( size_t(36), points.size() );
}
