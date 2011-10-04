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

GridPointDataList * PolygonReader::readPolygon(	const std::vector<GridPointData> & points, FileId dataId ) const
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
	for (std::vector<GridPointData>::const_iterator it = points.begin(); it
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
PolygonReader::gridPointsInPolygon( std::vector<GridPointData> & pointsInPolygon, const GEOSGeom polygon )
{
	BoundingBox bounds = getBounds( polygon );
	//elog(DEBUG1, GEOSGeomToWKT(polygon) );
	int startI = (bounds.left_ - reader_.placeSpecification().startX_) / reader_.placeSpecification().xIncrement_;
	if (startI < 0) startI = 0;
	int endI = ((bounds.right_ - reader_.placeSpecification().startX_) / reader_.placeSpecification().xIncrement_) + 1;
	if (endI > reader_.placeSpecification().xNumber_) endI = reader_.placeSpecification().xNumber_;
	int startJ = (bounds.bottom_ - reader_.placeSpecification().startY_) / reader_.placeSpecification().yIncrement_;
	if (startJ < 0) startJ = 0;
	int endJ = ((bounds.top_ - reader_.placeSpecification().startY_) / reader_.placeSpecification().yIncrement_) + 1;
	if (endJ > reader_.placeSpecification().yNumber_) endJ = reader_.placeSpecification().yNumber_;
	char res = 0;
	GEOSCoordSequence * seq;
	GEOSGeom point;
	double x;
	double y;
	for (int j = startJ; j < endJ; j++ ) {
		for (int i = startI; i < endI; i++) {
			x = reader_.placeSpecification().startX_ + (i * reader_.placeSpecification().xIncrement_);
			y = reader_.placeSpecification().startY_ + (j * reader_.placeSpecification().yIncrement_);
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
				pointsInPolygon.push_back(posPt);
			}
			GEOSGeom_destroy(point);
		}
	}
	// Return
	return ( pointsInPolygon.size() > 0 );
}

PolygonReader::BoundingBox
PolygonReader::getBounds( const GEOSGeom polygon )
{
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
	BoundingBox ret;;
	ret.left_ = coord.lon;
	ret.top_ = coord.lat;
	ret.right_ = coord.lon;
	ret.bottom_ = coord.lat;
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
		if (coord.lon < ret.left_)
			ret.left_ = coord.lon;
		else
		if (coord.lon > ret.right_)
			ret.right_ = coord.lon;
		if (coord.lat < ret.bottom_)
			ret.bottom_ = coord.lat;
		else
		if (coord.lat > ret.top_ )
			ret.top_ = coord.lat;
	}
	return ret;
}
