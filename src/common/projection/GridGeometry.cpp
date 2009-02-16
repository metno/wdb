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

#include <GridGeometry.h>
#include <gridRotate.h>
#include <wdbException.h>
#include <wdbLogHandler.h>
#include <wdbMath.h>
#include <sstream>
#include <iterator>
#include <cstring>

using namespace std;

GridGeometry::GridGeometry(const string & projDef, Orientation o,
						   size_type xNum, size_type yNum,
						   double xIncr,
						   double yIncr,
						   double startingX,
						   double startingY ) :
							   orientation_(o)
{
	xNumber_ = xNum;
	yNumber_ = yNum;
	xIncrement_ = xIncr;
	yIncrement_ = yIncr;
	startingLongitude_ = startingX;
	startingLatitude_ = startingY;

	projDefinition_ = new char[projDef.size() + 1];
	strcpy(projDefinition_, projDef.c_str());
}

GridGeometry::GridGeometry(const PlaceSpecification & ps, Orientation o)
	: PlaceSpecification(ps), orientation_(o)
{
	projDefinition_ = new char[strlen(ps.projDefinition_) + 1];
	strcpy(projDefinition_, ps.projDefinition_);
}

GridGeometry::GridGeometry(const GridGeometry & cpy)
{
	projDefinition_ = 0;
	operator = (cpy);
}

GridGeometry::~GridGeometry()
{
	delete projDefinition_;
}

const GridGeometry & GridGeometry::operator = (const GridGeometry & cpy)
{
	delete projDefinition_;

	orientation_ = cpy.orientation_;
	xNumber_ = cpy.xNumber_;
	yNumber_ = cpy.yNumber_;
	xIncrement_ = cpy.xIncrement_;
	yIncrement_ = cpy.yIncrement_;
	startingLongitude_ = cpy.startingLongitude_;
	startingLatitude_ = cpy.startingLatitude_;

	projDefinition_ = new char[strlen(cpy.projDefinition_) + 1];
	strcpy(projDefinition_, cpy.projDefinition_);

	return * this;
}


const string & GridGeometry::wktRepresentation() const
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.gridGeometry.wktRepresentation" );
	if ( geometry_.empty() )
	{
		ostringstream geo;
		geo << "POLYGON(" << * this << ")";
		geometry_ = geo.str();
	}
	log.debugStream() << geometry_;
	return geometry_;
}

bool GridGeometry::isRotated() const
{
	const string projDef( projDefinition_ );

	// If function returns end of line (npos) on both
	// find checks, we have failed to find either +lon_0 and
	// +o_lat_p
	if ( projDef.find("+lon_0=") == string::npos and
			projDef.find("+o_lat_p=") == string::npos )
		return false;
	return true;
}

GridGeometry::Point GridGeometry::point(GridGeometry::Corner c) const
{
	CornerCoordinates::const_iterator f = corners_.find(c);
	if ( f == corners_.end() )
	{
		size_t idx = getIndex(c);
		Point lonlat = point(xFromIndex(idx, *this), yFromIndex(idx, *this));
		f = corners_.insert(make_pair(c, lonlat)).first;
	}
	return f->second;
}

GridGeometry::Point GridGeometry::point(GridGeometry::size_type x, GridGeometry::size_type y) const
{
	Point start(x ,y);
	Point p = unprojectedLonLat_(start);
	project_(p);

	// cout << projDefinition_ << "  ->  " << DEFAULT_PROJECTION << endl;

	return p;
}

void GridGeometry::setOrientation(GridGeometry::Orientation o)
{
	WDB_LOG & log = WDB_LOG::getInstance( "wdb.gridGeometry.setOrientation" );
	if ( o == orientation_ )
	{
		log.debug( "Grid was already in requested format" );
		return;
	}
	if ( LeftUpperHorizontal == o or LeftLowerHorizontal == o )
	{
		const std::string orientation = LeftUpperHorizontal == o ?
				"LeftUpperHorizontal" : "LeftLowerHorizontal";
		log.debugStream() << "Changing orientation to " << orientation;
		orientation_ = o;
		startingLatitude_ += (yIncrement_ * (yNumber_ -1));
		yIncrement_ = -yIncrement_;
	}
	else
		throw wdb::WdbException("Unable to handle requested grid orientation", __func__);
}

GridGeometry::Point GridGeometry::unprojectedLonLat_(const Point & p) const
{
	Point ret(startingLongitude_ + (p.x * xIncrement_), startingLatitude_ + (p.y * yIncrement_));

	// cout << "Unprojected: " << ret << endl;
	if ( not isMetric( projDefinition_ ) ) {
		ret.x *= DEG_TO_RAD;
		ret.y *= DEG_TO_RAD;
	}

	return ret;
}

void GridGeometry::project_(Point & p) const
{
	static const WdbProjection targetProjection(DEFAULT_PROJECTION);

	WDB_LOG & log = WDB_LOG::getInstance( "wdb.gridGeometry.project_" );
	// Rotate coordinate and transform to WGS84.
	const WdbProjection projection(projDefinition_);

	//	if ( not isRotated() )
	//		projection.datumTransform( targetProjection, 1, & p.x, & p.y );
	//	else
	log.debugStream()  << "Project: " << p << " from " << projDefinition_;

	projection.transform( targetProjection, 1, & p.x, & p.y );
	p.x = wdb::round( p.x * RAD_TO_DEG, 4 );
	p.y = wdb::round( p.y * RAD_TO_DEG, 4 );

	log.debugStream() << "Result: " << p << " in " << DEFAULT_PROJECTION;
}

size_t GridGeometry::getIndex(GridGeometry::Corner c) const
{
	// KLUDGE: This should actually be sensitive to orientation.
	switch ( c )
	{
	case UpperLeft:
		return indexFromXy( 0, yNumber_-1, *this );
	case UpperRight:
		return indexFromXy( xNumber_-1, yNumber_-1, *this );
	case LowerLeft:
		return indexFromXy( 0, 0, *this );
	case LowerRight:
		return indexFromXy( xNumber_-1, 0, *this );
	}

	throw wdb::WdbException("Bad point corner request", __func__);
}


ostream & operator <<(ostream & s, const GridGeometry & p)
{
	const GridGeometry::Point firstPoint = p.point(GridGeometry::LowerLeft);
	return s << '(' <<
		firstPoint << "," <<
		p.point(GridGeometry::LowerRight) << "," <<
		p.point(GridGeometry::UpperRight) << "," <<
		p.point(GridGeometry::UpperLeft) << "," <<
		firstPoint <<
		')';
}

ostream & operator << (ostream & s, const GridGeometry::Point & p)
{
	return s << p.x << ' ' << p.y;
}
