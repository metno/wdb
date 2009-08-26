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

#include "BaseDataReader.h"
#include <geos.h>


namespace
{

struct ComparePlaceSpecification
{
	bool operator () (const PlaceSpecification & a, const PlaceSpecification & b) const
	{
		if ( a.startX_ != b.startX_ ) return a.startX_ < b.startX_;
		if ( a.startY_ != b.startY_ ) return a.startY_ < b.startY_;
		if ( a.xNumber_ != b.xNumber_ ) return a.xNumber_ < b.xNumber_;
		if ( a.yNumber_ != b.yNumber_ ) return a.yNumber_ < b.yNumber_;
		if ( a.xIncrement_ != b.xIncrement_ ) return a.xIncrement_ < b.xIncrement_;
		if ( a.yIncrement_ != b.yIncrement_ ) return a.yIncrement_ < b.yIncrement_;
		return 0 > strcmp(a.projDefinition_, b.projDefinition_);
	}
};

}


const BaseDataReader & BaseDataReader::getInstance(const PlaceSpecification & ps)
{
	//typedef boost::shared_ptr<BaseDataReader> ReaderPtr;
	typedef BaseDataReader * ReaderPtr;
	typedef std::map<PlaceSpecification, ReaderPtr, ComparePlaceSpecification> ReaderCache;

	static ReaderCache readerCache;

	ReaderPtr & reader = readerCache[ps];
	if ( ! reader )
		reader = ReaderPtr(new BaseDataReader(ps));

	return * reader;
}


BaseDataReader::BaseDataReader(const PlaceSpecification & ps) :
	ps_(ps), projection_(ps.projDefinition_)
{
	// ensures that ps_.projDefinition_ is deleted once *this is deleted
	ps_.projDefinition_ = (char *) projection_.str().c_str();
}

BaseDataReader::~BaseDataReader()
{
	// NOOP
}

GEOSGeom BaseDataReader::getGeometry(double x, double y) const
{
	std::pair<double,double> idx(x,y);
	IdxCache::const_iterator find = idxCache_.find(idx);
	if ( find == idxCache_.end() )
	{
		if ( idxCache_.size() > 31 )
			idxCache_.clear();

		lonlat ret = wdbTransform(x, y, & ps_);
		GEOSCoordSeq coordinate = GEOSCoordSeq_create(1, 2);
		GEOSCoordSeq_setX(coordinate, 0, ret.lon);
		GEOSCoordSeq_setY(coordinate, 0, ret.lat);
		GEOSGeom geo = GEOSGeom_createPoint(coordinate);

		IdxCache::value_type toInsert(idx, GEOSGeomWrapper(geo));

		find = idxCache_.insert(idxCache_.begin(), toInsert);
	}

	return find->second.copy();
}

lonlat BaseDataReader::getExactIndex(GEOSGeom location) const
{
	GEOSGeomWrapper loc = GEOSGeomWrapper::createFromCopyOf(location);
	GeomCache::const_iterator find = geomCache_.find(GEOSGeomWrapper(loc));
	if ( find == geomCache_.end() )
	{
		if ( geomCache_.size() > 31 )
			geomCache_.clear();
		geos::Point * g = (geos::Point *) location;
		const geos::Coordinate * coord = g->getCoordinate();

		lonlat ll;
		ll.lon = coord->x;
		ll.lat = coord->y;
		lonlat ret = rTransform( ll, & ps_ );

		GeomCache::value_type toInsert(loc, ret);

		find = geomCache_.insert(geomCache_.begin(), toInsert);
	}

	return find->second;
}

bool BaseDataReader::indexRefersToPointOutsideFile(double x, double y) const
{
	return x < 0 or y < 0 or x >= ps_.xNumber_ or y >= ps_.yNumber_;
}

bool BaseDataReader::readPoint(GridPointData & out, int x, int y, FileId dataId) const
{
	if ( indexRefersToPointOutsideFile(x, y) )
		return false;
	out.x = x;
	out.y = y;
	out.location = getGeometry( x, y );
	out.value = readFloatFromFile(dataId, indexFromXy(x, y, ps_) );

	return true;
}
