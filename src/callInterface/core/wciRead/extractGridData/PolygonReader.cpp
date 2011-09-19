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
#include <vector>

PolygonReader::PolygonReader(const BaseDataReader & reader) :
	reader_(reader)
{
	// NOOP
}

PolygonReader::~PolygonReader()
{
	// NOOP
}

GridPointDataList * PolygonReader::readPolygon(	std::vector<GridPointData> & points, FileId dataId ) const
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

GridPointDataList * PolygonReader::read( const GEOSGeom location, InterpolationType interpolation, FileId dataId )
{
	std::vector <GridPointData> points;
	gridPointsInPolygon( points, location );
	GridPointDataList * list = 0;
	if ( points.size() > 0 )
		list = readPolygon( points, dataId );
	else {
		list = GridPointDataListNew(0);
	}
	return list;
}

bool
PolygonReader::gridPointsInPolygon( std::vector<GridPointData> & pos,
										 const GEOSGeom polygon )
{
	setBounds( polygon );
	//elog(DEBUG1, GEOSGeomToWKT(polygon) );
	double x;
	double y;
	int startI = (left_ - startX_) / incX_;
	if (startI < 0) startI = 0;
	int endI = ((right_ - startX_) / incX_) + 1;
	if (endI > numX_) endI = numX_;
	int startJ = (bottom_ - startY_) / incY_;
	if (startJ < 0) startJ = 0;
	int endJ = ((top_ - startY_) / incY_) + 1;
	if (endJ > numY_) endJ = numY_;
	char res = 0;
	GEOSCoordSequence * seq;
	GEOSGeom point;
	for (int j = startJ; j < endJ; j++ ) {
		for (int i = startI; i < endI; i++) {
			x = startX_ + (i * incX_);
			y = startY_ + (j * incY_);
			WdbProjection prj( reader_.placeSpecification().projDefinition_ );
			if ( ! isMetric( reader_.placeSpecification().projDefinition_ ) ) {
				x *= DEG_TO_RAD;
				y *= DEG_TO_RAD;
			}
			prj.transformToDefault( 1, &x, &y );
			if ( ! isMetric( DEFAULT_PROJECTION ) ) {
				x *= RAD_TO_DEG;
				y *= RAD_TO_DEG;
			}

			// Intersects
			seq = GEOSCoordSeq_create(1, 2);
			GEOSCoordSeq_setX(seq, 0, x);
			GEOSCoordSeq_setY(seq, 0, y);
			point = GEOSGeom_createPoint(seq);
			//elog(DEBUG1, GEOSGeomToWKT(point) );
			res = GEOS_DLL GEOSIntersects(polygon, point);
			if (res == 1) {
				GridPointData posPt;
				posPt.x = i;
				posPt.y = j;
				pos.push_back(posPt);
			}
			GEOSGeom_destroy(point);
		}
	}
	// Return
	return ( pos.size() > 0 );
}

void
PolygonReader::setBounds( const GEOSGeom polygon )
{
	startX_ = reader_.placeSpecification().startX_;
	startY_ = reader_.placeSpecification().startY_;
	incX_   = reader_.placeSpecification().xIncrement_;
	incY_   = reader_.placeSpecification().yIncrement_;
	numX_  	= reader_.placeSpecification().xNumber_;
	numY_  	= reader_.placeSpecification().yNumber_;
	GEOSGeom outerRing = const_cast<GEOSGeom>(GEOSGetExteriorRing( polygon ));
	if ( outerRing == NULL )
		throw std::runtime_error( "Outer ring of polygon/shape is NULL" );
	GEOSCoordSeq coordSeq = const_cast<GEOSCoordSeq>(GEOSGeom_getCoordSeq( outerRing ));
	if ( coordSeq == NULL )
		throw std::runtime_error( "Coordinate sequence of polygon/shape returned NULL" );
	unsigned int size;
	if ( GEOSCoordSeq_getSize( coordSeq, &size ) == 0 )
		throw std::runtime_error( "Error when getting size of outer ring of polygon/shape" );
	// Calculate Bounds
	WdbProjection prj( reader_.placeSpecification().projDefinition_ );
	lonlat coord;
	// Initialize
	GEOSCoordSeq_getX( coordSeq, 0, &coord.lon );
	GEOSCoordSeq_getY( coordSeq, 0, &coord.lat );
	if ( ! isMetric( DEFAULT_PROJECTION ) ) {
		coord.lon *=  DEG_TO_RAD;
		coord.lat *= DEG_TO_RAD;
	}
	prj.transformFromDefault( 1, &coord.lon, &coord.lat );
	if ( ! isMetric( reader_.placeSpecification().projDefinition_ ) ) {
		coord.lon *= RAD_TO_DEG;
		coord.lat *= RAD_TO_DEG;
	}
	left_ = coord.lon;
	top_ = coord.lat;
	right_ = coord.lon;
	bottom_ = coord.lat;
	for ( unsigned int i = 1; i < size; i++ ) {
		GEOSCoordSeq_getX( coordSeq, i, &coord.lon );
		GEOSCoordSeq_getY( coordSeq, i, &coord.lat );
		if ( ! isMetric( DEFAULT_PROJECTION ) ) {
			coord.lon *=  DEG_TO_RAD;
			coord.lat *= DEG_TO_RAD;
		}
		prj.transformFromDefault( 1, &coord.lon, &coord.lat );
		if ( ! isMetric( reader_.placeSpecification().projDefinition_ ) ) {
			coord.lon *= RAD_TO_DEG;
			coord.lat *= RAD_TO_DEG;
		}
		if (coord.lon < left_)
			left_ = coord.lon;
		else
		if (coord.lon > right_)
			right_ = coord.lon;
		if (coord.lat < bottom_)
			bottom_ = coord.lat;
		else
		if (coord.lat > top_ )
			top_ = coord.lat;
	}
}
