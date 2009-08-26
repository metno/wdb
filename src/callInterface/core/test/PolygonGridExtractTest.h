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

#ifndef POLYGONGRIDEXTRACTTEST_H_
#define POLYGONGRIDEXTRACTTEST_H_

#include <cppunit/extensions/HelperMacros.h>
#include <PlaceSpecification.h>

class PolygonGridExtractTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PolygonGridExtractTest );
	CPPUNIT_TEST( testLineSegmentIntersect );
	CPPUNIT_TEST( testLineSegmentNotIntersect );
	CPPUNIT_TEST( testLineInPolygon );
	CPPUNIT_TEST( testPointsInPolygon );
	CPPUNIT_TEST( testPolygonInsideGrid );
	CPPUNIT_TEST( testPolygonOutsideGrid_Above );
	CPPUNIT_TEST( testPolygonOutsideGrid_Below );
	CPPUNIT_TEST( testPolygonOutsideGrid_Left );
	CPPUNIT_TEST( testPolygonOutsideGrid_Right );
	CPPUNIT_TEST( testPolygonContainsGrid );
	CPPUNIT_TEST( testPolygonPreciselyOnGrid );
	CPPUNIT_TEST( testPolygonPartlyOutsideGrid_Top );
	CPPUNIT_TEST( testPolygonPartlyOutsideGrid_Bottom );
	CPPUNIT_TEST( testPolygonPartlyOutsideGrid_Left );
	CPPUNIT_TEST( testPolygonPartlyOutsideGrid_Right );
	CPPUNIT_TEST( testMetricGrid );
	CPPUNIT_TEST_SUITE_END( );
public:
	PolygonGridExtractTest();
	virtual ~PolygonGridExtractTest();

	virtual void setUp();
	virtual void tearDown();

	/// Test line segment intersection
	void testLineSegmentIntersect();
	/// Test line segment non-intersection
	void testLineSegmentNotIntersect();
	/// Test line in polygon
	void testLineInPolygon();
	/// Test points from polygon
	void testPointsInPolygon();

	/// Test points from polygon
	void testPolygonInsideGrid();
	/// Test points from polygon
	void testPolygonOutsideGrid_Above();
	/// Test points from polygon
	void testPolygonOutsideGrid_Below();
	/// Test points from polygon
	void testPolygonOutsideGrid_Right();
	/// Test points from polygon
	void testPolygonOutsideGrid_Left();
	/// Test points from polygon
	void testPolygonContainsGrid();
	/// Test points from polygon
	void testPolygonPreciselyOnGrid();
	/// Test points from polygon
	void testPolygonPartlyOutsideGrid_Top();
	/// Test points from polygon
	void testPolygonPartlyOutsideGrid_Bottom();
	/// Test points from polygon
	void testPolygonPartlyOutsideGrid_Left();
	/// Test points from polygon
	void testPolygonPartlyOutsideGrid_Right();

	/// Test if metric grids are handled correctly
	void testMetricGrid();

private:
	PlaceSpecification ps;

};

#endif /* POLYGONGRIDEXTRACTTEST_H_ */
