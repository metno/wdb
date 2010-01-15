/*
 wdb

 Copyright (C) 2010 met.no

 Contact information:
 Norwegian Meteorological Institute
 Box 43 Blindern
 0313 OSLO
 NORWAY
 E-mail: post@met.no

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

#include "GeometryCache.h"
#include <PlaceSpecification.h>

namespace
{
struct ltPlaceSpecification
{
	bool operator () (const PlaceSpecification & a, const PlaceSpecification & b)
	{
		if ( a.startX_ != b.startX_ )
			return a.startX_ < b.startX_;

		if (a.startY_ != b.startY_)
			return a.startY_ < b.startY_;

		if (a.xNumber_ != b.xNumber_)
			return a.xNumber_ < b.xNumber_;

		if (a.yNumber_ != b.yNumber_)
			return a.yNumber_ < b.yNumber_;

		if (a.xIncrement_ != b.xIncrement_)
			return a.xIncrement_ < b.xIncrement_;

		if (a.yIncrement_ != b.yIncrement_)
			return a.yIncrement_ < b.yIncrement_;

		return strcmp(a.projDefinition_, b.projDefinition_) < 0;
	}
};

typedef std::map<PlaceSpecification, GeometryCache, ltPlaceSpecification> CacheCollection;
CacheCollection cache;
}


GeometryCache::GeomToWkt GeometryCache::geomToWkt;

GeometryCache & GeometryCache::get(const PlaceSpecification & ps)
{
	CacheCollection::iterator find = cache.find(ps);
	if ( find == cache.end() )
	{
		PlaceSpecification key = ps;
		key.projDefinition_ = strdup(ps.projDefinition_);
		find = cache.insert(std::make_pair(key, GeometryCache())).first;
	}
	return find->second;
}


GeometryCache::GeometryCache()
{
}

GeometryCache::~GeometryCache()
{
	for ( GeomToLonLat::iterator it = geomToLonLat.begin(); it != geomToLonLat.end(); ++ it )
		GEOSGeom_destroy(it->first);
}

void GeometryCache::setGeom(double lon, double lat, GEOSGeom geometryToInsert)
{
	lonLatToGeom[std::make_pair(lon, lat)] = GEOSGeomWrapper(geometryToInsert);
}

const GEOSGeom GeometryCache::getGeom(double lon, double lat) const
{
	LonLatToGeom::const_iterator find = lonLatToGeom.find(std::make_pair(lon, lat));
	if ( find == lonLatToGeom.end() )
		return NULL;
	return find->second.get();
}

void GeometryCache::setLonLat(const GEOSGeom geometryToInsert, double lon, double lat)
{
	LongitudeLatitude l;
	l.lon = lon;
	l.lat = lat;
	geomToLonLat[GEOSGeom_clone(geometryToInsert)] = l;
}

const LongitudeLatitude * GeometryCache::getLonLat(const GEOSGeom geom) const
{
	GeomToLonLat::const_iterator find = geomToLonLat.find(geom);
	if ( find == geomToLonLat.end() )
		return NULL;
	return & find->second;
}

void GeometryCache::setWkt(const GEOSGeom geometry, const text * wkt)
{
	text * value = (text *) malloc(VARSIZE(wkt));
	memcpy(value, wkt, VARSIZE(wkt));
	geomToWkt[GEOSGeom_clone(geometry)] = value;
}

text * GeometryCache::getWkt(const GEOSGeom geometry)
{
	GeomToWkt::const_iterator find = geomToWkt.find(geometry);
	if ( find == geomToWkt.end() )
		return NULL;
	return find->second;
}


void GeometryCache::clear()
{
	for ( CacheCollection::iterator it = cache.begin(); it != cache.end(); ++ it )
		free(it->first.projDefinition_);
	cache.clear();

	for ( GeometryCache::GeomToWkt::iterator it = GeometryCache::geomToWkt.begin(); it != GeometryCache::geomToWkt.end(); ++ it )
	{
		GEOSGeom_destroy(it->first);
		free(it->second);
	}
	GeometryCache::geomToWkt.clear();
}
