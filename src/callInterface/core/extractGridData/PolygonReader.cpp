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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "PolygonReader.h"
#include <WdbProjection.h>
#include <sstream>

PolygonReader::PolygonReader(const PlaceSpecification & ps) :
	reader_(BaseDataReader::getInstance(ps))
{
	// NOOP
}

PolygonReader::~PolygonReader()
{
	// NOOP
}

GridPointDataList * PolygonReader::read( GEOSGeom location, InterpolationType interpolation, FileId dataId ) const
{
	GridPointDataList * list = 0;
	std::vector <GridPointData> polygon;
	std::vector <GridPointData> points;
	extractPolygon( polygon, location, interpolation );

	PolygonGridExtract g( reader_.placeSpecification() );
	g.gridPointsInPolygon( points, polygon );

	if ( points.size() > 0 )
		list = readPolygon( points, dataId );
	else {
		list = GridPointDataListNew( 0 );
	}
	return list;
}

void PolygonReader::extractPolygon( std::vector<GridPointData> & polygon, GEOSGeom location, InterpolationType interpolation ) const
{
	GEOSGeom outerRing = GEOSGetExteriorRing( location );
	if ( outerRing == NULL )
		throw std::runtime_error( "Outer ring of location is NULL" );

	GEOSCoordSeq coordSeq = GEOSGeom_getCoordSeq( outerRing );
	if ( coordSeq == NULL )
		throw std::runtime_error( "Coordinate sequence returned NULL" );

	unsigned int size;
	if ( GEOSCoordSeq_getSize( coordSeq, &size ) == 0 )
		throw std::runtime_error( "Error when getting size of outer ring" );
	if ( size < 4 )
		throw std::runtime_error( "Polygon has less than 3 edges" );

	WdbProjection prj( reader_.placeSpecification().projDefinition_ );
	lonlat coord;
	for ( unsigned int i = 0; i < size; i++ ) {
		GEOSCoordSeq_getX( coordSeq, i, &coord.lon );
		GEOSCoordSeq_getY( coordSeq, i, &coord.lat );

		if ( ! isMetric( DEFAULT_PROJECTION ) ) {
			coord.lon *=  DEG_TO_RAD;
			coord.lat *= DEG_TO_RAD;
		}
		prj.transformFromDefault( 1, &coord.lon, &coord.lat );
		GridPointData gpt;
		gpt.location = NULL;
		gpt.value = 0;
		if ( ! isMetric( reader_.placeSpecification().projDefinition_ ) ) {
			coord.lon *= RAD_TO_DEG;
			coord.lat *= RAD_TO_DEG;
		}
		gpt.x = coord.lon;
		gpt.y = coord.lat;
		polygon.push_back( gpt );
	}
}

GridPointDataList * PolygonReader::readPolygon(
		std::vector<GridPointData> & points, FileId dataId) const
{
	size_t noOfPointsToReturn = 0;
	for (std::vector<GridPointData>::const_iterator it = points.begin(); it != points.end(); ++it)
		if (not reader_.indexRefersToPointOutsideFile(it->x, it->y))
			++noOfPointsToReturn;

	const unsigned MAX_RETURN = 100000;
	if (noOfPointsToReturn > MAX_RETURN)
	{
		std::ostringstream errorMsg;
		errorMsg << "Too many rows to return! A single field will return "
				<< noOfPointsToReturn << " rows, but maximum is " << MAX_RETURN;
		throw std::runtime_error(errorMsg.str());
	}

	GridPointDataList * ret = GridPointDataListNew(noOfPointsToReturn);

	unsigned pos = 0;
	for (std::vector<GridPointData>::iterator it = points.begin(); it
			!= points.end() and pos < noOfPointsToReturn; ++it)
		if (reader_.readPoint(ret->data[pos], it->x, it->y, dataId))
			++pos;

	return ret;
}

