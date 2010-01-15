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

#include "PolygonReaderTest.h"
#include <wciRead/extractGridData/PolygonReader.h>
#include <interpolationType.h>

float readFloatFromFile(long long id, int position)
{
	return 0.0;
}

void GridPointDataDestroy(struct GridPointData * toDestroy)
{
	GEOSGeom_destroy(toDestroy->location);
}

struct GridPointDataList * GridPointDataListNew(size_t size)
{
	struct GridPointDataList * dl = (struct GridPointDataList *) malloc(sizeof(struct GridPointDataList) * size);
	dl->count = size;
	if ( size != 0 )
		dl->data = (GridPointData *) malloc(sizeof(struct GridPointData) * size);
	else
		dl->data = NULL;

	return dl;
}

void GridPointDataListDelete(struct GridPointDataList * list)
{
	for ( unsigned int i = 0; i < list->count; ++ i )
		GridPointDataDestroy(list->data + i);
	if ( list->data != NULL )
		free(list->data);
	free(list);
}


using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION( PolygonReaderTest );

PolygonReaderTest::PolygonReaderTest( )
{
	// NOOP
}

PolygonReaderTest::~PolygonReaderTest( )
{
	// NOOP
}

void msg(const char * fmt, ...)
{
	printf(fmt);
}

void PolygonReaderTest::setUp()
{
	ps.startX_ = -6.739;
	ps.startY_ = -16.039;
	ps.xNumber_ = 850;
	ps.yNumber_ = 1100;
	ps.xIncrement_ = 0.036;
	ps.yIncrement_ = 0.036;
	//ps.poleLongitude = -40;
	//ps.poleLatitude = -22;
	ps.projDefinition_ = "+proj=ob_tran +o_proj=longlat +lon_0=-24 +o_lat_p=23.5 +a=6367470.0 +no_defs";

	initGEOS(msg, msg);
}

void PolygonReaderTest::tearDown()
{
	finishGEOS();
}

void PolygonReaderTest::testExtractPolygon()
{
	std::vector<GridPointData> polygon;

	GEOSGeomWrapper location(GEOSGeomFromWKT("POLYGON(( 11.34 60.75, 11.34 62.75, 13.34 62.75, 11.34 60.75 ))"));

	PolygonReader reader(BaseDataReader::getInstance(ps));
	reader.extractPolygon(polygon, location, Nearest);

	CPPUNIT_ASSERT_EQUAL(std::vector<GridPointData>::size_type(4), polygon.size());

	double x = polygon[0].x;
	double y = polygon[0].y;

	for ( unsigned i = 1; i < polygon.size() -1; ++ i )
		CPPUNIT_ASSERT(x != polygon[i].x or y != polygon[i].y);
}

void PolygonReaderTest::testExtractPolygonMetric()
{
	ps.projDefinition_ = "+proj=utm +zone=32 +ellps=WGS84 +datum=WGS84 +units=m +no_defs";

	std::vector<GridPointData> polygon;

	GEOSGeomWrapper location(GEOSGeomFromWKT("POLYGON(( 11.34 60.75, 11.34 62.75, 13.34 62.75, 11.34 60.75 ))"));

	PolygonReader reader(BaseDataReader::getInstance(ps));
	reader.extractPolygon(polygon, location, Nearest);

	CPPUNIT_ASSERT_EQUAL(std::vector<GridPointData>::size_type(4), polygon.size());

	double x = polygon[0].x;
	double y = polygon[0].y;

	for ( std::vector<GridPointData>::size_type i = 1; i < polygon.size() -1; ++ i )
		CPPUNIT_ASSERT(x != polygon[i].x or y != polygon[i].y);
}
